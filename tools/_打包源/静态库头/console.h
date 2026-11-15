#pragma once
#include "types.h"

// @name 标准输出
// @description 将文本写入标准输出或标准错误；方向 2 表示标准错误，其他值表示标准输出。
// @param_name direction 输出方向
// @param_type direction 整数型
// @param direction 可以被省略。本参数提供内容所输出到的设备，可以为以下常量值之一： 1、#标准输出设备；
// 2、#标准错误设备。如果省略本参数，默认为“#标准输出设备”。
// @param_name value 欲输出内容
// @param_type value 文本型
// @param value 本参数只能为文本、数值、逻辑值或日期时间。如果内容为文本且包含多行，可在各行之间用回车符 (即“字符
// (13)”)、换行符 (即“字符 (10)”) 或回车换行符的组合 (即：“字符 (13) + 字符 (10)”) 来分隔。
void CoreConsoleWrite(std::int32_t direction, const std::string &value);
// @name 标准输出
// @description 省略输出方向时写入标准输出。
// @param_name value 欲输出内容
// @param_type value 文本型
void CoreConsoleWrite(const std::string &value);
// @name 标准输入
// @description 从标准输入读取一行；关闭回显仅在 Windows 控制台输入句柄可用时生效。
// @return_type 文本型
// @param_name echo 是否回显
// @param_type echo 逻辑型
// @param echo
// 可以被省略。本参数决定输入时是否显示所输入字符，为假不显示，为真显示。如果被省略，默认值为真，即回显。可以通过将本参数设置为假以输入密码等特殊信息。
std::string CoreConsoleRead(bool echo = true);
