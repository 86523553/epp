#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""生成 epp 占位图标集（16x16 PNG）。

原项目的 assets/icons 用的是某商业图标库（VS 2026 那套），有版权，不能随开源仓库分发。
本脚本生成一套风格统一的自制占位图标，文件名与 src/main.cpp 里 ICON_MAP 的
file 字段一一对应 —— 把任意同名 16x16 PNG 覆盖进去，界面图标就换成你自己的。

用法：
    python tools/gen_placeholder_icons.py            # 输出到 assets/icons/
    python tools/gen_placeholder_icons.py 输出目录

依赖：Pillow（pip install pillow）
"""

import os
import re
import sys

from PIL import Image, ImageDraw

W = H = 16
EDGE = (108, 118, 140, 255)     # 描边（中性灰蓝，加深一档保证浅色主题下也清晰）
FILL = (108, 118, 140, 70)      # 内填充（半透明）
BLUE = (86, 140, 220, 255)
GREEN = (94, 176, 116, 255)
RED = (214, 92, 92, 255)
YELLOW = (222, 176, 74, 255)


def canvas():
    return Image.new("RGBA", (W, H), (0, 0, 0, 0))


def box(d, x0, y0, x1, y1, color=EDGE, fill=False):
    d.rectangle([x0, y0, x1, y1], outline=color, fill=FILL if fill else None)


def lines(d, rows, color=EDGE, x0=2, x1=13):
    for y in rows:
        d.line([x0, y, x1, y], fill=color)


# ── 各类符号 ────────────────────────────────────────────────
def ic_doc(d):                       # 文档
    d.polygon([(4, 1), (10, 1), (13, 4), (13, 14), (4, 14)], outline=EDGE)
    lines(d, [6, 9], x0=6, x1=11)


def ic_folder(d):                    # 文件夹
    d.polygon([(1, 4), (6, 4), (7, 6), (14, 6), (14, 13), (1, 13)], outline=EDGE)


def ic_save(d):                      # 软盘
    box(d, 2, 2, 13, 13, fill=True)
    d.rectangle([5, 2, 10, 6], outline=EDGE)
    d.rectangle([5, 9, 10, 13], outline=EDGE)


def ic_arrow_r(d):                   # 播放
    d.polygon([(4, 2), (13, 8), (4, 14)], fill=GREEN)


def ic_square(d):                    # 停止
    d.rectangle([4, 4, 12, 12], fill=RED)


def ic_pause(d):                     # 暂停
    d.rectangle([4, 3, 6, 13], fill=YELLOW)
    d.rectangle([10, 3, 12, 13], fill=YELLOW)


def ic_cross(d):                     # 关闭
    d.line([3, 3, 12, 12], fill=EDGE, width=2)
    d.line([12, 3, 3, 12], fill=EDGE, width=2)


def ic_undo(d, flip=False):          # 撤销/重做
    d.arc([2, 4, 13, 14], 200, 340, fill=BLUE, width=2)
    if flip:
        d.polygon([(12, 2), (15, 7), (9, 7)], fill=BLUE)
    else:
        d.polygon([(4, 2), (1, 7), (7, 7)], fill=BLUE)


def ic_copy(d):                      # 复制
    box(d, 2, 2, 9, 9)
    box(d, 6, 6, 13, 13, fill=True)


def ic_cut(d):                       # 剪切
    d.line([3, 2, 11, 11], fill=EDGE, width=2)
    d.line([13, 2, 5, 11], fill=EDGE, width=2)
    d.ellipse([2, 10, 6, 14], outline=EDGE)
    d.ellipse([10, 10, 14, 14], outline=EDGE)


def ic_paste(d):                     # 粘贴
    box(d, 3, 2, 12, 14, fill=True)
    d.rectangle([6, 1, 9, 4], outline=EDGE)


def ic_search(d):                    # 查找
    d.ellipse([2, 2, 10, 10], outline=BLUE, width=2)
    d.line([9, 9, 14, 14], fill=BLUE, width=2)


def ic_gear(d):                      # 设置
    d.ellipse([4, 4, 11, 11], outline=EDGE, width=2)
    for a in range(0, 360, 45):
        import math
        x = 7.5 + 6 * math.cos(math.radians(a))
        y = 7.5 + 6 * math.sin(math.radians(a))
        d.ellipse([x - 1, y - 1, x + 1, y + 1], fill=EDGE)


def ic_refresh(d):                   # 刷新
    d.arc([2, 2, 13, 13], 30, 320, fill=EDGE, width=2)
    d.polygon([(11, 1), (15, 5), (9, 6)], fill=EDGE)


def ic_build(d):                     # 编译 / 生成
    box(d, 3, 3, 12, 12, fill=True)
    lines(d, [6, 9], x0=6, x1=9, color=BLUE)


def ic_align(d, mode):               # 对齐
    if mode == "left":
        d.line([2, 1, 2, 14], fill=BLUE, width=1)
        lines(d, [4, 8, 12], x0=4, x1=13)
    elif mode == "center":
        d.line([7, 1, 7, 14], fill=BLUE, width=1)
        lines(d, [4, 12], x0=4, x1=11)
        lines(d, [8], x0=2, x1=13)
    else:
        d.line([13, 1, 13, 14], fill=BLUE, width=1)
        lines(d, [4, 8, 12], x0=2, x1=11)


def ic_control(d):                   # 组件（通用小控件）
    box(d, 2, 3, 13, 12, fill=True)
    d.line([2, 6, 13, 6], fill=EDGE)
    d.rectangle([2, 3, 4, 4], fill=BLUE)


def ic_panel(d):                     # 面板
    box(d, 1, 2, 14, 13, fill=True)
    d.rectangle([1, 2, 14, 5], fill=(154, 163, 181, 90))


def ic_pin(d):                       # 停靠/固定
    d.polygon([(8, 1), (11, 5), (10, 9), (6, 9), (5, 5)], outline=EDGE)
    d.line([8, 9, 8, 14], fill=EDGE, width=2)


def ic_dot(d, color):                # 状态点
    d.ellipse([4, 4, 12, 12], fill=color)


def ic_text(d, ch):                  # 兜底：一个字符
    try:
        from PIL import ImageFont
        f = ImageFont.truetype("C:/Windows/Fonts/arialbd.ttf", 11)
        d.text(((W - 7) / 2, 2), ch, font=f, fill=EDGE)
    except Exception:
        box(d, 3, 3, 12, 12)


# ── 名字 → 图形 ────────────────────────────────────────────
def draw_for(name):
    img = canvas()
    d = ImageDraw.Draw(img)
    n = name.lower()

    if n.startswith("status."):
        ic_dot(d, {"status.error": RED, "status.warning": YELLOW,
                   "status.info": BLUE}.get(n, GREEN))
    elif n.startswith("align."):
        ic_align(d, n.split(".")[-1])
    elif n.startswith("panel."):
        ic_panel(d)
    elif n.startswith("ctl."):
        ic_control(d)
    elif n in ("pin",):
        ic_pin(d)
    elif "new" in n:
        ic_doc(d); d.line([8, 7, 8, 13], fill=GREEN, width=1)
        d.line([5, 10, 11, 10], fill=GREEN, width=1)
    elif "open" in n or "folder" in n:
        ic_folder(d)
    elif "saveall" in n:
        ic_save(d); d.rectangle([1, 1, 6, 6], outline=EDGE)
    elif "save" in n:
        ic_save(d)
    elif "undo" in n:
        ic_undo(d)
    elif "redo" in n:
        ic_undo(d, flip=True)
    elif "copy" in n:
        ic_copy(d)
    elif "cut" in n:
        ic_cut(d)
    elif "paste" in n:
        ic_paste(d)
    elif "find" in n or "search" in n:
        ic_search(d)
    elif "settings" in n:
        ic_gear(d)
    elif "refresh" in n:
        ic_refresh(d)
    elif "build" in n or "generate" in n:
        ic_build(d)
    elif n.endswith(".run") or "run" == n.split(".")[-1]:
        ic_arrow_r(d)
    elif "pause" in n:
        ic_pause(d)
    elif n.endswith(".stop") or "stop" in n:
        ic_square(d)
    elif "close" in n:
        ic_cross(d)
    else:
        ch = (name.split(".")[-1] or "?")[0].upper()
        ic_text(d, ch)
    return img


def main():
    out = sys.argv[1] if len(sys.argv) > 1 else os.path.join(
        os.path.dirname(os.path.dirname(os.path.abspath(__file__))), "assets", "icons")
    os.makedirs(out, exist_ok=True)

    cpp = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))),
                       "src", "main.cpp")
    names = []
    if os.path.exists(cpp):
        text = open(cpp, encoding="utf-8", errors="ignore").read()
        block = re.search(r"ICON_MAP\[\]\s*=\s*\{(.*?)\};", text, re.S)
        if block:
            names = re.findall(r'\{\s*"([^"]+)"\s*,\s*"([^"]+)"\s*\}', block.group(1))
    if not names:
        print("没从 src/main.cpp 里解析到 ICON_MAP，退出")
        return 1

    for logical, fname in names:
        draw_for(logical).save(os.path.join(out, fname + ".png"))

    # 标题栏 logo（EPP 三字），没有就用蓝底方块兜底
    logo = canvas().resize((64, 16))
    ImageDraw.Draw(logo).rectangle([0, 0, 63, 15], outline=BLUE)
    try:
        from PIL import ImageFont
        f = ImageFont.truetype("C:/Windows/Fonts/arialbd.ttf", 12)
        ImageDraw.Draw(logo).text((6, 1), "EPP", font=f, fill=BLUE)
    except Exception:
        pass
    logo.save(os.path.join(out, "epp_logo.png"))

    print(f"生成 {len(names)} 个图标 + epp_logo.png → {out}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
