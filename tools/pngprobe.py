import os
# 取样截图像素颜色（纯标准库）
import sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from pngcrop import read_png

path = sys.argv[1]
w, h, bpp, pix = read_png(path)
stride = w * bpp
def px(x, y):
    o = y * stride + x * bpp
    return (pix[o], pix[o+1], pix[o+2])
# 命令行传入 x,y 列表
args = sys.argv[2:]
if args:
    i = 0
    while i + 1 < len(args) or i < len(args):
        x, y = int(args[i]), int(args[i+1])
        r, g, b = px(x, y)
        print(f'({x},{y}) = #{r:02X}{g:02X}{b:02X}')
        i += 2
else:
    # 自动扫描：打印每行第一段非白非黑区域的颜色统计
    from collections import Counter
    cnt = Counter()
    for y in range(0, h, 3):
        for x in range(0, w, 3):
            r, g, b = px(x, y)
            if (r, g, b) != (255, 255, 255) and (r, g, b) != (0, 0, 0):
                cnt[(r//8*8, g//8*8, b//8*8)] += 1
    for c, n in cnt.most_common(30):
        print(f'#{c[0]:02X}{c[1]:02X}{c[2]:02X}  x{n}')
