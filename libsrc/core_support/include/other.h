#pragma once
#include "types.h"

// @name 选择
// @description 根据条件返回两个通用值中的一个；返回值按 std::any 值语义复制。
// @return_type 通用型
// @param_name condition 用作选择的逻辑值
// @param_type condition 逻辑型
// @param condition 如果该值为真，将返回第一个待选择项，否则将返回第二个待选择项。
// @param_name true_value 待选择项一
// @param_type true_value 通用型
// @param true_value 如果给定逻辑值为真，命令将返回本选择项。
// @param_name false_value 待选择项二
// @param_type false_value 通用型
// @param false_value 如果给定逻辑值为假，命令将返回本选择项。
std::any CoreSelect(bool condition, const std::any &true_value, const std::any &false_value);
// @name 取数据类型尺寸
// @description 返回核心数据类型标识对应的 C++ 存储尺寸；动态容器类型返回对象本身尺寸。
// @return_type 整数型
// @param_name type_id 欲取其尺寸的数据类型
// @param_type type_id 整数型
// @param type_id 参数值可以为以下常量： 1、#字节型； 2、#短整数型； 3、#整数型； 4、#长整数型； 5、#小数型；
// 6、#双精度小数型； 7、#逻辑型； 8、#日期时间型； 9、#子程序指针型。
std::int32_t CoreDataTypeSize(std::int32_t type_id);
// @name 取颜色值
// @description 将红、绿、蓝分量限制到 0 至 255 后组合为 Windows COLORREF 数值。
// @return_type 整数型
// @param_name red 红色
// @param_type red 整数型
// @param red 数值范围从 0 到 255，表示颜色中的红色成份。
// @param_name green 绿色
// @param_type green 整数型
// @param green 数值范围从 0 到 255，表示颜色中的绿色成份。
// @param_name blue 蓝色
// @param_type blue 整数型
// @param blue 数值范围从 0 到 255，表示颜色中的蓝色成份。
std::int32_t CoreRgb(std::int32_t red, std::int32_t green, std::int32_t blue);
// @name 取硬盘特征字
// @description 返回系统盘卷序列号；读取失败时返回 0。
// @return_type 整数型
std::int32_t CoreSystemVolumeSerial();
// @name 取系统语言
// @description 返回 Windows 用户默认语言标识符。
// @return_type 整数型
std::int32_t CoreSystemLanguage();
// @name 置DLL装载目录
// @description 设置进程 DLL 搜索目录并返回原目录；空文本恢复默认搜索规则。
// @return_type 文本型
// @param_name path DLL装载目录
// @param_type path 文本型
// @param path 可以被省略。本参数指定系统对DLL命令配置中DLL库文件的优先装载路径，如果被省略，则默认值为空文本。
std::string CoreSetDllDirectory(const std::string &path = "");
// @name 运行控制台程序
// @description 执行控制台命令并合并捕获标准输出和标准错误，返回 UTF-8/本地编码原始文本。
// @return_type 文本型
// @param_name command_line 欲运行的命令行
// @param_type command_line 文本型
// @param command_line 要执行的控制台命令行；标准输出和标准错误会合并后作为返回文本。
std::string CoreRunConsole(const std::string &command_line);
// @name 取核心库版本
// @description 返回核心支持库接口版本 1.0。
// @return_type 双精度小数型
long double CoreLibraryVersion();
