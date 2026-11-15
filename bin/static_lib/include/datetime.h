#pragma once
#include "types.h"

// @name 到时间
// @description 解析 YYYY-MM-DD、YYYY-MM-DD HH:MM:SS 或 ISO 8601 基本日期时间文本。
// @return_type 日期时间型
// @param_name value 欲转换的文本
// @param_type value 文本型
// @param value 文本内容应按以下格式之一提供，年份后的时间部分可以省略：
sulang::core::DateTime CoreToTime(const std::string &value);
// @name 增减时间
// @description 按年份、季度、月份、周、日、小时、分钟或秒增减时间。
// @return_type 日期时间型
// @param_name value 时间
// @param_type value 日期时间型
// @param value 指定时间。
// @param_name part 被增加部分
// @param_type part 整数型
// @param part 参数值指定增加或减少时间的哪一部分，可以为以下常量： 1、#年份； 2、#季度； 3、#月份； 4、#周； 5、#日；
// 6、#小时； 7、#分钟； 8、#秒。
// @param_name amount 增加值
// @param_type amount 整数型
// @param amount 相对于被增加部分的增加或减少数值。
sulang::core::DateTime CoreChangeTime(sulang::core::DateTime value, std::int32_t part, std::int32_t amount);
// @name 取时间间隔
// @description 返回“时间1减时间2”在指定单位上的有符号间隔。
// @return_type 双精度小数型
// @param_name first 时间1
// @param_type first 日期时间型
// @param first 指定时间1。
// @param_name second 时间2
// @param_type second 日期时间型
// @param second 指定时间2。
// @param_name part 取间隔部分
// @param_type part 整数型
// @param part 参数值指定取时间间隔的单位，可以为以下常量： 1、#年份； 2、#季度； 3、#月份； 4、#周； 5、#日；
// 6、#小时； 7、#分钟； 8、#秒。
long double CoreTimeDifference(sulang::core::DateTime first, sulang::core::DateTime second, std::int32_t part);
// @name 取某月天数
// @description 返回指定年月的天数；月份无效时返回 0。
// @return_type 整数型
// @param_name year 年份
// @param_type year 整数型
// @param year 参数值从 100 到 9999。
// @param_name month 月份
// @param_type month 整数型
// @param month 参数值从 1 到 12。
std::int32_t CoreDaysInMonth(std::int32_t year, std::int32_t month);
// @name 时间到文本
// @description 将时间格式化为完整日期时间、日期或时间文本。
// @return_type 文本型
// @param_name value 欲转换到文本的时间
// @param_type value 日期时间型
// @param value 指定欲转换到文本的时间。
// @param_name part 转换部分
// @param_type part 整数型
// @param part 可以被省略。参数值可以为以下常量： 1、#全部转换； 2、#日期部分；
// 3、#时间部分。如果省略了本参数，默认为“#全部转换”。
std::string CoreTimeToText(sulang::core::DateTime value, std::int32_t part = 1);
// @name 取时间部分
// @description 返回年份、季度、月份、周、日、星期、小时、分钟或秒。
// @return_type 整数型
// @param_name value 欲取其部分的时间
// @param_type value 日期时间型
// @param value 指定欲取其部分的时间。
// @param_name part 欲取的时间部分
// @param_type part 整数型
// @param part 参数值可以为以下常量： 1、#年份； 2、#季度； 3、#月份； 4、#自年首周数； 5、#日； 6、#小时； 7、#分钟；
// 8、#秒； 9、#星期几； 10、#自年首天数。其中：自年首周数、自年首天数均从 1 开始。
std::int32_t CoreTimePart(sulang::core::DateTime value, std::int32_t part);
// @name 取年份
// @description 返回本地时间中的四位年份。
// @return_type 整数型
// @param_name value 时间
// @param_type value 日期时间型
// @param value 指定时间。
std::int32_t CoreYear(sulang::core::DateTime value);
// @name 取月份
// @description 返回本地时间中的月份，范围为 1 至 12。
// @return_type 整数型
// @param_name value 时间
// @param_type value 日期时间型
// @param value 指定时间。
std::int32_t CoreMonth(sulang::core::DateTime value);
// @name 取日
// @description 返回本地时间中的月内日期。
// @return_type 整数型
// @param_name value 时间
// @param_type value 日期时间型
// @param value 指定时间。
std::int32_t CoreDay(sulang::core::DateTime value);
// @name 取星期几
// @description 返回 1 到 7，其中 1 表示星期日。
// @return_type 整数型
// @param_name value 时间
// @param_type value 日期时间型
// @param value 指定时间。
std::int32_t CoreWeekday(sulang::core::DateTime value);
// @name 取小时
// @description 返回本地时间中的小时，范围为 0 至 23。
// @return_type 整数型
// @param_name value 时间
// @param_type value 日期时间型
// @param value 指定时间。
std::int32_t CoreHour(sulang::core::DateTime value);
// @name 取分钟
// @description 返回本地时间中的分钟，范围为 0 至 59。
// @return_type 整数型
// @param_name value 时间
// @param_type value 日期时间型
// @param value 指定时间。
std::int32_t CoreMinute(sulang::core::DateTime value);
// @name 取秒
// @description 返回本地时间中的秒，范围为 0 至 60。
// @return_type 整数型
// @param_name value 时间
// @param_type value 日期时间型
// @param value 指定时间。
std::int32_t CoreSecond(sulang::core::DateTime value);
// @name 指定时间
// @description 创建本地时区时间，非法日期会抛出异常。
// @return_type 日期时间型
// @param_name year 年
// @param_type year 整数型
// @param year 指定年。
// @param_name month 月
// @param_type month 整数型
// @param month 可以被省略。如果本参数被省略，默认为 1 月。
// @param_name day 日
// @param_type day 整数型
// @param day 可以被省略。如果本参数被省略，默认为 1 日。
// @param_name hour 小时
// @param_type hour 整数型
// @param hour 可以被省略。如果本参数被省略，默认为 0 时。
// @param_name minute 分钟
// @param_type minute 整数型
// @param minute 可以被省略。如果本参数被省略，默认为 0 分钟。
// @param_name second 秒
// @param_type second 整数型
// @param second 可以被省略。如果本参数被省略，默认为 0 秒。
sulang::core::DateTime CoreMakeTime(std::int32_t year, std::int32_t month = 1, std::int32_t day = 1, std::int32_t hour = 0,
                                      std::int32_t minute = 0, std::int32_t second = 0);
// @name 取现行时间
// @description 返回系统当前时间。
// @return_type 日期时间型
sulang::core::DateTime CoreNow();
// @name 置现行时间
// @description 将系统本地时间设置为指定值；权限不足或时间超出 Windows 可表示范围时返回假。
// @return_type 逻辑型
// @param_name value 欲设置的时间
// @param_type value 日期时间型
// @param value 指定要写入系统时钟的本地日期和时间。
bool CoreSetCurrentTime(sulang::core::DateTime value);
// @name 取日期
// @description 返回同一天零时的时间值。
// @return_type 日期时间型
// @param_name value 时间
// @param_type value 日期时间型
// @param value 指定时间。
sulang::core::DateTime CoreDatePart(sulang::core::DateTime value);
// @name 取时间
// @description 返回从当天零时开始的时分秒时长，日期部分使用 Unix 纪元日期承载。
// @return_type 日期时间型
// @param_name value 时间
// @param_type value 日期时间型
// @param value 指定时间。
sulang::core::DateTime CoreClockPart(sulang::core::DateTime value);
