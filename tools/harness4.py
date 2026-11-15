"""向导自验 v2：直接读原生 EDIT 的文字（跨进程 WM_GETTEXT），不靠截图猜。
   顺带验证：退格只删一个字符、空白处拖面板、拖后输入框跟着走。
"""
import ctypes, ctypes.wintypes as w, os, subprocess, time
from PIL import Image

user32 = ctypes.WinDLL("user32", use_last_error=True)
gdi32  = ctypes.WinDLL("gdi32", use_last_error=True)
APP    = r"<仓库根>\环境\ide.exe"
ENVDIR = r"<仓库根>\环境"
OUT    = r"%USERPROFILE%\AppData\Local\hermes\cache\scratch"

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
        cls = ctypes.create_unicode_buffer(64)
        user32.GetClassNameW(h, cls, 64)
        if cls.value.upper() == "EDIT": out.append(h)
        return True
    user32.EnumChildWindows(hwnd, cb, 0)
    return out

def text_of(h):
    n = user32.GetWindowTextLengthW(h)
    b = ctypes.create_unicode_buffer(n + 2)
    user32.GetWindowTextW(h, b, n + 2)
    return b.value

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

def send(x):
    user32.SendInput(1, ctypes.byref(x), ctypes.sizeof(INPUT))

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

def main():
    p = subprocess.Popen([APP], cwd=ENVDIR, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    hwnd = window_of_pid(p.pid)
    print("HWND", hwnd, "PID", p.pid)
    if not hwnd:
        print("NO_WINDOW"); p.kill(); return
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
    W, H = rc.right, rc.bottom
    pt = w.POINT(0, 0); user32.ClientToScreen(hwnd, ctypes.byref(pt))
    ox, oy = pt.x, pt.y
    sc = 1.5
    pw = min(int(880 * sc), W - 48); ph = min(int(500 * sc), H - 48)
    px = (W - pw) // 2; py = (H - ph) // 2

    name_cx = px + int(348 * sc) + (pw - int(348 * sc) - 36) // 2
    name_cy = py + int(244 * sc) + 33

    # 1) 点名称框
    mouse(0x0002, ox + name_cx, oy + name_cy); time.sleep(0.1); mouse(0x0004)
    time.sleep(0.6)
    eds = edits_of(hwnd)
    print("EDIT 个数", len(eds))
    if not eds:
        print("NO_EDIT —— 点输入框没弹出原生 EDIT"); p.kill(); return
    print("点开时文字:", repr(text_of(eds[0])))

    # 2) 全选 → 打字
    i = INPUT(); i.type = 1; i.u.ki = KI(0x11, 0, 0, 0, None); send(i)
    tap(0x41)
    i = INPUT(); i.type = 1; i.u.ki = KI(0x11, 0, 0x0002, 0, None); send(i)
    time.sleep(0.2)
    type_text("测试项目A")
    time.sleep(0.5)
    print("打字后:", repr(text_of(eds[0])))

    # 3) 退格一次
    tap(0x08); time.sleep(0.4)
    print("退格一次后:", repr(text_of(eds[0])))

    # 4) 点空白 → 拖面板
    capture(hwnd, os.path.join(OUT, "v0_before.png"))
    dx0 = px + pw // 2; dy0 = py + 8
    mouse(0x0002, ox + dx0, oy + dy0); time.sleep(0.15)
    for i2 in range(1, 21):
        mouse(0x0001, ox + dx0 - 3 * i2, oy + dy0 + 2 * i2); time.sleep(0.012)
    time.sleep(0.4)
    mouse(0x0004); time.sleep(0.6)
    capture(hwnd, os.path.join(OUT, "v1_after.png"))

    # 拖完之后输入框是否跟着走
    for e in edits_of(hwnd):
        r = w.RECT(); user32.GetWindowRect(e, ctypes.byref(r))
        print("EDIT 位置", r.left - ox, r.top - oy, r.right - r.left, r.bottom - r.top,
              "visible", bool(user32.IsWindowVisible(e)))
    print("DONE")
    p.kill()

main()
