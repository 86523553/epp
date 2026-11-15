#include "file.h"

#include <windows.h>

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <map>
#include <memory>
#include <mutex>
#include <vector>

namespace
{
struct OpenFileState
{
    ~OpenFileState()
    {
        if (handle != INVALID_HANDLE_VALUE)
            CloseHandle(handle);
    }

    HANDLE handle = INVALID_HANDLE_VALUE;
    bool readable = false;
    bool writable = false;
};

std::mutex files_mutex;
std::map<std::int32_t, std::unique_ptr<OpenFileState>> files;
std::int32_t next_handle = 1;

OpenFileState *find_file(std::int32_t handle)
{
    const auto found = files.find(handle);
    return found == files.end() ? nullptr : found->second.get();
}

bool open_options(std::int32_t mode, DWORD &access, DWORD &creation, bool &readable, bool &writable)
{
    switch (mode)
    {
    case 1:
        access = GENERIC_READ;
        creation = OPEN_EXISTING;
        readable = true;
        writable = false;
        return true;
    case 2:
        access = GENERIC_WRITE;
        creation = OPEN_EXISTING;
        readable = false;
        writable = true;
        return true;
    case 3:
        access = GENERIC_READ | GENERIC_WRITE;
        creation = OPEN_EXISTING;
        readable = true;
        writable = true;
        return true;
    case 4:
        access = GENERIC_WRITE;
        creation = CREATE_ALWAYS;
        readable = false;
        writable = true;
        return true;
    case 5:
        access = GENERIC_WRITE;
        creation = OPEN_ALWAYS;
        readable = false;
        writable = true;
        return true;
    case 6:
        access = GENERIC_READ | GENERIC_WRITE;
        creation = OPEN_ALWAYS;
        readable = true;
        writable = true;
        return true;
    default:
        return false;
    }
}

bool sharing_options(std::int32_t sharing, DWORD &share_mode)
{
    switch (sharing)
    {
    case 1:
        share_mode = FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE;
        return true;
    case 2:
        share_mode = FILE_SHARE_WRITE;
        return true;
    case 3:
        share_mode = FILE_SHARE_READ;
        return true;
    case 4:
        share_mode = 0;
        return true;
    default:
        return false;
    }
}

std::wstring wide_path(const std::string &value)
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

bool position(OpenFileState &file, LARGE_INTEGER &result)
{
    LARGE_INTEGER distance{};
    return SetFilePointerEx(file.handle, distance, &result, FILE_CURRENT) != FALSE;
}

bool shift_file_data(OpenFileState &file, const sulang::core::ByteArray *insert, std::int32_t remove)
{
    if (!file.readable || !file.writable || remove < 0)
        return false;
    LARGE_INTEGER current{};
    LARGE_INTEGER size{};
    if (!position(file, current) || !GetFileSizeEx(file.handle, &size) || current.QuadPart > size.QuadPart)
        return false;
    const auto removed = std::min<std::int64_t>(remove, size.QuadPart - current.QuadPart);
    const auto inserted = insert ? static_cast<std::int64_t>(insert->size()) : 0;
    if (inserted > INT32_MAX || size.QuadPart - removed > INT64_MAX - inserted)
        return false;
    constexpr DWORD chunk_size = 64 * 1024;
    std::vector<std::uint8_t> buffer(chunk_size);
    auto seek = [&](std::int64_t offset) {
        LARGE_INTEGER target{};
        target.QuadPart = offset;
        return SetFilePointerEx(file.handle, target, nullptr, FILE_BEGIN) != FALSE;
    };
    const auto tail_start = current.QuadPart + removed;
    const auto tail_size = size.QuadPart - tail_start;
    const auto shift = inserted - removed;
    if (shift > 0)
    {
        if (!seek(size.QuadPart + shift) || !SetEndOfFile(file.handle))
            return false;
        for (std::int64_t remaining = tail_size; remaining > 0;)
        {
            const auto amount = static_cast<DWORD>(std::min<std::int64_t>(remaining, chunk_size));
            const auto source = tail_start + remaining - amount;
            DWORD read = 0;
            DWORD written = 0;
            if (!seek(source) || !ReadFile(file.handle, buffer.data(), amount, &read, nullptr) || read != amount ||
                !seek(source + shift) || !WriteFile(file.handle, buffer.data(), amount, &written, nullptr) ||
                written != amount)
                return false;
            remaining -= amount;
        }
    }
    else if (shift < 0)
    {
        for (std::int64_t copied = 0; copied < tail_size;)
        {
            const auto amount = static_cast<DWORD>(std::min<std::int64_t>(tail_size - copied, chunk_size));
            DWORD read = 0;
            DWORD written = 0;
            if (!seek(tail_start + copied) || !ReadFile(file.handle, buffer.data(), amount, &read, nullptr) ||
                read != amount || !seek(tail_start + copied + shift) ||
                !WriteFile(file.handle, buffer.data(), amount, &written, nullptr) || written != amount)
                return false;
            copied += amount;
        }
    }
    if (insert && !insert->empty())
    {
        DWORD written = 0;
        if (!seek(current.QuadPart) || !WriteFile(file.handle, insert->data(), static_cast<DWORD>(insert->size()),
                                                &written, nullptr) || written != insert->size())
            return false;
    }
    if (shift < 0 && (!seek(size.QuadPart + shift) || !SetEndOfFile(file.handle)))
        return false;
    return seek(current.QuadPart + inserted);
}
} // namespace

