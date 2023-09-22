#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

// Local header files
#include "network/packets/invalidrequest/invalidrequest.hpp"
#include "network/packets/syncingpacket/syncingpacket.hpp"

namespace srilakshmikanthanp::clipbirdesk::utility::functions {
/**
 * @brief Convert the Packet to QByteArray
 *
 * @tparam Packet
 * @param packet
 */
template <typename Packet>
QByteArray toQByteArray(const Packet& packet) {
  return packet.toBytes();
}

/**
 * @brief Convert the QByteArray to Packet
 *
 * @tparam Packet
 * @param data
 * @return Packet
 */
template <typename Packet>
Packet fromQByteArray(const QByteArray& data) {
  return Packet::fromBytes(data);
}
}  // namespace srilakshmikanthanp::clipbirdesk::utility::functions
