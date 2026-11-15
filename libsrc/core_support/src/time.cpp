#include "datetime.h"

#include <array>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <windows.h>

namespace
{
std::tm local_time(sulang::core::DateTime value)
{
    const auto raw = std::chrono::system_clock::to_time_t(value);
    std::tm result{};
#ifdef _WIN32
    localtime_s(&result, &raw);
#else
    localtime_r(&raw, &result);
#endif
    return result;
}
sulang::core::DateTime from_tm(std::tm value)
{
    const auto raw = std::mktime(&value);
    if (raw == static_cast<std::time_t>(-1))
        throw std::invalid_argument("日期时间无效");
    return std::chrono::system_clock::from_time_t(raw);
}
std::int32_t part_value(const std::tm &value, std::int32_t part)
{
    switch (part)
    {
    case 1:
        return value.tm_year + 1900;
    case 2:
        return value.tm_mon / 3 + 1;
    case 3:
        return value.tm_mon + 1;
    case 4:
        return value.tm_yday / 7 + 1;
    case 5:
        return value.tm_mday;
    case 6:
        return value.tm_hour;
    case 7:
        return value.tm_min;
    case 8:
        return value.tm_sec;
    case 9:
        return value.tm_wday + 1;
    case 10:
        return value.tm_yday + 1;
    default:
        return 0;
    }
}
} // namespace

sulang::core::DateTime CoreToTime(const std::string &value)
{
    std::tm parsed{};
    std::istringstream stream(value);
    stream >> std::get_time(&parsed, value.find('T') != std::string::npos   ? "%Y-%m-%dT%H:%M:%S"
                                     : value.find(':') != std::string::npos ? "%Y-%m-%d %H:%M:%S"
                                                                            : "%Y-%m-%d");
    if (stream.fail())
        throw std::invalid_argument("无法解析日期时间文本");
    parsed.tm_isdst = -1;
    return from_tm(parsed);
}
sulang::core::DateTime CoreChangeTime(sulang::core::DateTime value, std::int32_t part, std::int32_t amount)
{
    if (part >= 1 && part <= 3)
    {
        auto item = local_time(value);
        if (part == 1)
            item.tm_year += amount;
        else if (part == 2)
            item.tm_mon += amount * 3;
        else
            item.tm_mon += amount;
        item.tm_isdst = -1;
        return from_tm(item);
    }
    const std::array<long long, 9> seconds = {0, 0, 0, 0, 7 * 86400, 86400, 3600, 60, 1};
    if (part < 4 || part > 8)
        return value;
    return value + std::chrono::seconds(seconds[static_cast<std::size_t>(part)] * amount);
}
long double CoreTimeDifference(sulang::core::DateTime first, sulang::core::DateTime second, std::int32_t part)
{
    const auto seconds = std::chrono::duration<long double>(first - second).count();
    switch (part)
    {
    case 1:
        return seconds / (365.2425L * 86400);
    case 2:
        return seconds / (365.2425L / 4 * 86400);
    case 3:
        return seconds / (365.2425L / 12 * 86400);
    case 4:
        return seconds / (7 * 86400);
    case 5:
        return seconds / 86400;
    case 6:
        return seconds / 3600;
    case 7:
        return seconds / 60;
    default:
        return seconds;
    }
}
std::int32_t CoreDaysInMonth(std::int32_t year, std::int32_t month)
{
    if (month < 1 || month > 12)
        return 0;
    static constexpr std::array<int, 12> days = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month != 2)
        return days[static_cast<std::size_t>(month - 1)];
    return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) ? 29 : 28;
}
std::string CoreTimeToText(sulang::core::DateTime value, std::int32_t part)
{
    const auto item = local_time(value);
    std::ostringstream stream;
    stream << std::put_time(&item, part == 2 ? "%Y-%m-%d" : part == 3 ? "%H:%M:%S" : "%Y-%m-%d %H:%M:%S");
    return stream.str();
}
std::int32_t CoreTimePart(sulang::core::DateTime value, std::int32_t part)
{
    return part_value(local_time(value), part);
}
std::int32_t CoreYear(sulang::core::DateTime value)
{
    return part_value(local_time(value), 1);
}
std::int32_t CoreMonth(sulang::core::DateTime value)
{
    return part_value(local_time(value), 3);
}
std::int32_t CoreDay(sulang::core::DateTime value)
{
    return part_value(local_time(value), 5);
}
std::int32_t CoreWeekday(sulang::core::DateTime value)
{
    return part_value(local_time(value), 9);
}
std::int32_t CoreHour(sulang::core::DateTime value)
{
    return part_value(local_time(value), 6);
}
std::int32_t CoreMinute(sulang::core::DateTime value)
{
    return part_value(local_time(value), 7);
}
std::int32_t CoreSecond(sulang::core::DateTime value)
{
    return part_value(local_time(value), 8);
}
sulang::core::DateTime CoreMakeTime(std::int32_t year, std::int32_t month, std::int32_t day, std::int32_t hour,
                                      std::int32_t minute, std::int32_t second)
{
    std::tm item{};
    item.tm_year = year - 1900;
    item.tm_mon = month - 1;
    item.tm_mday = day;
    item.tm_hour = hour;
    item.tm_min = minute;
    item.tm_sec = second;
    item.tm_isdst = -1;
    const auto result = from_tm(item);
    const auto checked = local_time(result);
    if (checked.tm_year != year - 1900 || checked.tm_mon != month - 1 || checked.tm_mday != day ||
        checked.tm_hour != hour || checked.tm_min != minute || checked.tm_sec != second)
        throw std::invalid_argument("日期时间超出有效范围");
    return result;
}
sulang::core::DateTime CoreNow()
{
    return std::chrono::system_clock::now();
}
bool CoreSetCurrentTime(sulang::core::DateTime value)
{
    const auto item = local_time(value);
    const auto year = item.tm_year + 1900;
    if (year < 1601 || year > 30827)
        return false;
    SYSTEMTIME target{};
    target.wYear = static_cast<WORD>(year);
    target.wMonth = static_cast<WORD>(item.tm_mon + 1);
    target.wDay = static_cast<WORD>(item.tm_mday);
    target.wHour = static_cast<WORD>(item.tm_hour);
    target.wMinute = static_cast<WORD>(item.tm_min);
    target.wSecond = static_cast<WORD>(item.tm_sec);
    const auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(value.time_since_epoch()).count();
    target.wMilliseconds = static_cast<WORD>((milliseconds % 1000 + 1000) % 1000);
    return SetLocalTime(&target) != FALSE;
}
sulang::core::DateTime CoreDatePart(sulang::core::DateTime value)
{
    auto item = local_time(value);
    item.tm_hour = item.tm_min = item.tm_sec = 0;
    item.tm_isdst = -1;
    return from_tm(item);
}
sulang::core::DateTime CoreClockPart(sulang::core::DateTime value)
{
    const auto item = local_time(value);
    return CoreMakeTime(2000, 1, 1, item.tm_hour, item.tm_min, item.tm_sec);
}
