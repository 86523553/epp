#include "disk.h"

#include <chrono>
#include <filesystem>
#include <fstream>
#include <limits>
#include <random>
#include <windows.h>

namespace
{
struct FileSearchState
{
    ~FileSearchState()
    {
        if (handle != INVALID_HANDLE_VALUE)
            FindClose(handle);
    }
    HANDLE handle = INVALID_HANDLE_VALUE;
    std::int32_t attributes = 0;
};
thread_local FileSearchState file_search;

std::wstring wide(const std::string &value)
{
    const int size =
        MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, value.data(), static_cast<int>(value.size()), nullptr, 0);
    std::wstring result(static_cast<std::size_t>(std::max(0, size)), L'\0');
    if (size > 0)
        MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, value.data(), static_cast<int>(value.size()), result.data(),
                            size);
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
std::filesystem::path path_from_utf8(const std::string &value)
{
    return std::filesystem::path(wide(value));
}
std::string path_to_utf8(const std::filesystem::path &value)
{
    return utf8(value.native());
}
std::wstring drive_root(const std::string &value)
{
    std::wstring drive = wide(value);
    if (drive.empty())
    {
        wchar_t current[MAX_PATH]{};
        const DWORD length = GetCurrentDirectoryW(MAX_PATH, current);
        if (length == 0 || length >= MAX_PATH)
            return {};
        drive.assign(current, length);
    }
    if (drive.size() == 1 && ((drive[0] >= L'A' && drive[0] <= L'Z') ||
                              (drive[0] >= L'a' && drive[0] <= L'z')))
        drive.push_back(L':');
    if (drive.size() < 2 || drive[1] != L':')
        return {};
    drive.resize(2);
    drive.push_back(L'\\');
    return drive;
}
} // namespace

std::string CoreFindFile(const std::string &pattern, std::int32_t attributes)
{
    WIN32_FIND_DATAW entry{};
    if (!pattern.empty())
    {
        if (file_search.handle != INVALID_HANDLE_VALUE)
            FindClose(file_search.handle);
        file_search.handle = FindFirstFileW(wide(pattern).c_str(), &entry);
        file_search.attributes = attributes;
        if (file_search.handle == INVALID_HANDLE_VALUE)
            return {};
    }
    else if (file_search.handle == INVALID_HANDLE_VALUE)
        return {};
    else if (!FindNextFileW(file_search.handle, &entry))
    {
        FindClose(file_search.handle);
        file_search.handle = INVALID_HANDLE_VALUE;
        return {};
    }
    for (;;)
    {
        const auto name = std::wstring(entry.cFileName);
        const auto bits = entry.dwFileAttributes;
        const bool directory = (bits & FILE_ATTRIBUTE_DIRECTORY) != 0;
        if (name != L"." && name != L".." &&
            ((file_search.attributes == 0 && !directory) ||
             (file_search.attributes != 0 &&
              (bits & static_cast<DWORD>(file_search.attributes)) == static_cast<DWORD>(file_search.attributes))))
            return utf8(name);
        if (!FindNextFileW(file_search.handle, &entry))
        {
            FindClose(file_search.handle);
            file_search.handle = INVALID_HANDLE_VALUE;
            return {};
        }
    }
}

std::int64_t CoreDiskTotalSpace(const std::string &path)
{
    std::error_code error;
    const auto info = std::filesystem::space(path_from_utf8(path.empty() ? "." : path), error);
    return error || info.capacity > static_cast<std::uintmax_t>(std::numeric_limits<std::int64_t>::max())
               ? -1
               : static_cast<std::int64_t>(info.capacity);
}
std::int64_t CoreDiskFreeSpace(const std::string &path)
{
    std::error_code error;
    const auto info = std::filesystem::space(path_from_utf8(path.empty() ? "." : path), error);
    return error || info.available > static_cast<std::uintmax_t>(std::numeric_limits<std::int64_t>::max())
               ? -1
               : static_cast<std::int64_t>(info.available);
}
std::string CoreDiskLabel(const std::string &drive)
{
    const auto root = drive_root(drive);
    if (root.empty())
        return {};
    wchar_t label[MAX_PATH]{};
    if (!GetVolumeInformationW(root.c_str(), label, MAX_PATH, nullptr, nullptr, nullptr, nullptr, 0))
        return {};
    return utf8(label);
}
bool CoreSetDiskLabel(const std::string &drive, const std::string &label)
{
    const auto root = drive_root(drive);
    return !root.empty() && SetVolumeLabelW(root.c_str(), wide(label).c_str()) != FALSE;
}

