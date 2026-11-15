#pragma once
#include "types.h"

// @name 取命令行
// @description 返回当前进程的 UTF-8 命令行参数数组，第一项为可执行文件名。
// @return_type 文本型数组
std::vector<std::string> CoreCommandLineArguments();
// @name 取运行目录
// @description 返回当前工作目录的 UTF-8 路径。
// @return_type 文本型
std::string CoreWorkingDirectory();
// @name 取执行文件名
// @description 返回当前进程可执行文件的绝对 UTF-8 路径。
// @return_type 文本型
std::string CoreExecutablePath();
// @name 读环境变量
// @description 返回环境变量值；变量不存在时返回空文本。
// @return_type 文本型
// @param_name name 环境变量名称
// @param_type name 文本型
// @param name 指定环境变量名称。
std::string CoreGetEnvironment(const std::string &name);
// @name 写环境变量
// @description 设置当前进程环境变量；值为空时仍保留空值。
// @return_type 逻辑型
// @param_name name 环境变量名称
// @param_type name 文本型
// @param name 指定环境变量名称。
// @param_name value 欲写入内容
// @param_type value 文本型
// @param value 指定欲写入内容。
bool CoreSetEnvironment(const std::string &name, const std::string &value);
