#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QtTypes>

namespace srilakshmikanthanp::clipbirdesk::types::enums {
/// @brief IP type used in the discovery packet
enum class IPType : quint8 {
  IPv4 = 0x00,
  IPv6 = 0x01,
};

/// @brief Allowed Error Codes
enum ErrorCode : quint8 {
  CodingError   = 0x01,
  InvalidPacket = 0x02,
};

/// @brief Allowed Authentication Types
enum AuthType : quint8 {
  AuthReq = 0x00,
  AuthRes = 0x01,
};

/// @brief Allowed Authentication Status
enum AuthStatus : quint8 {
  AuthOkay  = 0x00,
  AuthFail  = 0x01,
};
}  // namespace srilakshmikanthanp::clipbirdesk::types::enums
