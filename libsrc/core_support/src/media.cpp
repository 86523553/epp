#include "media.h"

#include <windows.h>
#include <mmsystem.h>

#include <mutex>
#include <string>

namespace
{
std::mutex media_mutex;
constexpr wchar_t alias_name[] = L"sulang_core_mp3";
std::wstring wide(const std::string &value)
{
    const int size = MultiByteToWideChar(CP_UTF8, 0, value.data(), static_cast<int>(value.size()), nullptr, 0);
    std::wstring result(static_cast<std::size_t>(std::max(0, size)), L'\0');
    if (size > 0)
        MultiByteToWideChar(CP_UTF8, 0, value.data(), static_cast<int>(value.size()), result.data(), size);
    return result;
}
bool mci(const std::wstring &command, std::wstring *output = nullptr)
{
    wchar_t buffer[128]{};
    const auto code = mciSendStringW(command.c_str(), output ? buffer : nullptr, output ? 128 : 0, nullptr);
    if (code == 0 && output)
        *output = buffer;
    return code == 0;
}
} // namespace

bool CorePlayMusic(const std::string &file, bool loop)
{
    const auto path = wide(file);
    return PlaySoundW(path.c_str(), nullptr, SND_FILENAME | SND_ASYNC | (loop ? SND_LOOP : 0)) != FALSE;
}
void CoreStopMusic()
{
    std::scoped_lock lock(media_mutex);
    PlaySoundW(nullptr, nullptr, 0);
    mci(std::wstring(L"close ") + alias_name);
}
bool CorePlayMp3(std::int32_t repeat_count, const std::string &file)
{
    std::scoped_lock lock(media_mutex);
    mci(std::wstring(L"close ") + alias_name);
    if (repeat_count != -1 && repeat_count != 1)
        return false;
    if (!mci(L"open \"" + wide(file) + L"\" type mpegvideo alias " + alias_name))
        return false;
    std::wstring command = std::wstring(L"play ") + alias_name;
    if (repeat_count == -1)
        command += L" repeat";
    return mci(command);
}

bool CorePlayMp3(const std::string &file)
{
    return CorePlayMp3(1, file);
}
bool CorePlayMp3Sync(const std::string &file, std::int32_t start_ms)
{
    std::scoped_lock lock(media_mutex);
    mci(std::wstring(L"close ") + alias_name);
    if (!mci(L"open \"" + wide(file) + L"\" type mpegvideo alias " + alias_name))
        return false;
    return mci(std::wstring(L"play ") + alias_name + L" from " + std::to_wstring(std::max<std::int32_t>(0, start_ms)) +
               L" wait");
}
void CorePauseMp3()
{
    std::scoped_lock lock(media_mutex);
    mci(std::wstring(L"pause ") + alias_name);
}
void CoreResumeMp3()
{
    std::scoped_lock lock(media_mutex);
    mci(std::wstring(L"resume ") + alias_name);
}
std::int32_t CoreMp3State()
{
    std::scoped_lock lock(media_mutex);
    std::wstring value;
    if (!mci(std::wstring(L"status ") + alias_name + L" mode", &value))
        return -1;
    return value == L"playing" ? 1 : value == L"paused" ? -1 : 0;
}
