#include "bytes.h"

#include <algorithm>
#include <cstring>
#include <stdexcept>
#include <type_traits>

namespace
{
template <typename T>
void AppendBinaryValue(sulang::core::ByteArray &result, const T &value)
{
    static_assert(std::is_trivially_copyable_v<T>);
    const auto *bytes = reinterpret_cast<const std::uint8_t *>(&value);
    result.insert(result.end(), bytes, bytes + sizeof(T));
}

template <typename T>
bool TryAppendScalar(const std::any &value, sulang::core::ByteArray &result)
{
    if (value.type() != typeid(T))
        return false;
    AppendBinaryValue(result, std::any_cast<T>(value));
    return true;
}

template <typename T>
sulang::core::ByteArray BinaryFromVector(const std::vector<T> &values)
{
    sulang::core::ByteArray result;
    result.reserve(values.size() * sizeof(T));
    for (const auto &value : values)
        AppendBinaryValue(result, value);
    return result;
}

template <typename T>
bool TryAppendVector(const std::any &value, sulang::core::ByteArray &result)
{
    if (value.type() != typeid(std::vector<T>))
        return false;
    result = BinaryFromVector(std::any_cast<const std::vector<T> &>(value));
    return true;
}

template <typename T>
T ReadBinaryValue(const sulang::core::ByteArray &value, std::size_t offset)
{
    if (offset > value.size() || sizeof(T) > value.size() - offset)
        throw std::out_of_range("字节集数据读取越界");
    T result{};
    std::memcpy(&result, value.data() + offset, sizeof(T));
    return result;
}

template <typename T>
T ReadPointerValue(std::uintptr_t address)
{
    if (address == 0)
        throw std::invalid_argument("内存数据指针不能为空");
    T result{};
    std::memcpy(&result, reinterpret_cast<const void *>(address), sizeof(T));
    return result;
}
} // namespace

sulang::core::ByteArray CoreToBinary(const std::any &value)
{
    if (!value.has_value())
        throw std::invalid_argument("到字节集缺少输入数据");
    const auto &type = value.type();
    if (type == typeid(sulang::core::ByteArray))
        return std::any_cast<sulang::core::ByteArray>(value);
    if (type == typeid(std::string))
    {
        const auto &text = std::any_cast<const std::string &>(value);
        return {text.begin(), text.end()};
    }
    if (type == typeid(const char *))
    {
        const auto *text = std::any_cast<const char *>(value);
        return text ? sulang::core::ByteArray(text, text + std::strlen(text)) : sulang::core::ByteArray{};
    }
    sulang::core::ByteArray result;
    if (TryAppendScalar<std::int8_t>(value, result) || TryAppendScalar<std::uint8_t>(value, result) ||
        TryAppendScalar<std::int16_t>(value, result) || TryAppendScalar<std::uint16_t>(value, result) ||
        TryAppendScalar<std::int32_t>(value, result) || TryAppendScalar<std::uint32_t>(value, result) ||
        TryAppendScalar<std::int64_t>(value, result) || TryAppendScalar<std::uint64_t>(value, result) ||
        TryAppendScalar<float>(value, result) || TryAppendScalar<double>(value, result) ||
        TryAppendScalar<long double>(value, result) || TryAppendScalar<bool>(value, result))
        return result;
    if (type == typeid(sulang::core::DateTime))
    {
        const auto time = std::any_cast<sulang::core::DateTime>(value);
        AppendBinaryValue(result, std::chrono::duration_cast<std::chrono::seconds>(time.time_since_epoch()).count());
        return result;
    }
    if (TryAppendVector<std::int8_t>(value, result) || TryAppendVector<std::int16_t>(value, result) ||
        TryAppendVector<std::uint16_t>(value, result) || TryAppendVector<std::int32_t>(value, result) ||
        TryAppendVector<std::uint32_t>(value, result) || TryAppendVector<std::int64_t>(value, result) ||
        TryAppendVector<std::uint64_t>(value, result) || TryAppendVector<float>(value, result) ||
        TryAppendVector<double>(value, result) || TryAppendVector<long double>(value, result))
        return result;
    throw std::invalid_argument("到字节集不支持该数据类型");
}

