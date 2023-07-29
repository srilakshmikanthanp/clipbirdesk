// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "syncingpacket.hpp"

namespace srilakshmikanthanp::clipbirdesk::network::packets {
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
qint32 SyncingPacket::size() const noexcept {
  size_t size = (
    sizeof(this->packetType) + sizeof(this->packetLength) + sizeof(this->itemCount)
  );

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
  // write the packet length
  out << packet.packetLength;

  // write the packet type
  out << packet.packetType;

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
  // read the packet length
  in >> packet.packetLength;

  // check if stream is valid
  if (in.status() != QDataStream::Ok) {
    throw types::except::MalformedPacket(
        types::enums::ErrorCode::CodingError, "Invalid Packet Length"
    );
  }

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
    throw types::except::NotThisPacket("Not Authentication Packet");
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
    throw types::except::MalformedPacket(
      types::enums::ErrorCode::CodingError, "Invalid Payloads"
    );
  }

  // return the stream
  return in;
}
}  // namespace srilakshmikanthanp::clipbirdesk::network::packets
