# 图标（占位素材）

这里的 91 个 16×16 PNG 是**自制占位图标**（对应 `src/main.cpp` 里 96 条 `ICON_MAP` 映射，
文件名重复的共用一个图），用来让编译出来的界面不至于一片黑块。

原项目的这套图标取自某商业图标库（VS 2026 主题那套），有版权，不能随开源仓库分发，因此换成了本目录这批。

## 想换图标

文件名与 `src/main.cpp` 里 `ICON_MAP` 的第二个字段一一对应（`{ "逻辑名", "文件名" }`），
所以只要**同名覆盖**即可，代码不用改：

```
assets/icons/Run.png       ← cmd.run       播放/运行
assets/icons/Save.png      ← cmd.save      保存
assets/icons/AlignLeft.png ← align.left    左对齐
...
```

要求：16×16 PNG，带透明通道。深色主题下图标会被自动提亮（`icon_lift()`），所以原图用深灰单色即可。

## 想整套重新生成

```bash
python tools/gen_placeholder_icons.py
```

会从 `src/main.cpp` 解析 `ICON_MAP` 并重画全部图标（需要 Pillow）。
