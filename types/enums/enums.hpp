#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QtTypes>

namespace srilakshmikanthanp::clipbirdesk::types::enums {
/// @brief IP type used in the discovery packet
enum class IPType : quint8 {
  IPv4 = 0,
  IPv6 = 1,
};

/// @brief Allowed Error Codes
enum ErrorCode : quint8 {
  CodingError = 0x01,
  SSLError = 0x02,
};
}  // namespace srilakshmikanthanp::clipbirdesk::types::enums
