#include "packet.hpp"

namespace srilakshmikanthanp::clipbirdesk::utility::functions {
/**
 * @brief Create the Authentication
 *
 *
 * @param authStatus
 *
 * @return Authentication
 */
packets::Authentication createPacket(params::AuthenticationParams params) {
  packets::Authentication packet;
  packet.setAuthStatus(params.authStatus);
  return packet;
}

/**
 * @brief Create the CertificateExchangePacket
 *
 *
 * @param certificate
 *
 * @return CertificateExchangePacket
 */
packets::CertificateExchangePacket createPacket(params::CertificateExchangePacketParams params) {
  packets::CertificateExchangePacket packet;
  packet.setCertificate(params.certificate);
  return packet;
}

/**
 * @brief Create the ErrorMessage
 *
 *
 * @param errorCode
 * @param errorMessage
 *
 * @return ErrorMessage
 */
packets::InvalidRequest createPacket(params::InvalidPacketParams params) {
  packets::InvalidRequest packet;
  packet.setErrorCode(params.errorCode);
  packet.setErrorMessage(params.errorMessage.toUtf8());
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
packets::SyncingItem createPacket(params::SyncingItemParams params) {
  packets::SyncingItem syncItem;
  syncItem.setMimeType(params.mimeType.toUtf8());
  syncItem.setPayload(params.payload);
  return syncItem;
}

/**
 * @brief Create the SyncingPacket
 *
 *
 * @param items
 *
 * @return SyncingPacket
 */
packets::SyncingPacket createPacket(params::SyncingPacketParams params) {
  packets::SyncingPacket packet;
  QVector<packets::SyncingItem> items;
  items.reserve(params.items.size());

  for (const auto& [mime, payload] : params.items) {
    items.push_back(createPacket({mime, payload}));
  }

  packet.setItems(items);
  return packet;
}

/**
 * @brief Create the PingPongPacket
 *
 *
 * @param pingType
 *
 * @return PingPongPacket
 */
packets::PingPongPacket createPacket(params::PingPacketParams params) {
  packets::PingPongPacket packet;
  packet.setPingType(params.pingType);
  return packet;
}
}  // namespace srilakshmikanthanp::clipbirdesk::utility::functions
