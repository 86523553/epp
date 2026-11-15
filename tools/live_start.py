import ctypes, ctypes.wintypes as w, os, subprocess, time
from PIL import ImageGrab
user32 = ctypes.WinDLL("user32", use_last_error=True)
APP = r"<仓库根>\环境\ide.exe"
ENVDIR = os.path.dirname(APP)
OUT = r"%USERPROFILE%\AppData\Local\hermes\cache\scratch\live_start.png"
LOG = r"%USERPROFILE%\AppData\Local\hermes\cache\scratch\pd5.txt"

def find_main(timeout=12.0):
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

if os.path.exists(LOG): os.remove(LOG)
env = dict(os.environ); env["IDE_DRAG_LOG"] = LOG
p = subprocess.Popen([APP], cwd=ENVDIR, env=env)
hwnd, rect = find_main()
print("HWND", hwnd, rect)
if hwnd:
    user32.SetForegroundWindow(hwnd)
    time.sleep(1.2)
    user32.PostMessageW(hwnd, 0x0100, 0x1B, 0)      # VK_ESCAPE：关掉启动向导
    time.sleep(0.6)
    im = ImageGrab.grab(bbox=rect)
    im.save(OUT)
    print("GRAB", im.size)
    pt = w.POINT(0, 0); user32.ClientToScreen(hwnd, ctypes.byref(pt))
    print("CLIENT_ORIGIN", pt.x, pt.y)
print("PID", p.pid)
