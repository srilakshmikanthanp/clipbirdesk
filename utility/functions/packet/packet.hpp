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
#include "network/packets/discoverypacket/discoverypacket.hpp"
#include "network/packets/invalidrequest/invalidrequest.hpp"
#include "network/packets/syncingpacket/syncingpacket.hpp"
#include "types/enums/enums.hpp"
#include "utility/functions/ipconv/ipconv.hpp"

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
 * @brief Parameters for the ErrorMessage
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
    internals::DiscoveryPacketParams params);

/**
 * @brief Create the ErrorMessage
 *
 * @param packetType
 * @param errorCode
 * @param errorMessage
 *
 * @return ErrorMessage
 */
network::packets::InvalidRequest createPacket(
    internals::InvalidPacketParams params);

/**
 * @brief Create the SyncingItem
 *
 * @param mime
 * @param payload
 *
 * @return SyncingItem
 */
network::packets::SyncingItem createPacket(
    internals::SyncingItemParams params);

/**
 * @brief Create the SyncingPacket
 *
 * @param packetType
 * @param items
 *
 * @return SyncingPacket
 */
network::packets::SyncingPacket createPacket(
    internals::SyncingPacketParams params);

/**
 * @brief Create the SyncingPacket
 *
 * @param packetType
 * @param items
 *
 * @return SyncingPacket
 */
network::packets::SyncingPacket createPacket(
    quint8 packetType, QVector<QPair<QString, QByteArray>> items);
}  // namespace srilakshmikanthanp::clipbirdesk::utility::functions
