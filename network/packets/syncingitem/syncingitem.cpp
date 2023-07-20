// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "syncingitem.hpp"

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
    sizeof(this->mimeLength) +
    this->mimeType.size() +
    sizeof(this->payloadLength) +
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
}  // namespace srilakshmikanthanp::clipbirdesk::network::packets
