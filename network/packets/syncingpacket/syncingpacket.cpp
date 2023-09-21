// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "syncingpacket.hpp"

namespace srilakshmikanthanp::clipbirdesk::network::packets {
/**
 * @brief Set the Mime Length object
 */
void SyncingItem::setMimeLength(quint32 length) {
  this->mimeLength = length;
}

/**
 * @brief Get the Mime Length object
 *
 * @return qint32
 */
quint32 SyncingItem::getMimeLength() const noexcept {
  return this->mimeLength;
}

/**
 * @brief Set the Mime Type object
 *
 * @param type
 */
void SyncingItem::setMimeType(const QByteArray& type) {
  if (type.size() != this->mimeLength) {
    throw std::invalid_argument("Invalid Mime Type");
  } else {
    this->mimeType = type;
  }
}

/**
 * @brief Get the Mime Type object
 *
 * @return QByteArray
 */
QByteArray SyncingItem::getMimeType() const noexcept {
  return this->mimeType;
}

/**
 * @brief Set the Payload Length object
 *
 * @param length
 */
void SyncingItem::setPayloadLength(quint32 length) {
  this->payloadLength = length;
}

/**
 * @brief Get the Payload Length object
 *
 * @return qint32
 */
quint32 SyncingItem::getPayloadLength() const noexcept {
  return this->payloadLength;
}

/**
 * @brief Set the Payload object
 *
 * @param payload
 */
void SyncingItem::setPayload(const QByteArray& payload) {
  if (payload.size() != this->payloadLength) {
    throw std::invalid_argument("Invalid Payload");
  } else {
    this->payload = payload;
  }
}

/**
 * @brief Get the Payload object
 *
 * @return QByteArray
 */
QByteArray SyncingItem::getPayload() const noexcept {
  return this->payload;
}

/**
 * @brief Get the size of the packet
 *
 * @return size_t
 */
quint32 SyncingItem::size() const noexcept {
  return quint32(
    sizeof(this->mimeLength) +
    this->mimeType.size() +
    sizeof(this->payloadLength) +
    this->payload.size()
  );
}

/**
 * @brief From Bytes
 */
SyncingItem SyncingItem::fromBytes(const QByteArray &array) {
  // using the utility functions
  using utility::functions::fromQByteArray;

  // Create the SyncingItem
  SyncingItem payload;

  // Set the Mime Length
  payload.mimeLength = fromQByteArray<quint32>(array.mid(0, 4));
  payload.mimeType = array.mid(4, payload.mimeLength);
  payload.payloadLength = fromQByteArray<quint32>(array.mid(4 + payload.mimeLength, 4));
  payload.payload = array.mid(8 + payload.mimeLength, payload.payloadLength);

  // Return the SyncingItem
  return payload;
}

/**
 * @brief to Bytes
 */
QByteArray SyncingItem::toBytes(const SyncingItem& payload) {
  // using the toQByteArray function
  using utility::functions::toQByteArray;

  // Create the QByteArray
  QByteArray array;

  // Set the Mime Length
  array.append(toQByteArray(payload.mimeLength));
  array.append(payload.mimeType);
  array.append(toQByteArray(payload.payloadLength));
  array.append(payload.payload);

  // Return the QByteArray
  return array;
}

/**
 * @brief Set the Packet Length object
 *
 * @param length
 */
void SyncingPacket::setPacketLength(quint32 length) {
  this->packetLength = length;
}

/**
 * @brief Get the Packet Length object
 *
 * @return qint32
 */
quint32 SyncingPacket::getPacketLength() const noexcept {
  return this->packetLength;
}

/**
 * @brief Set the Packet Type object
 *
 * @param type
 */
void SyncingPacket::setPacketType(quint32 type) {
  if (type != PacketType::SyncPacket) {
    throw std::invalid_argument("Invalid Packet Type");
  }
}

/**
 * @brief Get the Packet Type object
 *
 * @return quint32
 */
quint32 SyncingPacket::getPacketType() const noexcept {
  return this->packetType;
}

/**
 * @brief Set the Item Count object
 *
 * @param count
 */
void SyncingPacket::setItemCount(quint32 count) {
  this->itemCount = count;
}

/**
 * @brief Get the Item Count object
 *
 * @return qint32
 */
quint32 SyncingPacket::getItemCount() const noexcept {
  return this->itemCount;
}

/**
 * @brief Set the Payloads object
 *
 * @param payloads
 */
void SyncingPacket::setItems(const QVector<SyncingItem>& payloads) {
  if (payloads.size() != this->itemCount) {
    throw std::invalid_argument("Invalid Payloads");
  }

  this->items = payloads;
}

/**
 * @brief Get the Payloads object
 *
 * @return QVector<Payload>
 */
QVector<SyncingItem> SyncingPacket::getItems() const noexcept {
  return this->items;
}

/**
 * @brief Get the size of the packet
 *
 * @return size_t
 */
quint32 SyncingPacket::size() const noexcept {
  size_t size = (
    sizeof(this->packetType) + sizeof(this->packetLength) + sizeof(this->itemCount)
  );

  for (const auto& payload : this->items) {
    size += payload.size();
  }

  return qint32(size);
}

/**
 * @brief From Bytes
 */
SyncingPacket SyncingPacket::fromBytes(const QByteArray &array) {

}

/**
 * @brief to Bytes
 */
QByteArray SyncingPacket::toBytes(const SyncingPacket& packet) {

}
}  // namespace srilakshmikanthanp::clipbirdesk::network::packets
