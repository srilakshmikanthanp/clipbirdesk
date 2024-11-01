#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

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
