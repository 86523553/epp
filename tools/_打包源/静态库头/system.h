#pragma once
#include "types.h"

// @name 运行
// @description 启动命令行；选择等待时返回进程退出成功状态，否则返回启动状态。
// @return_type 逻辑型
// @param_name command_line 欲运行的命令行
// @param_type command_line 文本型
// @param command_line 指定欲运行的命令行。
// @param_name wait 是否等待程序运行完毕
// @param_type wait 逻辑型
// @param wait 初始值为“假”。
// @param_name show_mode 被运行程序窗口显示方式
// @param_type show_mode 整数型
// @param show_mode 可以被省略。参数值可以为以下常量之一：1、#隐藏窗口； 2、#普通激活； 3、#最小化激活；
// 4、#最大化激活； 5、#普通不激活； 6、#最小化不激活。如果省略本参数，默认为“普通激活”方式。
bool CoreRun(const std::string &command_line, bool wait, std::int32_t show_mode = 2);
// @name 取剪辑板文本
// @description 返回 Windows 剪贴板中的 Unicode 文本，无文本时返回空文本。
// @return_type 文本型
std::string CoreClipboardText();
// @name 置剪辑板文本
// @description 使用 Unicode 格式覆盖 Windows 剪贴板文本。
// @return_type 逻辑型
// @param_name value 准备置入剪辑板的文本
// @param_type value 文本型
// @param value 指定准备置入剪辑板的文本。
bool CoreSetClipboardText(const std::string &value);
// @name 剪辑板中可有文本
// @description 判断剪贴板是否包含 Unicode 或 ANSI 文本。
// @return_type 逻辑型
bool CoreClipboardHasText();
// @name 清除剪辑板
// @description 清空 Windows 剪贴板。
void CoreClearClipboard();
// @name 取屏幕宽度
// @description 返回主显示器宽度，单位为像素。
// @return_type 整数型
std::int32_t CoreScreenWidth();
// @name 取屏幕高度
// @description 返回主显示器高度，单位为像素。
// @return_type 整数型
std::int32_t CoreScreenHeight();
// @name 取鼠标水平位置
// @description 返回鼠标指针的屏幕横坐标，单位为像素。
// @return_type 整数型
std::int32_t CoreCursorX();
// @name 取鼠标垂直位置
// @description 返回鼠标指针的屏幕纵坐标，单位为像素。
// @return_type 整数型
std::int32_t CoreCursorY();
// @name 取颜色数
// @description 返回主显示设备可表达的颜色数量；超过 32 位整数范围时返回最大整数。
// @return_type 整数型
std::int32_t CoreColorCount();
// @name 取默认底色
// @description 返回 Windows 默认窗口背景色的 RGB 颜色值。
// @return_type 整数型
std::int32_t CoreDefaultBackgroundColor();
// @name 信息框
// @description 显示系统消息框并返回所选按钮编号。
// @return_type 整数型
// @param_name message 提示信息
// @param_type message 文本型
// @param message 要显示的 UTF-8 提示文本。
// @param_name buttons 按钮
// @param_type buttons 整数型
// @param buttons Windows MessageBox 按钮和图标标志组合。
// @param_name title 窗口标题
// @param_type title 文本型
// @param title 信息框标题文本。
std::int32_t CoreMessageBox(const std::string &message, std::int32_t buttons, const std::string &title = "信息：");
// @name 鸣叫
// @description 播放系统默认提示音。
void CoreBeep();
// @name 取启动时间
// @description 返回系统启动以来的毫秒数，使用 64 位计时器后截取为长整数。
// @return_type 长整数型
std::int64_t CoreTickCount();
// @name 置等待鼠标
// @description 将当前线程光标设置为系统等待光标。
void CoreSetWaitCursor();
// @name 恢复鼠标
// @description 恢复调用置等待鼠标之前保存的光标。
void CoreRestoreCursor();
// @name 延时
// @description 阻塞当前线程指定毫秒数，负数按零处理。
// @param_name milliseconds 欲等待的时间
// @param_type milliseconds 整数型
// @param milliseconds 本参数指定欲暂停程序执行的时间，单位为毫秒。
void CoreSleep(std::int32_t milliseconds);
// @name 读配置项
// @description 从 INI 文件读取文本值。
// @return_type 文本型
// @param_name file 配置文件名
// @param_type file 文本型
// @param file 指定配置文件的名称，通常以.ini作为文件名后缀。
// @param_name section 节名称
// @param_type section 文本型
// @param section 包含欲读入配置项所处节的名称。
// @param_name key 配置项名称
// @param_type key 文本型
// @param key 参数值指定欲读入配置项在其节中的名称。
// @param_name default_value 默认文本
// @param_type default_value 文本型
// @param default_value
// 可以被省略。如果指定配置项不存在，将返回此默认文本。如果指定配置项不存在且本参数被省略，将返回空文本。
std::string CoreReadIni(const std::string &file, const std::string &section, const std::string &key,
                          const std::string &default_value = "");
// @name 写配置项
// @description 向 INI 文件写入文本值。
// @return_type 逻辑型
// @param_name file 配置文件名
// @param_type file 文本型
// @param file 指定配置文件的名称，通常以.ini作为文件名后缀。
// @param_name section 节名称
// @param_type section 文本型
// @param section 包含欲写入配置项所处节的名称。
// @param_name key 配置项名称
// @param_type key 文本型
// @param key 可以被省略。参数值指定欲写入配置项在其节中的名称。如果参数值被省略，则删除指定节及其下的所有配置项。
// @param_name value 欲写入值
// @param_type value 文本型
// @param value 可以被省略。参数值指定欲写入到指定配置项中的文本。如果参数值被省略，则删除所指定配置项。
bool CoreWriteIni(const std::string &file, const std::string &section, const std::string &key = "",
                    const std::string &value = "");
