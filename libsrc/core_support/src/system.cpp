#include "system.h"
#include "text.h"
#include "environment.h"

#include <algorithm>
#include <array>
#include <chrono>
#include <cstring>
#include <filesystem>
#include <limits>
#include <string>
#include <thread>
#include <vector>
#include <windows.h>

namespace
{
HCURSOR previous_cursor = nullptr;
std::wstring wide(const std::string &value)
{
    if (value.empty())
        return {};
    const int size =
        MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, value.data(), static_cast<int>(value.size()), nullptr, 0);
    if (size <= 0)
        return {};
    std::wstring result(static_cast<std::size_t>(size), L'\0');
    MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, value.data(), static_cast<int>(value.size()), result.data(),
                        size);
    return result;
}
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
std::filesystem::path path_from_utf8(const std::string &value)
{
    return std::filesystem::path(wide(value));
}
std::string path_to_utf8(const std::filesystem::path &value)
{
    return utf8(value.native());
}
HKEY registry_root(std::int32_t root)
{
    switch (root)
    {
    case 1: return HKEY_CLASSES_ROOT;
    case 2: return HKEY_CURRENT_CONFIG;
    case 3: return HKEY_CURRENT_USER;
    case 4: return HKEY_LOCAL_MACHINE;
    case 5: return HKEY_USERS;
    default: return nullptr;
    }
}
bool registry_path(const std::string &path, std::wstring &key, std::wstring &name)
{
    const auto full = wide(path);
    const auto separator = full.rfind(L'\\');
    if (full.empty() || separator == std::wstring::npos || separator == 0)
        return false;
    key = full.substr(0, separator);
    name = full.substr(separator + 1);
    return true;
}
bool registry_key_exists(std::int32_t root, const std::wstring &path)
{
    const auto hive = registry_root(root);
    if (!hive || path.empty())
        return false;
    HKEY opened = nullptr;
    const auto status = RegOpenKeyExW(hive, path.c_str(), 0, KEY_READ, &opened);
    if (status == ERROR_SUCCESS)
        RegCloseKey(opened);
    return status == ERROR_SUCCESS;
}
bool registry_value(std::int32_t root, const std::string &path, DWORD &type, std::vector<std::uint8_t> &data)
{
    const auto hive = registry_root(root);
    std::wstring key;
    std::wstring name;
    if (!hive || !registry_path(path, key, name))
        return false;
    HKEY opened = nullptr;
    if (RegOpenKeyExW(hive, key.c_str(), 0, KEY_QUERY_VALUE, &opened) != ERROR_SUCCESS)
        return false;
    DWORD size = 0;
    auto status = RegQueryValueExW(opened, name.c_str(), nullptr, &type, nullptr, &size);
    if (status == ERROR_SUCCESS)
    {
        data.resize(size);
        status = RegQueryValueExW(opened, name.c_str(), nullptr, &type, data.data(), &size);
        data.resize(size);
    }
    RegCloseKey(opened);
    return status == ERROR_SUCCESS;
}
} // namespace

