#pragma once
#include "types.h"

// @name 打开文件
// @description 使用 Windows 文件共享规则打开二进制文件并返回大于零的进程内句柄；失败返回 0。
// @return_type 整数型
// @param_name path 欲打开的文件名称
// @param_type path 文本型
// @param path 指定欲打开的文件名称。
// @param_name mode 打开方式
// @param_type mode 整数型
// @param mode 1 读入、2 写出、3 读写，文件必须存在；4 重写会创建或清空；5 改写会创建且仅写；6 改读会创建且可读写。
// @param_name sharing 共享方式
// @param_type sharing 整数型
// @param sharing 1 无限制、2 禁止其它句柄读取、3 禁止其它句柄写入、4 禁止其它句柄读写；该限制由 Windows 内核执行。
std::int32_t CoreOpenFile(const std::string &path, std::int32_t mode = 3, std::int32_t sharing = 1);
// @name 关闭文件
// @description 关闭进程内文件句柄；无效句柄不会产生异常。
// @param_name handle 欲关闭的文件号
// @param_type handle 整数型
// @param handle 参数值指明欲关闭的文件号，该文件号由“打开文件”命令所返回。
void CoreCloseFile(std::int32_t handle);
// @name 关闭所有文件
// @description 关闭核心支持库当前打开的全部文件句柄。
void CoreCloseAllFiles();
// @name 移动读写位置
// @description 按文件首、文件尾或当前位置移动读写位置；起点分别为 1、2、3。
// @return_type 逻辑型
// @param_name handle 欲进行操作的文件号
// @param_type handle 整数型
// @param handle 该文件号由“打开文件”命令所返回。
// @param_name origin 起始移动位置
// @param_type origin 整数型
// @param origin 可以被省略。参数值指定从文件中开始移动的位置。可以为以下常量之一：1、#文件首； 2、#文件尾；
// 3、#现行位置。如果本参数被省略，默认值为“#文件首”。
// @param_name offset 移动距离
// @param_type offset 长整数型
// @param offset 参数值指定相对于起始移动位置的移动长度。
bool CoreSeekFile(std::int32_t handle, std::int32_t origin, std::int64_t offset);
// @name 移动读写位置
// @description 省略起始位置时从文件首移动。
// @return_type 逻辑型
// @param_name handle 欲进行操作的文件号
// @param_type handle 整数型
// @param_name offset 移动距离
// @param_type offset 长整数型
bool CoreSeekFile(std::int32_t handle, std::int64_t offset);
// @name 移到文件首
// @description 将文件读写位置移动到文件首。
// @return_type 逻辑型
// @param_name handle 欲进行操作的文件号
// @param_type handle 整数型
// @param handle 该文件号由“打开文件”命令所返回。
bool CoreSeekFileBegin(std::int32_t handle);
// @name 移到文件尾
// @description 将文件读写位置移动到文件尾。
// @return_type 逻辑型
// @param_name handle 欲进行操作的文件号
// @param_type handle 整数型
// @param handle 该文件号由“打开文件”命令所返回。
bool CoreSeekFileEnd(std::int32_t handle);
// @name 读入字节集
// @description 从当前位置读取最多指定数量的字节；数量小于零时读取到文件尾。
// @return_type 字节集
// @param_name handle 欲读入数据的文件号
// @param_type handle 整数型
// @param handle 该文件号由“打开文件”命令所返回。
// @param_name length 欲读入数据的长度
// @param_type length 整数型
// @param length 指定欲读入数据的长度。
sulang::core::ByteArray CoreReadBinary(std::int32_t handle, std::int32_t length);
// @name 写出字节集
// @description 从当前位置写入字节集。
// @return_type 逻辑型
// @param_name handle 欲写出数据的文件号
// @param_type handle 整数型
// @param handle 该文件号由“打开文件”命令所返回。
// @param_name data 欲写出的字节集数据
// @param_type data 字节集
// @param data 指定欲写出的字节集数据。
bool CoreWriteBinary(std::int32_t handle, const sulang::core::ByteArray &data);
// @name 读入文本
// @description 从当前位置读取指定字节数的文本；长度小于零时读取到文件尾。
// @return_type 文本型
// @param_name handle 欲读入文本数据的文件号
// @param_type handle 整数型
// @param handle 该文件号由“打开文件”命令所返回。
// @param_name length 欲读入文本数据的长度
// @param_type length 整数型
// @param length 可以被省略。如果本参数被省略，默认读入文件中的所有文本数据。
std::string CoreReadText(std::int32_t handle, std::int32_t length = -1);
// @name 写出文本
// @description 按原始 UTF-8 字节写入文本。
// @return_type 逻辑型
// @param_name handle 欲写出文本的文件号
// @param_type handle 整数型
// @param handle 该文件号由“打开文件”命令所返回。
// @param_name value 欲写出的文本
// @param_type value 文本型
// @param value
// 参数值如果不为文本类型数据，将自动进行转换，如果无法转换（即数据类型为字节集、子程序指针、库或用户自定义数据类型），则不写出此数据。
bool CoreWriteText(std::int32_t handle, const std::string &value);
// @name 读入一行
// @description 读取一行并移除行结束符。
// @return_type 文本型
// @param_name handle 欲读入文本数据的文件号
// @param_type handle 整数型
// @param handle 该文件号由“打开文件”命令所返回。
std::string CoreReadLine(std::int32_t handle);
// @name 写文本行
// @description 写入文本并追加平台标准换行符。
// @return_type 逻辑型
// @param_name handle 欲写出文本的文件号
// @param_type handle 整数型
// @param handle 该文件号由“打开文件”命令所返回。
// @param_name value 欲写出的文本
// @param_type value 文本型
// @param value
// 参数值如果不为文本类型数据，将自动进行转换，如果无法转换（即数据类型为字节集、子程序指针、库或用户自定义数据类型），则不写出此数据。
bool CoreWriteLine(std::int32_t handle, const std::string &value);