// @name 取配置节名
// @description 返回 INI 文件中的全部节名称。
// @return_type 文本型数组
// @param_name file 配置文件名
// @param_type file 文本型
// @param file 指定配置文件的名称，通常以.ini作为文件名后缀。
std::vector<std::string> CoreIniSections(const std::string &file);
// @name 取操作系统类别
// @description Windows 平台返回 1，其他平台返回 0。
// @return_type 整数型
std::int32_t CoreOperatingSystem();
// @name 取程序名称
// @description 返回当前程序的完整路径、文件名或所在目录；类型分别为 0、1、2。
// @return_type 文本型
// @param_name kind 欲取的程序名称类型
// @param_type kind 整数型
// @param kind 1:当前语句所处子程序名称; 2:当前语句所处程序集名称;
// 3:当前语句所处易语言程序的全路径文件名称(注意如果编译时易语言程序没有保存,将为空);
// 4:当前语句所处程序配置信息中的程序名称;  5:当前语句所处程序配置信息中的作者名称;
// 6:当前语句所处易语言程序的无路径和后缀文件名称(注意如果编译时易语言程序没有保存,将为空)。
std::string CoreApplicationName(std::int32_t kind);
// @name 取操作系统类别2
// @description 返回可读的操作系统名称和版本架构信息。
// @return_type 文本型
std::string CoreOperatingSystemText();
// @name 取最后错误
// @description 返回当前线程的系统最后错误代码。
// @return_type 整数型
std::int32_t CoreLastError();
// @name 取统一文本
// @description 将 UTF-8 文本编码为 UTF-8 或小端 UTF-16 字节，可追加结束零。
// @return_type 字节集
// @param_name value 待转换常量文本
// @param_type value 文本型
// @param value 提供将被转换到Unicode字节集的常量文本。
// @param_name wide 转换到宽文本
// @param_type wide 逻辑型
// @param wide 可以被省略。本参数如果为真则转换到UTF-16宽字符文本,否则转换为UTF-8文本.本参数如果被省略,则默认值为真.
// @param_name append_zero 添加结束零字符
// @param_type append_zero 逻辑型
// @param append_zero 可以被省略。本参数如果为真则在文本尾部添加结束零字符,为假则不添加.本参数如果被省略,则默认值为真.
sulang::core::ByteArray CoreUnifiedText(const std::string &value, bool wide = true, bool append_zero = true);
// @name 取统一文本长度
// @description 返回统一文本编码后的字节长度，不包含结束零。
// @return_type 整数型
// @param_name value 待转换常量文本
// @param_type value 文本型
// @param value 提供将被转换到Unicode字节集的常量文本。
// @param_name wide 转换到宽文本
// @param_type wide 逻辑型
// @param wide 可以被省略。本参数如果为真则转换到UTF-16宽字符文本,否则转换为UTF-8文本.本参数如果被省略,则默认值为真.
std::int32_t CoreUnifiedTextLength(const std::string &value, bool wide = true);

// @name 取文本注册项
// @description 读取 Windows 注册表的 REG_SZ/REG_EXPAND_SZ 值，缺失或类型不符时返回默认文本。
// @return_type 文本型
// @param_name root 根目录
// @param_type root 整数型
// @param_name path 全路径注册项名
// @param_type path 文本型
// @param_name fallback 默认文本
// @param_type fallback 文本型
std::string CoreReadRegistryText(std::int32_t root, const std::string &path, const std::string &fallback = "");

// @name 取数值注册项
// @description 读取 REG_DWORD 值，缺失或类型不符时返回默认数值。
// @return_type 整数型
// @param_name root 根目录
// @param_type root 整数型
// @param_name path 全路径注册项名
// @param_type path 文本型
// @param_name fallback 默认数值
// @param_type fallback 整数型
std::int32_t CoreReadRegistryNumber(std::int32_t root, const std::string &path, std::int32_t fallback = 0);

// @name 取字节集注册项
// @description 读取 REG_BINARY 值，缺失或类型不符时返回默认字节集。
// @return_type 字节集
// @param_name root 根目录
// @param_type root 整数型
// @param_name path 全路径注册项名
// @param_type path 文本型
// @param_name fallback 默认字节集
// @param_type fallback 字节集
sulang::core::ByteArray CoreReadRegistryBinary(std::int32_t root, const std::string &path,
                                               const sulang::core::ByteArray &fallback = {});

// @name 写注册项
// @description 保存文本、32 位数值或字节集；不支持的值类型返回假。
// @return_type 逻辑型
// @param_name root 根目录
// @param_type root 整数型
// @param_name path 全路径注册项名
// @param_type path 文本型
// @param_name value 欲写入值
// @param_type value 通用型
bool CoreWriteRegistry(std::int32_t root, const std::string &path, const std::any &value);

// @name 删除注册项
// @description 删除指定值；末尾反斜杠表示默认值。删除子键时须先清空其内容。
// @return_type 逻辑型
// @param_name root 根目录
// @param_type root 整数型
// @param_name path 全路径注册项名
// @param_type path 文本型
bool CoreDeleteRegistry(std::int32_t root, const std::string &path);

// @name 注册项是否存在
// @description 判断指定注册表值或子键是否存在。
// @return_type 逻辑型
// @param_name root 根目录
// @param_type root 整数型
// @param_name path 全路径注册项名
// @param_type path 文本型
bool CoreRegistryExists(std::int32_t root, const std::string &path);

// @name 取运行时数据类型
// @description 返回核心类型的稳定编号；未知自定义类型返回 0。
// @return_type 整数型
// @param_name value 欲取其数据类型的数据
// @param_type value 通用型
std::int32_t CoreRuntimeDataType(const std::any &value);
