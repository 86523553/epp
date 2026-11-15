#pragma once

#include <any>
#include <chrono>
#include <cstdint>
#include <string>
#include <vector>

namespace sulang::core
{
using ByteArray = std::vector<std::uint8_t>;
using StringArray = std::vector<std::string>;
using DateTime = std::chrono::system_clock::time_point;
} // namespace sulang::core
