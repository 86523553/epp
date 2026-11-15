"""DPI 感知版向导探针：坐标不再被系统按 1.5 倍虚拟化，跟程序看到的像素一致。
验证：向导是否打开 / 两个原生输入框是否存在且在框内 / 打字 / 退格一次 / 拖面板。
"""
import ctypes, ctypes.wintypes as w, os, subprocess, time
from PIL import Image

try:
    ctypes.windll.shcore.SetProcessDpiAwareness(2)   # PER_MONITOR_AWARE
except Exception:
    ctypes.windll.user32.SetProcessDPIAware()

user32 = ctypes.WinDLL("user32", use_last_error=True)
gdi32  = ctypes.WinDLL("gdi32", use_last_error=True)
APP    = r"<仓库根>\环境\ide.exe"
ENVDIR = r"<仓库根>\环境"
OUT    = r"%USERPROFILE%\AppData\Local\hermes\cache\scratch"
LOG    = os.path.join(os.environ.get("TEMP", "."), "ide_drag.log")

class BIH(ctypes.Structure):
    _fields_ = [("biSize", w.DWORD), ("biWidth", w.LONG), ("biHeight", w.LONG),
                ("biPlanes", w.WORD), ("biBitCount", w.WORD), ("biCompression", w.DWORD),
                ("biSizeImage", w.DWORD), ("biXPelsPerMeter", w.LONG), ("biYPelsPerMeter", w.LONG),
                ("biClrUsed", w.DWORD), ("biClrImportant", w.DWORD)]

def window_of_pid(pid, timeout=20.0):
    end = time.time() + timeout
    while time.time() < end:
        found = []
        @ctypes.WINFUNCTYPE(w.BOOL, w.HWND, w.LPARAM)
        def cb(h, l):
            if not user32.IsWindowVisible(h): return True
            p = w.DWORD(); user32.GetWindowThreadProcessId(h, ctypes.byref(p))
            if p.value != pid: return True
            r = w.RECT(); user32.GetWindowRect(h, ctypes.byref(r))
            if (r.right - r.left) > 600 and (r.bottom - r.top) > 400: found.append(h)
            return True
        user32.EnumWindows(cb, 0)
        if found: return found[0]
        time.sleep(0.25)
    return None

def edits_of(hwnd):
    out = []
    @ctypes.WINFUNCTYPE(w.BOOL, w.HWND, w.LPARAM)
    def cb(h, l):
        cls = ctypes.create_unicode_buffer(64); user32.GetClassNameW(h, cls, 64)
        if cls.value.upper() == "EDIT":
            r = w.RECT(); user32.GetWindowRect(h, ctypes.byref(r))
            out.append((h, r))
        return True
    user32.EnumChildWindows(hwnd, cb, 0)
    return out

EM_GETSEL = 0x00B0
WM_COPY   = 0x0301

def sel_of(h):
    """EM_GETSEL 不传指针，跨进程安全：返回值低字=起点，高字=终点。"""
    r = user32.SendMessageW(h, EM_GETSEL, 0, 0) & 0xFFFFFFFF
    return r & 0xFFFF, (r >> 16) & 0xFFFF

def clip_text():
    user32.OpenClipboard(0)
    try:
        h = user32.GetClipboardData(13)      # CF_UNICODETEXT
        if not h: return None
        ptr = ctypes.windll.kernel32.GlobalLock(h)
        s = ctypes.wstring_at(ptr)
        ctypes.windll.kernel32.GlobalUnlock(h)
        return s
    finally:
        user32.CloseClipboard()