// @name 插入字节集
// @description 在当前读写位置插入字节集，原有后续内容顺延；成功返回真。
// @return_type 逻辑型
// @param_name handle 欲写出数据的文件号
// @param_type handle 整数型
// @param_name data 欲写出的字节集数据
// @param_type data 字节集
bool CoreInsertBinary(std::int32_t handle, const sulang::core::ByteArray &data);

// @name 插入文本
// @description 在当前读写位置插入 UTF-8 文本，原有后续内容顺延。
// @return_type 逻辑型
// @param_name handle 欲写出文本的文件号
// @param_type handle 整数型
// @param_name value 欲写出的文本
// @param_type value 文本型
bool CoreInsertText(std::int32_t handle, const std::string &value);

// @name 插入文本行
// @description 在当前读写位置插入 UTF-8 文本及 CRLF 换行。
// @return_type 逻辑型
// @param_name handle 欲写出文本的文件号
// @param_type handle 整数型
// @param_name value 欲写出的文本
// @param_type value 文本型
bool CoreInsertLine(std::int32_t handle, const std::string &value);

// @name 删除数据
// @description 从当前读写位置删除指定字节数，后续内容前移。
// @return_type 逻辑型
// @param_name handle 文件号
// @param_type handle 整数型
// @param_name length 欲删除数据的字节数
// @param_type length 整数型
bool CoreDeleteFileData(std::int32_t handle, std::int32_t length);

// @name 锁住文件
// @description 对已打开文件的指定字节范围加排他锁，重试时间为毫秒；-1 表示持续等待。
// @return_type 逻辑型
// @param_name handle 欲加锁或解锁的文件号
// @param_type handle 整数型
// @param_name offset 欲加锁或解锁的位置
// @param_type offset 整数型
// @param_name length 欲加锁或解锁的长度
// @param_type length 整数型
// @param_name retry_ms 加锁重试时间
// @param_type retry_ms 整数型
bool CoreLockFile(std::int32_t handle, std::int32_t offset, std::int32_t length, std::int32_t retry_ms = 0);

// @name 解锁文件
// @description 用与加锁时完全一致的范围释放文件锁。
// @return_type 逻辑型
// @param_name handle 欲加锁或解锁的文件号
// @param_type handle 整数型
// @param_name offset 欲加锁或解锁的位置
// @param_type offset 整数型
// @param_name length 欲加锁或解锁的长度
// @param_type length 整数型
bool CoreUnlockFile(std::int32_t handle, std::int32_t offset, std::int32_t length);
// @name 是否在文件尾
// @description 判断句柄是否已经到达文件尾。
// @return_type 逻辑型
// @param_name handle 文件号
// @param_type handle 整数型
// @param handle 参数值指明欲操作的文件号，该文件号必须由“打开文件”命令所返回。
// @param_name text_mode 是否为判断文本已读完
// @param_type text_mode 逻辑型
// @param text_mode 可以被省略。如果本参数值为真而且下一即将读入的字节为字节值 0 或 26
// 等文本终止标记符或者当前读写位置已处于该文件数据的尾部，命令将返回真。如果本参数被省略或为假，将不进行此检查。
bool CoreFileEof(std::int32_t handle, bool text_mode = false);
// @name 取读写位置
// @description 返回当前字节位置；失败时返回 -1。
// @return_type 长整数型
// @param_name handle 文件号
// @param_type handle 整数型
// @param handle 参数值指明欲操作的文件号，该文件号必须由“打开文件”命令所返回。
std::int64_t CoreFilePosition(std::int32_t handle);
// @name 取文件长度
// @description 返回打开文件的字节长度；失败时返回 -1，且尽量保留原读写位置。
// @return_type 长整数型
// @param_name handle 文件号
// @param_type handle 整数型
// @param handle 参数值指明欲操作的文件号，该文件号必须由“打开文件”命令所返回。
std::int64_t CoreOpenedFileLength(std::int32_t handle);
