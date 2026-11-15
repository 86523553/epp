#pragma once
#include "types.h"

// @name 寻找文件
// @description 使用 Windows 通配符逐个返回文件或目录名称；传入新模式重置查找，省略模式继续上次查找。
// @return_type 文本型
// @param_name pattern 欲寻找的文件或目录名称
// @param_type pattern 文本型
// @param pattern 可包含 * 与 ?；省略时继续当前线程的上一次查找。
// @param_name attributes 欲寻找文件的属性
// @param_type attributes 整数型
// @param attributes 可组合只读 1、隐藏 2、系统 4、子目录 16 和存档 32；默认排除目录。
std::string CoreFindFile(const std::string &pattern = "", std::int32_t attributes = 0);

// @name 取磁盘总空间
// @description 返回指定路径所在文件系统的总字节数，失败时返回 -1。
// @return_type 长整数型
// @param_name path 磁盘驱动器字符
// @param_type path 文本型
// @param path 可以被省略。类似“A”、“B”、“C”等，只取用给定文本的第一个字符。如果本参数被省略，默认使用当前驱动器。
std::int64_t CoreDiskTotalSpace(const std::string &path = "");
// @name 取磁盘剩余空间
// @description 返回指定路径所在文件系统可供当前进程使用的字节数，失败时返回 -1。
// @return_type 长整数型
// @param_name path 磁盘驱动器字符
// @param_type path 文本型
// @param path 可以被省略。类似“A”、“B”、“C”等，只取用给定文本的第一个字符。如果本参数被省略，默认使用当前驱动器。
std::int64_t CoreDiskFreeSpace(const std::string &path = "");
// @name 取磁盘卷标
// @description 返回指定磁盘的卷标；读取失败时返回空文本。
// @return_type 文本型
// @param_name drive 磁盘驱动器字符
// @param_type drive 文本型
// @param drive 可以被省略。类似“A”、“B”、“C”等，只取第一个字符；省略时使用当前驱动器。
std::string CoreDiskLabel(const std::string &drive = "");
// @name 置磁盘卷标
// @description 设置指定磁盘的卷标，成功返回真，失败返回假。
// @return_type 逻辑型
// @param_name drive 磁盘驱动器字符
// @param_type drive 文本型
// @param drive 可以被省略。类似“A”、“B”、“C”等，只取第一个字符；省略时使用当前驱动器。
// @param_name label 新的磁盘卷标
// @param_type label 文本型
// @param label 欲设置的卷标文本。
bool CoreSetDiskLabel(const std::string &drive, const std::string &label);
// @name 置磁盘卷标
// @description 省略驱动器时设置当前驱动器的卷标。
// @return_type 逻辑型
// @param_name label 新的磁盘卷标
// @param_type label 文本型
bool CoreSetDiskLabel(const std::string &label);
// @name 改变驱动器
// @description 将当前进程的默认驱动器切换到指定盘符，成功返回真。
// @return_type 逻辑型
// @param_name drive 驱动器字符
// @param_type drive 文本型
// @param drive 类似“A”、“B”、“C”等，只取第一个字符。
bool CoreChangeDrive(const std::string &drive);
// @name 改变目录
// @description 修改当前工作目录。
// @return_type 逻辑型
// @param_name path 欲改变到的目录
// @param_type path 文本型
// @param path
// 参数值指明哪个目录将成为新的缺省目录，可能会包含驱动器。如果没有指定驱动器，则在当前的驱动器上改变缺省目录。
bool CoreChangeDirectory(const std::string &path);
// @name 取当前目录
// @description 返回当前工作目录。
// @return_type 文本型
std::string CoreCurrentDirectory();
// @name 创建目录
// @description 递归创建目录；目录已经存在时也返回真。
// @return_type 逻辑型
// @param_name path 欲创建的目录名称
// @param_type path 文本型
// @param path 指定欲创建的目录名称。
bool CoreCreateDirectory(const std::string &path);
// @name 删除目录
// @description 删除空目录；不会递归删除目录内容。
// @return_type 逻辑型
// @param_name path 欲删除的目录名称
// @param_type path 文本型
// @param path 该目录应实际存在，如果目录中存在文件或子目录，将被一并删除，因此使用本命令请千万慎重。
bool CoreRemoveDirectory(const std::string &path);
// @name 复制文件
// @description 复制文件并覆盖已存在目标。
// @return_type 逻辑型
// @param_name source 被复制的文件名
// @param_type source 文本型
// @param source 指定被复制的文件名。
// @param_name target 复制到的文件名
// @param_type target 文本型
// @param target 指定复制到的文件名。
bool CoreCopyFile(const std::string &source, const std::string &target);
// @name 移动文件
// @description 移动或重命名文件；目标存在时操作失败。
// @return_type 逻辑型
// @param_name source 被移动的文件
// @param_type source 文本型
// @param source 指定被移动的文件。
// @param_name target 移动到的位置
// @param_type target 文本型
// @param target 指定移动到的位置。
bool CoreMoveFile(const std::string &source, const std::string &target);
// @name 删除文件
// @description 删除普通文件或符号链接，不递归删除目录。
// @return_type 逻辑型
// @param_name path 欲删除的文件名
// @param_type path 文本型
// @param path 指定欲删除的文件名。
bool CoreDeleteFile(const std::string &path);
// @name 文件更名
// @description 重命名文件或目录；目标存在时操作失败。
// @return_type 逻辑型
// @param_name source 欲更名的原文件或目录名
// @param_type source 文本型
// @param source 指定欲更名的原文件或目录名。
// @param_name target 欲更改为的现文件或目录名
// @param_type target 文本型
// @param target 指定欲更改为的现文件或目录名。
bool CoreRenamePath(const std::string &source, const std::string &target);
// @name 文件是否存在
// @description 判断路径是否为可访问的普通文件。
// @return_type 逻辑型
// @param_name path 欲测试的文件名称
// @param_type path 文本型
// @param path 指定欲测试的文件名称。
bool CoreFileExists(const std::string &path);
// @name 取文件属性
// @description 返回只读、隐藏、系统、目录和存档属性的组合值；失败返回 -1。
// @return_type 整数型
// @param_name path 文件或目录名称
// @param_type path 文本型
// @param path 指定要读取属性的文件或目录。
std::int32_t CoreFileAttributes(const std::string &path);
// @name 置文件属性
// @description 设置只读、隐藏、系统和存档属性，成功返回真。
// @return_type 逻辑型
// @param_name path 文件名称
// @param_type path 文本型
// @param path 指定要设置属性的文件。
// @param_name attributes 属性组合值
// @param_type attributes 整数型
// @param attributes 可以组合只读 1、隐藏 2、系统 4 和存档 32。
bool CoreSetFileAttributes(const std::string &path, std::int32_t attributes);
// @name 取文件时间
// @description 返回文件最后写入时间，读取失败时抛出异常。
// @return_type 日期时间型
// @param_name path 文件名
// @param_type path 文本型
// @param path 指定文件名。
sulang::core::DateTime CoreFileDateTime(const std::string &path);
// @name 取文件尺寸
// @description 返回文件字节数，读取失败时返回 -1。
// @return_type 长整数型
// @param_name path 文件名
// @param_type path 文本型
// @param path 指定文件名。
std::int64_t CoreFileLength(const std::string &path);
// @name 取临时文件名
// @description 在指定目录安全创建空临时文件并返回其完整路径。
// @return_type 文本型
// @param_name directory 目录名
// @param_type directory 文本型
// @param directory 可以被省略。如果省略本参数，默认将使用系统的标准临时目录。
std::string CoreTemporaryFile(const std::string &directory = "");
// @name 读入文件
// @description 一次读取文件全部字节，失败时抛出异常。
// @return_type 字节集
// @param_name path 文件名
// @param_type path 文本型
// @param path 指定文件名。
sulang::core::ByteArray CoreReadWholeFile(const std::string &path);
// @name 写到文件
// @description 覆盖写入全部字节，并在写入成功后返回真。
// @return_type 逻辑型
// @param_name path 文件名
// @param_type path 文本型
// @param path 指定文件名。
// @param_name data 欲写入文件的数据
// @param_type data 字节集
// @param data 指定欲写入文件的数据。
bool CoreWriteWholeFile(const std::string &path, const sulang::core::ByteArray &data);
