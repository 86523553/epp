#pragma once
#include "types.h"

#include <cstddef>

// @name 到字节集
// @description 将基本值或数值数组转换为原始字节；文本按 UTF-8 编码，整数和小数使用当前平台的内存字节序。
// @return_type 字节集
// @param_name value 欲转换为字节集的数据
// @param_type value 通用型
// @param value 支持整数、小数、逻辑值、文本、字节集和数值数组；不支持的对象抛出 invalid_argument。
sulang::core::ByteArray CoreToBinary(const std::any &value);

// @name 取字节集数据
// @description 按文档类型编号读取字节集数据；索引从 1 开始，越界或类型编号无效时抛出异常。
// @return_type 通用型
// @param_name value 欲取出其中数据的字节集
// @param_type value 字节集
// @param value 提供待读取的字节集。
// @param_name type 欲取出数据的类型
// @param_type type 整数型
// @param type 1 字节、2 短整数、3 整数、4 长整数、5 小数、6 双精度小数、7 逻辑、8 日期时间、9 指针、10 文本。
// @param_name start 起始索引位置
// @param_type start 整数型
// @param start 可以省略，默认从第 1 字节开始；本接口只读取索引，不修改传入变量。
std::any CoreGetBinaryElement(const sulang::core::ByteArray &value, std::int32_t type,
                              std::int32_t start = 1);

// @name 指针到字节集
// @description 从调用方保证有效的内存地址复制指定长度；空指针或负长度抛出 invalid_argument。
// @return_type 字节集
// @param_name address 内存数据指针
// @param_type address 整数型
// @param address 必须是当前进程中仍有效的完整指针值；64 位进程不可截断为 32 位，易代码可使用长整数保存地址。
// @param_name length 内存数据长度
// @param_type length 整数型
// @param length 欲复制的字节数。
sulang::core::ByteArray CorePointerToBinary(std::uintptr_t address, std::int32_t length);

// @name 指针到整数
// @description 从有效内存地址复制一个 32 位整数。
// @return_type 整数型
// @param_name address 内存数据指针
// @param_type address 整数型
// @param address 调用方负责保证地址有效且至少可读 4 字节；64 位进程使用长整数保存地址。
std::int32_t CorePointerToInt32(std::uintptr_t address);

// @name 指针到小数
// @description 从有效内存地址复制一个 32 位小数。
// @return_type 小数型
// @param_name address 内存数据指针
// @param_type address 整数型
// @param address 调用方负责保证地址有效且至少可读 4 字节；64 位进程使用长整数保存地址。
float CorePointerToFloat(std::uintptr_t address);

// @name 指针到双精度小数
// @description 从有效内存地址复制一个 64 位双精度小数。
// @return_type 双精度小数型
// @param_name address 内存数据指针
// @param_type address 整数型
// @param address 调用方负责保证地址有效且至少可读 8 字节；64 位进程使用长整数保存地址。
double CorePointerToDouble(std::uintptr_t address);