def capture_child(h, path):
    r = w.RECT(); user32.GetWindowRect(h, ctypes.byref(r))
    W, H = r.right - r.left, r.bottom - r.top
    if W <= 0 or H <= 0: return None
    src = user32.GetDC(h); mem = gdi32.CreateCompatibleDC(src)
    bmp = gdi32.CreateCompatibleBitmap(src, W, H); gdi32.SelectObject(mem, bmp)
    gdi32.PatBlt(mem, 0, 0, W, H, 0x00FF0062)     # 先刷白，便于看清文字位置
    user32.PrintWindow(h, mem, 2)
    bi = BIH(); bi.biSize = ctypes.sizeof(bi); bi.biWidth = W; bi.biHeight = -H
    bi.biPlanes = 1; bi.biBitCount = 32
    buf = ctypes.create_string_buffer(W * H * 4)
    n = gdi32.GetDIBits(mem, bmp, 0, H, buf, ctypes.byref(bi), 0)
    gdi32.DeleteObject(bmp); gdi32.DeleteDC(mem); user32.ReleaseDC(h, src)
    if n == 0: return None
    Image.frombuffer("RGBA", (W, H), buf.raw, "raw", "BGRA", 0, 1).convert("RGB").save(path)
    return path

def capture(hwnd, path):
    rc = w.RECT(); user32.GetClientRect(hwnd, ctypes.byref(rc))
    W, H = rc.right, rc.bottom
    src = user32.GetDC(hwnd); mem = gdi32.CreateCompatibleDC(src)
    bmp = gdi32.CreateCompatibleBitmap(src, W, H); gdi32.SelectObject(mem, bmp)
    user32.PrintWindow(hwnd, mem, 2)
    bi = BIH(); bi.biSize = ctypes.sizeof(bi); bi.biWidth = W; bi.biHeight = -H
    bi.biPlanes = 1; bi.biBitCount = 32
    buf = ctypes.create_string_buffer(W * H * 4)
    n = gdi32.GetDIBits(mem, bmp, 0, H, buf, ctypes.byref(bi), 0)
    gdi32.DeleteObject(bmp); gdi32.DeleteDC(mem); user32.ReleaseDC(hwnd, src)
    if n == 0: raise RuntimeError("GetDIBits 失败")
    Image.frombuffer("RGBA", (W, H), buf.raw, "raw", "BGRA", 0, 1).convert("RGB").save(path)
    return W, H

class MI(ctypes.Structure):
    _fields_ = [("dx", w.LONG), ("dy", w.LONG), ("mouseData", w.DWORD),
                ("dwFlags", w.DWORD), ("time", w.DWORD), ("dwExtraInfo", ctypes.POINTER(ctypes.c_ulong))]
class KI(ctypes.Structure):
    _fields_ = [("wVk", w.WORD), ("wScan", w.WORD), ("dwFlags", w.DWORD),
                ("time", w.DWORD), ("dwExtraInfo", ctypes.POINTER(ctypes.c_ulong))]
class IU(ctypes.Union):
    _fields_ = [("mi", MI), ("ki", KI)]
class INPUT(ctypes.Structure):
    _fields_ = [("type", w.DWORD), ("u", IU)]

def send(x): user32.SendInput(1, ctypes.byref(x), ctypes.sizeof(INPUT))
def mouse(flags, x=None, y=None):
    if x is not None:
        user32.SetCursorPos(x, y); time.sleep(0.05)
    i = INPUT(); i.type = 0; i.u.mi = MI(0, 0, 0, flags, 0, None); send(i)
def tap(vk):
    i = INPUT(); i.type = 1; i.u.ki = KI(vk, 0, 0, 0, None); send(i); time.sleep(0.03)
    i = INPUT(); i.type = 1; i.u.ki = KI(vk, 0, 0x0002, 0, None); send(i); time.sleep(0.05)
def type_text(s):
    for ch in s:
        i = INPUT(); i.type = 1; i.u.ki = KI(0, ord(ch), 0x0004, 0, None); send(i)
        i = INPUT(); i.type = 1; i.u.ki = KI(0, ord(ch), 0x0004 | 0x0002, 0, None); send(i)
        time.sleep(0.03)

