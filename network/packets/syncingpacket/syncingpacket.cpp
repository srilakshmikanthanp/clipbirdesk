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
 * @brief To Stream
 */
void SyncingItem::toStream(QDataStream& stream) const {
  stream << this->mimeLength;
  stream.writeRawData(this->mimeType.data(), this->mimeLength);
  stream << this->payloadLength;
  stream.writeRawData(this->payload.data(), this->payloadLength);
}

/**
 * @brief to Bytes
 */
QByteArray SyncingItem::toBytes() const {
  // create the stream
  auto byteArr = QByteArray();
  auto stream  = QDataStream(&byteArr, QIODevice::WriteOnly);

  // set the byte order
  stream.setByteOrder(QDataStream::BigEndian);

  // serialize the packet
  this->toStream(stream);

  // Return the QByteArray
  return byteArr;
}

/**
 * @brief From Stream
 */
SyncingItem SyncingItem::fromStream(QDataStream& stream) {
  // using the utility functions
  using types::except::MalformedPacket;
  using types::enums::ErrorCode;

  // Create the SyncingItem
  SyncingItem pack;

  // Read the Packet Fields
  stream >> pack.mimeLength; pack.mimeType.resize(pack.mimeLength);
  stream.readRawData(pack.mimeType.data(), pack.mimeLength);
  stream >> pack.payloadLength;pack.payload.resize(pack.payloadLength);
  stream.readRawData(pack.payload.data(), pack.payloadLength);

  // if the stream is not good
  if (stream.status() != QDataStream::Ok) {
    throw MalformedPacket(ErrorCode::CodingError, "SyncingItem");
  }

  // return the payload
  return pack;
}

/**
 * @brief From Bytes
 */
SyncingItem SyncingItem::fromBytes(const QByteArray &array) {
  // create the stream
  auto stream = QDataStream(array);

  // set the byte order
  stream.setByteOrder(QDataStream::BigEndian);

  // return the payload
  return SyncingItem::fromStream(stream);
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
  size_t size = (sizeof(this->packetType) + sizeof(this->packetLength) + sizeof(this->itemCount));

  for (const auto& payload : this->items) {
    size += payload.size();
  }

  return qint32(size);
}

/**
 * @brief to Bytes
 */
QByteArray SyncingPacket::toBytes() const {
  // create the stream
  auto byteArr = QByteArray();
  auto stream  = QDataStream(&byteArr, QIODevice::WriteOnly);

  // set the byte order
  stream.setByteOrder(QDataStream::BigEndian);

  // Write the fields
  stream << this->packetLength;
  stream << this->packetType;
  stream << this->itemCount;

  // Write the Payloads
  for (const auto& payload : this->items) {
    payload.toStream(stream);
  }

  // Return the QByteArray
  return byteArr;
}

/**
 * @brief From Bytes
 */
SyncingPacket SyncingPacket::fromBytes(const QByteArray &array) {
  // create the stream
  auto stream = QDataStream(array);

  // using the utility functions
  using types::except::MalformedPacket;
  using types::enums::ErrorCode;

  // set the byte order
  stream.setByteOrder(QDataStream::BigEndian);

  // Create the SyncingPacket
  SyncingPacket packet;

  // Read the Packet Fields
  stream >> packet.packetLength;
  stream >> packet.packetType;
  stream >> packet.itemCount;

  // if the stream is not good
  if (stream.status() != QDataStream::Ok) {
    throw MalformedPacket(ErrorCode::CodingError, "SyncingPacket");
  }

  // check the packet type
  if (packet.packetType != PacketType::SyncPacket) {
    throw types::except::NotThisPacket("Not SyncingPacket");
  }

  // Read the Payloads
  for (quint32 i = 0; i < packet.itemCount; i++) {
    packet.items.push_back(SyncingItem::fromStream(stream));
  }

  // if the stream is not good
  if (stream.status() != QDataStream::Ok) {
    throw MalformedPacket(ErrorCode::CodingError, "SyncingPacket");
  }

  // return the packet
  return packet;
}
}  // namespace srilakshmikanthanp::clipbirdesk::network::packets
