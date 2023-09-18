// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "syncingpacket.hpp"

namespace srilakshmikanthanp::clipbirdesk::network::packets {
/**
 * @brief From Proto Packet
 */
SyncingItem SyncingItem::fromProtoPacket(proto::syncingpacket::SyncingItem item) {
  // create the payload
  SyncingItem payload;

  // set the mime length
  payload.setMimeLength(item.mime_length());

  // set the mime type
  payload.setMimeType(QByteArray::fromStdString(item.mime_type()));

  // set the payload length
  payload.setPayloadLength(item.payload_length());

  // set the payload
  payload.setPayload(QByteArray::fromStdString(item.payload()));

  // return the payload
  return payload;
}

/**
 * @brief to Proto Packet
 */
proto::syncingpacket::SyncingItem SyncingItem::toProtoPacket(const SyncingItem& item) {
  // create proto packet
  proto::syncingpacket::SyncingItem packet;

  // set the mime length
  packet.set_mime_length(item.getMimeLength());

  // set the mime type
  packet.set_mime_type(item.getMimeType().toStdString());

  // set the payload length
  packet.set_payload_length(item.getPayloadLength());

  // set the payload
  packet.set_payload(item.getPayload().toStdString());

  // return the packet
  return packet;
}

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
  // using syncing item from protobuf
  using ProtoPacket = srilakshmikanthanp::clipbirdesk::proto::syncingpacket::SyncingItem;

  // create proto packet
  ProtoPacket packet;

  // parse the packet
  if (!packet.ParseFromArray(array.data(), array.size())) {
    throw std::invalid_argument("Invalid Packet");
  }

  // return the payload
  return fromProtoPacket(packet);
}

/**
 * @brief to Bytes
 */
QByteArray SyncingItem::toBytes(const SyncingItem& payload) {
  // using syncing item from protobuf
  using ProtoPacket = srilakshmikanthanp::clipbirdesk::proto::syncingpacket::SyncingItem;

  // create proto packet
  ProtoPacket packet = toProtoPacket(payload);

  // create the array
  QByteArray array(packet.ByteSizeLong(), 0);

  // serialize the packet
  if (!packet.SerializeToArray(array.data(), array.size())) {
    throw std::invalid_argument("Invalid Packet");
  }

  // return the array
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
  // using syncing packet from protobuf
  using ProtoPacket = srilakshmikanthanp::clipbirdesk::proto::syncingpacket::SyncingPacket;

  // create proto packet
  ProtoPacket packet;

  // parse the packet
  if (!packet.ParseFromArray(array.data(), array.size())) {
    throw std::invalid_argument("Invalid Packet");
  }

  // create the packet
  SyncingPacket payload;

  // set the packet length
  payload.setPacketLength(packet.packet_length());

  // set the packet type
  payload.setPacketType(packet.packet_type());

  // set the item count
  payload.setItemCount(packet.item_count());

  // set the payloads
  QVector<SyncingItem> payloads;

  // set the payloads
  for (const auto& item : packet.items()) {
    payloads.push_back(SyncingItem::fromProtoPacket(item));
  }

  // set the payloads
  payload.setItems(payloads);

  // return the payload
  return payload;
}

/**
 * @brief to Bytes
 */
QByteArray SyncingPacket::toBytes(const SyncingPacket& packet) {
  // using syncing packet from protobuf
  using ProtoPacket = srilakshmikanthanp::clipbirdesk::proto::syncingpacket::SyncingPacket;

  // create proto packet
  ProtoPacket protoPacket;

  // set the packet length
  protoPacket.set_packet_length(packet.getPacketLength());

  // set the packet type
  protoPacket.set_packet_type(packet.getPacketType());

  // set the item count
  protoPacket.set_item_count(packet.getItemCount());

  // set the payloads
  for (const auto& item : packet.getItems()) {
    protoPacket.add_items()->CopyFrom(SyncingItem::toProtoPacket(item));
  }

  // create the array
  QByteArray array(protoPacket.ByteSizeLong(), 0);

  // serialize the packet
  if (!protoPacket.SerializeToArray(array.data(), array.size())) {
    throw std::invalid_argument("Invalid Packet");
  }

  // return the array
  return array;
}
}  // namespace srilakshmikanthanp::clipbirdesk::network::packets
