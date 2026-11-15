#pragma once
#include "types.h"

// @name 指针到文本
// @description 复制有效地址处以零字节结尾的 UTF-8 文本；调用方必须保证地址及结束零有效。
// @return_type 文本型
// @param_name address 内存文本指针
// @param_type address 整数型
// @param address 必须传入当前进程中的完整指针值，64 位进程可使用长整数保存地址。
std::string CorePointerToText(std::uintptr_t address);

// @name 取文本长度
// @description 返回 UTF-8 文本包含的 Unicode 字符数量；无效字节按单个字符处理。
// @return_type 整数型
// @param_name value 文本数据
// @param_type value 文本型
// @param_name value 文本数据
// @param_type value 文本型
// @param value 参数值指定欲检查其长度的文本数据。
std::int32_t CoreTextLength(const std::string &value);
// @name 取文本左边
// @description 返回文本左侧指定数量的 Unicode 字符，数量小于等于零时返回空文本。
// @return_type 文本型
// @param_name value 欲取其部分的文本
// @param_type value 文本型
// @param_name count 欲取出字符的数目
// @param_type count 整数型
// @param_name value 欲取其部分的文本
// @param_type value 文本型
// @param value 指定欲取其部分的文本。
// @param_name count 欲取出字符的数目
// @param_type count 整数型
// @param count 指定欲取出字符的数目。
std::string CoreTextLeft(const std::string &value, std::int32_t count);
// @name 取文本右边
// @description 返回文本右侧指定数量的 Unicode 字符，数量小于等于零时返回空文本。
// @return_type 文本型
// @param_name value 欲取其部分的文本
// @param_type value 文本型
// @param value 指定欲取其部分的文本。
// @param_name count 欲取出字符的数目
// @param_type count 整数型
// @param count 指定欲取出字符的数目。
std::string CoreTextRight(const std::string &value, std::int32_t count);
// @name 取文本中间
// @description 从以 1 为起点的位置返回指定数量的 Unicode 字符，越界时返回空文本。
// @return_type 文本型
// @param_name value 欲取其部分的文本
// @param_type value 文本型
// @param value 指定欲取其部分的文本。
// @param_name start 起始取出位置
// @param_type start 整数型
// @param start 1为首位置，2为第2个位置，如此类推。
// @param_name count 欲取出字符的数目
// @param_type count 整数型
// @param count 指定欲取出字符的数目。
std::string CoreTextMid(const std::string &value, std::int32_t start, std::int32_t count);
// @name 字符
// @description 返回指定 0 到 255 字节代码对应的单字节文本。
// @return_type 文本型
// @param_name code 欲取其字符的字符代码
// @param_type code 字节型
// @param code 指定欲取其字符的字符代码。
std::string CoreCharacter(std::uint8_t code);
// @name 取代码
// @description 返回以 1 为起点的 UTF-8 字符代码点，位置越界时返回 0。
// @return_type 整数型
// @param_name value 欲取字符代码的文本
// @param_type value 文本型
// @param value 指定欲取字符代码的文本。
// @param_name position 欲取其代码的字符位置
// @param_type position 整数型
// @param position 可以被省略。1为首位置，2为第2个位置，如此类推。如果本参数被省略，默认为首位置。
std::int32_t CoreCharacterCode(const std::string &value, std::int32_t position = 1);
// @name 寻找文本
// @description 从以 1 为起点的位置向后查找文本，未找到时返回 -1。
// @return_type 整数型
// @param_name value 被搜寻的文本
// @param_type value 文本型
// @param value 指定被搜寻的文本。
// @param_name needle 欲寻找的文本
// @param_type needle 文本型
// @param needle 指定欲寻找的文本。
// @param_name start 起始搜寻位置
// @param_type start 整数型
// @param start 可以被省略。位置值从 1 开始。如果本参数被省略，默认为 1 。
// @param_name ignore_case 是否不区分大小写
// @param_type ignore_case 逻辑型
// @param ignore_case 初始值为“假”。为真不区分大小写，为假区分。
std::int32_t CoreFindText(const std::string &value, const std::string &needle, std::int32_t start,
                          bool ignore_case);
