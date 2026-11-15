#include "numeric.h"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <typeinfo>

namespace
{
long double any_number(const std::any &value)
{
    if (value.type() == typeid(std::string))
        return std::stold(std::any_cast<std::string>(value));
    if (value.type() == typeid(const char *))
        return std::stold(std::any_cast<const char *>(value));
    if (value.type() == typeid(bool))
        return std::any_cast<bool>(value) ? 1 : 0;
    if (value.type() == typeid(std::uint8_t))
        return std::any_cast<std::uint8_t>(value);
    if (value.type() == typeid(std::int16_t))
        return std::any_cast<std::int16_t>(value);
    if (value.type() == typeid(std::int32_t))
        return std::any_cast<std::int32_t>(value);
    if (value.type() == typeid(int))
        return std::any_cast<int>(value);
    if (value.type() == typeid(std::int64_t))
        return std::any_cast<std::int64_t>(value);
    if (value.type() == typeid(float))
        return std::any_cast<float>(value);
    if (value.type() == typeid(double))
        return std::any_cast<double>(value);
    if (value.type() == typeid(long double))
        return std::any_cast<long double>(value);
    throw std::invalid_argument("通用值不是可转换数值类型");
}
template <class T> T checked_integral(const std::any &value)
{
    const auto number = any_number(value);
    if (!std::isfinite(number) || number < static_cast<long double>(std::numeric_limits<T>::min()) ||
        number > static_cast<long double>(std::numeric_limits<T>::max()))
        throw std::out_of_range("数值超出目标整数范围");
    return static_cast<T>(number);
}
std::string integer_chinese(std::uint64_t value, bool simplified)
{
    const char *digits[] = {"零", "壹", "贰", "叁", "肆", "伍", "陆", "柒", "捌", "玖"};
    const char *units[] = {"", "拾", "佰", "仟"};
    const char *groups[] = {"", "万", "亿", "兆"};
    if (value == 0)
        return digits[0];
    std::string result;
    bool pending_zero = false;
    for (int group = 0; value > 0 && group < 4; ++group)
    {
        const auto block = static_cast<unsigned>(value % 10000);
        value /= 10000;
        std::string part;
        bool zero = false;
        unsigned divisor = 1000;
        for (int position = 3; position >= 0; --position, divisor /= 10)
        {
            const auto digit = (block / divisor) % 10;
            if (digit == 0)
            {
                if (!part.empty())
                    zero = true;
            }
            else
            {
                if (zero)
                    part += digits[0];
                part += digits[digit];
                part += units[position];
                zero = false;
            }
        }
        if (!part.empty())
        {
            if (pending_zero && !result.empty() && result.rfind(digits[0], 0) != 0)
                result = std::string(digits[0]) + result;
            result = part + groups[group] + result;
        }
        pending_zero = block < 1000 && value > 0;
    }
    if (!simplified)
    {
        const std::pair<const char *, const char *> replacements[] = {{"万", "萬"}, {"亿", "億"}, {"陆", "陸"}};
        for (const auto &[from, to] : replacements)
        {
            std::size_t position = 0;
            while ((position = result.find(from, position)) != std::string::npos)
            {
                result.replace(position, std::char_traits<char>::length(from), to);
                position += std::char_traits<char>::length(to);
            }
        }
    }
    return result;
}
} // namespace