bool CoreRun(const std::string &command_line, bool wait, std::int32_t show_mode)
{
    auto command = wide(command_line);
    STARTUPINFOW startup{};
    startup.cb = sizeof(startup);
    startup.dwFlags = STARTF_USESHOWWINDOW;
    startup.wShowWindow = static_cast<WORD>(show_mode <= 0 ? SW_SHOWNORMAL : show_mode);
    PROCESS_INFORMATION process{};
    if (!CreateProcessW(nullptr, command.data(), nullptr, nullptr, FALSE, 0, nullptr, nullptr, &startup, &process))
        return false;
    if (wait)
        WaitForSingleObject(process.hProcess, INFINITE);
    DWORD exit_code = 0;
    if (wait)
        GetExitCodeProcess(process.hProcess, &exit_code);
    CloseHandle(process.hThread);
    CloseHandle(process.hProcess);
    return !wait || exit_code == 0;
}
std::string CoreClipboardText()
{
    if (!OpenClipboard(nullptr))
        return {};
    std::string result;
    if (const auto handle = GetClipboardData(CF_UNICODETEXT); handle != nullptr)
    {
        if (const auto text = static_cast<const wchar_t *>(GlobalLock(handle)); text != nullptr)
        {
            result = utf8(text);
            GlobalUnlock(handle);
        }
    }
    CloseClipboard();
    return result;
}
bool CoreSetClipboardText(const std::string &value)
{
    const auto text = wide(value);
    if (!OpenClipboard(nullptr))
        return false;
    bool success = false;
    if (EmptyClipboard())
    {
        const auto bytes = (text.size() + 1) * sizeof(wchar_t);
        const auto handle = GlobalAlloc(GMEM_MOVEABLE, bytes);
        if (handle != nullptr)
        {
            if (auto *target = GlobalLock(handle); target != nullptr)
            {
                std::memcpy(target, text.c_str(), bytes);
                GlobalUnlock(handle);
                success = SetClipboardData(CF_UNICODETEXT, handle) != nullptr;
            }
            if (!success)
                GlobalFree(handle);
        }
    }
    CloseClipboard();
    return success;
}
bool CoreClipboardHasText()
{
    return IsClipboardFormatAvailable(CF_UNICODETEXT) || IsClipboardFormatAvailable(CF_TEXT);
}
void CoreClearClipboard()
{
    if (OpenClipboard(nullptr))
    {
        EmptyClipboard();
        CloseClipboard();
    }
}
std::int32_t CoreScreenWidth()
{
    return GetSystemMetrics(SM_CXSCREEN);
}
std::int32_t CoreScreenHeight()
{
    return GetSystemMetrics(SM_CYSCREEN);
}
std::int32_t CoreCursorX()
{
    POINT point{};
    return GetCursorPos(&point) ? point.x : 0;
}
std::int32_t CoreCursorY()
{
    POINT point{};
    return GetCursorPos(&point) ? point.y : 0;
}
std::int32_t CoreColorCount()
{
    HDC context = GetDC(nullptr);
    if (!context)
        return 0;
    const int bits = GetDeviceCaps(context, BITSPIXEL) * GetDeviceCaps(context, PLANES);
    ReleaseDC(nullptr, context);
    if (bits >= 31)
        return std::numeric_limits<std::int32_t>::max();
    return 1 << bits;
}
std::int32_t CoreDefaultBackgroundColor()
{
    const COLORREF color = GetSysColor(COLOR_WINDOW);
    return static_cast<std::int32_t>(color & 0x00FFFFFF);
}
std::int32_t CoreMessageBox(const std::string &message, std::int32_t buttons, const std::string &title)
{
    return MessageBoxW(nullptr, wide(message).c_str(), wide(title).c_str(), static_cast<UINT>(buttons));
}
void CoreBeep()
{
    MessageBeep(MB_OK);
}
std::int64_t CoreTickCount()
{
    return static_cast<std::int64_t>(GetTickCount64());
}
void CoreSetWaitCursor()
{
    previous_cursor = SetCursor(LoadCursorW(nullptr, MAKEINTRESOURCEW(32514)));
}
void CoreRestoreCursor()
{
    if (previous_cursor)
    {
        SetCursor(previous_cursor);
        previous_cursor = nullptr;
    }
}
void CoreSleep(std::int32_t milliseconds)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(std::max<std::int32_t>(0, milliseconds)));
}
std::string CoreReadIni(const std::string &file, const std::string &section, const std::string &key,
                          const std::string &default_value)
{
    std::wstring buffer(32768, L'\0');
    const auto size = GetPrivateProfileStringW(wide(section).c_str(), wide(key).c_str(), wide(default_value).c_str(),
                                               buffer.data(), static_cast<DWORD>(buffer.size()), wide(file).c_str());
    buffer.resize(size);
    return utf8(buffer);
}
bool CoreWriteIni(const std::string &file, const std::string &section, const std::string &key,
                    const std::string &value)
{
    const auto wide_section = wide(section);
    const auto wide_key = wide(key);
    const auto wide_value = wide(value);
    const auto wide_file = wide(file);
    return WritePrivateProfileStringW(wide_section.c_str(), key.empty() ? nullptr : wide_key.c_str(),
                                      key.empty() || value.empty() ? nullptr : wide_value.c_str(),
                                      wide_file.c_str()) != FALSE;
}
std::vector<std::string> CoreIniSections(const std::string &file)
{
    std::wstring buffer(65536, L'\0');
    const auto size =
        GetPrivateProfileSectionNamesW(buffer.data(), static_cast<DWORD>(buffer.size()), wide(file).c_str());
    std::vector<std::string> result;
    for (std::size_t index = 0; index < size && buffer[index] != L'\0';)
    {
        std::wstring item = &buffer[index];
        result.push_back(utf8(item));
        index += item.size() + 1;
    }
    return result;
}
std::int32_t CoreOperatingSystem()
{
    return 1;
}
std::string CoreApplicationName(std::int32_t kind)
{
    const auto path = path_from_utf8(CoreExecutablePath());
    if (kind == 1)
        return path_to_utf8(path.filename());
    if (kind == 2)
        return path_to_utf8(path.parent_path());
    return path_to_utf8(path);
}
std::string CoreOperatingSystemText()
{
    SYSTEM_INFO info{};
    GetNativeSystemInfo(&info);
    return std::string("Windows ") + (info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64   ? "x64"
                                      : info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_ARM64 ? "ARM64"
                                                                                                    : "x86");
}
std::int32_t CoreLastError()
{
    return static_cast<std::int32_t>(GetLastError());
}
sulang::core::ByteArray CoreUnifiedText(const std::string &value, bool is_wide, bool append_zero)
{
    auto result = is_wide ? CoreTextToUtf16(value) : CoreTextToUtf8(value);
    if (append_zero)
    {
        result.push_back(0);
        if (is_wide)
            result.push_back(0);
    }
    return result;
}
std::int32_t CoreUnifiedTextLength(const std::string &value, bool is_wide)
{
    return static_cast<std::int32_t>((is_wide ? CoreTextToUtf16(value) : CoreTextToUtf8(value)).size());
}

