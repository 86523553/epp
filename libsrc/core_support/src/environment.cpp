#include "environment.h"

#include <windows.h>

#include <cstdlib>
#include <filesystem>
#include <shellapi.h>
#include <stdexcept>

namespace
{
std::string utf8(const std::wstring &value)
{
    if (value.empty())
        return {};
    const int size =
        WideCharToMultiByte(CP_UTF8, 0, value.data(), static_cast<int>(value.size()), nullptr, 0, nullptr, nullptr);
    std::string result(static_cast<std::size_t>(size), '\0');
    WideCharToMultiByte(CP_UTF8, 0, value.data(), static_cast<int>(value.size()), result.data(), size, nullptr,
                        nullptr);
    return result;
}
std::wstring wide(const std::string &value)
{
    if (value.empty())
        return {};
    const int size =
        MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, value.data(), static_cast<int>(value.size()), nullptr, 0);
    if (size <= 0)
        throw std::invalid_argument("文本不是有效 UTF-8");
    std::wstring result(static_cast<std::size_t>(size), L'\0');
    MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, value.data(), static_cast<int>(value.size()), result.data(),
                        size);
    return result;
}
std::string path_utf8(const std::filesystem::path &value)
{
    return utf8(value.native());
}
} // namespace

std::vector<std::string> CoreCommandLineArguments()
{
    int count = 0;
    LPWSTR *values = CommandLineToArgvW(GetCommandLineW(), &count);
    if (values == nullptr)
        return {};
    std::vector<std::string> result;
    result.reserve(static_cast<std::size_t>(count));
    for (int index = 0; index < count; ++index)
        result.push_back(utf8(values[index]));
    LocalFree(values);
    return result;
}
std::string CoreWorkingDirectory()
{
    return path_utf8(std::filesystem::current_path());
}
std::string CoreExecutablePath()
{
    std::wstring buffer(32768, L'\0');
    const auto size = GetModuleFileNameW(nullptr, buffer.data(), static_cast<DWORD>(buffer.size()));
    if (size == 0 || size >= buffer.size())
        return {};
    buffer.resize(size);
    return utf8(buffer);
}
std::string CoreGetEnvironment(const std::string &name)
{
    const auto key = wide(name);
    const auto size = GetEnvironmentVariableW(key.c_str(), nullptr, 0);
    if (size == 0)
        return {};
    std::wstring value(size, L'\0');
    const auto written = GetEnvironmentVariableW(key.c_str(), value.data(), size);
    if (written == 0)
        return {};
    value.resize(written);
    return utf8(value);
}
bool CoreSetEnvironment(const std::string &name, const std::string &value)
{
    const auto key = wide(name);
    const auto content = wide(value);
    return SetEnvironmentVariableW(key.c_str(), content.c_str()) != FALSE;
}
