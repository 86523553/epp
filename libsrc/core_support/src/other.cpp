#include "other.h"

#include <algorithm>
#include <array>
#include <cstdio>
#include <filesystem>
#include <limits>
#include <stdexcept>
#include <windows.h>

namespace
{
std::wstring wide(const std::string &value)
{
    const int size = MultiByteToWideChar(CP_UTF8, 0, value.data(), static_cast<int>(value.size()), nullptr, 0);
    std::wstring result(static_cast<std::size_t>(std::max(0, size)), L'\0');
    if (size > 0)
        MultiByteToWideChar(CP_UTF8, 0, value.data(), static_cast<int>(value.size()), result.data(), size);
    return result;
}
std::string utf8(const std::wstring &value)
{
    const int size =
        WideCharToMultiByte(CP_UTF8, 0, value.data(), static_cast<int>(value.size()), nullptr, 0, nullptr, nullptr);
    std::string result(static_cast<std::size_t>(std::max(0, size)), '\0');
    if (size > 0)
        WideCharToMultiByte(CP_UTF8, 0, value.data(), static_cast<int>(value.size()), result.data(), size, nullptr,
                            nullptr);
    return result;
}
} // namespace
std::any CoreSelect(bool condition, const std::any &true_value, const std::any &false_value)
{
    return condition ? true_value : false_value;
}
std::int32_t CoreDataTypeSize(std::int32_t type_id)
{
    switch (type_id)
    {
    case 1:
        return sizeof(std::uint8_t);
    case 2:
        return sizeof(std::int16_t);
    case 3:
        return sizeof(std::int32_t);
    case 4:
        return sizeof(std::int64_t);
    case 5:
        return sizeof(double);
    case 6:
        return sizeof(long double);
    case 7:
        return sizeof(bool);
    case 8:
        return sizeof(sulang::core::DateTime);
    case 9:
        return sizeof(void (*)());
    case 10:
        return sizeof(std::string);
    default:
        return 0;
    }
}
std::int32_t CoreRgb(std::int32_t red, std::int32_t green, std::int32_t blue)
{
    return RGB(std::clamp(red, 0, 255), std::clamp(green, 0, 255), std::clamp(blue, 0, 255));
}
std::int32_t CoreSystemVolumeSerial()
{
    wchar_t system[MAX_PATH]{};
    if (!GetWindowsDirectoryW(system, MAX_PATH))
        return 0;
    wchar_t root[] = {system[0], L':', L'\\', L'\0'};
    DWORD serial = 0;
    return GetVolumeInformationW(root, nullptr, 0, &serial, nullptr, nullptr, nullptr, 0)
               ? static_cast<std::int32_t>(serial)
               : 0;
}
std::int32_t CoreSystemLanguage()
{
    return static_cast<std::int32_t>(GetUserDefaultLangID());
}
std::string CoreSetDllDirectory(const std::string &path)
{
    std::wstring previous(32768, L'\0');
    const auto size = GetDllDirectoryW(static_cast<DWORD>(previous.size()), previous.data());
    previous.resize(size);
    return SetDllDirectoryW(path.empty() ? nullptr : wide(path).c_str()) ? utf8(previous) : std::string{};
}
std::string CoreRunConsole(const std::string &command_line)
{
    const std::string command = command_line + " 2>&1";
    FILE *pipe = _popen(command.c_str(), "rb");
    if (!pipe)
        return {};
    std::string result;
    std::array<char, 4096> buffer{};
    while (const auto read = std::fread(buffer.data(), 1, buffer.size(), pipe))
        result.append(buffer.data(), read);
    _pclose(pipe);
    return result;
}
long double CoreLibraryVersion()
{
    return 1.0L;
}
