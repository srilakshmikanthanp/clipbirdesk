#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

// Local header files
#include "network/packets/DiscoveryPacket.hpp"
#include "network/packets/InvalidPacket.hpp"
#include "network/packets/SyncingPacket.hpp"

namespace srilakshmikanthanp::clipbirdesk::utility::functions {
/**
 * @brief Convert the Packet to QByteArray
 *
 * @tparam Packet
 * @param packet
 */
template <typename Packet>
QByteArray toQByteArray(const Packet& packet) {
  // create the byte array
  QByteArray data;

  // create the data stream
  QDataStream stream(&data, QIODevice::WriteOnly);

  // set the version
  stream.setVersion(QDataStream::Qt_5_15);

  // set Byte Order
  stream.setByteOrder(QDataStream::BigEndian);

  // write the packet
  stream << packet;

  // return the data
  return data;
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
  // create the packet
  Packet packet;

  // create the data stream
  QDataStream stream(data);

  // set the version
  stream.setVersion(QDataStream::Qt_5_15);

  // set Byte Order
  stream.setByteOrder(QDataStream::BigEndian);

  // read the packet
  stream >> packet;

  // return the packet
  return packet;
}
} // namespace srilakshmikanthanp::clipbirdesk::utility::functions
