#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

// Qt header files
#include <QByteArray>
#include <QHostAddress>
#include <QPair>
#include <QString>
#include <QVector>
#include <QtTypes>

// Local header files
#include "network/packets/DiscoveryPacket.hpp"
#include "network/packets/InvalidPacket.hpp"
#include "network/packets/SyncingPacket.hpp"
#include "types/enums/enums.hpp"
#include "utility/functions/ipconv.hpp"

namespace srilakshmikanthanp::clipbirdesk::utility::functions::internals {
/**
 * @brief Parameters for the DiscoveryPacket
 */
struct DiscoveryPacketParams {
  quint8 packetType;
  types::enums::IPType ipType;
  const QHostAddress& ipaddr;
  quint16 port;
};

/**
 * @brief Parameters for the InvalidPacket
 */
struct InvalidPacketParams {
  quint8 packetType;
  quint8 errorCode;
  const QString& errorMessage;
};

/**
 * @brief parameters for the SyncingItem
 */
struct SyncingItemParams {
  const QString& mimeType;
  const QByteArray& payload;
};

/**
 * @brief parameters for the SyncingPacket
 */
struct SyncingPacketParams {
  quint8 packetType;
  QVector<QPair<QString, QByteArray>> items;
};
}  // namespace srilakshmikanthanp::clipbirdesk::utility::functions::internals

namespace srilakshmikanthanp::clipbirdesk::utility::functions {
/**
 * @brief Create the DiscoveryPacket
 *
 * @param packetType
 * @param ipType
 * @param ipaddr
 * @param port
 *
 * @return DiscoveryPacket
 */
network::packets::DiscoveryPacket createPacket(
    internals::DiscoveryPacketParams params) {
  // create the packet
  network::packets::DiscoveryPacket packet;

  // set the packet type
  packet.setPacketType(params.packetType);

  // set the ip type
  packet.setIpType(params.ipType);

  // set the ip address
  if (params.ipType == types::enums::IPType::IPv4) {
    packet.setHostIp(toIPV4QByteArray(params.ipaddr));
  } else {
    packet.setHostIp(toIPV6QByteArray(params.ipaddr));
  }

  // set the port
  packet.setHostPort(params.port);

  // set the packet length
  packet.setPacketLength(packet.size());

  // return the packet
  return packet;
}

/**
 * @brief Create the InvalidPacket
 *
 * @param packetType
 * @param errorCode
 * @param errorMessage
 *
 * @return InvalidPacket
 */
inline network::packets::InvalidPacket createPacket(
    internals::InvalidPacketParams params) {
  // create the packet
  network::packets::InvalidPacket packet;

  // set the packet type
  packet.setPacketType(params.packetType);

  // set the error code
  packet.setErrorCode(params.errorCode);

  // set the error message
  packet.setErrorMessage(params.errorMessage.toUtf8());

  // set the packet length
  packet.setPacketLength(packet.size());

  // return the packet
  return packet;
}

/**
 * @brief Create the SyncingItem
 *
 * @param mime
 * @param payload
 *
 * @return SyncingItem
 */
inline network::packets::SyncingItem createPacket(
    internals::SyncingItemParams params) {
  // create the SyncingItem
  network::packets::SyncingItem syncItem;

  // set the mime length
  syncItem.setMimeLength(params.mimeType.size());

  // set the mime type
  syncItem.setMimeType(params.mimeType.toUtf8());

  // set the payload length
  syncItem.setPayloadLength(params.payload.size());

  // set the payload
  syncItem.setPayload(params.payload);

  // return the SyncingItem
  return syncItem;
}

/**
 * @brief Create the SyncingPacket
 *
 * @param packetType
 * @param items
 *
 * @return SyncingPacket
 */
inline network::packets::SyncingPacket createPacket(
    internals::SyncingPacketParams params) {
  // create the packet
  network::packets::SyncingPacket packet;

  // set the packet type
  packet.setPacketType(params.packetType);

  // set the item count
  packet.setItemCount(params.items.size());

  // Convert the items to SyncingItem
  QVector<network::packets::SyncingItem> items;

  // reserve the memory
  items.reserve(params.items.size());

  // convert the items
  for (const auto& [mime, payload] : params.items) {
    items.push_back(createPacket({mime, payload}));
  }

  // set the items
  packet.setItems(items);

  // set the packet length
  packet.setPacketLength(packet.size());

  // return the packet
  return packet;
}
}  // namespace srilakshmikanthanp::clipbirdesk::utility::functions
