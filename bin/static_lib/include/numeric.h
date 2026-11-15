#pragma once
#include "types.h"

// @name 到数值
// @description 将文本或核心数值类型转换为双精度数值，转换失败时抛出标准异常。
// @return_type 双精度小数型
// @param_name value 待转换的文本或数值
// @param_type value 通用型
// @param value 指定待转换的文本或数值。
long double CoreToNumber(const std::any &value);
// @name 数值到大写
// @description 将整数部分转换为中文大写数字，支持简体和繁体字形。
// @return_type 文本型
// @param_name value 欲转换形式的数值
// @param_type value 双精度小数型
// @param value 指定欲转换形式的数值。
// @param_name simplified 是否转换为简体
// @param_type simplified 逻辑型
// @param simplified 初始值为“假”。如果参数值为假，则转换为繁体。
std::string CoreNumberToChinese(long double value, bool simplified);
// @name 数值到金额
// @description 将数值按分四舍五入并转换为中文大写金额。
// @return_type 文本型
// @param_name value 欲转换形式的数值
// @param_type value 双精度小数型
// @param value 指定欲转换形式的数值。
// @param_name simplified 是否转换为简体
// @param_type simplified 逻辑型
// @param simplified 初始值为“假”。如果参数值为假，则转换为繁体。
std::string CoreNumberToMoney(long double value, bool simplified);
// @name 数值到格式文本
// @description 按指定小数位四舍五入，可增加千分位分隔；小数位小于零时向整数高位舍入。
// @return_type 文本型
// @param_name value 欲转换为文本的数值
// @param_type value 双精度小数型
// @param value 指定欲转换为文本的数值。
// @param_name decimals 小数保留位数
// @param_type decimals 整数型
// @param decimals
// 可以被省略。如果大于0，表示小数点右边应四舍五入保留的位数；如果等于0，表示舍入到整数；如果小于0，表示小数点左边舍入到的位置。例如：数值到格式文本
// (1056.65, 1, 假) 返回 “1056.7”； 数值到格式文本 (1056.65, 0, 假) 返回 “1057”； 数值到格式文本 (1056.65, -1, 假) 返回
// “1060”。如果省略本参数，则默认为保留所有实际存在的小数位。
// @param_name thousands 是否进行千分位分隔
// @param_type thousands 逻辑型
// @param thousands 初始值为“假”。如果参数值为真，数值文本的每个千分位上都将被自动插入一个逗号进行分隔。
std::string CoreFormatNumber(long double value, std::int32_t decimals, bool thousands = false);
// @name 数值到格式文本
// @description 省略小数位时保留输入数值的有效小数位。
// @return_type 文本型
// @param_name value 欲转换为文本的数值
// @param_type value 双精度小数型
std::string CoreFormatNumber(long double value);
// @name 数值到格式文本
// @description 省略小数位但指定千分位分隔时，保留有效小数位。
// @return_type 文本型
// @param_name value 欲转换为文本的数值
// @param_type value 双精度小数型
// @param_name thousands 是否进行千分位分隔
// @param_type thousands 逻辑型
std::string CoreFormatNumber(long double value, bool thousands);
// @name 取十六进制文本
// @description 返回整数的大写十六进制文本，不添加前缀。
// @return_type 文本型
// @param_name value 欲取进制文本的数值
// @param_type value 整数型
// @param value 指定欲取进制文本的数值。
std::string CoreHexText(std::int32_t value);
// @name 取八进制文本
// @description 返回整数的八进制文本，不添加前缀。
// @return_type 文本型
// @param_name value 欲取进制文本的数值
// @param_type value 整数型
// @param value 指定欲取进制文本的数值。
std::string CoreOctalText(std::int32_t value);
// @name 到字节
// @description 转换为 8 位无符号整数，超出范围时抛出异常。
// @return_type 字节型
// @param_name value 待转换的文本或数值
// @param_type value 通用型
// @param value 指定待转换的文本或数值。
std::uint8_t CoreToByte(const std::any &value);
// @name 到短整数
// @description 转换为 16 位有符号整数，超出范围时抛出异常。
// @return_type 短整数型
// @param_name value 待转换的文本或数值
// @param_type value 通用型
// @param value 指定待转换的文本或数值。
std::int16_t CoreToShort(const std::any &value);
// @name 到整数
// @description 转换为 32 位有符号整数，超出范围时抛出异常。
// @return_type 整数型
// @param_name value 待转换的文本或数值
// @param_type value 通用型
// @param value 指定待转换的文本或数值。
std::int32_t CoreToInt(const std::any &value);
// @name 到长整数
// @description 转换为 64 位有符号整数，超出范围时抛出异常。
// @return_type 长整数型
// @param_name value 待转换的文本或数值
// @param_type value 通用型
// @param value 指定待转换的文本或数值。
std::int64_t CoreToLong(const std::any &value);
// @name 到小数
// @description 转换为双精度浮点数。
// @return_type 小数型
// @param_name value 待转换的文本或数值
// @param_type value 通用型
// @param value 指定待转换的文本或数值。
double CoreToFloat(const std::any &value);
// @name 十六进制
// @description 解析可带 0x 前缀的十六进制文本，非法文本会抛出异常。
// @return_type 整数型
// @param_name value 十六进制文本常量
// @param_type value 文本型
// @param value 本参数指定欲转换为整数的十六进制文本常量。
std::int32_t CoreParseHex(const std::string &value);
// @name 二进制
// @description 解析可带 0b 前缀的二进制文本，非法文本会抛出异常。
// @return_type 整数型
// @param_name value 二进制文本常量
// @param_type value 文本型
// @param value 本参数指定欲转换为整数的二进制文本常量。
std::int32_t CoreParseBinary(const std::string &value);
// @name 反转整数字节序
// @description 反转 32 位整数的四个字节。
// @return_type 整数型
// @param_name value 待反转的整数值
// @param_type value 整数型
// @param value 提供所欲反转其字节序的整数值。
std::int32_t CoreReverseIntBytes(std::int32_t value);
