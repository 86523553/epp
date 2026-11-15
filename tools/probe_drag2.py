import ctypes, ctypes.wintypes as w, os, subprocess, time
from PIL import ImageGrab, Image
user32 = ctypes.WinDLL("user32", use_last_error=True)
APP = r"<仓库根>\环境\ide.exe"
ENVDIR = os.path.dirname(APP)
OUT = r"%USERPROFILE%\AppData\Local\hermes\cache\scratch\probe_drag2.png"
LOG = r"%USERPROFILE%\AppData\Local\hermes\cache\scratch\pd4.txt"

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
            ww, hh = r.right - r.left, r.bottom - r.top
            if ww > 600 and hh > 400 and r.left > -10000:
                best = (h, (r.left, r.top, r.right, r.bottom))
            return True
        user32.EnumWindows(cb, 0)
        if best:
            return best
        time.sleep(0.25)
    return (None, None)

if os.path.exists(LOG): os.remove(LOG)
env = dict(os.environ)
env.update({"IDE_PROBE_DRAG": "1", "IDE_PROBE_HOLD": "1", "IDE_DRAG_LOG": LOG})
p = subprocess.Popen([APP], cwd=ENVDIR, env=env)
hwnd, rect = find_main()
print("HWND", hwnd, rect)
if hwnd:
    user32.SetForegroundWindow(hwnd)
time.sleep(2.2)
if hwnd:
    im = ImageGrab.grab(bbox=rect)
    im.save(OUT)
    print("GRAB", im.size)
p.kill()
if os.path.exists(LOG):
    print("LOG:")
    for line in open(LOG, encoding="utf-8", errors="replace").read().strip().split("\n")[-3:]:
        print("  ", line)
if hwnd:
    im = Image.open(OUT).convert("RGB")
    px = im.load()
    bad = 0
    for y in range(0, im.height, 2):
        for x in range(0, im.width, 2):
            r_, g_, b_ = px[x, y]
            if max(r_, g_, b_) - min(r_, g_, b_) > 60 and max(r_, g_, b_) > 100:
                bad += 1
    print("高饱和采样:", bad, "/", (im.width // 2) * (im.height // 2))