long double CoreToNumber(const std::any &value)
{
    return any_number(value);
}
std::string CoreNumberToChinese(long double value, bool simplified)
{
    const bool negative = value < 0;
    const auto absolute = static_cast<std::uint64_t>(std::llround(std::fabs(value)));
    return std::string(negative ? "负" : "") + integer_chinese(absolute, simplified);
}
std::string CoreNumberToMoney(long double value, bool simplified)
{
    const bool negative = value < 0;
    const auto cents = static_cast<std::uint64_t>(std::llround(std::fabs(value) * 100.0L));
    const auto yuan = cents / 100;
    const auto jiao = static_cast<unsigned>((cents / 10) % 10);
    const auto fen = static_cast<unsigned>(cents % 10);
    const char *digits[] = {"零", "壹", "贰", "叁", "肆", "伍", "陆", "柒", "捌", "玖"};
    std::string result = std::string(negative ? "负" : "") + integer_chinese(yuan, simplified) + "元";
    if (jiao == 0 && fen == 0)
        result += "整";
    else
    {
        if (jiao)
            result += std::string(digits[jiao]) + "角";
        if (fen)
            result += std::string(digits[fen]) + "分";
    }
    return result;
}
std::string CoreFormatNumber(long double value, std::int32_t decimals, bool thousands)
{
    const long double factor = std::pow(10.0L, static_cast<long double>(decimals));
    const long double rounded = decimals >= 0
                                    ? std::round(value * factor) / factor
                                    : std::round(value / std::pow(10.0L, -decimals)) * std::pow(10.0L, -decimals);
    std::ostringstream stream;
    if (decimals >= 0)
        stream << std::fixed << std::setprecision(decimals);
    stream << rounded;
    auto result = stream.str();
    if (!thousands)
        return result;
    const auto point = result.find('.');
    std::size_t end = point == std::string::npos ? result.size() : point;
    const std::size_t begin = !result.empty() && (result[0] == '-' || result[0] == '+') ? 1 : 0;
    for (std::size_t position = end; position > begin + 3; position -= 3)
        result.insert(position - 3, 1, ',');
    return result;
}

std::string CoreFormatNumber(long double value)
{
    std::ostringstream stream;
    stream << std::fixed << std::setprecision(std::numeric_limits<double>::digits10) << value;
    auto result = stream.str();
    const auto point = result.find('.');
    if (point != std::string::npos)
    {
        while (result.size() > point + 1 && result.back() == '0')
            result.pop_back();
        if (!result.empty() && result.back() == '.')
            result.pop_back();
    }
    return result;
}

std::string CoreFormatNumber(long double value, bool thousands)
{
    auto result = CoreFormatNumber(value);
    if (!thousands)
        return result;
    const auto point = result.find('.');
    const auto end = point == std::string::npos ? result.size() : point;
    const std::size_t begin = !result.empty() && (result[0] == '-' || result[0] == '+') ? 1 : 0;
    for (std::size_t position = end; position > begin + 3; position -= 3)
        result.insert(position - 3, 1, ',');
    return result;
}
std::string CoreHexText(std::int32_t value)
{
    std::ostringstream stream;
    stream << std::uppercase << std::hex << static_cast<std::uint32_t>(value);
    return stream.str();
}
std::string CoreOctalText(std::int32_t value)
{
    std::ostringstream stream;
    stream << std::oct << static_cast<std::uint32_t>(value);
    return stream.str();
}
std::uint8_t CoreToByte(const std::any &value)
{
    return checked_integral<std::uint8_t>(value);
}
std::int16_t CoreToShort(const std::any &value)
{
    return checked_integral<std::int16_t>(value);
}
std::int32_t CoreToInt(const std::any &value)
{
    return checked_integral<std::int32_t>(value);
}
std::int64_t CoreToLong(const std::any &value)
{
    return checked_integral<std::int64_t>(value);
}
double CoreToFloat(const std::any &value)
{
    return static_cast<double>(any_number(value));
}
std::int32_t CoreParseHex(const std::string &value)
{
    std::size_t used = 0;
    const auto result = std::stoll(value, &used, 16);
    if (used != value.size())
        throw std::invalid_argument("十六进制文本包含非法字符");
    return static_cast<std::int32_t>(result);
}
std::int32_t CoreParseBinary(const std::string &value)
{
    const auto text = value.rfind("0b", 0) == 0 || value.rfind("0B", 0) == 0 ? value.substr(2) : value;
    std::size_t used = 0;
    const auto result = std::stoll(text, &used, 2);
    if (used != text.size())
        throw std::invalid_argument("二进制文本包含非法字符");
    return static_cast<std::int32_t>(result);
}
std::int32_t CoreReverseIntBytes(std::int32_t value)
{
    const auto source = static_cast<std::uint32_t>(value);
    const auto result = ((source & 0x000000FFu) << 24) | ((source & 0x0000FF00u) << 8) | ((source & 0x00FF0000u) >> 8) |
                        ((source & 0xFF000000u) >> 24);
    return static_cast<std::int32_t>(result);
}
