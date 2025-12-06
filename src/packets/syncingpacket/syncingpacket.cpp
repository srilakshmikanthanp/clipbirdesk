#include "syncingpacket.hpp"

namespace srilakshmikanthanp::clipbirdesk::packets {

/**
 * @brief Get the Mime Length object
 *
 * @return qint32
 */
quint32 SyncingItem::getMimeLength() const noexcept {
  return this->mimeType.size();
}

/**
 * @brief Set the Mime Type object
 *
 * @param type
 */
void SyncingItem::setMimeType(const QByteArray& type) {
  this->mimeType = type;
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
 * @brief Get the Payload Length object
 *
 * @return qint32
 */
quint32 SyncingItem::getPayloadLength() const noexcept {
  return this->payload.size();
}

/**
 * @brief Set the Payload object
 *
 * @param payload
 */
void SyncingItem::setPayload(const QByteArray& payload) {
  this->payload = payload;
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
    sizeof(decltype(std::declval<SyncingItem>().getMimeLength())) +
    this->mimeType.size() +
    sizeof(decltype(std::declval<SyncingItem>().getPayloadLength())) +
    this->payload.size()
  );
}

/**
 * @brief To Stream
 */
void SyncingItem::toStream(QDataStream& stream) const {
  stream << this->getMimeLength();
  stream.writeRawData(this->mimeType.data(), this->getMimeLength());
  stream << this->getPayloadLength();
  stream.writeRawData(this->payload.data(), this->getPayloadLength());
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
  using common::types::exceptions::MalformedPacket;
  using common::types::enums::ErrorCode;

  // Create the SyncingItem
  SyncingItem pack;

  quint32 mimeLength;
  QByteArray mimeType;
  quint32 payloadLength;
  QByteArray payload;

  // Read the Packet Fields
  stream >> mimeLength;
  mimeType.resize(mimeLength);
  stream.readRawData(mimeType.data(), mimeLength);
  stream >> payloadLength;
  payload.resize(payloadLength);
  stream.readRawData(payload.data(), payloadLength);

  // if the stream is not good
  if (stream.status() != QDataStream::Ok) {
    throw MalformedPacket(ErrorCode::CodingError, "SyncingItem");
  }

  pack.setMimeType(mimeType);
  pack.setPayload(payload);

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
 * @brief Get the Packet Length object
 *
 * @return qint32
 */
quint32 SyncingPacket::getPacketLength() const noexcept {
  size_t size = (sizeof(decltype(std::declval<SyncingPacket>().getPacketLength())) + sizeof(this->packetType) + sizeof(decltype(std::declval<SyncingPacket>().getItemCount())));

  for (const auto& payload : this->items) {
    size += payload.size();
  }

  return qint32(size);
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
 * @brief Get the Item Count object
 *
 * @return qint32
 */
quint32 SyncingPacket::getItemCount() const noexcept {
  return this->items.size();
}

/**
 * @brief Set the Payloads object
 *
 * @param payloads
 */
void SyncingPacket::setItems(const QVector<SyncingItem>& payloads) {
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
 * @brief to Bytes
 */
QByteArray SyncingPacket::toBytes() const {
  // create the stream
  auto byteArr = QByteArray();
  auto stream  = QDataStream(&byteArr, QIODevice::WriteOnly);

  // set the byte order
  stream.setByteOrder(QDataStream::BigEndian);

  // Write the fields
  stream << this->getPacketLength();
  stream << this->packetType;
  stream << this->getItemCount();

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
  using common::types::exceptions::MalformedPacket;
  using common::types::enums::ErrorCode;

  // set the byte order
  stream.setByteOrder(QDataStream::BigEndian);

  // Create the SyncingPacket
  SyncingPacket packet;

  quint32 packetLength;
  quint32 packetType;
  quint32 itemCount;

  // Read the Packet Fields
  stream >> packetLength;
  stream >> packetType;
  stream >> itemCount;

  // check the packet type
  if (packetType != PacketType::SYNCING_PACKET) {
    throw common::types::exceptions::NotThisPacket("Not SyncingPacket");
  }

  // if the stream is not good
  if (stream.status() != QDataStream::Ok) {
    throw MalformedPacket(ErrorCode::CodingError, "SyncingPacket");
  }

  auto items = QVector<SyncingItem>();

  // Read the Payloads
  for (quint32 i = 0; i < itemCount; i++) {
    items.push_back(SyncingItem::fromStream(stream));
  }

  // if the stream is not good
  if (stream.status() != QDataStream::Ok) {
    throw MalformedPacket(ErrorCode::CodingError, "SyncingPacket");
  }

  packet.setItems(items);

  // return the packet
  return packet;
}
}  // namespace srilakshmikanthanp::clipbirdesk::packets
