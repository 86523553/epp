"""拖动实测夹具（不依赖 PIL）：启动 ide.exe → 按 PID 找窗口 → PrintWindow 存 BMP →
在白窗体范围内网格试点，命中组件后真实鼠标拖动 → 读 %TEMP%\\ide_drag.log 的每帧耗时。

用法：python 工具\\drag_bench.py [tag]
"""
import ctypes, ctypes.wintypes as w, os, subprocess, sys, time, struct

user32 = ctypes.WinDLL("user32", use_last_error=True)
gdi32 = ctypes.WinDLL("gdi32", use_last_error=True)

APP = r"<仓库根>\环境\ide.exe"
ENVDIR = os.path.dirname(APP)
TMP = os.environ.get("TEMP", ".")
LOGP = os.path.join(TMP, "ide_drag.log")

SRC_E = r"桌面\新建 易程序 (2).e"
PROJ = os.path.join(TMP, "proj_test.e")


class BIH(ctypes.Structure):
    _fields_ = [("biSize", w.DWORD), ("biWidth", w.LONG), ("biHeight", w.LONG),
                ("biPlanes", w.WORD), ("biBitCount", w.WORD), ("biCompression", w.DWORD),
                ("biSizeImage", w.DWORD), ("biXPelsPerMeter", w.LONG), ("biYPelsPerMeter", w.LONG),
                ("biClrUsed", w.DWORD), ("biClrImportant", w.DWORD)]


def window_of_pid(pid, timeout=25.0):
    end = time.time() + timeout
    while time.time() < end:
        found = []

        @ctypes.WINFUNCTYPE(w.BOOL, w.HWND, w.LPARAM)
        def cb(h, l):
            if not user32.IsWindowVisible(h):
                return True
            p = w.DWORD()
            user32.GetWindowThreadProcessId(h, ctypes.byref(p))
            if p.value != pid:
                return True
            r = w.RECT()
            user32.GetWindowRect(h, ctypes.byref(r))
            if (r.right - r.left) > 600 and (r.bottom - r.top) > 400:
                found.append(h)
            return True

        user32.EnumWindows(cb, 0)
        if found:
            return found[0]
        time.sleep(0.25)
    return None


def grab(hwnd, path):
    rc = w.RECT()
    user32.GetClientRect(hwnd, ctypes.byref(rc))
    W, H = rc.right, rc.bottom
    src = user32.GetDC(hwnd)
    mem = gdi32.CreateCompatibleDC(src)
    bmp = gdi32.CreateCompatibleBitmap(src, W, H)
    gdi32.SelectObject(mem, bmp)
    user32.PrintWindow(hwnd, mem, 2)
    bi = BIH()
    bi.biSize = ctypes.sizeof(bi); bi.biWidth = W; bi.biHeight = -H
    bi.biPlanes = 1; bi.biBitCount = 32; bi.biCompression = 0
    buf = ctypes.create_string_buffer(W * H * 4)
    n = gdi32.GetDIBits(mem, bmp, 0, H, buf, ctypes.byref(bi), 0)
    gdi32.DeleteObject(bmp); gdi32.DeleteDC(mem); user32.ReleaseDC(hwnd, src)
    if n == 0:
        return None, None
    pix = memoryview(buf.raw)
    # 写成 32 位 BMP（自顶向下用负高度）
    hdr = struct.pack("<2sIHHI", b"BM", 14 + 40 + W * H * 4, 0, 0, 54)
    info = struct.pack("<IiiHHIIiiII", 40, W, -H, 1, 32, 0, W * H * 4, 2835, 2835, 0, 0)
    with open(path, "wb") as f:
        f.write(hdr); f.write(info); f.write(pix)
    return W, H, pix


def px(pix, W, x, y):
    i = (y * W + x) * 4
    return pix[i + 2], pix[i + 1], pix[i]


class MOUSEINPUT(ctypes.Structure):
    _fields_ = [("dx", w.LONG), ("dy", w.LONG), ("mouseData", w.DWORD),
                ("dwFlags", w.DWORD), ("time", w.DWORD), ("dwExtraInfo", ctypes.c_void_p)]


class INPUT(ctypes.Structure):
    _fields_ = [("type", w.DWORD), ("mi", MOUSEINPUT)]


def send(flags):
    inp = INPUT(0, MOUSEINPUT(0, 0, 0, flags, 0, None))
    user32.SendInput(1, ctypes.byref(inp), ctypes.sizeof(INPUT))


def log_text():
    try:
        return open(LOGP, encoding="utf-8", errors="replace").read()
    except OSError:
        return ""


