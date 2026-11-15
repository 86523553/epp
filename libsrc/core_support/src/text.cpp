#include "text.h"

#include <algorithm>
#include <cctype>
#include <cstring>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <typeinfo>

std::string CorePointerToText(std::uintptr_t address)
{
    if (address == 0)
        throw std::invalid_argument("内存文本指针不能为空");
    return std::string(reinterpret_cast<const char *>(address));
}

namespace
{
std::vector<std::uint32_t> decode_utf8(const std::string &value)
{
    std::vector<std::uint32_t> result;
    for (std::size_t index = 0; index < value.size();)
    {
        const auto first = static_cast<unsigned char>(value[index]);
        std::uint32_t code = 0xFFFD;
        std::size_t length = 1;
        if (first < 0x80)
        {
            code = first;
        }
        else if ((first & 0xE0) == 0xC0 && index + 1 < value.size())
        {
            code = first & 0x1F;
            length = 2;
        }
        else if ((first & 0xF0) == 0xE0 && index + 2 < value.size())
        {
            code = first & 0x0F;
            length = 3;
        }
        else if ((first & 0xF8) == 0xF0 && index + 3 < value.size())
        {
            code = first & 0x07;
            length = 4;
        }
        bool valid = true;
        for (std::size_t offset = 1; valid && offset < length; ++offset)
        {
            const auto next = static_cast<unsigned char>(value[index + offset]);
            if ((next & 0xC0) != 0x80)
            {
                valid = false;
            }
            else
            {
                code = (code << 6) | (next & 0x3F);
            }
        }
        if (!valid || (length == 2 && code < 0x80) || (length == 3 && code < 0x800) ||
            (length == 4 && (code < 0x10000 || code > 0x10FFFF)) || (code >= 0xD800 && code <= 0xDFFF))
        {
            code = 0xFFFD;
            length = 1;
        }
        result.push_back(code);
        index += length;
    }
    return result;
}

void append_utf8(std::string &result, std::uint32_t code)
{
    if (code <= 0x7F)
    {
        result.push_back(static_cast<char>(code));
    }
    else if (code <= 0x7FF)
    {
        result.push_back(static_cast<char>(0xC0 | (code >> 6)));
        result.push_back(static_cast<char>(0x80 | (code & 0x3F)));
    }
    else if (code <= 0xFFFF)
    {
        result.push_back(static_cast<char>(0xE0 | (code >> 12)));
        result.push_back(static_cast<char>(0x80 | ((code >> 6) & 0x3F)));
        result.push_back(static_cast<char>(0x80 | (code & 0x3F)));
    }
    else
    {
        result.push_back(static_cast<char>(0xF0 | (code >> 18)));
        result.push_back(static_cast<char>(0x80 | ((code >> 12) & 0x3F)));
        result.push_back(static_cast<char>(0x80 | ((code >> 6) & 0x3F)));
        result.push_back(static_cast<char>(0x80 | (code & 0x3F)));
    }
}

std::string encode_utf8(const std::vector<std::uint32_t> &codes)
{
    std::string result;
    for (const auto code : codes)
        append_utf8(result, code);
    return result;
}

std::string ascii_case(std::string value, bool upper)
{
    std::transform(value.begin(), value.end(), value.begin(), [upper](unsigned char item) {
        return static_cast<char>(upper ? std::toupper(item) : std::tolower(item));
    });
    return value;
}

std::string normalized_case(const std::string &value, bool ignore_case)
{
    return ignore_case ? ascii_case(value, false) : value;
}

std::size_t byte_offset_for_character(const std::string &value, std::int32_t position)
{
    if (position <= 1)
        return 0;
    std::size_t offset = 0;
    std::int32_t current = 1;
    while (offset < value.size() && current < position)
    {
        const auto first = static_cast<unsigned char>(value[offset]);
        std::size_t length = first < 0x80             ? 1
                             : (first & 0xE0) == 0xC0 ? 2
                             : (first & 0xF0) == 0xE0 ? 3
                             : (first & 0xF8) == 0xF0 ? 4
                                                      : 1;
        offset += std::min(length, value.size() - offset);
        ++current;
    }
    return offset;
}
} // namespace

