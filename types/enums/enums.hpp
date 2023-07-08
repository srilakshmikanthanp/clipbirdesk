#pragma once // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <cstdint>

namespace srilakshmikanthanp::clipbirdesk::types::enums {
/// @brief IP type
enum class IPType : std::uint8_t {
  IPv4 = 0,
  IPv6 = 1,
};
}  // namespace srilakshmikanthanp::clipbirdesk::types
