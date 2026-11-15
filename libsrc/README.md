# 支持库源码（怎么封装成 IDE 能用的东西）

这个目录是 epp 用的支持库**全部来源**。支持库在 epp 里是三块东西拼起来的，别混：

| # | 块 | 在哪 | 作用 |
|---|---|---|---|
| ① | **元数据**（中文命令/属性/事件清单） | `打包源/核心支持库.elib`（文本库）+ `out_krnln/`（全量产线） | IDE 的支持库页、命令提示、属性面板全靠它 —— **只描述，不实现** |
| ② | **命令实现**（C++） | `core_support/`（17 个 `.cpp` + 19 个 `.h`，来自 su-lang，MIT） | 编成 `libcore_support.a`，用户工程编译时链接进去就是真能跑的 `Core*` 函数 |
| ③ | **组件库**（42 个控件的属性/事件声明 + 实现） | `mfc_controls/` + `mfc_sdk/` | IDE 只读头文件里的 `@component/@name/@type` 注解来画组件箱与属性页；实现那层要 MSVC + ATL，见下 |

元数据和实现**必须对齐**：照抄 688 条命令进库、而 `.a` 里只有 161 个 `Core*` 函数，结果是"看得见、编不过"。

---

## ① 元数据：`.elib` 文本库 → `.epp` 单文件包

`.elib` 是纯文本（GBK），一节一节写命令/数据类型/常量/组件。`打包源/核心支持库.elib` 就是 IDE 实际加载的那份（237 条命令 / 42 组件 / 675 属性 / 53 事件）。

打成 IDE 真正吃的单文件包（**必须 `--store`，不能压缩**）：

```bash
python tools/pack_epp.py --name 核心支持库 \
    --elib libsrc/打包源/核心支持库.elib \
    --comp-dir libsrc/out_krnln/components \
    --out libsrc/核心支持库.epp --store
```

为什么必须 stored：IDE 里读包只有 `epp_zip_entry()` 约 60 行——读 EOCD → 中央目录 → 本地头 → 按偏移直取字节，**没有实现 inflate**。这是刻意的取舍（零依赖、零解压内存）。

打出来的包内部长这样：

```
manifest.json    中文元信息（格式/版本/统计）
命令.txt          中文命令表
组件.txt          中文组件表
include/*.h       组件类声明（IDE 启动解到 %LOCALAPPDATA%\epp\lib\libN\include 给编译器用）
```

`out_krnln/` 是另一条线：`tools/gen_from_krnln.py` 从易语言支持库导出 `krnln.txt` 生成**全量**元数据（688 命令 / 710 属性 / 13 公共事件 / 295 公共方法 / 42 组件头）。`krnln.txt` 属于易语言自身的数据，不在本仓库，需要自备；没有它就用 `打包源/` 里现成的 `.elib`，够跑。

## ② 命令实现：编静态库 `libcore_support.a`

源码自包含（只 include 自己目录的头 + 标准库），**C++20**，Windows x86/x64 都行：

```bash
cd libsrc/core_support
g++ -std=c++20 -O2 -Iinclude -c src/*.cpp
ar rcs libcore_support.a *.o
```

需要额外链接这些系统库（用户在 IDE 里 F7 时会自动带上）：`ws2_32 iphlpapi winmm user32 shell32 gdi32`。

产物放 `bin/static_lib/libcore_support.a`（IDE 用 `lib_static_dir()` 找 `<exe>\static_lib`），头文件放 `bin/static_lib/include/`（已附）。

## ③ 组件库：只当"元数据源"，实现是 MSVC 那边的事

`mfc_controls/` 里每个控件是一组 `SULANG_MFC_PROPERTY/EVENT/METHOD` 宏声明 + 注释元数据，IDE 启动时解析这些头文件填组件箱（42 个组件、675 属性、53 事件就是这么来的）。

源码实现依赖 `mfc_sdk/include/mfc_component_sdk.h` 与 `<atlconv.h>`（ATL，MSVC 专有）—— **MinGW 编不了**，也不必编：IDE 只读头文件注解。

## 许可证

`core_support` / `mfc_controls` / `mfc_sdk` 来自 **su-lang**（MIT），见 `LICENSE-su-lang-MIT.txt`。
元数据 `.elib` 里的中文命令名、属性名，来源于易语言（吴涛 / 大有吴涛）的**公开接口规范**，不包含其代码或资源文件。
