// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "syncingpacket.hpp"

namespace srilakshmikanthanp::clipbirdesk::network::packets {

/**
 * @brief Set the Mime Length object
 */
void SyncingItem::setMimeLength(qint32 length) {
  this->mimeLength = length;
}

/**
 * @brief Get the Mime Length object
 *
 * @return qint32
 */
qint32 SyncingItem::getMimeLength() const noexcept {
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
void SyncingItem::setPayloadLength(qint32 length) {
  this->payloadLength = length;
}

/**
 * @brief Get the Payload Length object
 *
 * @return qint32
 */
qint32 SyncingItem::getPayloadLength() const noexcept {
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
size_t SyncingItem::size() const noexcept {
  return (
      sizeof(this->mimeLength) + this->mimeType.size() + sizeof(this->payloadLength) +
      this->payload.size()
  );
}

/**
 * @brief Overloaded operator<< for QDataStream
 *
 * @param out
 * @param payload
 */
QDataStream& operator<<(QDataStream& out, const SyncingItem& payload) {
  // write the mime length
  out << payload.mimeLength;

  // check the mime length
  if (payload.mimeLength != payload.mimeType.size()) {
    throw std::invalid_argument("Invalid Mime Length");
  }

  // write the mime type
  out.writeRawData(payload.mimeType.data(), payload.mimeLength);

  // write the payload length
  out << payload.payloadLength;

  // check the payload length
  if (payload.payloadLength != payload.payload.size()) {
    throw std::invalid_argument("Invalid Payload Length");
  }

  // write the payload
  out.writeRawData(payload.payload.data(), payload.payloadLength);

  // return the stream
  return out;
}

/**
 * @brief Overloaded operator>> for QDataStream
 *
 * @param in
 * @param payload
 */
QDataStream& operator>>(QDataStream& in, SyncingItem& payload) {
  // read the mime length
  in >> payload.mimeLength;

  // check if stream is valid
  if (in.status() != QDataStream::Ok) {
    throw types::except::MalformedPacket(
        types::enums::ErrorCode::CodingError, "Invalid Mime Length"
    );
  }

  // resize the mime type
  payload.mimeType.resize(payload.mimeLength);

  // read the mime type
  in.readRawData(payload.mimeType.data(), payload.mimeLength);

  // check if stream is valid
  if (in.status() != QDataStream::Ok) {
    throw types::except::MalformedPacket(types::enums::ErrorCode::CodingError, "Invalid Mime Type");
  }

  // read the payload length
  in >> payload.payloadLength;

  // check if stream is valid
  if (in.status() != QDataStream::Ok) {
    throw types::except::MalformedPacket(
        types::enums::ErrorCode::CodingError, "Invalid Payload Length"
    );
  }

  // resize the payload
  payload.payload.resize(payload.payloadLength);

  // read the payload
  in.readRawData(payload.payload.data(), payload.payloadLength);

  // check if stream is valid
  if (in.status() != QDataStream::Ok) {
    throw types::except::MalformedPacket(
        types::enums::ErrorCode::CodingError, "Invalid Payload Attempt"
    );
  }

  // return the stream
  return in;
}

//------------------------- SyncingPacket -------------------------//

/**
 * @brief Set the Packet Type object
 *
 * @param type
 */
void SyncingPacket::setPacketType(quint8 type) {
  if (type != PacketType::SyncPacket) {
    throw std::invalid_argument("Invalid Packet Type");
  }
}

/**
 * @brief Get the Packet Type object
 *
 * @return quint8
 */
quint8 SyncingPacket::getPacketType() const noexcept {
  return this->packetType;
}

/**
 * @brief Set the Packet Length object
 *
 * @param length
 */
void SyncingPacket::setPacketLength(qint32 length) {
  this->packetLength = length;
}

/**
 * @brief Get the Packet Length object
 *
 * @return qint32
 */
qint32 SyncingPacket::getPacketLength() const noexcept {
  return this->packetLength;
}

/**
 * @brief Set the Item Count object
 *
 * @param count
 */
void SyncingPacket::setItemCount(qint32 count) {
  this->itemCount = count;
}

/**
 * @brief Get the Item Count object
 *
 * @return qint32
 */
qint32 SyncingPacket::getItemCount() const noexcept {
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
size_t SyncingPacket::size() const noexcept {
  size_t size = sizeof(this->packetType) + sizeof(this->packetLength) + sizeof(this->itemCount);

  for (const auto& payload : this->items) {
    size += payload.size();
  }

  return size;
}

/**
 * @brief Overloaded operator<< for QDataStream
 *
 * @param out
 * @param packet
 */
QDataStream& operator<<(QDataStream& out, const SyncingPacket& packet) {
  // write the packet type
  out << packet.packetType;

  // write the packet length
  out << packet.packetLength;

  // write the item count
  out << packet.itemCount;

  // check enough payloads
  if (packet.itemCount != packet.items.size()) {
    throw std::invalid_argument("Invalid Payloads");
  }

  // write the payloads
  for (const auto& payload : packet.items) {
    out << payload;
  }

  // return the stream
  return out;
}

/**
 * @brief Overloaded operator>> for QDataStream
 *
 * @param in
 * @param packet
 */
QDataStream& operator>>(QDataStream& in, SyncingPacket& packet) {
  // read the packet type
  in >> packet.packetType;

  // check if stream is valid
  if (in.status() != QDataStream::Ok) {
    throw types::except::MalformedPacket(
        types::enums::ErrorCode::CodingError, "Invalid Packet Type"
    );
  }

  // check the packet type
  if (packet.packetType != SyncingPacket::PacketType::SyncPacket) {
    throw types::except::MalformedPacket(
        types::enums::ErrorCode::CodingError, "Invalid Packet Type"
    );
  }

  // read the packet length
  in >> packet.packetLength;

  // check if stream is valid
  if (in.status() != QDataStream::Ok) {
    throw types::except::MalformedPacket(
        types::enums::ErrorCode::CodingError, "Invalid Packet Length"
    );
  }

  // read the item count
  in >> packet.itemCount;

  // check if stream is valid
  if (in.status() != QDataStream::Ok) {
    throw types::except::MalformedPacket(
        types::enums::ErrorCode::CodingError, "Invalid Item Count"
    );
  }

  // read the payloads
  for (int i = 0; i < packet.itemCount; i++) {
    SyncingItem payload;

    in >> payload;

    packet.items.push_back(payload);
  }

  // check if stream is valid
  if (in.status() != QDataStream::Ok) {
    throw types::except::MalformedPacket(types::enums::ErrorCode::CodingError, "Invalid Payloads");
  }

  // return the stream
  return in;
}
}  // namespace srilakshmikanthanp::clipbirdesk::network::packets