// @name 取字节集长度
// @description 返回字节集的字节数量。
// @return_type 整数型
// @param_name value 字节集数据
// @param_type value 字节集
// @param value 参数值指定欲检查其长度的字节集数据。
std::int32_t CoreBinaryLength(const sulang::core::ByteArray &value);
// @name 取字节集左边
// @description 返回字节集左侧指定数量的字节。
// @return_type 字节集
// @param_name value 欲取其部分的字节集
// @param_type value 字节集
// @param value 指定欲取其部分的字节集。
// @param_name count 欲取出字节的数目
// @param_type count 整数型
// @param count 指定欲取出字节的数目。
sulang::core::ByteArray CoreBinaryLeft(const sulang::core::ByteArray &value, std::int32_t count);
// @name 取字节集右边
// @description 返回字节集右侧指定数量的字节。
// @return_type 字节集
// @param_name value 欲取其部分的字节集
// @param_type value 字节集
// @param value 指定欲取其部分的字节集。
// @param_name count 欲取出字节的数目
// @param_type count 整数型
// @param count 指定欲取出字节的数目。
sulang::core::ByteArray CoreBinaryRight(const sulang::core::ByteArray &value, std::int32_t count);
// @name 取字节集中间
// @description 从以 1 为起点的位置返回指定数量的字节。
// @return_type 字节集
// @param_name value 欲取其部分的字节集
// @param_type value 字节集
// @param value 指定欲取其部分的字节集。
// @param_name start 起始取出位置
// @param_type start 整数型
// @param start 1为首位置，2为第2个位置，如此类推。
// @param_name count 欲取出字节的数目
// @param_type count 整数型
// @param count 指定欲取出字节的数目。
sulang::core::ByteArray CoreBinaryMid(const sulang::core::ByteArray &value, std::int32_t start, std::int32_t count);
// @name 寻找字节集
// @description 从以 1 为起点的位置查找子字节集，未找到时返回 -1。
// @return_type 整数型
// @param_name value 被搜寻的字节集
// @param_type value 字节集
// @param value 指定被搜寻的字节集。
// @param_name needle 欲寻找的字节集
// @param_type needle 字节集
// @param needle 指定欲寻找的字节集。
// @param_name start 起始搜寻位置
// @param_type start 整数型
// @param start
// 可以被省略。1为首位置，2为第2个位置，如此类推，如果被省略，则“寻找字节集”命令默认从首部开始，“倒找字节集”命令默认从尾部开始。
std::int32_t CoreFindBinary(const sulang::core::ByteArray &value, const sulang::core::ByteArray &needle,
                              std::int32_t start = 1);
// @name 倒找字节集
// @description 从指定位置向前查找子字节集，未找到时返回 -1。
// @return_type 整数型
// @param_name value 被搜寻的字节集
// @param_type value 字节集
// @param value 指定被搜寻的字节集。
// @param_name needle 欲寻找的字节集
// @param_type needle 字节集
// @param needle 指定欲寻找的字节集。
// @param_name start 起始搜寻位置
// @param_type start 整数型
// @param start
// 可以被省略。1为首位置，2为第2个位置，如此类推，如果被省略，则“寻找字节集”命令默认从首部开始，“倒找字节集”命令默认从尾部开始。
std::int32_t CoreFindBinaryReverse(const sulang::core::ByteArray &value, const sulang::core::ByteArray &needle,
                                     std::int32_t start = 0);
// @name 字节集替换
// @description 从以 1 为起点的位置替换指定字节数量。
// @return_type 字节集
// @param_name value 欲替换其部分的字节集
// @param_type value 字节集
// @param value 指定欲替换其部分的字节集。
// @param_name start 起始替换位置
// @param_type start 整数型
// @param start 替换的起始位置，1为首位置，2为第2个位置，如此类推。
// @param_name length 替换长度
// @param_type length 整数型
// @param length 指定替换长度。
// @param_name replacement 用作替换的字节集
// @param_type replacement 字节集
// @param replacement 可以被省略。如果本参数被省略，则删除字节集中的指定部分。
sulang::core::ByteArray CoreReplaceBinary(const sulang::core::ByteArray &value, std::int32_t start,
                                            std::int32_t length, const sulang::core::ByteArray &replacement = {});
// @name 子字节集替换
// @description 替换子字节集，可指定起始位置和最大替换次数。
// @return_type 字节集
// @param_name value 欲被替换的字节集
// @param_type value 字节集
// @param value 指定欲被替换的字节集。
// @param_name needle 欲被替换的子字节集
// @param_type needle 字节集
// @param needle 指定欲被替换的子字节集。
// @param_name replacement 用作替换的子字节集
// @param_type replacement 字节集
// @param replacement 可以被省略。如果省略本参数，则默认为空字节集。
// @param_name start 进行替换的起始位置
// @param_type start 整数型
// @param start 可以被省略。被替换子字节集的起始搜索位置。如果省略，默认从1开始。
// @param_name count 替换进行的次数
// @param_type count 整数型
// @param count 可以被省略。对子字节集进行替换的次数。如果省略，默认进行所有可能的替换。
sulang::core::ByteArray CoreReplaceSubBinary(const sulang::core::ByteArray &value,
                                               const sulang::core::ByteArray &needle,
                                               const sulang::core::ByteArray &replacement = {}, std::int32_t start = 1,
                                               std::int32_t count = 0);
