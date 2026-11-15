import ctypes, ctypes.wintypes as w, os, subprocess, time
from PIL import ImageGrab, Image
user32 = ctypes.WinDLL("user32", use_last_error=True)
APP = r"<仓库根>\环境\ide.exe"
ENVDIR = os.path.dirname(APP)
OUT = r"%USERPROFILE%\AppData\Local\hermes\cache\scratch\probe_fast.png"
LOG = r"%USERPROFILE%\AppData\Local\hermes\cache\scratch\pd3.txt"

def find_window(timeout=10.0):
    end = time.time() + timeout
    while time.time() < end:
        found = []
        @ctypes.WINFUNCTYPE(w.BOOL, w.HWND, w.LPARAM)
        def cb(h, l):
            if not user32.IsWindowVisible(h):
                return True
            n = user32.GetWindowTextLengthW(h)
            if n:
                buf = ctypes.create_unicode_buffer(n + 1)
                user32.GetWindowTextW(h, buf, n + 1)
                if "易语言" in buf.value:
                    found.append(h)
            return True
        user32.EnumWindows(cb, 0)
        if found:
            return found[0]
        time.sleep(0.2)
    return None

if os.path.exists(LOG):
    os.remove(LOG)
env = dict(os.environ)
env["IDE_PROBE_DRAG"] = "1"
env["IDE_PROBE_HOLD"] = "1"
env["IDE_DRAG_LOG"] = LOG
p = subprocess.Popen([APP], cwd=ENVDIR, env=env)
hwnd = find_window()
print("HWND", hwnd)
if hwnd:
    user32.SetForegroundWindow(hwnd)
time.sleep(2.8)                     # 探针在 0.9s 触发，40 帧跑完极快
if hwnd:
    r = w.RECT()
    user32.GetWindowRect(hwnd, ctypes.byref(r))
    im = ImageGrab.grab(bbox=(r.left, r.top, r.right, r.bottom))
    im.save(OUT)
    print("GRAB", im.size, r.left, r.top)
p.kill()

if os.path.exists(LOG):
    print("LOG:")
    for line in open(LOG, encoding="utf-8", errors="replace").read().strip().split("\n")[-4:]:
        print("  ", line)

im = Image.open(OUT).convert("RGB")
px = im.load()
bad = 0
for y in range(0, im.height, 3):
    for x in range(0, im.width, 3):
        r_, g_, b_ = px[x, y]
        mx, mn = max(r_, g_, b_), min(r_, g_, b_)
        if mx - mn > 60 and mx > 100:
            bad += 1
print("高饱和像素采样:", bad, "/", (im.width // 3) * (im.height // 3))
