"""外部夹具 v2：按进程 PID 锁定我们自己的 ide.exe 窗口（绝不碰真实的易语言窗口）。
真实鼠标拖动 + PrintWindow 抓图 + 读拖动日志。
"""
import ctypes, ctypes.wintypes as w, os, subprocess, sys, time, glob, shutil
from PIL import Image

user32 = ctypes.WinDLL("user32", use_last_error=True)
gdi32 = ctypes.WinDLL("gdi32", use_last_error=True)
APP = r"<仓库根>\环境\ide.exe"
ENVDIR = r"<仓库根>\环境"
SCRATCH = r"%USERPROFILE%\AppData\Local\hermes\cache\scratch"

def find_project():
    pats = [r"桌面\*.e",
            r"<仓库根>\*.e"]
    cands = []
    for pat in pats:
        for c in glob.glob(pat):
            try:
                if os.path.getsize(c) > 1000:
                    cands.append(c)
            except OSError:
                pass
    # 优先用带控件的那个工程（行为登录窗口）
    pref = [c for c in cands if "新建 易程序 (2)" in c]
    if pref:
        cands = pref + [c for c in cands if c not in pref]
    if not cands:
        return None
    # 拷到 ASCII 路径，避免中文路径/参数出问题
    dst = os.path.join(os.environ.get("TEMP", "."), "proj_test.e")
    shutil.copyfile(cands[0], dst)
    print("SRC", cands[0], "->", dst)
    return dst

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
            if not user32.IsWindowVisible(h):
                return True
            p = w.DWORD()
            user32.GetWindowThreadProcessId(h, ctypes.byref(p))
            if p.value != pid:
                return True
            r = w.RECT(); user32.GetWindowRect(h, ctypes.byref(r))
            if (r.right - r.left) > 600 and (r.bottom - r.top) > 400:
                found.append(h)
            return True
        user32.EnumWindows(cb, 0)
        if found:
            return found[0]
        time.sleep(0.25)
    return None

def capture(hwnd, path, tries=6):
    rc = w.RECT(); user32.GetClientRect(hwnd, ctypes.byref(rc))
    W, H = rc.right, rc.bottom
    src = user32.GetDC(hwnd)
    mem = gdi32.CreateCompatibleDC(src)
    bmp = gdi32.CreateCompatibleBitmap(src, W, H)
    gdi32.SelectObject(mem, bmp)
    user32.PrintWindow(hwnd, mem, 2)
    bi = BIH(); bi.biSize = ctypes.sizeof(bi); bi.biWidth = W; bi.biHeight = -H
    bi.biPlanes = 1; bi.biBitCount = 32; bi.biCompression = 0
    buf = ctypes.create_string_buffer(W * H * 4)
    n = gdi32.GetDIBits(mem, bmp, 0, H, buf, ctypes.byref(bi), 0)
    gdi32.DeleteObject(bmp); gdi32.DeleteDC(mem); user32.ReleaseDC(hwnd, src)
    if n == 0:
        if tries > 1:
            time.sleep(0.4)
            return capture(hwnd, path, tries - 1)
        raise RuntimeError("GetDIBits 失败")
    Image.frombuffer("RGBA", (W, H), buf.raw, "raw", "BGRA", 0, 1).convert("RGB").save(path)
    return W, H

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
    proj = find_project()
    print("PROJECT", proj)
    args = [APP] + ([proj] if proj else [])
    p = subprocess.Popen(args, cwd=ENVDIR, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    hwnd = window_of_pid(p.pid)
    print("HWND", hwnd, "PID", p.pid)
    if not hwnd:
        print("NO_WINDOW"); p.kill(); return
    # 强制置顶 + 抢前台（不然鼠标甩到别的窗口去了）
    user32.ShowWindow(hwnd, 9)                       # SW_RESTORE
    user32.SetWindowPos(hwnd, -1, 0, 0, 0, 0, 0x0001 | 0x0002 | 0x0040)   # HWND_TOPMOST
    fg = user32.GetForegroundWindow()
    if fg != hwnd:
        tid_fg = user32.GetWindowThreadProcessId(fg, None)
        tid_me = ctypes.windll.kernel32.GetCurrentThreadId()
        user32.AttachThreadInput(tid_me, tid_fg, True)
        user32.SetForegroundWindow(hwnd)
        user32.BringWindowToTop(hwnd)
        user32.AttachThreadInput(tid_me, tid_fg, False)
    time.sleep(2.5)
    print("FOREGROUND_OK", user32.GetForegroundWindow() == hwnd)
    pre = os.path.join(SCRATCH, f"{tag}_pre.png")
    W, H = capture(hwnd, pre)
    print("PRE", W, H)

    # 找白色窗体包围盒
    im = Image.open(pre); px = im.load()
    minx, miny, maxx, maxy = W, H, 0, 0
    for y in range(0, H, 2):
        for x in range(0, W, 2):
            r_, g_, b_ = px[x, y]
            if r_ > 250 and g_ > 250 and b_ > 250:
                minx = min(minx, x); miny = min(miny, y)
                maxx = max(maxx, x); maxy = max(maxy, y)
    print("WHITE_BBOX", minx, miny, maxx, maxy)

    pt = w.POINT(0, 0); user32.ClientToScreen(hwnd, ctypes.byref(pt))
    ox, oy = pt.x, pt.y
    # 组件位置：窗口属性里按钮1 在 (120,168) 88x28，窗体标题栏 ~30
    if len(sys.argv) >= 5:
        ccx, ccy = int(sys.argv[2]), int(sys.argv[3])
        ddx, ddy = int(sys.argv[4]), (int(sys.argv[5]) if len(sys.argv) > 5 else 40)
        cx = ox + ccx
        cy = oy + ccy
    else:
        ccx = ccy = 0
        ddx, ddy = 160, 50
        cx = ox + minx + 120 + 44
        cy = oy + miny + 30 + 168 + 14
    print("DRAG_FROM", cx, cy)
    log = os.path.join(os.environ.get("TEMP", "."), "ide_drag.log")

    def log_text():
        try:
            return open(log, encoding="utf-8", errors="replace").read()
        except OSError:
            return ""

    def try_drag(dx0, dy0, ddx, ddy, tagx):
        nonlocal cx, cy
        x0, y0 = cx + dx0, cy + dy0
        open(log, "w").close()
        user32.SetCursorPos(x0, y0); time.sleep(0.25)
        send(0x0002); time.sleep(0.2)
        txt = log_text()
        if "drag=1" not in txt and "drag=2" not in txt:
            # 没点到组件（框选/无效），换个点再试
            send(0x0004); time.sleep(0.15)
            return None
        steps = 24
        for i in range(1, steps + 1):
            user32.SetCursorPos(x0 + int(ddx * i / steps), y0 + int(ddy * i / steps))
            time.sleep(0.012)
        time.sleep(0.35)
        mid = os.path.join(SCRATCH, f"{tagx}_mid.png")
        capture(hwnd, mid)
        send(0x0004)
        time.sleep(0.5)
        capture(hwnd, os.path.join(SCRATCH, f"{tagx}_post.png"))
        return mid

    offsets = [(0, 0), (0, -12), (0, 12), (-24, 0), (24, 0), (0, -24), (0, 24)]
    mid = None
    for (ox2, oy2) in offsets:
        mid = try_drag(ox2, oy2, ddx, ddy, tag)
        if mid:
            print("HIT_OFFSET", ox2, oy2)
            break
    if mid:
        print("IMG", mid)
    else:
        print("NO_HIT —— 所有候选点都没点到组件")
    p.kill()
    print("IMG", mid)

main()