std::any CoreGetBinaryElement(const sulang::core::ByteArray &value, std::int32_t type, std::int32_t start)
{
    if (start < 1 || static_cast<std::size_t>(start - 1) > value.size())
        throw std::out_of_range("字节集起始索引越界");
    const auto offset = static_cast<std::size_t>(start - 1);
    switch (type)
    {
    case 1:
        return ReadBinaryValue<std::uint8_t>(value, offset);
    case 2:
        return ReadBinaryValue<std::int16_t>(value, offset);
    case 3:
        return ReadBinaryValue<std::int32_t>(value, offset);
    case 4:
        return ReadBinaryValue<std::int64_t>(value, offset);
    case 5:
        return ReadBinaryValue<float>(value, offset);
    case 6:
        return ReadBinaryValue<double>(value, offset);
    case 7:
        return ReadBinaryValue<std::uint8_t>(value, offset) != 0;
    case 8:
    {
        const auto seconds = ReadBinaryValue<std::int64_t>(value, offset);
        return sulang::core::DateTime(std::chrono::seconds(seconds));
    }
    case 9:
        return ReadBinaryValue<std::uintptr_t>(value, offset);
    case 10:
    {
        auto end = std::find(value.begin() + static_cast<std::ptrdiff_t>(offset), value.end(), 0);
        return std::string(value.begin() + static_cast<std::ptrdiff_t>(offset), end);
    }
    default:
        throw std::invalid_argument("字节集数据类型编号无效");
    }
}

sulang::core::ByteArray CorePointerToBinary(std::uintptr_t address, std::int32_t length)
{
    if (length < 0 || (address == 0 && length > 0))
        throw std::invalid_argument("内存地址或数据长度无效");
    if (length == 0)
        return {};
    const auto *bytes = reinterpret_cast<const std::uint8_t *>(address);
    return {bytes, bytes + length};
}

std::int32_t CorePointerToInt32(std::uintptr_t address)
{
    return ReadPointerValue<std::int32_t>(address);
}

float CorePointerToFloat(std::uintptr_t address)
{
    return ReadPointerValue<float>(address);
}

double CorePointerToDouble(std::uintptr_t address)
{
    return ReadPointerValue<double>(address);
}