bool CoreSetDiskLabel(const std::string &label)
{
    return CoreSetDiskLabel("", label);
}
bool CoreChangeDrive(const std::string &drive)
{
    const auto root = drive_root(drive);
    return !root.empty() && SetCurrentDirectoryW(root.c_str()) != FALSE;
}
bool CoreChangeDirectory(const std::string &path)
{
    std::error_code error;
    std::filesystem::current_path(path_from_utf8(path), error);
    return !error;
}
std::string CoreCurrentDirectory()
{
    std::error_code error;
    const auto result = std::filesystem::current_path(error);
    return error ? std::string{} : path_to_utf8(result);
}
bool CoreCreateDirectory(const std::string &path)
{
    std::error_code error;
    const auto item = path_from_utf8(path);
    return std::filesystem::create_directories(item, error) || (!error && std::filesystem::is_directory(item));
}
bool CoreRemoveDirectory(const std::string &path)
{
    std::error_code error;
    return std::filesystem::remove(path_from_utf8(path), error) && !error;
}
bool CoreCopyFile(const std::string &source, const std::string &target)
{
    std::error_code error;
    return std::filesystem::copy_file(path_from_utf8(source), path_from_utf8(target),
                                      std::filesystem::copy_options::overwrite_existing, error) &&
           !error;
}
bool CoreMoveFile(const std::string &source, const std::string &target)
{
    std::error_code error;
    std::filesystem::rename(path_from_utf8(source), path_from_utf8(target), error);
    return !error;
}
bool CoreDeleteFile(const std::string &path)
{
    std::error_code error;
    const auto item = path_from_utf8(path);
    if (std::filesystem::is_directory(item, error))
        return false;
    return !error && std::filesystem::remove(item, error);
}
bool CoreRenamePath(const std::string &source, const std::string &target)
{
    return CoreMoveFile(source, target);
}
bool CoreFileExists(const std::string &path)
{
    std::error_code error;
    return std::filesystem::is_regular_file(path_from_utf8(path), error) && !error;
}
std::int32_t CoreFileAttributes(const std::string &path)
{
    const DWORD attributes = GetFileAttributesW(path_from_utf8(path).c_str());
    if (attributes == INVALID_FILE_ATTRIBUTES)
        return -1;
    std::int32_t result = 0;
    if ((attributes & FILE_ATTRIBUTE_READONLY) != 0)
        result |= 1;
    if ((attributes & FILE_ATTRIBUTE_HIDDEN) != 0)
        result |= 2;
    if ((attributes & FILE_ATTRIBUTE_SYSTEM) != 0)
        result |= 4;
    if ((attributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
        result |= 16;
    if ((attributes & FILE_ATTRIBUTE_ARCHIVE) != 0)
        result |= 32;
    return result;
}
bool CoreSetFileAttributes(const std::string &path, std::int32_t attributes)
{
    const auto item = path_from_utf8(path);
    const DWORD current = GetFileAttributesW(item.c_str());
    if (current == INVALID_FILE_ATTRIBUTES)
        return false;
    constexpr DWORD managed = FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM |
                              FILE_ATTRIBUTE_ARCHIVE;
    DWORD next = current & ~managed;
    if ((attributes & 1) != 0)
        next |= FILE_ATTRIBUTE_READONLY;
    if ((attributes & 2) != 0)
        next |= FILE_ATTRIBUTE_HIDDEN;
    if ((attributes & 4) != 0)
        next |= FILE_ATTRIBUTE_SYSTEM;
    if ((attributes & 32) != 0)
        next |= FILE_ATTRIBUTE_ARCHIVE;
    return SetFileAttributesW(item.c_str(), next) != FALSE;
}
sulang::core::DateTime CoreFileDateTime(const std::string &path)
{
    const auto file_time = std::filesystem::last_write_time(path_from_utf8(path));
    return std::chrono::time_point_cast<std::chrono::system_clock::duration>(
        file_time - std::filesystem::file_time_type::clock::now() + std::chrono::system_clock::now());
}
std::int64_t CoreFileLength(const std::string &path)
{
    std::error_code error;
    const auto size = std::filesystem::file_size(path_from_utf8(path), error);
    return error || size > static_cast<std::uintmax_t>(std::numeric_limits<std::int64_t>::max())
               ? -1
               : static_cast<std::int64_t>(size);
}
std::string CoreTemporaryFile(const std::string &directory)
{
    const auto base = directory.empty() ? std::filesystem::temp_directory_path() : path_from_utf8(directory);
    std::random_device random;
    for (int attempt = 0; attempt < 100; ++attempt)
    {
        const auto candidate = base / ("sulang_" + std::to_string(random()) + ".tmp");
        if (!std::filesystem::exists(candidate))
        {
            std::ofstream stream(candidate, std::ios::binary);
            if (stream)
                return path_to_utf8(candidate);
        }
    }
    return {};
}
sulang::core::ByteArray CoreReadWholeFile(const std::string &path)
{
    std::ifstream stream(path_from_utf8(path), std::ios::binary);
    if (!stream)
        throw std::runtime_error("无法打开文件读取");
    return {std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>()};
}
bool CoreWriteWholeFile(const std::string &path, const sulang::core::ByteArray &data)
{
    std::ofstream stream(path_from_utf8(path), std::ios::binary | std::ios::trunc);
    if (!stream)
        return false;
    stream.write(reinterpret_cast<const char *>(data.data()), static_cast<std::streamsize>(data.size()));
    return stream.good();
}