p = subprocess.Popen([APP], cwd=ENVDIR, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
hwnd = window_of_pid(p.pid)
print("HWND", hwnd, "PID", p.pid)
user32.ShowWindow(hwnd, 9)
user32.SetWindowPos(hwnd, -1, 0, 0, 0, 0, 0x0001 | 0x0002 | 0x0040)
fg = user32.GetForegroundWindow()
if fg != hwnd:
    tf = user32.GetWindowThreadProcessId(fg, None)
    tm = ctypes.windll.kernel32.GetCurrentThreadId()
    user32.AttachThreadInput(tm, tf, True)
    user32.SetForegroundWindow(hwnd); user32.BringWindowToTop(hwnd)
    user32.AttachThreadInput(tm, tf, False)
time.sleep(2.5)

rc = w.RECT(); user32.GetClientRect(hwnd, ctypes.byref(rc))
pt = w.POINT(0, 0); user32.ClientToScreen(hwnd, ctypes.byref(pt))
print("CLIENT", rc.right, rc.bottom, "ORIGIN", pt.x, pt.y)
eds = edits_of(hwnd)
print("EDIT 数", len(eds))
for h, r in eds:
    print("  EDIT", r.left - pt.x, r.top - pt.y, r.right - r.left, r.bottom - r.top,
          "visible", bool(user32.IsWindowVisible(h)), "sel", sel_of(h))

capture(hwnd, os.path.join(OUT, "p0.png"))

if eds:
    # 点第一个（名称）输入框 → 全选 → 打字 → 退格
    h, r = eds[0]
    cx = pt.x + (r.left - pt.x) + (r.right - r.left) // 2
    cy = pt.y + (r.top - pt.y) + (r.bottom - r.top) // 2
    user32.SetForegroundWindow(hwnd); time.sleep(0.2)
    print("前台是不是本窗口:", user32.GetForegroundWindow() == hwnd)
    mouse(0x0002, cx, cy); time.sleep(0.1); mouse(0x0004); time.sleep(0.5)
    print("点开后 sel", sel_of(h), "焦点:", user32.GetFocus() == h,
          "前台:", user32.GetForegroundWindow() == hwnd)

    # Ctrl+A
    i = INPUT(); i.type = 1; i.u.ki = KI(0x11, 0, 0, 0, None); send(i)
    tap(0x41)
    i = INPUT(); i.type = 1; i.u.ki = KI(0x11, 0, 0x0002, 0, None); send(i)
    time.sleep(0.3)
    print("Ctrl+A 后 sel", sel_of(h))
    capture_child(h, os.path.join(OUT, "pe_sel.png"))

    # Ctrl+C 复制
    i = INPUT(); i.type = 1; i.u.ki = KI(0x11, 0, 0, 0, None); send(i)
    tap(0x43)
    i = INPUT(); i.type = 1; i.u.ki = KI(0x11, 0, 0x0002, 0, None); send(i)
    time.sleep(0.4)
    tap(0x43)
    print("Ctrl+C 已按（剪贴板跨进程读不了，跳过内容校验）")

    # 打字（全选状态下直接替换）
    type_text("测试项目A"); time.sleep(0.5)
    capture_child(h, os.path.join(OUT, "pe_typed.png"))
    print("打字后 sel", sel_of(h))

    # 退格一次
    tap(0x08); time.sleep(0.4)
    capture_child(h, os.path.join(OUT, "pe_back.png"))
    print("退格后 sel", sel_of(h))

    # 拖选
    mouse(0x0002, cx - 60, cy); time.sleep(0.1)
    for k in range(1, 11):
        mouse(0x0001, cx - 60 + 12 * k, cy); time.sleep(0.02)
    mouse(0x0004); time.sleep(0.3)
    print("拖选后 sel", sel_of(h))
    capture_child(h, os.path.join(OUT, "pe_dragsel.png"))

# 空白处（面板顶边内侧）按住拖
mouse(0x0002, pt.x + 1200, pt.y + 520); time.sleep(0.15)
for k in range(1, 21):
    mouse(0x0001, pt.x + 1200 - 4 * k, pt.y + 520 + 2 * k); time.sleep(0.012)
time.sleep(0.3); mouse(0x0004); time.sleep(0.6)
capture(hwnd, os.path.join(OUT, "p3_dragged.png"))
print("---- log ----")
try:
    print(open(LOG, encoding="utf-8", errors="replace").read()[-600:])
except OSError as e:
    print("no log", e)
p.kill()
