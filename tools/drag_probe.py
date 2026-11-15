import ctypes, ctypes.wintypes as w, os, subprocess, sys, time, json

user32 = ctypes.WinDLL("user32", use_last_error=True)
gdi32 = ctypes.WinDLL("gdi32", use_last_error=True)

APP = r"<仓库根>\环境\ide.exe"
ENVDIR = os.path.dirname(APP)

def find_window(timeout=8.0):
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
                if "易语言" in buf.value and "IDE" in buf.value.upper():
                    found.append((h, buf.value))
            return True
        user32.EnumWindows(cb, 0)
        if found:
            return found[0]
        time.sleep(0.2)
    return (None, None)

def grab(hwnd, path):
    rect = w.RECT()
    user32.GetClientRect(hwnd, ctypes.byref(rect))
    W, H = rect.right, rect.bottom
    hdc = user32.GetDC(hwnd)
    mdc = gdi32.CreateCompatibleDC(hdc)
    bmp = gdi32.CreateCompatibleBitmap(hdc, W, H)
    gdi32.SelectObject(mdc, bmp)
    ok = user32.PrintWindow(hwnd, mdc, 2)   # PW_RENDERFULLCONTENT
    # save via GDI+ not available; use BMP writer
    class BITMAPINFOHEADER(ctypes.Structure):
        _fields_ = [("biSize", w.DWORD), ("biWidth", w.LONG), ("biHeight", w.LONG),
                    ("biPlanes", w.WORD), ("biBitCount", w.WORD), ("biCompression", w.DWORD),
                    ("biSizeImage", w.DWORD), ("biXPelsPerMeter", w.LONG), ("biYPelsPerMeter", w.LONG),
                    ("biClrUsed", w.DWORD), ("biClrImportant", w.DWORD)]
    bi = BITMAPINFOHEADER()
    bi.biSize = ctypes.sizeof(bi); bi.biWidth = W; bi.biHeight = -H
    bi.biPlanes = 1; bi.biBitCount = 32; bi.biCompression = 0
    buf = ctypes.create_string_buffer(W * H * 4)
    gdi32.GetDIBits(mdc, bmp, 0, H, buf, ctypes.byref(bi), 0)
    # 写 BMP（32bpp BI_RGB）
    import struct
    hdr = b"BM" + struct.pack("<IHHI", 14 + 40 + len(buf), 0, 0, 14 + 40)
    hdr += struct.pack("<IiiHHIIiiII", 40, W, -H, 1, 32, 0, len(buf), 0, 0, 0, 0)
    with open(path, "wb") as f:
        f.write(hdr); f.write(buf.raw)
    gdi32.DeleteObject(bmp); gdi32.DeleteDC(mdc); user32.ReleaseDC(hwnd, hdc)
    return ok, W, H

def to_png(bmp_path, png_path):
    from PIL import Image
    Image.open(bmp_path).save(png_path)

# 鼠标（真实输入）
MOUSEEVENTF_LEFTDOWN, MOUSEEVENTF_LEFTUP = 0x0002, 0x0004
class MOUSEINPUT(ctypes.Structure):
    _fields_ = [("dx", w.LONG), ("dy", w.LONG), ("mouseData", w.DWORD),
                ("dwFlags", w.DWORD), ("time", w.DWORD), ("dwExtraInfo", ctypes.POINTER(ctypes.c_ulong))]
class INPUT(ctypes.Structure):
    _fields_ = [("type", w.DWORD), ("mi", MOUSEINPUT)]

def send(flags):
    inp = INPUT(0, MOUSEINPUT(0, 0, 0, flags, 0, None))
    user32.SendInput(1, ctypes.byref(inp), ctypes.sizeof(INPUT))

def move_to(x, y):
    user32.SetCursorPos(int(x), int(y))

if __name__ == "__main__":
    x0, y0 = int(sys.argv[1]), int(sys.argv[2])       # 起点（屏幕坐标）
    dx, dy = int(sys.argv[3]), int(sys.argv[4])       # 位移
    steps  = int(sys.argv[5]) if len(sys.argv) > 5 else 12
    out_pre, out_mid = sys.argv[6], sys.argv[7]
    env = dict(os.environ)
    env["IDE_DRAG_LOG"] = os.path.join(os.path.dirname(out_pre), "drag_log.txt")
    if os.path.exists(env["IDE_DRAG_LOG"]):
        os.remove(env["IDE_DRAG_LOG"])
    p = subprocess.Popen([APP], cwd=ENVDIR, env=env)
    hwnd, title = find_window()
    if not hwnd:
        print("NO_WINDOW"); p.kill(); sys.exit(1)
    print("HWND", hwnd, title)
    user32.SetForegroundWindow(hwnd)
    time.sleep(0.8)
    grab(hwnd, out_pre.replace(".png", ".bmp")); to_png(out_pre.replace(".png", ".bmp"), out_pre)
    move_to(x0, y0); time.sleep(0.2)
    send(MOUSEEVENTF_LEFTDOWN); time.sleep(0.1)
    for i in range(1, steps + 1):
        move_to(x0 + dx * i / steps, y0 + dy * i / steps)
        time.sleep(0.03)
    time.sleep(0.25)
    grab(hwnd, out_mid.replace(".png", ".bmp")); to_png(out_mid.replace(".png", ".bmp"), out_mid)
    send(MOUSEEVENTF_LEFTUP); time.sleep(0.2)
    p.kill()
    log = env["IDE_DRAG_LOG"]
    if os.path.exists(log):
        with open(log, encoding="utf-8", errors="replace") as f:
            lines = f.read().strip().split("\n")[-15:]
        print("LOG:")
        for l in lines:
            print(" ", l)
    print("DONE")
