import ctypes, ctypes.wintypes as w, os, subprocess, sys, time, struct
user32 = ctypes.WinDLL("user32", use_last_error=True)
gdi32 = ctypes.WinDLL("gdi32", use_last_error=True)
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

def grab(hwnd, path):
    rc = w.RECT(); user32.GetClientRect(hwnd, ctypes.byref(rc))
    W, H = rc.right, rc.bottom
    hdc = user32.GetDC(hwnd); mdc = gdi32.CreateCompatibleDC(hdc)
    bmp = gdi32.CreateCompatibleBitmap(hdc, W, H); gdi32.SelectObject(mdc, bmp)
    user32.PrintWindow(hwnd, mdc, 2)
    class BIH(ctypes.Structure):
        _fields_ = [("biSize", w.DWORD), ("biWidth", w.LONG), ("biHeight", w.LONG),
                    ("biPlanes", w.WORD), ("biBitCount", w.WORD), ("biCompression", w.DWORD),
                    ("biSizeImage", w.DWORD), ("biXPelsPerMeter", w.LONG), ("biYPelsPerMeter", w.LONG),
                    ("biClrUsed", w.DWORD), ("biClrImportant", w.DWORD)]
    bi = BIH(); bi.biSize = ctypes.sizeof(bi); bi.biWidth = W; bi.biHeight = -H
    bi.biPlanes = 1; bi.biBitCount = 32; bi.biCompression = 0
    buf = ctypes.create_string_buffer(W * H * 4)
    gdi32.GetDIBits(mdc, bmp, 0, H, buf, ctypes.byref(bi), 0)
    hdr = b"BM" + struct.pack("<IHHI", 14 + 40 + len(buf), 0, 0, 14 + 40)
    hdr += struct.pack("<IiiHHIIiiII", 40, W, -H, 1, 32, 0, len(buf), 0, 0, 0, 0)
    tmp = path.replace(".png", ".bmp")
    with open(tmp, "wb") as f:
        f.write(hdr); f.write(buf.raw)
    gdi32.DeleteObject(bmp); gdi32.DeleteDC(mdc); user32.ReleaseDC(hwnd, hdc)
    from PIL import Image
    Image.open(tmp).save(path)
    return W, H

if os.path.exists(LOG): os.remove(LOG)
env = dict(os.environ); env["IDE_PROBE_DRAG"] = "1"; env["IDE_PROBE_HOLD"] = "1"; env["IDE_DRAG_LOG"] = LOG
p = subprocess.Popen([APP], cwd=ENVDIR, env=env)
hwnd = find_window()
print("HWND", hwnd)
time.sleep(2.6)                       # 等探针跑完
W, H = grab(hwnd, OUT)
print("GRAB", W, H)
p.kill()
if os.path.exists(LOG):
    print("LOG:")
    for line in open(LOG, encoding="utf-8", errors="replace").read().strip().split("\n")[-4:]:
        print("  ", line)

# 检测彩条：数饱和度高的像素
from PIL import Image
im = Image.open(OUT).convert("RGB")
px = im.load()
bad = 0
for y in range(0, im.height, 3):
    for x in range(0, im.width, 3):
        r, g, b = px[x, y]
        mx, mn = max(r, g, b), min(r, g, b)
        if mx - mn > 60 and mx > 100:
            bad += 1
print("高饱和像素采样数:", bad, "总采样:", (im.width // 3) * (im.height // 3))
