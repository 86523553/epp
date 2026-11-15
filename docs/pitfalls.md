# 已知坑与踩坑记录（边干边加，一条一行，写清「现象 → 原因 → 对策」）

> 规则：**踩到就记，别攒着**。同一个坑第二次踩到 → 说明这条记录写得不够具体。

## 环境 / 工具

| # | 现象 | 原因 | 对策 |
|---|---|---|---|
| E1 | 往含中文的路径写文件，命令行工具报 `client returned ERROR on write` / `No such file` | 原生 exe（curl/7z 等）从 MSYS bash 收参数时编码被打乱 | 先写到纯 ASCII 临时目录（`%LOCALAPPDATA%\Temp`），再用 bash `mv` 移到中文目录；Python 直接用 Unicode 路径没问题 |
| E2 | 脚本里用 `$TMPDIR` 找不到文件 | 前台 bash 与后台进程的 `$TMPDIR` 取值不一致 | 后台任务里一律写**绝对路径**（`C:/Users/.../`），别依赖环境变量 |
| E3 | 解包 1.3 万个图标的官方包很慢 | 杀软逐文件扫描 | 一次性解完就够，别反复解压；用 Python `zipfile`（实测 7.4 秒）比命令行 7z 更快 |
| E5 | PowerShell 5.1 跑「PowerShell 工具写入的 .ps1」时，中文路径参数变空、`WriteAllText` 报「空路径名是非法的」 | 本仓库写出的 .ps1 是 **UTF-8 无 BOM**，PS 5.1 按 ANSI 解码，中文默认值被解析坏 | 探针脚本一律**纯 ASCII**（注释也用英文），产物先写 `$env:TEMP` 再 `Copy-Item` 进中文目录 |

## 易语言实测取证（2026-11-06，5.95 加密狗版）

| # | 现象 | 原因 | 对策 |
|---|---|---|---|
| L1 | 按文档 §13-A 导菜单树，`GetMenu` 返回 0 条 | 易语言菜单是**自绘**在 `Afx:400000:b` 客户区里的，没有标准菜单句柄 | 菜单结构只能靠截图 + OCR/肉眼，或从 GBK 字符串与资源里取证；不要指望 `GetMenuString` |
| L2 | 组件箱不在左边 | 实测在**右侧**：`窗口组件箱` x=1846 w=131，内部 128 个 `Button` 28×28 排 4 列，无文字标签 | 本产品按 §1.1 放左侧并加宽到 264；若用户要还原原样，改 `layout.json` 即可 |
| L3 | 工具条不只一条 | 实测三条：标准 25 / 对齐 25 / 定位 25（共 97 高），另有一条 `WTWindow` 与 MDI 客户区 | 本产品合成单行 36 + 溢出 `»`（§6.1） |
| L4 | 底部面板高度 | 实测「状态夹」272 高，含 `SysTabControl32` + 树/列表/编辑框多套 | 本产品输出面板 200，页签条置底（§21.3） |

## 官方图标库（`assets/icons/`）

| # | 现象 | 原因 | 对策 |
|---|---|---|---|
| I1 | 找不到 `NewFile` / `SearchInFiles` / `Properties` / `SolutionExplorer` | 微软命名与我以为的不一样 | 真名是 `New` / `Search`+`FindInFile` / `PropertiesFolderClosed` / `SolutionExplorerViews`；**先在 `vs/…Image Library.html` 里搜**，别猜 |
| I2 | `SendToBack` 找不到 | 微软自己的笔误 | 实际文件名 `SendtoBack`（小写 t） |
| I3 | 浅色主题没有对应图标 | 官方只有 `DarkTheme`，浅色只有 `PhoneLightTheme` | 见 `open-questions.md` Q2 |
| I4 | 以为「易语言控件图标」得自绘 | 官方包里带整套 WinForms 控件图标 | 用 `Button` / `TextBox` / `Label` / `ComboBox` / `ListBox` / `GroupBox` / `Tab` / `Timer` / `Image` / `ProgressBar` / `TreeView` / `DataGrid` / `Panel` / `CheckBoxChecked` / `WindowsForm` / `UserControl` / `Component` / `Dialog`，零自绘 |

## D2D / DPI（开工后按实际踩到的补）

| # | 现象 | 原因 | 对策 |
|---|---|---|---|
| D1 | 1px 线变粗或消失 | DIP 未按物理像素对齐 | 线宽 1.0f，边界 `round(pos*scale)`；必要时 `D2D1_ANTIALIAS_MODE_ALIASED` |
| D2 | 设备丢失后白屏 | 未处理 `D2DERR_RECREATE_TARGET` / `DXGI_ERROR_DEVICE_REMOVED` | 统一 `DeviceResources::HandleDeviceLost()` 重建画笔/位图/swapchain |
| D3 | 改字号后行高不更新 | 文字度量缓存 key 少了字体/字号 | key 必须含 `(族,字号,字重,行高, dpiScale, 区域)` |

## 流程 / 协作