std::int32_t CoreTextLength(const std::string &value)
{
    return static_cast<std::int32_t>(decode_utf8(value).size());
}

std::string CoreTextLeft(const std::string &value, std::int32_t count)
{
    if (count <= 0)
        return {};
    auto codes = decode_utf8(value);
    codes.resize(std::min<std::size_t>(codes.size(), static_cast<std::size_t>(count)));
    return encode_utf8(codes);
}

std::string CoreTextRight(const std::string &value, std::int32_t count)
{
    if (count <= 0)
        return {};
    const auto codes = decode_utf8(value);
    const auto size = std::min<std::size_t>(codes.size(), static_cast<std::size_t>(count));
    return encode_utf8({codes.end() - static_cast<std::ptrdiff_t>(size), codes.end()});
}

std::string CoreTextMid(const std::string &value, std::int32_t start, std::int32_t count)
{
    if (start <= 0 || count <= 0)
        return {};
    const auto codes = decode_utf8(value);
    const auto begin = std::min<std::size_t>(codes.size(), static_cast<std::size_t>(start - 1));
    const auto end = std::min(codes.size(), begin + static_cast<std::size_t>(count));
    return encode_utf8(
        {codes.begin() + static_cast<std::ptrdiff_t>(begin), codes.begin() + static_cast<std::ptrdiff_t>(end)});
}

std::string CoreCharacter(std::uint8_t code)
{
    return std::string(1, static_cast<char>(code));
}

std::int32_t CoreCharacterCode(const std::string &value, std::int32_t position)
{
    const auto codes = decode_utf8(value);
    if (position <= 0 || static_cast<std::size_t>(position) > codes.size())
        return 0;
    return static_cast<std::int32_t>(codes[static_cast<std::size_t>(position - 1)]);
}

std::int32_t CoreFindText(const std::string &value, const std::string &needle, std::int32_t start, bool ignore_case)
{
    const auto source = normalized_case(value, ignore_case);
    const auto target = normalized_case(needle, ignore_case);
    const auto offset = byte_offset_for_character(source, std::max<std::int32_t>(1, start));
    const auto found = source.find(target, offset);
    if (found == std::string::npos)
        return -1;
    return CoreTextLength(source.substr(0, found)) + 1;
}

std::int32_t CoreFindText(const std::string &value, const std::string &needle)
{
    return CoreFindText(value, needle, 1, false);
}

std::int32_t CoreFindText(const std::string &value, const std::string &needle, std::int32_t start)
{
    return CoreFindText(value, needle, start, false);
}

std::int32_t CoreFindText(const std::string &value, const std::string &needle, bool ignore_case)
{
    return CoreFindText(value, needle, 1, ignore_case);
}

std::int32_t CoreFindTextReverse(const std::string &value, const std::string &needle, std::int32_t start,
                                   bool ignore_case)
{
    const auto source = normalized_case(value, ignore_case);
    const auto target = normalized_case(needle, ignore_case);
    const auto offset = start <= 0 ? std::string::npos : byte_offset_for_character(source, start);
    const auto found = source.rfind(target, offset);
    if (found == std::string::npos)
        return -1;
    return CoreTextLength(source.substr(0, found)) + 1;
}

std::int32_t CoreFindTextReverse(const std::string &value, const std::string &needle)
{
    return CoreFindTextReverse(value, needle, 0, false);
}

std::int32_t CoreFindTextReverse(const std::string &value, const std::string &needle, std::int32_t start)
{
    return CoreFindTextReverse(value, needle, start, false);
}

std::int32_t CoreFindTextReverse(const std::string &value, const std::string &needle, bool ignore_case)
{
    return CoreFindTextReverse(value, needle, 0, ignore_case);
}

std::string CoreUpper(const std::string &value)
{
    return ascii_case(value, true);
}
std::string CoreLower(const std::string &value)
{
    return ascii_case(value, false);
}