// @name 寻找文本
// @description 省略起始位置时从文本首部寻找。
// @return_type 整数型
// @param_name value 被搜寻的文本
// @param_type value 文本型
// @param_name needle 欲寻找的文本
// @param_type needle 文本型
std::int32_t CoreFindText(const std::string &value, const std::string &needle);
// @name 寻找文本
// @description 指定起始位置时按区分大小写的默认方式寻找。
// @return_type 整数型
// @param_name value 被搜寻的文本
// @param_type value 文本型
// @param_name needle 欲寻找的文本
// @param_type needle 文本型
// @param_name start 起始搜寻位置
// @param_type start 整数型
std::int32_t CoreFindText(const std::string &value, const std::string &needle, std::int32_t start);
// @name 寻找文本
// @description 指定大小写规则且省略起始位置时从文本首部寻找。
// @return_type 整数型
// @param_name value 被搜寻的文本
// @param_type value 文本型
// @param_name needle 欲寻找的文本
// @param_type needle 文本型
// @param_name ignore_case 是否不区分大小写
// @param_type ignore_case 逻辑型
std::int32_t CoreFindText(const std::string &value, const std::string &needle, bool ignore_case);
// @name 倒找文本
// @description 从指定位置向前查找文本，起始位置小于等于零时从尾部开始，未找到时返回 -1。
// @return_type 整数型
// @param_name value 被搜寻的文本
// @param_type value 文本型
// @param value 指定被搜寻的文本。
// @param_name needle 欲寻找的文本
// @param_type needle 文本型
// @param needle 指定欲寻找的文本。
// @param_name start 起始搜寻位置
// @param_type start 整数型
// @param start 可以被省略。位置值从 1 开始。如果本参数被省略，默认为从被搜寻文本的尾部开始。
// @param_name ignore_case 是否不区分大小写
// @param_type ignore_case 逻辑型
// @param ignore_case 初始值为“假”。为真不区分大小写，为假区分。
std::int32_t CoreFindTextReverse(const std::string &value, const std::string &needle, std::int32_t start,
                                   bool ignore_case);
// @name 倒找文本
// @description 省略起始位置时从文本末尾倒找。
// @return_type 整数型
// @param_name value 被搜寻的文本
// @param_type value 文本型
// @param_name needle 欲寻找的文本
// @param_type needle 文本型
std::int32_t CoreFindTextReverse(const std::string &value, const std::string &needle);
// @name 倒找文本
// @description 指定起始位置时按区分大小写的默认方式倒找。
// @return_type 整数型
// @param_name value 被搜寻的文本
// @param_type value 文本型
// @param_name needle 欲寻找的文本
// @param_type needle 文本型
// @param_name start 起始搜寻位置
// @param_type start 整数型
std::int32_t CoreFindTextReverse(const std::string &value, const std::string &needle, std::int32_t start);
// @name 倒找文本
// @description 指定大小写规则且省略起始位置时从文本末尾倒找。
// @return_type 整数型
// @param_name value 被搜寻的文本
// @param_type value 文本型
// @param_name needle 欲寻找的文本
// @param_type needle 文本型
// @param_name ignore_case 是否不区分大小写
// @param_type ignore_case 逻辑型
std::int32_t CoreFindTextReverse(const std::string &value, const std::string &needle, bool ignore_case);
// @name 到大写
// @description 按当前 C++ 区域设置把文本中的大小写字符转换为大写。
// @return_type 文本型
// @param_name value 欲变换的文本
// @param_type value 文本型
// @param value 指定欲变换的文本。
std::string CoreUpper(const std::string &value);
// @name 到小写
// @description 按当前 C++ 区域设置把文本中的大小写字符转换为小写。
// @return_type 文本型
// @param_name value 欲变换的文本
// @param_type value 文本型
// @param value 指定欲变换的文本。
std::string CoreLower(const std::string &value);
// @name 到全角
// @description 将 ASCII 空格及可打印字符转换为对应的全角 Unicode 字符。
// @return_type 文本型
// @param_name value 欲变换的文本
// @param_type value 文本型
// @param value 指定欲变换的文本。
std::string CoreToFullWidth(const std::string &value);
// @name 到半角
// @description 将全角空格及全角 ASCII 字符转换为半角字符。
// @return_type 文本型
// @param_name value 欲变换的文本
// @param_type value 文本型
// @param value 指定欲变换的文本。
std::string CoreToHalfWidth(const std::string &value);
// @name 到文本
// @description 返回通用值的稳定文本表示，支持核心数值、逻辑和文本类型。
// @return_type 文本型
// @param_name value 待转换的数据
// @param_type value 通用型
// @param value 提供参数数据时可以同时提供数组或非数组数据。参数值只能为数值、逻辑值、字节集、日期时间或数值型数组。
std::string CoreToText(const std::any &value);
// @name 删首空
// @description 删除文本开头的空白字符。
// @return_type 文本型
// @param_name value 欲删除空格的文本
// @param_type value 文本型
// @param value 指定欲删除空格的文本。
std::string CoreTrimLeft(const std::string &value);
// @name 删尾空
// @description 删除文本末尾的空白字符。
// @return_type 文本型
// @param_name value 欲删除空格的文本
// @param_type value 文本型
// @param value 指定欲删除空格的文本。
std::string CoreTrimRight(const std::string &value);
// @name 删首尾空
// @description 删除文本首尾的空白字符。
// @return_type 文本型
// @param_name value 欲删除空格的文本
// @param_type value 文本型
// @param value 指定欲删除空格的文本。
std::string CoreTrim(const std::string &value);
// @name 删全部空
// @description 删除文本中的全部空白字符。
// @return_type 文本型
// @param_name value 欲删除空格的文本
// @param_type value 文本型
// @param value 指定欲删除空格的文本。
std::string CoreRemoveWhitespace(const std::string &value);
// @name 文本替换
// @description 从以 1 为起点的位置替换指定字符数量。
// @return_type 文本型
// @param_name value 欲被替换的文本
// @param_type value 文本型
// @param value 指定欲被替换的文本。
// @param_name start 起始替换位置
// @param_type start 整数型
// @param start 替换的起始位置，1为首位置，2为第2个位置，如此类推。
// @param_name length 替换长度
// @param_type length 整数型
// @param length 指定替换长度。
// @param_name replacement 用作替换的文本
// @param_type replacement 文本型
// @param replacement 可以被省略。如果本参数被省略，则删除文本中的指定部分。
std::string CoreReplaceText(const std::string &value, std::int32_t start, std::int32_t length,
                              const std::string &replacement = "");
