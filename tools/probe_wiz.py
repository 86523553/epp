"""最小复现：只点一次「项目名称」框，读日志和 EDIT 文字。"""
import ctypes, ctypes.wintypes as w, os, subprocess, time

user32 = ctypes.WinDLL("user32", use_last_error=True)
APP    = r"<仓库根>\环境\ide.exe"
ENVDIR = r"<仓库根>\环境"
LOG    = os.path.join(os.environ.get("TEMP", "."), "ide_drag.log")

def window_of_pid(pid, timeout=20.0):
    end = time.time() + timeout
    while time.time() < end:
        found = []
        @ctypes.WINFUNCTYPE(w.BOOL, w.HWND, w.LPARAM)
        def cb(h, l):
            if not user32.IsWindowVisible(h): return True
            p = w.DWORD(); user32.GetWindowThreadProcessId(h, ctypes.byref(p))
            if p.value != pid: return True
            r = w.RECT(); user32.GetWindowRect(h, ctypes.byref(r))
            if (r.right - r.left) > 600 and (r.bottom - r.top) > 400: found.append(h)
            return True
        user32.EnumWindows(cb, 0)
        if found: return found[0]
        time.sleep(0.25)
    return None

def edits_of(hwnd):
    out = []
    @ctypes.WINFUNCTYPE(w.BOOL, w.HWND, w.LPARAM)
    def cb(h, l):
        cls = ctypes.create_unicode_buffer(64); user32.GetClassNameW(h, cls, 64)
        if cls.value.upper() == "EDIT":
            r = w.RECT(); user32.GetWindowRect(h, ctypes.byref(r))
            out.append((h, r))
        return True
    user32.EnumChildWindows(hwnd, cb, 0)
    return out

def text_of(h):
    n = user32.GetWindowTextLengthW(h)
    b = ctypes.create_unicode_buffer(n + 2); user32.GetWindowTextW(h, b, n + 2)
    return b.value

p = subprocess.Popen([APP], cwd=ENVDIR, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
hwnd = window_of_pid(p.pid)
print("HWND", hwnd, "PID", p.pid)
user32.ShowWindow(hwnd, 9)
user32.SetWindowPos(hwnd, -1, 0, 0, 0, 0, 0x0001 | 0x0002 | 0x0040)
time.sleep(2.5)

rc = w.RECT(); user32.GetClientRect(hwnd, ctypes.byref(rc))
pt = w.POINT(0, 0); user32.ClientToScreen(hwnd, ctypes.byref(pt))
print("CLIENT", rc.right, rc.bottom, "ORIGIN", pt.x, pt.y)
print("启动即有 EDIT:",
      [(r.left - pt.x, r.top - pt.y, r.right - r.left, r.bottom - r.top, text_of(h)) for h, r in edits_of(hwnd)])

# 单次点击：面板里「项目名称」框中点（按 g_s=1.0 布局：横向 707+254, 纵向 607+17）
cx, cy = pt.x + 900, pt.y + 624
user32.SetCursorPos(cx, cy); time.sleep(0.2)
user32.mouse_event(0x0002, 0, 0, 0, 0); time.sleep(0.1)
user32.mouse_event(0x0004, 0, 0, 0, 0); time.sleep(0.8)
print("点击后 EDIT:",
      [(r.left - pt.x, r.top - pt.y, r.right - r.left, r.bottom - r.top,
        text_of(h), bool(user32.IsWindowVisible(h))) for h, r in edits_of(hwnd)])
print("---- log ----")
try:
    print(open(LOG, encoding="utf-8", errors="replace").read()[-800:])
except OSError as e:
    print("no log", e)
p.kill()