std::string CoreToFullWidth(const std::string &value)
{
    auto codes = decode_utf8(value);
    for (auto &code : codes)
    {
        if (code == 0x20)
            code = 0x3000;
        else if (code >= 0x21 && code <= 0x7E)
            code += 0xFEE0;
    }
    return encode_utf8(codes);
}

std::string CoreToHalfWidth(const std::string &value)
{
    auto codes = decode_utf8(value);
    for (auto &code : codes)
    {
        if (code == 0x3000)
            code = 0x20;
        else if (code >= 0xFF01 && code <= 0xFF5E)
            code -= 0xFEE0;
    }
    return encode_utf8(codes);
}

std::string CoreToText(const std::any &value)
{
    if (!value.has_value())
        return {};
    if (value.type() == typeid(std::string))
        return std::any_cast<std::string>(value);
    if (value.type() == typeid(const char *))
        return std::any_cast<const char *>(value);
    if (value.type() == typeid(bool))
        return std::any_cast<bool>(value) ? "真" : "假";
    std::ostringstream stream;
    if (value.type() == typeid(std::uint8_t))
        stream << static_cast<unsigned>(std::any_cast<std::uint8_t>(value));
    else if (value.type() == typeid(std::int16_t))
        stream << std::any_cast<std::int16_t>(value);
    else if (value.type() == typeid(std::int32_t))
        stream << std::any_cast<std::int32_t>(value);
    else if (value.type() == typeid(std::int64_t))
        stream << std::any_cast<std::int64_t>(value);
    else if (value.type() == typeid(float))
        stream << std::any_cast<float>(value);
    else if (value.type() == typeid(double))
        stream << std::any_cast<double>(value);
    else if (value.type() == typeid(long double))
        stream << std::setprecision(18) << std::any_cast<long double>(value);
    else
        return "<不支持的通用型>";
    return stream.str();
}

std::string CoreTrimLeft(const std::string &value)
{
    const auto found =
        std::find_if_not(value.begin(), value.end(), [](unsigned char item) { return std::isspace(item) != 0; });
    return {found, value.end()};
}
std::string CoreTrimRight(const std::string &value)
{
    const auto found = std::find_if_not(value.rbegin(), value.rend(), [](unsigned char item) {
                           return std::isspace(item) != 0;
                       }).base();
    return {value.begin(), found};
}
std::string CoreTrim(const std::string &value)
{
    return CoreTrimRight(CoreTrimLeft(value));
}
std::string CoreRemoveWhitespace(const std::string &value)
{
    std::string result;
    std::copy_if(value.begin(), value.end(), std::back_inserter(result),
                 [](unsigned char item) { return std::isspace(item) == 0; });
    return result;
}

std::string CoreReplaceText(const std::string &value, std::int32_t start, std::int32_t length,
                              const std::string &replacement)
{
    auto codes = decode_utf8(value);
    const auto replacement_codes = decode_utf8(replacement);
    const auto begin = start <= 0 ? 0 : std::min<std::size_t>(codes.size(), static_cast<std::size_t>(start - 1));
    const auto end = std::min(codes.size(), begin + static_cast<std::size_t>(std::max<std::int32_t>(0, length)));
    codes.erase(codes.begin() + static_cast<std::ptrdiff_t>(begin), codes.begin() + static_cast<std::ptrdiff_t>(end));
    codes.insert(codes.begin() + static_cast<std::ptrdiff_t>(begin), replacement_codes.begin(),
                 replacement_codes.end());
    return encode_utf8(codes);
}

std::string CoreReplaceSubstring(const std::string &value, const std::string &needle, const std::string &replacement,
                                   std::int32_t start, std::int32_t count, bool case_sensitive)
{
    if (needle.empty())
        return value;
    std::string result = value;
    std::string search = case_sensitive ? result : ascii_case(result, false);
    const auto target = case_sensitive ? needle : ascii_case(needle, false);
    std::size_t position = byte_offset_for_character(result, std::max<std::int32_t>(1, start));
    std::int32_t replaced = 0;
    while ((position = search.find(target, position)) != std::string::npos && (count <= 0 || replaced < count))
    {
        result.replace(position, needle.size(), replacement);
        search.replace(position, needle.size(), case_sensitive ? replacement : ascii_case(replacement, false));
        position += replacement.size();
        ++replaced;
    }
    return result;
}