| # | 现象 | 原因 | 对策 |
|---|---|---|---|
| P1 | 一口气写完再给用户看，被打回 | 用户要求 Part-by-Part | 一次只做一个 Part，交付后停下等确认 |
| P2 | AI 说「做完了」但用户看不出 | 缺可验证凭据 | 每 Part 必交：四档 DPI 截图 + `ui.*` 调用实际输出 + `bridge.log` 片段 |
| P3 | 交接后新 AI 不知道做到哪 | 台账没更新 | 每完成一项立刻改 `progress.md`；交接前填 `handoff.md` |

## GDI+ 与 DPI 缩放（2026-11-08 实际踩到）

| # | 现象 | 原因 | 对策 |
|---|---|---|---|
| G1 | 150% 缩放下网格只铺满窗体 2/3，窗体拉宽后"只有半截有网格" | 网格/标题栏按 `cw*zoom` 画，窗体本体按 `cw*zoom*g_s` 画，**漏乘 `g_s`** | 画布内一切 DIP→像素换算统一走 `S(v*z)`，不要手写 `v*z` |
| G2 | 点阵根本没画出来（只剩主线交点处的深色点） | 点阵用 `SetPixelV`，同一 DC 上还有 GDI+ 在画，GDI 的像素被 GDI+ 缓冲覆盖 | 点阵也用 `fill(dc, RECT{px,py,px+1,py+1}, c)`，与主线走同一条 GDI+ 路径 |
| G3 | 截图自验一直"通过"，真机一直"不对" | 自验渲染跑在 `g_s=1.0` | 自验必须带 `--dpi 150`（`g_s=1.5`）跑一遍，再和 100% 对比 |
| G4 | 网格大小设 16 看着还是 32 | 主线间距写死 32，与 `form.grid` 脱钩 | 主线 = `grid*4`（下限 16），随设置走 |
| G5 | 新建/重置设计窗口后网格设置被冲掉 | `form_starter()` 里硬编码 `grid=8 / on / snap` | `form_starter()` 从 ini 读 `grid_size` / `grid` / `snap`；设置应用时立即落盘 |
| G6 | 拖设置面板一卡一卡 | 每帧整窗重绘（画布+左栏+提示表格+壁纸） | 拖动期间贴静态层缓存，只重画设置面板 |
| G7 | 设置面板压在提示区上时按钮点不动 | 提示表格的点击判定排在设置面板之前，把点击吃掉 | 设置面板打开时提示表格让位（`!g_settingsOpen`） |
| G8 | 自绘"输入框"难用（没光标、不能点选/全选） | 自绘假框 | 需要输入的地方用原生 `EDIT` 子窗口，`EM_SETRECTNP` 竖直居中，`WM_CTLCOLOREDIT` 跟主题 |
| G9 | 拖动窗体/控件时图形跑得比鼠标快（拉大后鼠标已经不在右下角） | 位移换算除以 `zoom`，但鼠标位移是像素，还差 `g_s` | 一律 `/ (zoom * g_s)`；改尺寸时的平移基准同理 `tdx*z*g_s` |
| G10 | 网格看着像表格，跟易语言不一致 | 画了横竖主线 | 易语言是**网点**：只有点阵，无主线；且"网点间隔"宽高分开（`form.grid` / `form.gridY`），设置项就叫"网点间隔-宽/高"、"显示网点"、"对齐到网点" |
| G11 | 拖组件「拖快了跟不上鼠标」 | 拖动期间每帧整窗重绘（实测 28.2 ms/帧），鼠标消息还会积压 | **背景底图缓存**：按下时让被拖组件隐身、把画布画一次抓成底图；每帧只 `BitBlt` 底图 + 画被拖的那个控件（实测 **0.21 ms/帧**，2400×1500 画布，快 130 倍）；配合背缓冲复用 + `PeekMessage(WM_MOUSEMOVE)` 合并积压移动。虚影方案（易语言那样甩虚线框）实测用户不接受，作废 |
| G12 | 设计器画的窗口比运行时窗口大一圈（150% 缩放） | 设计器按 DIP 画（880 DIP×1.5=1320 px），生成的程序把 880 当像素且没声明 DPI 感知 | codegen 里加 `SetProcessDPIAware()`，客户区 `kClientW * g_kSc`，控件坐标与字体同乘 `g_kSc`（`g_kSc = GetDeviceCaps(LOGPIXELSX)/96`） |
| G13 | 拖动时旧位置残留一个组件（残影） | 抓底图的顺序错了：底图必须是"已经隐藏被拖组件"的那一帧，否则底图里就带着组件本体；另外选中框要在同一帧一起跳过，否则旧位置留一圈蓝框 | 顺序固定为 `g_hideDragCtl=true` → `InvalidateRect(canvas)` → `UpdateWindow()` → `cvs_base_capture()`；`paint_ctl_layer` 里本体和选中框都要按 `g_hideDragCtl` 跳过。自验手段：`IDE_TEST_DRAG=1 ./ide.exe --screenshot x.png` 会另存一张 `drag_mid.png`（模拟拖动中那一帧）|
| G14 | 组件能被拖到窗体外面 | 拖动时没做边界钳制 | 拖动/改大小都按窗体客户区 `[0, cw-w] × [0, (ch-caption)-h]` 钳制，吸附用 `grid`×`gridY` 分别算 |