// @name 子文本替换
// @description 从指定位置开始替换子文本，可限制次数；次数小于等于零表示全部替换。
// @return_type 文本型
// @param_name value 欲被替换的文本
// @param_type value 文本型
// @param value 指定欲被替换的文本。
// @param_name needle 欲被替换的子文本
// @param_type needle 文本型
// @param needle 指定欲被替换的子文本。
// @param_name replacement 用作替换的子文本
// @param_type replacement 文本型
// @param replacement 可以被省略。如果本参数被省略，默认为空文本。
// @param_name start 进行替换的起始位置
// @param_type start 整数型
// @param start 可以被省略。参数值指定被替换子文本的起始搜索位置。如果省略，默认从 1 开始。
// @param_name count 替换进行的次数
// @param_type count 整数型
// @param count 可以被省略。参数值指定对子文本进行替换的次数。如果省略，默认进行所有可能的替换。
// @param_name case_sensitive 是否区分大小写
// @param_type case_sensitive 逻辑型
// @param case_sensitive 初始值为“真”。为真区分大小写，为假不区分。
std::string CoreReplaceSubstring(const std::string &value, const std::string &needle, const std::string &replacement,
                                   std::int32_t start, std::int32_t count, bool case_sensitive);
// @name 子文本替换
// @description 使用区分大小写的默认方式替换全部匹配文本。
// @return_type 文本型
// @param_name value 欲被替换的文本
// @param_type value 文本型
// @param_name needle 欲被替换的子文本
// @param_type needle 文本型
std::string CoreReplaceSubstring(const std::string &value, const std::string &needle);
// @name 子文本替换
// @description 指定替换文本，其余参数使用默认值。
// @return_type 文本型
// @param_name value 欲被替换的文本
// @param_type value 文本型
// @param_name needle 欲被替换的子文本
// @param_type needle 文本型
// @param_name replacement 用作替换的子文本
// @param_type replacement 文本型
std::string CoreReplaceSubstring(const std::string &value, const std::string &needle,
                                   const std::string &replacement);
// @name 子文本替换
// @description 指定起始位置，其余参数使用默认值。
// @return_type 文本型
// @param_name value 欲被替换的文本
// @param_type value 文本型
// @param_name needle 欲被替换的子文本
// @param_type needle 文本型
// @param_name replacement 用作替换的子文本
// @param_type replacement 文本型
// @param_name start 进行替换的起始位置
// @param_type start 整数型
std::string CoreReplaceSubstring(const std::string &value, const std::string &needle,
                                   const std::string &replacement, std::int32_t start);
