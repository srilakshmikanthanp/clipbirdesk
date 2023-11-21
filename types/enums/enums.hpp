#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QtTypes>

namespace srilakshmikanthanp::clipbirdesk::types::enums {
/// @brief IP type used in the discovery packet
enum class IPType : quint32 {
  IPv4 = 0x00,
  IPv6 = 0x01,
};

/// @brief Allowed Error Codes
enum ErrorCode : quint32 {
  CodingError   = 0x01,
  InvalidPacket = 0x02,
};

/// @brief Allowed Authentication Status
enum AuthStatus : quint32 {
  AuthOkay  = 0x00,
  AuthFail  = 0x01,
};

/// @brief Allowed Ping Types
enum PingType : quint32 {
  Ping = 0x00,
  Pong = 0x01
};
}  // namespace srilakshmikanthanp::clipbirdesk::types::enums