## 设计器拖动跟手（2026-11-09，五轮才收敛）

> 完整分析见 **`文档/坑-设计器拖动跟手.md`**（现象 → 根因 → 五轮弯路 → 现行实现 → 定位方法）。

| # | 现象 | 原因 | 对策 |
|---|---|---|---|
| DR1 | 拖组件时控件**按比例**落后鼠标（鼠标走 100px，控件走 67px），拖越快差越多 | `ctl_rect()` 位置只乘 `zoom`、尺寸乘 `zoom*g_s`，两套系数 | 位置/尺寸/标题条统一 `k = zoom*g_s`；**100% 缩放下此 bug 不可见**，自验必须带 `--dpi 150` |
| DR2 | 「拖到某个位置就再也拖不动了」 | 拖动过程每帧把位移夹进窗体边界 | 拖动中 1:1 自由跟随，松手时才吸网点 + 收边 |
| DR3 | 快速甩动时控件差几十像素 | 用的是消息里已过期的 `lParam` 坐标 | 吸空 `WM_MOUSEMOVE` 队列后 `GetCursorPos + ScreenToClient` |
| DR4 | 拖动一顿一顿 | 拖动帧里混进整窗精绘（实测 18.9ms） | 拖动期 `g_canvasFastDrag` 恒真，只画画布；壁纸松手后补 |
| DR5 | 框选一堆再拖，只有一个跟着走 | 只处理 `sel[0]` | 按下快照 `dragSnapCtl` + 整组同一总位移 |
| DR6 | 拖动"晚一两帧才动" | GDI 批量提交 | 上屏后 `GdiFlush()` |
| DR7 | 以为"慢"是画得慢，反复优化渲染无效 | 没量就改 | 先分段计时：整窗 18.9ms / 画布 0.34ms / 单控件 0.07ms → 画不慢，是算错了 |

## DPI 感知（2026-11-09 升级为 Per-Monitor V2）

| # | 现象 | 原因 | 对策 |
|---|---|---|---|
| D4 | 运行中改 Windows 缩放 / 把窗口拖到另一块屏，界面发虚、比例不对，必须重启 | 只调了 `SetProcessDPIAware()`（系统级），`g_dpiScale` 启动算一次，之后被 DWM 位图拉伸 | 升级 PMv2（`SetProcessDpiAwarenessContext(-4)`，GetProcAddress 动态取）+ 处理 `WM_DPICHANGED`：重算 `g_s` → `fonts_rebuild()` → 清字宽缓存 `g_logDx` → 按建议矩形 `SetWindowPos` |
| D5 | 多屏不同 DPI 时，副屏上算出的布局尺寸不对 | 初始 DPI 用 `GetDC(NULL)`，永远给主屏/系统 DPI | `MonitorFromPoint` + `CreateDC("DISPLAY", 设备名)` + `GetDeviceCaps(LOGPIXELSX)` |
| D6 | 改分辨率后窗口一半在屏幕外 | 只在启动时把窗口钳进工作区 | `WM_DISPLAYCHANGE`：只收不放（只有超出工作区才挪/缩），然后重绘 |

## UI 自绘与输入法定位（2026-11-10）

| # | 现象 | 原因 | 对策 |
|---|---|---|---|
| G15 | 修改了输入框的圆角/背景绘制，界面上依然是四四方方的系统原生框 | 底层依然有残留的原生 `EDIT` 子窗口（`CreateWindowExW(L"EDIT", ...)`），它覆盖在了自绘背景之上 | 彻底废弃原生 EDIT 子窗口。用纯 GDI/GDI+ 绘制输入框，手动接管光标闪烁、键盘字符输入、全选高亮状态与焦点管理 |
| G16 | 输入法跨输入框时“首字闪烁”：在 A 框打字切到 B 框，打第一个字时输入法先在 A 框闪一下再瞬移到 B 框 | IME 是独立进程，它默认记得上一次的位置；如果在输入时才通过 `WM_IME_COMPOSITION` 改坐标，这就晚了一帧 | 在鼠标点击输入框（`WM_LBUTTONDOWN`）的那一刻，封装 `ime_update_pos()` 强制调用 `ImmSetCompositionWindow` 和 `ImmSetCandidateWindow` 更新坐标。先于输入法唤醒，彻底杜绝首字闪烁 |
| G17 | 每次画按钮/输入框都要手写几十行 `fillR`、`outlineR`、`tx`，代码极度冗余且容易漏状态 | 没有抽离基础 UI 组件，硬编码在业务逻辑中 | 抽取通用渲染函数（如 `ui_button` 统一样式与 hover 逻辑；`wiz_field` 统一处理输入框逻辑），以后任何地方需要直接用通用函数调用 |
