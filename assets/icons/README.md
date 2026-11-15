# 界面图标

95 个文件，全部是运行时要用的**真图**（沿用开发时的原样），文件名与 `src/main.cpp` 里 `ICON_MAP`
的逻辑名一一对应，**改名或删文件会让对应按钮变空白 / 退化成程序自绘**。

几个关键的：

| 文件 | 用在哪 | 备注 |
|---|---|---|
| `epp_logo.png` | 标题栏左上角、向导页 | 400 KB 金属字 logo，**原样绘制、不跟主题重上色**（`g_logoBmp`） |
| `epp.ico` | 程序图标 / 任务栏 | — |
| `Grid.png` | 设计器网格按钮 | — |
| `BringToFront.png` / `SendtoBack.png` | 置顶 / 置底按钮 | — |
| `VisualStudio.png` | 主题 / 关于相关处 | — |

其余是工具栏、左侧树、组件箱用的 16×16 PNG。

## 版权提醒

这套图标沿用开发时的原样。其中一部分（尤其 `VisualStudio.png` 和部分工具栏图形）可能源自
Visual Studio 官方图标集，版权归 Microsoft，**不属于本项目的 Unlicense 覆盖范围**。
公开分发前请自行确认；若要彻底规避，可整套换成自制图或 CC0 图标集 —— 文件名不变，代码无需改动。

`epp.ico` 与 `epp_logo.png` 是本项目自己的素材。

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