std::int32_t CoreBinaryLength(const sulang::core::ByteArray &value)
{
    return static_cast<std::int32_t>(value.size());
}
sulang::core::ByteArray CoreBinaryLeft(const sulang::core::ByteArray &value, std::int32_t count)
{
    const auto size = std::min<std::size_t>(value.size(), static_cast<std::size_t>(std::max<std::int32_t>(0, count)));
    return {value.begin(), value.begin() + static_cast<std::ptrdiff_t>(size)};
}
sulang::core::ByteArray CoreBinaryRight(const sulang::core::ByteArray &value, std::int32_t count)
{
    const auto size = std::min<std::size_t>(value.size(), static_cast<std::size_t>(std::max<std::int32_t>(0, count)));
    return {value.end() - static_cast<std::ptrdiff_t>(size), value.end()};
}
sulang::core::ByteArray CoreBinaryMid(const sulang::core::ByteArray &value, std::int32_t start, std::int32_t count)
{
    if (start <= 0 || count <= 0)
        return {};
    const auto begin = std::min<std::size_t>(value.size(), static_cast<std::size_t>(start - 1));
    const auto end = std::min(value.size(), begin + static_cast<std::size_t>(count));
    return {value.begin() + static_cast<std::ptrdiff_t>(begin), value.begin() + static_cast<std::ptrdiff_t>(end)};
}
std::int32_t CoreFindBinary(const sulang::core::ByteArray &value, const sulang::core::ByteArray &needle,
                              std::int32_t start)
{
    const auto begin =
        value.begin() + static_cast<std::ptrdiff_t>(std::min<std::size_t>(
                            value.size(), static_cast<std::size_t>(std::max<std::int32_t>(1, start) - 1)));
    const auto found = std::search(begin, value.end(), needle.begin(), needle.end());
    return found == value.end() ? -1 : static_cast<std::int32_t>(std::distance(value.begin(), found) + 1);
}
std::int32_t CoreFindBinaryReverse(const sulang::core::ByteArray &value, const sulang::core::ByteArray &needle,
                                     std::int32_t start)
{
    if (needle.empty())
        return start <= 0 ? static_cast<std::int32_t>(value.size()) + 1
                          : std::min<std::int32_t>(start, static_cast<std::int32_t>(value.size()) + 1);
    const auto end_index =
        start <= 0 ? value.size() : std::min<std::size_t>(value.size(), static_cast<std::size_t>(start));
    if (needle.size() > end_index)
        return -1;
    for (std::size_t index = end_index - needle.size() + 1; index-- > 0;)
    {
        if (std::equal(needle.begin(), needle.end(), value.begin() + static_cast<std::ptrdiff_t>(index)))
            return static_cast<std::int32_t>(index + 1);
        if (index == 0)
            break;
    }
    return -1;
}
sulang::core::ByteArray CoreReplaceBinary(const sulang::core::ByteArray &value, std::int32_t start,
                                            std::int32_t length, const sulang::core::ByteArray &replacement)
{
    auto result = value;
    const auto begin = start <= 0 ? 0 : std::min<std::size_t>(result.size(), static_cast<std::size_t>(start - 1));
    const auto end = std::min(result.size(), begin + static_cast<std::size_t>(std::max<std::int32_t>(0, length)));
    result.erase(result.begin() + static_cast<std::ptrdiff_t>(begin),
                 result.begin() + static_cast<std::ptrdiff_t>(end));
    result.insert(result.begin() + static_cast<std::ptrdiff_t>(begin), replacement.begin(), replacement.end());
    return result;
}
sulang::core::ByteArray CoreReplaceSubBinary(const sulang::core::ByteArray &value,
                                               const sulang::core::ByteArray &needle,
                                               const sulang::core::ByteArray &replacement, std::int32_t start,
                                               std::int32_t count)
{
    if (needle.empty())
        return value;
    auto result = value;
    std::size_t position = static_cast<std::size_t>(std::max<std::int32_t>(1, start) - 1);
    std::int32_t replaced = 0;
    while (position <= result.size() && (count <= 0 || replaced < count))
    {
        const auto found = std::search(result.begin() + static_cast<std::ptrdiff_t>(position), result.end(),
                                       needle.begin(), needle.end());
        if (found == result.end())
            break;
        position = static_cast<std::size_t>(std::distance(result.begin(), found));
        result.erase(found, found + static_cast<std::ptrdiff_t>(needle.size()));
        result.insert(result.begin() + static_cast<std::ptrdiff_t>(position), replacement.begin(), replacement.end());
        position += replacement.size();
        ++replaced;
    }
    return result;
}
sulang::core::ByteArray CoreZeroBytes(std::int32_t count)
{
    return sulang::core::ByteArray(static_cast<std::size_t>(std::max<std::int32_t>(0, count)), 0);
}
sulang::core::ByteArray CoreRepeatBinary(std::int32_t count, const sulang::core::ByteArray &value)
{
    sulang::core::ByteArray result;
    for (std::int32_t index = 0; index < count; ++index)
        result.insert(result.end(), value.begin(), value.end());
    return result;
}
std::vector<sulang::core::ByteArray> CoreSplitBinary(const sulang::core::ByteArray &value,
                                                       const sulang::core::ByteArray &separator, std::int32_t limit)
{
    if (value.empty())
        return {};
    if (separator.empty())
        return {value};
    std::vector<sulang::core::ByteArray> result;
    auto begin = value.begin();
    while (limit <= 0 || static_cast<std::int32_t>(result.size() + 1) < limit)
    {
        const auto found = std::search(begin, value.end(), separator.begin(), separator.end());
        if (found == value.end())
            break;
        result.emplace_back(begin, found);
        begin = found + static_cast<std::ptrdiff_t>(separator.size());
    }
    result.emplace_back(begin, value.end());
    return result;
}
std::int32_t CoreReadInt32(const sulang::core::ByteArray &value, std::int32_t offset, bool reverse_bytes)
{
    if (offset < 0 || static_cast<std::size_t>(offset) + 4 > value.size())
        throw std::out_of_range("字节集整数读取越界");
    std::uint8_t bytes[4];
    std::copy_n(value.begin() + offset, 4, bytes);
    if (reverse_bytes)
        std::reverse(std::begin(bytes), std::end(bytes));
    std::int32_t result;
    std::memcpy(&result, bytes, sizeof(result));
    return result;
}
bool CoreWriteInt32(sulang::core::ByteArray &value, std::int32_t offset, std::int32_t number, bool reverse_bytes)
{
    if (offset < 0 || static_cast<std::size_t>(offset) + 4 > value.size())
        return false;
    std::uint8_t bytes[4];
    std::memcpy(bytes, &number, sizeof(number));
    if (reverse_bytes)
        std::reverse(std::begin(bytes), std::end(bytes));
    std::copy(std::begin(bytes), std::end(bytes), value.begin() + offset);
    return true;
}
