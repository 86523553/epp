# SuLang 核心支持库

本支持库由 IDE 内置加载器直接读取 `.sulib` 清单和分类头文件。它是源码支持库：用户将它加入项目后，`src` 中声明的 C++ 源码会随当前项目一起编译，不依赖预生成 DLL。

## 分类规则

`capabilities` 声明了 `symbol_categories`。头文件使用英文文件名，清单中的 `header_categories` 将每个头文件映射到 IDE 显示的中文分类；当前分类与项目根目录“核心支持库”中的 17 个 TXT 文件一一对应。这样源码目录和 C++ include 始终符合跨平台命名习惯，同时不改变用户看到的分类名称。

## 接口原则

- 中文名称、参数说明和返回值说明写在函数或常量声明前的 `// @...` 元数据中。
- 英文 C++ 符号使用常规 `Core...` 命名，不再使用产品缩写 `Su` 前缀；内部辅助类型采用用途名称，避免与公开函数冲突。
- 文本统一使用 UTF-8 `std::string`；字节集使用 `std::vector<std::uint8_t>`；日期时间使用 `std::chrono::system_clock::time_point`。
- 失败语义优先使用明确的 `false`、`-1` 或标准异常，头文件说明中逐项注明。
- 裸内存指针读取命令已提供，但调用方须保证内存地址、长度和零终止文本有效；64 位进程须传完整指针值，不能沿用旧版 32 位整数截断地址。
- 原资料中依赖任意类型左值、多维数组、窗口对象、编译插件或未提供资源词典的命令不导出伪实现。各分类覆盖与缺项见项目文档 `docs/核心支持库文档核对.md`。

## 构建依赖

支持 Windows x86/x64，工具链为 MSVC 或 MinGW，语言标准为 C++20。清单自动链接 `ws2_32`、`iphlpapi`、`winmm`、`user32`、`shell32` 和 `gdi32`。

## 验证

在项目根目录运行：

```powershell
.\.venv\Scripts\python.exe tools\validate_support_library.py lib\core_support --compiler tools\mingw64\bin\g++.exe
.\.venv\Scripts\python.exe -m pytest tests\test_core_support_library.py -q
```
