"""向导自验：原生输入框（打字 / 退格只删一个）+ 空白处拖动面板。
按 PID 锁定自己启动的 ide.exe，绝不碰其它窗口。
"""
import ctypes, ctypes.wintypes as w, os, subprocess, sys, time
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

def capture(hwnd, path):
    """抓真实屏幕像素（含原生子窗口 EDIT），PrintWindow 抓不到子窗口。"""
    rc = w.RECT(); user32.GetClientRect(hwnd, ctypes.byref(rc))
    W, H = rc.right, rc.bottom
    pt = w.POINT(0, 0); user32.ClientToScreen(hwnd, ctypes.byref(pt))
    src = user32.GetDC(0); mem = gdi32.CreateCompatibleDC(src)
    gdi32.BitBlt(mem, 0, 0, W, H, src, pt.x, pt.y, 0x00CC0020)
    bmp = gdi32.CreateCompatibleBitmap(src, W, H); gdi32.SelectObject(mem, bmp)
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

def send_input(inp):
    user32.SendInput(1, ctypes.byref(inp), ctypes.sizeof(INPUT))

def mouse(flags, x=None, y=None):
    if x is not None: user32.SetCursorPos(x, y); time.sleep(0.05)
    inp = INPUT(); inp.type = 0
    inp.u.mi = MI(0, 0, 0, flags, 0, None)
    send_input(inp)

def key(vk, up=False):
    inp = INPUT(); inp.type = 1
    inp.u.ki = KI(vk, 0, 0x0002 if up else 0, 0, None)
    send_input(inp)

def tap(vk):
    key(vk); time.sleep(0.03); key(vk, True); time.sleep(0.05)

def type_text(s):
    for ch in s:
        inp = INPUT(); inp.type = 1
        inp.u.ki = KI(0, ord(ch), 0x0004, 0, None)   # KEYEVENTF_UNICODE
        send_input(inp)
        inp.u.ki = KI(0, ord(ch), 0x0004 | 0x0002, 0, None)
        send_input(inp)
        time.sleep(0.02)

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
    time.sleep(3.0)
    W, H = capture(hwnd, os.path.join(OUT, "w0.png"))
    print("CLIENT", W, H, "FG", user32.GetForegroundWindow() == hwnd)

    pt = w.POINT(0, 0); user32.ClientToScreen(hwnd, ctypes.byref(pt))
    ox, oy = pt.x, pt.y

    # 面板几何（DIP，g_s=1.5）：W=min(880*1.5, 客户宽-48)，居中
    sc = 1.5
    pw = min(int(880 * sc), W - 48); ph = min(int(500 * sc), H - 48)
    px = (W - pw) // 2; py = (H - ph) // 2
    name_cx = px + int(348 * sc) + (pw - int(348 * sc) - 36) // 2
    name_cy = py + int(244 * sc) + 33
    print("PANEL", px, py, pw, ph, "NAMEBOX", name_cx, name_cy)

    # 1) 点名称框 → Ctrl+A → 打字
    mouse(0x0002, ox + name_cx, oy + name_cy); time.sleep(0.1); mouse(0x0004)
    time.sleep(0.4)
    key(0x11); tap(0x41); key(0x11, True); time.sleep(0.2)
    type_text("测试项目A")
    time.sleep(0.5)
    capture(hwnd, os.path.join(OUT, "w1_typed.png"))

    # 2) 退格一次：只删一个字符
    tap(0x08); time.sleep(0.5)
    capture(hwnd, os.path.join(OUT, "w2_back1.png"))

    # 3) 空白处（面板顶边内侧、两半之间的空隙）按下并拖走
    dx0 = px + pw // 2; dy0 = py + 8
    mouse(0x0002, ox + dx0, oy + dy0); time.sleep(0.15)
    for i in range(1, 21):
        mouse(0x0001, ox + dx0 - 3 * i, oy + dy0 + 2 * i)
        time.sleep(0.012)
    time.sleep(0.3)
    capture(hwnd, os.path.join(OUT, "w3_moving.png"))
    mouse(0x0004); time.sleep(0.5)
    capture(hwnd, os.path.join(OUT, "w4_dropped.png"))
    print("DONE")
    p.kill()

main()