std::string CoreReadRegistryText(std::int32_t root, const std::string &path, const std::string &fallback)
{
    DWORD type = 0;
    std::vector<std::uint8_t> data;
    if (!registry_value(root, path, type, data) || (type != REG_SZ && type != REG_EXPAND_SZ) ||
        data.size() % sizeof(wchar_t) != 0)
        return fallback;
    std::wstring value(data.size() / sizeof(wchar_t), L'\0');
    if (!data.empty())
        std::memcpy(value.data(), data.data(), data.size());
    if (const auto terminator = value.find(L'\0'); terminator != std::wstring::npos)
        value.resize(terminator);
    return utf8(value);
}

std::int32_t CoreReadRegistryNumber(std::int32_t root, const std::string &path, std::int32_t fallback)
{
    DWORD type = 0;
    std::vector<std::uint8_t> data;
    if (!registry_value(root, path, type, data) || type != REG_DWORD || data.size() != sizeof(std::int32_t))
        return fallback;
    std::int32_t result = 0;
    std::memcpy(&result, data.data(), sizeof(result));
    return result;
}

sulang::core::ByteArray CoreReadRegistryBinary(std::int32_t root, const std::string &path,
                                               const sulang::core::ByteArray &fallback)
{
    DWORD type = 0;
    std::vector<std::uint8_t> data;
    return registry_value(root, path, type, data) && type == REG_BINARY ? data : fallback;
}

bool CoreWriteRegistry(std::int32_t root, const std::string &path, const std::any &value)
{
    const auto hive = registry_root(root);
    std::wstring key;
    std::wstring name;
    if (!hive || !registry_path(path, key, name))
        return false;
    DWORD type = 0;
    std::vector<std::uint8_t> data;
    if (value.type() == typeid(std::string) || value.type() == typeid(const char *))
    {
        const auto text = value.type() == typeid(std::string) ? std::any_cast<std::string>(value)
                                                               : std::string(std::any_cast<const char *>(value));
        const auto wide_text = wide(text) + L'\0';
        const auto *bytes = reinterpret_cast<const std::uint8_t *>(wide_text.data());
        data.assign(bytes, bytes + wide_text.size() * sizeof(wchar_t));
        type = REG_SZ;
    }
    else if (value.type() == typeid(std::int32_t))
    {
        const auto number = std::any_cast<std::int32_t>(value);
        const auto *bytes = reinterpret_cast<const std::uint8_t *>(&number);
        data.assign(bytes, bytes + sizeof(number));
        type = REG_DWORD;
    }
    else if (value.type() == typeid(sulang::core::ByteArray))
    {
        data = std::any_cast<sulang::core::ByteArray>(value);
        type = REG_BINARY;
    }
    if (type == 0)
        return false;
    HKEY opened = nullptr;
    if (RegCreateKeyExW(hive, key.c_str(), 0, nullptr, 0, KEY_SET_VALUE, nullptr, &opened, nullptr) != ERROR_SUCCESS)
        return false;
    const auto status = RegSetValueExW(opened, name.c_str(), 0, type, data.data(),
                                       static_cast<DWORD>(data.size()));
    RegCloseKey(opened);
    return status == ERROR_SUCCESS;
}

bool CoreDeleteRegistry(std::int32_t root, const std::string &path)
{
    const auto hive = registry_root(root);
    std::wstring key;
    std::wstring name;
    if (!hive || !registry_path(path, key, name))
        return false;
    HKEY opened = nullptr;
    if (RegOpenKeyExW(hive, key.c_str(), 0, KEY_SET_VALUE, &opened) == ERROR_SUCCESS)
    {
        const auto status = RegDeleteValueW(opened, name.c_str());
        RegCloseKey(opened);
        if (status == ERROR_SUCCESS)
            return true;
    }
    return !name.empty() && RegDeleteKeyW(hive, wide(path).c_str()) == ERROR_SUCCESS;
}

bool CoreRegistryExists(std::int32_t root, const std::string &path)
{
    DWORD type = 0;
    std::vector<std::uint8_t> data;
    if (registry_value(root, path, type, data))
        return true;
    const auto full = wide(path);
    return !full.empty() && full.back() != L'\\' && registry_key_exists(root, full);
}

std::int32_t CoreRuntimeDataType(const std::any &value)
{
    const auto &type = value.type();
    if (type == typeid(std::uint8_t) || type == typeid(std::int8_t))
        return 1;
    if (type == typeid(std::int16_t) || type == typeid(std::uint16_t))
        return 2;
    if (type == typeid(std::int32_t) || type == typeid(std::uint32_t))
        return 3;
    if (type == typeid(std::int64_t) || type == typeid(std::uint64_t))
        return 4;
    if (type == typeid(float))
        return 5;
    if (type == typeid(double) || type == typeid(long double))
        return 6;
    if (type == typeid(bool))
        return 7;
    if (type == typeid(sulang::core::DateTime))
        return 8;
    if (type == typeid(std::string) || type == typeid(const char *))
        return 10;
    if (type == typeid(sulang::core::ByteArray))
        return 11;
    return 0;
}