// @name 取空白字节集
// @description 返回指定数量的零字节。
// @return_type 字节集
// @param_name count 零字节数目
// @param_type count 整数型
// @param count 指定零字节数目。
sulang::core::ByteArray CoreZeroBytes(std::int32_t count);
// @name 取重复字节集
// @description 返回指定字节集重复若干次后的结果。
// @return_type 字节集
// @param_name count 重复次数
// @param_type count 整数型
// @param count 指定重复次数。
// @param_name value 待重复的字节集
// @param_type value 字节集
// @param value 该字节集将用于建立返回的字节集。如果为空，将返回一个空字节集。
sulang::core::ByteArray CoreRepeatBinary(std::int32_t count, const sulang::core::ByteArray &value);
// @name 分割字节集
// @description 按子字节集切分数据；限制小于等于零时返回全部结果。
// @return_type 字节集数组
// @param_name value 待分割字节集
// @param_type value 字节集
// @param value 如果参数值是一个长度为零的字节集，则返回一个空数组，即没有任何成员的数组。
// @param_name separator 用作分割的字节集
// @param_type separator 字节集
// @param separator 可以被省略。用于标识子字节集边界。本参数如果被省略，则默认使用字节 0 作为分隔符。
// @param_name limit 要返回的子字节集数目
// @param_type limit 整数型
// @param limit 可以被省略。如果被省略，则默认返回所有的子字节集。
std::vector<sulang::core::ByteArray> CoreSplitBinary(const sulang::core::ByteArray &value,
                                                       const sulang::core::ByteArray &separator = {0}, std::int32_t limit = 0);
// @name 取字节集内整数
// @description 从零起点偏移读取 32 位有符号整数，可选择反转字节序；越界时抛出异常。
// @return_type 整数型
// @param_name value 待处理的字节集
// @param_type value 字节集
// @param value 提供待处理的字节集。
// @param_name offset 欲获取整数所处偏移
// @param_type offset 整数型
// @param offset 提供所欲获取的整数值在字节集中的偏移位置。
// @param_name reverse_bytes 是否反转字节序
// @param_type reverse_bytes 逻辑型
// @param reverse_bytes
// 可以被省略。指定是否将所获取整数值的字节序进行反转,譬如十六进制整数0x12345678,反转后将为0x78563412.
// 本功能在与类似Java这样的语言进行数据交互时很有用处. 如果参数被省略，默认值为假.
std::int32_t CoreReadInt32(const sulang::core::ByteArray &value, std::int32_t offset, bool reverse_bytes = false);
// @name 置字节集内整数
// @description 在零起点偏移写入 32 位有符号整数，可选择反转字节序；越界时返回假。
// @return_type 逻辑型
// @param_name value 待处理的字节集
// @param_type value 字节集
// @param value 提供待处理的字节集。
// @param_name offset 欲设置整数所处偏移
// @param_type offset 整数型
// @param offset 提供所欲设置的整数值在字节集中的偏移位置。
// @param_name number 欲设置的整数值
// @param_type number 整数型
// @param number 提供所欲设置的具体整数值。
// @param_name reverse_bytes 是否反转字节序
// @param_type reverse_bytes 逻辑型
// @param reverse_bytes
// 可以被省略。指定是否将所设置整数的字节序进行反转,譬如十六进制整数0x12345678,反转后将为0x78563412.
// 本功能在与类似Java这样的语言进行数据交互时很有用处. 如果参数被省略，默认值为假.
bool CoreWriteInt32(sulang::core::ByteArray &value, std::int32_t offset, std::int32_t number, bool reverse_bytes = false);
