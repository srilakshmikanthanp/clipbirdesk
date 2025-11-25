#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Local header files
#include "packets/invalidrequest/invalidrequest.hpp"
#include "packets/syncingpacket/syncingpacket.hpp"
#include "packets/pingpongpacket/pingpongpacket.hpp"
#include "packets/authentication/authentication.hpp"
#include "packets/certificate_exchange_packet/certificate_exchange_packet.hpp"
#include "packets/network_packet.hpp"

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
