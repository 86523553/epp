#pragma once
#include "types.h"

// @name 取主机名
// @description 返回本机 DNS 主机名。
// @return_type 文本型
std::string CoreHostName();
// @name 通信测试
// @description 向 IPv4 主机发送 ICMP 回显请求，成功时返回往返毫秒数，失败时返回 -1。
// @return_type 整数型
// @param_name host 被测试主机地址
// @param_type host 文本型
// @param host 可以为主机名、IP地址等。
// @param_name timeout_ms 最长等待时间
// @param_type timeout_ms 整数型
// @param timeout_ms
// 可以被省略。指定最长等待被测试主机响应的时间，单位为毫秒。超过此时间即认为无法与被测试主机通讯。如果省略本参数，则默认为
// 10 * 1000 毫秒，即 10 秒。
std::int32_t CorePing(const std::string &host, std::int32_t timeout_ms = 10000);
// @name 转换为主机名
// @description 对 IPv4 或 IPv6 地址执行反向 DNS 查询，失败时返回空文本。
// @return_type 文本型
// @param_name address 欲转换IP地址
// @param_type address 文本型
// @param address 指定欲转换IP地址。
std::string CoreIpToHostName(const std::string &address);
// @name 转换为IP地址
// @description 将主机名解析为首个 IPv4 或 IPv6 数字地址，失败时返回空文本。
// @return_type 文本型
// @param_name host 欲转换主机名
// @param_type host 文本型
// @param host 指定欲转换主机名。
std::string CoreHostNameToIp(const std::string &host);
