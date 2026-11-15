import ctypes, ctypes.wintypes as w, subprocess, time, os
user32 = ctypes.WinDLL("user32", use_last_error=True)
APP = r"<仓库根>\环境\ide.exe"
ENVDIR = r"<仓库根>\环境"
E = r"<仓库根>\新建 易程序 (2).e"
print("exists:", os.path.exists(E))
for args in ([APP, E], [APP], [APP, "/c", E]):
    p = subprocess.Popen(args, cwd=ENVDIR)
    time.sleep(3.2)
    titles = []
    @ctypes.WINFUNCTYPE(w.BOOL, w.HWND, w.LPARAM)
    def cb(h, l):
        if not user32.IsWindowVisible(h):
            return True
        n = user32.GetWindowTextLengthW(h)
        if n:
            b = ctypes.create_unicode_buffer(n + 1)
            user32.GetWindowTextW(h, b, n + 1)
            if "易语言" in b.value:
                titles.append(b.value)
        return True
    user32.EnumWindows(cb, 0)
    print(args[1:], "->", titles)
    p.kill()
    time.sleep(0.5)
