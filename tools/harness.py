"""外部测试夹具：不碰 IDE 源码，靠 Win32 真实输入 + PrintWindow 抓图验证拖动手感/残影。
用法：python harness.py [截图前缀]
"""
import ctypes, ctypes.wintypes as w, os, subprocess, sys, time
from PIL import Image

user32 = ctypes.WinDLL("user32", use_last_error=True)
gdi32 = ctypes.WinDLL("gdi32", use_last_error=True)
APP = r"<仓库根>\环境\ide.exe"
ENVDIR = r"<仓库根>\环境"
E_FILE = r"<仓库根>\新建 易程序 (2).e"
SCRATCH = r"%USERPROFILE%\AppData\Local\hermes\cache\scratch"

class BITMAPINFOHEADER(ctypes.Structure):
    _fields_ = [("biSize", w.DWORD), ("biWidth", w.LONG), ("biHeight", w.LONG),
                ("biPlanes", w.WORD), ("biBitCount", w.WORD), ("biCompression", w.DWORD),
                ("biSizeImage", w.DWORD), ("biXPelsPerMeter", w.LONG), ("biYPelsPerMeter", w.LONG),
                ("biClrUsed", w.DWORD), ("biClrImportant", w.DWORD)]

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

def capture(hwnd, path, tries=5):
    rc = w.RECT(); user32.GetClientRect(hwnd, ctypes.byref(rc))
    W, H = rc.right, rc.bottom
    src = user32.GetDC(hwnd)
    mem = gdi32.CreateCompatibleDC(src)
    bmp = gdi32.CreateCompatibleBitmap(src, W, H)
    gdi32.SelectObject(mem, bmp)
    ok = user32.PrintWindow(hwnd, mem, 2)
    bi = BITMAPINFOHEADER()
    bi.biSize = ctypes.sizeof(bi); bi.biWidth = W; bi.biHeight = -H
    bi.biPlanes = 1; bi.biBitCount = 32; bi.biCompression = 0
    buf = ctypes.create_string_buffer(W * H * 4)
    n = gdi32.GetDIBits(mem, bmp, 0, H, buf, ctypes.byref(bi), 0)
    if n == 0:
        gdi32.DeleteObject(bmp); gdi32.DeleteDC(mem); user32.ReleaseDC(hwnd, src)
        if tries > 1:
            time.sleep(0.4)
            return capture(hwnd, path, tries - 1)
        raise RuntimeError("GetDIBits 失败")
    im = Image.frombuffer("RGBA", (W, H), buf.raw, "raw", "BGRA", 0, 1).convert("RGB")
    im.save(path)
    return im

class MOUSEINPUT(ctypes.Structure):
    _fields_ = [("dx", w.LONG), ("dy", w.LONG), ("mouseData", w.DWORD),
                ("dwFlags", w.DWORD), ("time", w.DWORD), ("dwExtraInfo", ctypes.POINTER(ctypes.c_ulong))]
class INPUT(ctypes.Structure):
    _fields_ = [("type", w.DWORD), ("mi", MOUSEINPUT)]

def send(flags):
    inp = INPUT(0, MOUSEINPUT(0, 0, 0, flags, 0, None))
    user32.SendInput(1, ctypes.byref(inp), ctypes.sizeof(INPUT))

def main():
    tag = sys.argv[1] if len(sys.argv) > 1 else "h"
    p = subprocess.Popen([APP, E_FILE], cwd=ENVDIR)
    hwnd, rect = find_main()
    if not hwnd:
        print("NO_WINDOW"); p.kill(); return
    # 顶到最前（PrintWindow 不受遮挡影响，但真实输入要窗口活跃）
    user32.SetWindowPos(hwnd, -1, 0, 0, 0, 0, 0x0001 | 0x0002 | 0x0040)  # TOPMOST|NOSIZE|NOSIZE|SHOWWINDOW
    user32.SetForegroundWindow(hwnd)
    time.sleep(2.5)
    pre = os.path.join(SCRATCH, f"{tag}_pre.png")
    im = capture(hwnd, pre)
    print("PRE", im.size)

    # 找白色窗体（最大连续白块）——用它推算组件位置
    px = im.load()
    W, H = im.size
    minx, miny, maxx, maxy = W, H, 0, 0
    for y in range(0, H, 2):
        for x in range(0, W, 2):
            r_, g_, b_ = px[x, y]
            if r_ > 248 and g_ > 248 and b_ > 248:
                if x < minx: minx = x
                if y < miny: miny = y
                if x > maxx: maxx = x
                if y > maxy: maxy = y
    print("WHITE_BBOX", minx, miny, maxx, maxy)
    if maxx <= minx:
        print("NO_FORM"); capture(hwnd, os.path.join(SCRATCH, f"{tag}_nofrom.png")); p.kill(); return

    pt = w.POINT(0, 0); user32.ClientToScreen(hwnd, ctypes.byref(pt))
    ox, oy = pt.x, pt.y
    # 组件按设计坐标放：按钮(120,168) 88x28，窗体标题栏约 30
    zoom = 1.0
    cx = ox + minx + int(120 * zoom) + 44
    cy = oy + miny + 30 + int(168 * zoom) + 14
    print("DRAG_FROM", cx, cy)
    user32.SetCursorPos(cx, cy); time.sleep(0.25)
    send(0x0002); time.sleep(0.15)
    steps = 24
    for i in range(1, steps + 1):
        user32.SetCursorPos(cx + int(180 * i / steps), cy + int(60 * i / steps))
        time.sleep(0.012)
    time.sleep(0.3)
    mid = os.path.join(SCRATCH, f"{tag}_mid.png")
    im2 = capture(hwnd, mid)
    send(0x0004)
    time.sleep(0.4)
    post = os.path.join(SCRATCH, f"{tag}_post.png")
    capture(hwnd, post)
    p.kill()

    # 残影判据：拖动中那帧里，按钮区域（旧位置）应该是窗体白色/网点，不该还是按钮灰
    print("MID saved", mid)
    log = os.path.join(os.environ.get("TEMP", "."), "ide_drag.log")
    if os.path.exists(log):
        lines = open(log, encoding="utf-8", errors="replace").read().strip().split("\n")
        print("LOG 行数", len(lines))
        for l in lines[:3] + lines[-3:]:
            print("   ", l)

main()
