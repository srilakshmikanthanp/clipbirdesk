#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Qt header files
#include <QByteArray>
#include <QHostAddress>
#include <QPair>
#include <QString>
#include <QVector>
#include <QtTypes>

// Local header files
#include "packets/authentication/authentication.hpp"
#include "packets/invalidrequest/invalidrequest.hpp"
#include "packets/pingpongpacket/pingpongpacket.hpp"
#include "packets/syncingpacket/syncingpacket.hpp"
#include "types/enums/enums.hpp"
#include "utility/functions/ipconv/ipconv.hpp"

namespace srilakshmikanthanp::clipbirdesk::utility::functions::params {
/**
 * @brief parameters for Authentication
 */
struct AuthenticationParams {
  quint32 packetType;
  quint32 authStatus;
};

/**
 * @brief Parameters for the ErrorMessage
 */
struct InvalidPacketParams {
  quint32 packetType;
  quint32 errorCode;
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
  quint32 packetType;
  QVector<QPair<QString, QByteArray>> items;
};

/**
 * @brief PingPongPacket parameters
 */
struct PingPacketParams {
  quint32 packetType;
  quint32 pingType;
};
}  // namespace srilakshmikanthanp::clipbirdesk::utility::functions::params

namespace srilakshmikanthanp::clipbirdesk::utility::functions {
/**
 * @brief Create the Authentication
 *
 * @param packetType
 * @param authStatus
 *
 * @return Authentication
 */
network::packets::Authentication createPacket(params::AuthenticationParams params);

/**
 * @brief Create the ErrorMessage
 *
 * @param packetType
 * @param errorCode
 * @param errorMessage
 *
 * @return ErrorMessage
 */
network::packets::InvalidRequest createPacket(params::InvalidPacketParams params);

/**
 * @brief Create the SyncingItem
 *
 * @param mime
 * @param payload
 *
 * @return SyncingItem
 */
network::packets::SyncingItem createPacket(params::SyncingItemParams params);

/**
 * @brief Create the SyncingPacket
 *
 * @param packetType
 * @param items
 *
 * @return SyncingPacket
 */
network::packets::SyncingPacket createPacket(params::SyncingPacketParams params);

/**
 * @brief Create the PingPongPacket
 *
 * @param packetType
 * @param pingType
 *
 * @return PingPongPacket
 */
network::packets::PingPongPacket createPacket(params::PingPacketParams params);
}  // namespace srilakshmikanthanp::clipbirdesk::utility::functions