def main():
    tag = sys.argv[1] if len(sys.argv) > 1 else "b"
    if not os.path.exists(PROJ):
        import shutil
        shutil.copyfile(SRC_E, PROJ)
    p = subprocess.Popen([APP, PROJ], cwd=ENVDIR,
                         stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    hwnd = window_of_pid(p.pid)
    print("HWND", hwnd, "PID", p.pid)
    if not hwnd:
        p.kill(); print("NO_WINDOW"); return
    user32.ShowWindow(hwnd, 9)
    user32.SetWindowPos(hwnd, -1, 0, 0, 0, 0, 0x0001 | 0x0002 | 0x0040)
    fg = user32.GetForegroundWindow()
    if fg != hwnd:
        tid_fg = user32.GetWindowThreadProcessId(fg, None)
        tid_me = ctypes.windll.kernel32.GetCurrentThreadId()
        user32.AttachThreadInput(tid_me, tid_fg, True)
        user32.SetForegroundWindow(hwnd)
        user32.BringWindowToTop(hwnd)
        user32.AttachThreadInput(tid_me, tid_fg, False)
    time.sleep(3.0)
    shot = os.path.join(TMP, f"{tag}_pre.bmp")
    W, H, pix = grab(hwnd, shot)
    print("SHOT", shot, W, H, "fg_ok", user32.GetForegroundWindow() == hwnd)

    # 白窗体 bbox（设计器里的模拟窗体是纯白）
    minx, miny, maxx, maxy = W, H, 0, 0
    for y in range(0, H, 2):
        for x in range(0, W, 2):
            r, g, b = px(pix, W, x, y)
            if r > 250 and g > 250 and b > 250:
                if x < minx: minx = x
                if y < miny: miny = y
                if x > maxx: maxx = x
                if y > maxy: maxy = y
    print("WHITE_BBOX", minx, miny, maxx, maxy)

    pt = w.POINT(0, 0)
    user32.ClientToScreen(hwnd, ctypes.byref(pt))
    ox, oy = pt.x, pt.y
    print("CLIENT_ORIGIN", ox, oy)

    # 指定客户区坐标就直接用；否则网格试点
    if len(sys.argv) >= 3:
        hit = (int(sys.argv[2]), int(sys.argv[3]))
        print("HIT(args)", hit)
    else:
        hit = None
        tried = 0
        for yy in range(miny + 20, maxy - 10, 28):
            for xx in range(minx + 20, maxx - 10, 34):
                tried += 1
                open(LOGP, "w").close()
                user32.SetCursorPos(ox + xx, oy + yy)
                time.sleep(0.03)
                send(0x0002)
                time.sleep(0.08)
                t = log_text()
                send(0x0004)
                time.sleep(0.02)
                if "drag=1" in t or "drag=2" in t:
                    hit = (xx, yy)
                    break
                if tried > 260:
                    break
            if hit:
                break
        print("HIT", hit, "tried", tried)
    if not hit:
        p.kill(); print("NO_HIT"); return

    # 真实拖动：40 步，每步 6ms
    x0, y0 = ox + hit[0], oy + hit[1]
    open(LOGP, "w").close()
    user32.SetCursorPos(x0, y0); time.sleep(0.2)
    send(0x0002); time.sleep(0.1)
    steps = 40
    ddx, ddy = 180, 60
    t_start = time.time()
    for i in range(1, steps + 1):
        user32.SetCursorPos(x0 + int(ddx * i / steps), y0 + int(ddy * i / steps))
        time.sleep(0.006)
    dt = (time.time() - t_start) * 1000
    grab(hwnd, os.path.join(TMP, f"{tag}_mid.bmp"))
    send(0x0004)
    time.sleep(0.3)
    grab(hwnd, os.path.join(TMP, f"{tag}_post.bmp"))
    txt = log_text()
    direct = [l for l in txt.splitlines() if l.startswith("DIRECT")]
    full = [l for l in txt.splitlines() if l.startswith("FULL")]
    print("DRAG_WALL %.0f ms / %d 步" % (dt, steps))
    print("DIRECT 帧数", len(direct), "样例", direct[:2], direct[-2:] if len(direct) > 2 else "")
    print("FULL 帧数", len(full), "样例", full[:2])
    nums = []
    for l in direct:
        try:
            nums.append(float(l.split()[1]))
        except Exception:
            pass
    if nums:
        nums.sort()
        print("DIRECT ms  中位 %.2f  p90 %.2f  最差 %.2f" % (
            nums[len(nums) // 2], nums[int(len(nums) * 0.9)], nums[-1]))
    print("LOG_HEAD")
    for l in txt.splitlines()[:8]:
        print("  ", l)
    p.kill()


main()
