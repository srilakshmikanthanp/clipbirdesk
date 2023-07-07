#pragma once // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <cstdint>
#include <vector>

namespace srilakshmikanthanp::clipbirdesk::types {
/// @brief type for storing variable length data
using varies_t = std::vector<std::uint8_t>;

/// @brief IP type
enum class IPType : std::uint8_t {
  IPv4 = 0,
  IPv6 = 1,
};
}  // namespace srilakshmikanthanp::clipbirdesk::types
