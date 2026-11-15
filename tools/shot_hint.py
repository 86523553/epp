import ctypes, ctypes.wintypes as w, os, subprocess, time
from PIL import Image
try: ctypes.windll.shcore.SetProcessDpiAwareness(2)
except Exception: ctypes.windll.user32.SetProcessDPIAware()
u = ctypes.WinDLL("user32", use_last_error=True); g = ctypes.WinDLL("gdi32")
APP = r"<仓库根>\环境\ide.exe"
ENV = r"<仓库根>\环境"
OUT = r"%USERPROFILE%\AppData\Local\hermes\cache\scratch"

class BIH(ctypes.Structure):
    _fields_=[("biSize",w.DWORD),("biWidth",w.LONG),("biHeight",w.LONG),("biPlanes",w.WORD),
              ("biBitCount",w.WORD),("biCompression",w.DWORD),("biSizeImage",w.DWORD),
              ("biXPelsPerMeter",w.LONG),("biYPelsPerMeter",w.LONG),("biClrUsed",w.DWORD),("biClrImportant",w.DWORD)]

p = subprocess.Popen([APP], cwd=ENV, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
hwnd = None
for _ in range(60):
    found=[]
    @ctypes.WINFUNCTYPE(w.BOOL,w.HWND,w.LPARAM)
    def cb(h,l):
        if not u.IsWindowVisible(h): return True
        pid=w.DWORD(); u.GetWindowThreadProcessId(h,ctypes.byref(pid))
        if pid.value!=p.pid: return True
        r=w.RECT(); u.GetWindowRect(h,ctypes.byref(r))
        if (r.right-r.left)>600 and (r.bottom-r.top)>400: found.append(h)
        return True
    u.EnumWindows(cb,0)
    if found: hwnd=found[0]; break
    time.sleep(0.25)
time.sleep(2.5)
rc=w.RECT(); u.GetClientRect(hwnd,ctypes.byref(rc)); W,H=rc.right,rc.bottom
src=u.GetDC(hwnd); mem=g.CreateCompatibleDC(src); bmp=g.CreateCompatibleBitmap(src,W,H); g.SelectObject(mem,bmp)
u.PrintWindow(hwnd,mem,2)
bi=BIH(); bi.biSize=ctypes.sizeof(bi); bi.biWidth=W; bi.biHeight=-H; bi.biPlanes=1; bi.biBitCount=32
buf=ctypes.create_string_buffer(W*H*4)
g.GetDIBits(mem,bmp,0,H,buf,ctypes.byref(bi),0)
g.DeleteObject(bmp); g.DeleteDC(mem); u.ReleaseDC(hwnd,src)
im=Image.frombuffer("RGBA",(W,H),buf.raw,"raw","BGRA",0,1).convert("RGB")
im.save(os.path.join(OUT,"hint_full.png"))
im.crop((1000,1160,1500,1250)).resize((1000,180)).save(os.path.join(OUT,"hint_crop.png"))
print("OK",W,H)
p.kill()
