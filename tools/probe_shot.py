import ctypes, ctypes.wintypes as w, os, subprocess, sys, time, struct
user32 = ctypes.WinDLL("user32", use_last_error=True)
gdi32 = ctypes.WinDLL("gdi32", use_last_error=True)
APP = r"<仓库根>\环境\ide.exe"
ENVDIR = os.path.dirname(APP)

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
                r = w.RECT(); user32.GetWindowRect(h, ctypes.byref(r))
                if (r.right - r.left) > 400 and "易语言" in buf.value:
                    found.append((h, buf.value, r))
            return True
        user32.EnumWindows(cb, 0)
        if found:
            return found[0]
        time.sleep(0.25)
    return (None, None, None)

def grab(hwnd, path):
    rc = w.RECT(); user32.GetClientRect(hwnd, ctypes.byref(rc))
    W, H = rc.right, rc.bottom
    hdc = user32.GetDC(hwnd)
    mdc = gdi32.CreateCompatibleDC(hdc)
    bmp = gdi32.CreateCompatibleBitmap(hdc, W, H)
    gdi32.SelectObject(mdc, bmp)
    ok = user32.PrintWindow(hwnd, mdc, 2)
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
    with open(path, "wb") as f:
        f.write(hdr); f.write(buf.raw)
    gdi32.DeleteObject(bmp); gdi32.DeleteDC(mdc); user32.ReleaseDC(hwnd, hdc)
    return ok, W, H

if __name__ == "__main__":
    p = subprocess.Popen([APP], cwd=ENVDIR)
    hwnd, title, r = find_window()
    if not hwnd:
        print("NO_WINDOW"); p.kill(); sys.exit(1)
    print("HWND", hwnd, "|", title)
    print("WINRECT", r.left, r.top, r.right, r.bottom)
    time.sleep(0.6)
    ok, W, H = grab(hwnd, r"%USERPROFILE%\AppData\Local\hermes\cache\scratch\probe_win.bmp")
    print("GRAB", ok, W, H)
    from PIL import Image
    Image.open(r"%USERPROFILE%\AppData\Local\hermes\cache\scratch\probe_win.bmp").save(
        r"%USERPROFILE%\AppData\Local\hermes\cache\scratch\probe_win.png")
    # 客户区左上角的屏幕坐标
    pt = w.POINT(0, 0)
    user32.ClientToScreen(hwnd, ctypes.byref(pt))
    print("CLIENT_ORIGIN", pt.x, pt.y)
    p.kill()