std::string CoreReplaceSubstring(const std::string &value, const std::string &needle)
{
    return CoreReplaceSubstring(value, needle, "", 1, 0, true);
}

std::string CoreReplaceSubstring(const std::string &value, const std::string &needle,
                                   const std::string &replacement)
{
    return CoreReplaceSubstring(value, needle, replacement, 1, 0, true);
}

std::string CoreReplaceSubstring(const std::string &value, const std::string &needle,
                                   const std::string &replacement, std::int32_t start)
{
    return CoreReplaceSubstring(value, needle, replacement, start, 0, true);
}

std::string CoreReplaceSubstring(const std::string &value, const std::string &needle,
                                   const std::string &replacement, std::int32_t start, std::int32_t count)
{
    return CoreReplaceSubstring(value, needle, replacement, start, count, true);
}

std::string CoreSpaces(std::int32_t count)
{
    return std::string(static_cast<std::size_t>(std::max<std::int32_t>(0, count)), ' ');
}
std::string CoreRepeatText(std::int32_t count, const std::string &value)
{
    std::string result;
    for (std::int32_t index = 0; index < count; ++index)
        result += value;
    return result;
}
std::int32_t CoreCompareText(const std::string &left, const std::string &right, bool case_sensitive)
{
    const auto lhs = case_sensitive ? left : ascii_case(left, false);
    const auto rhs = case_sensitive ? right : ascii_case(right, false);
    return lhs < rhs ? -1 : lhs > rhs ? 1 : 0;
}

std::vector<std::string> CoreSplitText(const std::string &value, const std::string &separator, std::int32_t limit)
{
    if (separator.empty())
        return {value};
    std::vector<std::string> result;
    std::size_t position = 0;
    while (limit <= 0 || static_cast<std::int32_t>(result.size() + 1) < limit)
    {
        const auto found = value.find(separator, position);
        if (found == std::string::npos)
            break;
        result.push_back(value.substr(position, found - position));
        position = found + separator.size();
    }
    result.push_back(value.substr(position));
    return result;
}

sulang::core::ByteArray CoreTextToUtf8(const std::string &value)
{
    return {value.begin(), value.end()};
}
std::string CoreUtf8ToText(const sulang::core::ByteArray &value)
{
    return encode_utf8(decode_utf8({value.begin(), value.end()}));
}
sulang::core::ByteArray CoreTextToUtf16(const std::string &value)
{
    sulang::core::ByteArray result;
    for (auto code : decode_utf8(value))
    {
        if (code > 0xFFFF)
        {
            code -= 0x10000;
            const std::uint16_t high = static_cast<std::uint16_t>(0xD800 + (code >> 10));
            const std::uint16_t low = static_cast<std::uint16_t>(0xDC00 + (code & 0x3FF));
            result.insert(result.end(), {static_cast<std::uint8_t>(high), static_cast<std::uint8_t>(high >> 8),
                                         static_cast<std::uint8_t>(low), static_cast<std::uint8_t>(low >> 8)});
        }
        else
        {
            const auto unit = static_cast<std::uint16_t>(code);
            result.insert(result.end(), {static_cast<std::uint8_t>(unit), static_cast<std::uint8_t>(unit >> 8)});
        }
    }
    return result;
}
std::string CoreUtf16ToText(const sulang::core::ByteArray &value)
{
    std::vector<std::uint32_t> codes;
    for (std::size_t index = 0; index + 1 < value.size(); index += 2)
    {
        std::uint32_t code = static_cast<std::uint16_t>(value[index] | (value[index + 1] << 8));
        if (code >= 0xD800 && code <= 0xDBFF && index + 3 < value.size())
        {
            const auto low = static_cast<std::uint16_t>(value[index + 2] | (value[index + 3] << 8));
            if (low >= 0xDC00 && low <= 0xDFFF)
            {
                code = 0x10000 + ((code - 0xD800) << 10) + (low - 0xDC00);
                index += 2;
            }
        }
        codes.push_back(code);
    }
    return encode_utf8(codes);
}