// @name 子文本替换
// @description 指定替换次数，是否区分大小写保持默认值真。
// @return_type 文本型
// @param_name value 欲被替换的文本
// @param_type value 文本型
// @param_name needle 欲被替换的子文本
// @param_type needle 文本型
// @param_name replacement 用作替换的子文本
// @param_type replacement 文本型
// @param_name start 进行替换的起始位置
// @param_type start 整数型
// @param_name count 替换进行的次数
// @param_type count 整数型
std::string CoreReplaceSubstring(const std::string &value, const std::string &needle,
                                   const std::string &replacement, std::int32_t start, std::int32_t count);
// @name 取空白文本
// @description 返回由指定数量 ASCII 空格组成的文本。
// @return_type 文本型
// @param_name count 重复次数
// @param_type count 整数型
// @param count 指定重复次数。
std::string CoreSpaces(std::int32_t count);
// @name 取重复文本
// @description 返回指定文本重复若干次后的结果。
// @return_type 文本型
// @param_name count 重复次数
// @param_type count 整数型
// @param count 指定重复次数。
// @param_name value 待重复文本
// @param_type value 文本型
// @param value 该文本将用于建立返回的文本。如果为空，将返回一个空文本。
std::string CoreRepeatText(std::int32_t count, const std::string &value);
// @name 文本比较
// @description 比较两个文本，小于、等于或大于时分别返回 -1、0 或 1。
// @return_type 整数型
// @param_name left 待比较文本一
// @param_type left 文本型
// @param left 指定待比较文本一。
// @param_name right 待比较文本二
// @param_type right 文本型
// @param right 指定待比较文本二。
// @param_name case_sensitive 是否区分大小写
// @param_type case_sensitive 逻辑型
// @param case_sensitive 初始值为“真”。为真区分大小写，为假不区分。
std::int32_t CoreCompareText(const std::string &left, const std::string &right, bool case_sensitive);
// @name 分割文本
// @description 按分隔文本切分字符串；限制小于等于零时返回全部结果。
// @return_type 文本型数组
// @param_name value 待分割文本
// @param_type value 文本型
// @param value 如果参数值是一个长度为零的文本，则返回一个空数组，即没有任何成员的数组。
// @param_name separator 用作分割的文本
// @param_type separator 文本型
// @param separator
// 可以被省略。参数值用于标识子文本边界。如果被省略，则默认使用半角逗号字符作为分隔符。如果是一个长度为零的文本，则返回的数组仅包含一个成员，即完整的“待分割文本”。
// @param_name limit 要返回的子文本数目
// @param_type limit 整数型
// @param limit 可以被省略。如果被省略，则默认返回所有的子文本。
std::vector<std::string> CoreSplitText(const std::string &value, const std::string &separator = ",", std::int32_t limit = 0);
// @name 文本到UTF8
// @description 返回文本原始 UTF-8 字节。
// @return_type 字节集
// @param_name value 待转换的文本
// @param_type value 文本型
// @param value 提供待转换到UTF8格式的文本。
sulang::core::ByteArray CoreTextToUtf8(const std::string &value);
// @name UTF8到文本
// @description 校验并返回 UTF-8 字节对应的文本，无效序列使用替换字符。
// @return_type 文本型
// @param_name value 待转换的UTF8文本数据
// @param_type value 字节集
// @param value 提供待转换到通常文本的UTF8文本数据。
std::string CoreUtf8ToText(const sulang::core::ByteArray &value);
// @name 文本到UTF16
// @description 将 UTF-8 文本转换为小端 UTF-16 字节，不附加结束零。
// @return_type 字节集
// @param_name value 待转换的文本
// @param_type value 文本型
// @param value 提供待转换到UTF16格式的文本。
sulang::core::ByteArray CoreTextToUtf16(const std::string &value);
// @name UTF16到文本
// @description 将小端 UTF-16 字节转换为 UTF-8 文本，末尾孤立字节会被忽略。
// @return_type 文本型
// @param_name value 待转换的UTF16文本数据
// @param_type value 字节集
// @param value 提供待转换到通常文本的UTF16文本数据。
std::string CoreUtf16ToText(const sulang::core::ByteArray &value);
