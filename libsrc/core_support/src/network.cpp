#include "network.h"

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <icmpapi.h>

#include <array>
#include <cstring>

namespace
{
struct WinsockSession
{
    WinsockSession()
    {
        WSADATA data{};
        ready = WSAStartup(MAKEWORD(2, 2), &data) == 0;
    }
    ~WinsockSession()
    {
        if (ready)
            WSACleanup();
    }
    bool ready = false;
};
WinsockSession &winsock()
{
    static WinsockSession value;
    return value;
}
} // namespace

std::string CoreHostName()
{
    if (!winsock().ready)
        return {};
    std::array<char, 256> buffer{};
    return gethostname(buffer.data(), static_cast<int>(buffer.size())) == 0 ? std::string(buffer.data())
                                                                            : std::string{};
}
std::int32_t CorePing(const std::string &host, std::int32_t timeout_ms)
{
    if (!winsock().ready)
        return -1;
    addrinfo hints{};
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_RAW;
    addrinfo *info = nullptr;
    if (getaddrinfo(host.c_str(), nullptr, &hints, &info) != 0 || info == nullptr)
        return -1;
    const auto address = reinterpret_cast<sockaddr_in *>(info->ai_addr)->sin_addr.S_un.S_addr;
    freeaddrinfo(info);
    const HANDLE handle = IcmpCreateFile();
    if (handle == INVALID_HANDLE_VALUE)
        return -1;
    constexpr char payload[] = "SuLang";
    std::array<std::uint8_t, sizeof(ICMP_ECHO_REPLY) + sizeof(payload) + 32> reply{};
    const auto count =
        IcmpSendEcho(handle, address, const_cast<char *>(payload), sizeof(payload), nullptr, reply.data(),
                     static_cast<DWORD>(reply.size()), static_cast<DWORD>(std::max<std::int32_t>(1, timeout_ms)));
    IcmpCloseHandle(handle);
    return count == 0 ? -1 : static_cast<std::int32_t>(reinterpret_cast<PICMP_ECHO_REPLY>(reply.data())->RoundTripTime);
}
std::string CoreIpToHostName(const std::string &address)
{
    if (!winsock().ready)
        return {};
    sockaddr_storage storage{};
    int length = 0;
    auto *ipv4 = reinterpret_cast<sockaddr_in *>(&storage);
    auto *ipv6 = reinterpret_cast<sockaddr_in6 *>(&storage);
    if (InetPtonA(AF_INET, address.c_str(), &ipv4->sin_addr) == 1)
    {
        ipv4->sin_family = AF_INET;
        length = sizeof(sockaddr_in);
    }
    else if (InetPtonA(AF_INET6, address.c_str(), &ipv6->sin6_addr) == 1)
    {
        ipv6->sin6_family = AF_INET6;
        length = sizeof(sockaddr_in6);
    }
    else
        return {};
    std::array<char, NI_MAXHOST> host{};
    return getnameinfo(reinterpret_cast<sockaddr *>(&storage), length, host.data(), static_cast<DWORD>(host.size()),
                       nullptr, 0, NI_NAMEREQD) == 0
               ? std::string(host.data())
               : std::string{};
}
std::string CoreHostNameToIp(const std::string &host)
{
    if (!winsock().ready)
        return {};
    addrinfo hints{};
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    addrinfo *info = nullptr;
    if (getaddrinfo(host.c_str(), nullptr, &hints, &info) != 0 || info == nullptr)
        return {};
    std::array<char, INET6_ADDRSTRLEN> buffer{};
    const void *address = info->ai_family == AF_INET
                              ? static_cast<void *>(&reinterpret_cast<sockaddr_in *>(info->ai_addr)->sin_addr)
                              : static_cast<void *>(&reinterpret_cast<sockaddr_in6 *>(info->ai_addr)->sin6_addr);
    const auto result =
        InetNtopA(info->ai_family, const_cast<void *>(address), buffer.data(), static_cast<DWORD>(buffer.size()));
    freeaddrinfo(info);
    return result == nullptr ? std::string{} : std::string(buffer.data());
}
