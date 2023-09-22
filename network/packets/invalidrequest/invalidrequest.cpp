// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "invalidrequest.hpp"

namespace srilakshmikanthanp::clipbirdesk::network::packets {
/**
 * @brief Set the Packet Length object
 *
 * @param length
 */
void InvalidRequest::setPacketLength(quint32 length) {
  this->packetLength = length;
}

/**
 * @brief Get the Packet Length object
 *
 * @return qint32
 */
quint32 InvalidRequest::getPacketLength() const noexcept {
  return this->packetLength;
}

/**
 * @brief Set the Packet Type object
 *
 * @param type
 */
void InvalidRequest::setPacketType(quint32 type) {
  if (type != PacketType::RequestFailed) {
    throw std::invalid_argument("Invalid Packet Type");
  }
}

/**
 * @brief Get the Packet Type object
 *
 * @return quint32
 */
quint32 InvalidRequest::getPacketType() const noexcept {
  return this->packetType;
}

/**
 * @brief Set the Error Code object
 *
 * @param code
 */
void InvalidRequest::setErrorCode(quint32 code) {
  if (code == types::enums::ErrorCode::InvalidPacket) {
    this->errorCode = code;
    return;
  }

  if (code == types::enums::ErrorCode::CodingError) {
    this->errorCode = code;
    return;
  }

  throw std::invalid_argument("Invalid Error Code");
}

/**
 * @brief Get the Error Code object
 *
 * @return quint32
 */
quint32 InvalidRequest::getErrorCode() const noexcept {
  return this->errorCode;
}

/**
 * @brief Set the Error Message object
 *
 * @param message
 */
void InvalidRequest::setErrorMessage(QByteArray message) {
  this->errorMessage = message;
}

/**
 * @brief Get the Error Message object
 *
 * @return QByteArray
 */
QByteArray InvalidRequest::getErrorMessage() const noexcept {
  return this->errorMessage;
}

/**
 * @brief Get the Size of the Packet
 *
 * @return std::size_t
 */
quint32 InvalidRequest::size() const noexcept {
  return quint32(sizeof(packetType) + sizeof(packetLength) + sizeof(errorCode) + errorMessage.size());
}

/**
 * @brief Convert the InvalidRequest to QByteArray
 */
QByteArray InvalidRequest::toBytes() const {
  // create the stream
  auto byteArr = QByteArray();
  auto stream  = QDataStream(&byteArr, QIODevice::WriteOnly);

  // set the byte order
  stream.setByteOrder(QDataStream::BigEndian);

  // Write the fields
  stream << this->packetLength;
  stream << this->packetType;
  stream << this->errorCode;
  stream.writeRawData(this->errorMessage.data(), this->errorMessage.size());

  // Return the QByteArray
  return byteArr;
}

/**
 * @brief Convert the QByteArray to InvalidRequest
 */
InvalidRequest InvalidRequest::fromBytes(const QByteArray &array) {
  // create the stream
  auto stream = QDataStream(array);

  // using the utility functions
  using types::except::MalformedPacket;
  using types::enums::ErrorCode;

  // Create the InvalidRequest
  InvalidRequest packet;

  // set the byte order
  stream.setByteOrder(QDataStream::BigEndian);

  // Read the fields
  stream >> packet.packetLength;
  stream >> packet.packetType;
  stream >> packet.errorCode;

  // Read the message
  packet.errorMessage.resize(packet.packetLength - (
    sizeof(packet.packetType) +
    sizeof(packet.packetLength) +
    sizeof(packet.errorCode)
  ));

  // Read the message
  stream.readRawData(packet.errorMessage.data(), packet.errorMessage.size());

  // if stream is not good
  if (stream.status() != QDataStream::Ok) {
    throw MalformedPacket(ErrorCode::CodingError, "InvalidRequest");
  }

  // check packet type
  if (packet.packetType != PacketType::RequestFailed) {
    throw types::except::NotThisPacket("Not InvalidRequest Packet");
  }

  // check error code
  auto allowed = QVector<quint32>{
    types::enums::ErrorCode::InvalidPacket,
    types::enums::ErrorCode::CodingError
  };

  if (!allowed.contains(packet.errorCode)) {
    throw std::invalid_argument("Invalid Error Code");
  }

  // Return the InvalidRequest
  return packet;
}
}  // namespace srilakshmikanthanp::clipbirdesk::network::packets
