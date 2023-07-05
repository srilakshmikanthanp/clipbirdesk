#pragma once // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "../../net/packets/packets.hpp"

namespace srilakshmikanthanp::clipbirdesk::utility::functions::packet {
/**
 * @brief Calculate the Size of ServerDiscoveryPacket
 * @param packet
 * @return std::size_t
 */
std::size_t sizeOfPacket(net::packets::ServerDiscoveryPacket packet) {
  // get the size of packet type
  auto size = sizeof(net::packets::ServerDiscoveryPacket);

  // add the bytes for ip
  if (packet.getIpType() == 0x04) {
    size -= 12;
  }

  // Done return the size
  return size;
}

/**
 * @brief Calculate the Size of ClipbirdSyncPacket
 *
 * @param packet
 * @return std::size_t
 */
std::size_t sizeOfPacket(net::packets::ClipbirdSyncPacket packet) {
  // size of data type to subtract
  auto extra = sizeof(packet.getDataType()) + sizeof(packet.getData());

  // get the size of packet type
  auto size = sizeof(net::packets::ClipbirdSyncPacket) - extra;

  // Add the size of data type
  size += packet.getDataType().size() + packet.getData().size();

  // Done return the size
  return size;
}
}
