import ctypes, ctypes.wintypes as w, os, subprocess, time, sys
from PIL import ImageGrab
user32 = ctypes.WinDLL("user32", use_last_error=True)
APP = r"<仓库根>\环境\ide.exe"
ENVDIR = r"<仓库根>\环境"
E_FILE = r"<仓库根>\样例\新建 易程序.e"
OUT = r"%USERPROFILE%\AppData\Local\hermes\cache\scratch\live_design.png"

def find_main(timeout=15.0):
    end = time.time() + timeout
    while time.time() < end:
        best = None
        @ctypes.WINFUNCTYPE(w.BOOL, w.HWND, w.LPARAM)
        def cb(h, l):
            nonlocal best
            if not user32.IsWindowVisible(h):
                return True
            n = user32.GetWindowTextLengthW(h)
            if not n:
                return True
            buf = ctypes.create_unicode_buffer(n + 1)
            user32.GetWindowTextW(h, buf, n + 1)
            if "易语言" not in buf.value:
                return True
            r = w.RECT(); user32.GetWindowRect(h, ctypes.byref(r))
            if (r.right - r.left) > 600 and (r.bottom - r.top) > 400 and r.left > -10000:
                best = (h, (r.left, r.top, r.right, r.bottom))
            return True
        user32.EnumWindows(cb, 0)
        if best:
            return best
        time.sleep(0.25)
    return (None, None)

p = subprocess.Popen([APP, E_FILE], cwd=ENVDIR)
hwnd, rect = find_main()
print("HWND", hwnd, rect)
time.sleep(3.0)
if hwnd:
    user32.SetForegroundWindow(hwnd)
    time.sleep(0.8)
    r = w.RECT(); user32.GetWindowRect(hwnd, ctypes.byref(r))
    im = ImageGrab.grab(bbox=(r.left, r.top, r.right, r.bottom))
    im.save(OUT)
    print("GRAB", im.size)
    pt = w.POINT(0, 0); user32.ClientToScreen(hwnd, ctypes.byref(pt))
    print("CLIENT_ORIGIN", pt.x, pt.y)
print("PID", p.pid)