std::int32_t CoreOpenFile(const std::string &path, std::int32_t mode, std::int32_t sharing)
{
    DWORD access = 0;
    DWORD creation = 0;
    DWORD share_mode = 0;
    bool readable = false;
    bool writable = false;
    const auto native_path = wide_path(path);
    if (native_path.empty() || !open_options(mode, access, creation, readable, writable) ||
        !sharing_options(sharing, share_mode))
        return 0;

    auto item = std::make_unique<OpenFileState>();
    item->handle =
        CreateFileW(native_path.c_str(), access, share_mode, nullptr, creation, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (item->handle == INVALID_HANDLE_VALUE)
        return 0;
    item->readable = readable;
    item->writable = writable;

    std::scoped_lock lock(files_mutex);
    const auto handle = next_handle++;
    files.emplace(handle, std::move(item));
    return handle;
}

void CoreCloseFile(std::int32_t handle)
{
    std::scoped_lock lock(files_mutex);
    files.erase(handle);
}

void CoreCloseAllFiles()
{
    std::scoped_lock lock(files_mutex);
    files.clear();
}

bool CoreSeekFile(std::int32_t handle, std::int32_t origin, std::int64_t offset)
{
    std::scoped_lock lock(files_mutex);
    auto *item = find_file(handle);
    if (!item)
        return false;
    const DWORD method = origin == 2 ? FILE_END : origin == 3 ? FILE_CURRENT : FILE_BEGIN;
    LARGE_INTEGER distance{};
    distance.QuadPart = offset;
    return SetFilePointerEx(item->handle, distance, nullptr, method) != FALSE;
}

bool CoreSeekFile(std::int32_t handle, std::int64_t offset)
{
    return CoreSeekFile(handle, 1, offset);
}

bool CoreSeekFileBegin(std::int32_t handle)
{
    return CoreSeekFile(handle, 1, 0);
}

bool CoreSeekFileEnd(std::int32_t handle)
{
    return CoreSeekFile(handle, 2, 0);
}

sulang::core::ByteArray CoreReadBinary(std::int32_t handle, std::int32_t length)
{
    std::scoped_lock lock(files_mutex);
    auto *item = find_file(handle);
    if (!item || !item->readable)
        return {};
    if (length < 0)
    {
        LARGE_INTEGER current{};
        LARGE_INTEGER size{};
        if (!position(*item, current) || !GetFileSizeEx(item->handle, &size))
            return {};
        const auto remaining = std::max<std::int64_t>(0, size.QuadPart - current.QuadPart);
        length = static_cast<std::int32_t>(std::min<std::int64_t>(remaining, INT32_MAX));
    }
    sulang::core::ByteArray result(static_cast<std::size_t>(std::max(0, length)));
    DWORD read = 0;
    if (!result.empty() && !ReadFile(item->handle, result.data(), static_cast<DWORD>(result.size()), &read, nullptr))
        return {};
    result.resize(read);
    return result;
}

bool CoreWriteBinary(std::int32_t handle, const sulang::core::ByteArray &data)
{
    std::scoped_lock lock(files_mutex);
    auto *item = find_file(handle);
    if (!item || !item->writable || data.size() > MAXDWORD)
        return false;
    DWORD written = 0;
    return WriteFile(item->handle, data.data(), static_cast<DWORD>(data.size()), &written, nullptr) != FALSE &&
           written == data.size();
}

std::string CoreReadText(std::int32_t handle, std::int32_t length)
{
    const auto data = CoreReadBinary(handle, length);
    return {data.begin(), data.end()};
}

bool CoreWriteText(std::int32_t handle, const std::string &value)
{
    return CoreWriteBinary(handle, {value.begin(), value.end()});
}

std::string CoreReadLine(std::int32_t handle)
{
    std::scoped_lock lock(files_mutex);
    auto *item = find_file(handle);
    if (!item || !item->readable)
        return {};
    std::string result;
    char value = 0;
    DWORD read = 0;
    while (ReadFile(item->handle, &value, 1, &read, nullptr) != FALSE && read == 1)
    {
        if (value == '\n')
            break;
        if (value != '\r')
            result.push_back(value);
    }
    return result;
}

bool CoreWriteLine(std::int32_t handle, const std::string &value)
{
    return CoreWriteText(handle, value + "\r\n");
}

bool CoreInsertBinary(std::int32_t handle, const sulang::core::ByteArray &data)
{
    std::scoped_lock lock(files_mutex);
    auto *item = find_file(handle);
    return item && shift_file_data(*item, &data, 0);
}

bool CoreInsertText(std::int32_t handle, const std::string &value)
{
    return CoreInsertBinary(handle, {value.begin(), value.end()});
}

bool CoreInsertLine(std::int32_t handle, const std::string &value)
{
    return CoreInsertText(handle, value + "\r\n");
}

bool CoreDeleteFileData(std::int32_t handle, std::int32_t length)
{
    std::scoped_lock lock(files_mutex);
    auto *item = find_file(handle);
    return item && shift_file_data(*item, nullptr, length);
}

bool CoreLockFile(std::int32_t handle, std::int32_t offset, std::int32_t length, std::int32_t retry_ms)
{
    if (offset < 0 || length <= 0)
        return false;
    HANDLE native = INVALID_HANDLE_VALUE;
    {
        std::scoped_lock lock(files_mutex);
        auto *item = find_file(handle);
        if (!item)
            return false;
        if (!DuplicateHandle(GetCurrentProcess(), item->handle, GetCurrentProcess(), &native, 0, FALSE,
                             DUPLICATE_SAME_ACCESS))
            return false;
    }
    const auto started = std::chrono::steady_clock::now();
    for (;;)
    {
        OVERLAPPED region{};
        region.Offset = static_cast<DWORD>(offset);
        if (LockFileEx(native, LOCKFILE_EXCLUSIVE_LOCK | LOCKFILE_FAIL_IMMEDIATELY,
                       0, static_cast<DWORD>(length), 0, &region))
        {
            CloseHandle(native);
            return true;
        }
        if (retry_ms == 0 || (retry_ms > 0 &&
            std::chrono::steady_clock::now() - started >= std::chrono::milliseconds(retry_ms)))
        {
            CloseHandle(native);
            return false;
        }
        Sleep(10);
    }
}

bool CoreUnlockFile(std::int32_t handle, std::int32_t offset, std::int32_t length)
{
    if (offset < 0 || length <= 0)
        return false;
    std::scoped_lock lock(files_mutex);
    auto *item = find_file(handle);
    if (!item)
        return false;
    OVERLAPPED region{};
    region.Offset = static_cast<DWORD>(offset);
    return UnlockFileEx(item->handle, 0, static_cast<DWORD>(length), 0, &region) != FALSE;
}

bool CoreFileEof(std::int32_t handle, bool text_mode)
{
    std::scoped_lock lock(files_mutex);
    auto *item = find_file(handle);
    if (!item || !item->readable)
        return true;
    LARGE_INTEGER current{};
    LARGE_INTEGER size{};
    if (!position(*item, current) || !GetFileSizeEx(item->handle, &size) || current.QuadPart >= size.QuadPart)
        return true;
    if (!text_mode)
        return false;

    std::uint8_t value = 0;
    DWORD read = 0;
    const bool succeeded = ReadFile(item->handle, &value, 1, &read, nullptr) != FALSE;
    LARGE_INTEGER restore{};
    restore.QuadPart = current.QuadPart;
    SetFilePointerEx(item->handle, restore, nullptr, FILE_BEGIN);
    return !succeeded || read == 0 || value == 0 || value == 26;
}

std::int64_t CoreFilePosition(std::int32_t handle)
{
    std::scoped_lock lock(files_mutex);
    auto *item = find_file(handle);
    LARGE_INTEGER current{};
    return item && position(*item, current) ? current.QuadPart : -1;
}

std::int64_t CoreOpenedFileLength(std::int32_t handle)
{
    std::scoped_lock lock(files_mutex);
    auto *item = find_file(handle);
    LARGE_INTEGER size{};
    return item && GetFileSizeEx(item->handle, &size) != FALSE ? size.QuadPart : -1;
}
