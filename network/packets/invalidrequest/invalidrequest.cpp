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
 * @brief Convert the QByteArray to InvalidRequest
 */
InvalidRequest InvalidRequest::fromBytes(const QByteArray &array) {
  // using the utility functions
  using utility::functions::fromQByteArray;
  using types::except::MalformedPacket;
  using types::enums::ErrorCode;

  // Create the InvalidRequest
  InvalidRequest packet;

  // check length
  if (array.size() != 12) {
    throw types::except::NotThisPacket("Not InvalidRequest Packet");
  }

  // Set the Packet Length
  packet.packetLength = fromQByteArray<quint32>(array.mid(0, 4));

  // Set the Packet Type
  packet.packetType = fromQByteArray<quint32>(array.mid(4, 4));

  // Set the Error Code
  packet.errorCode = fromQByteArray<quint32>(array.mid(8, 4));

  // Msg Len
  auto msgLen = packet.packetLength - (
    sizeof(packet.packetType) +
    sizeof(packet.packetLength) +
    sizeof(packet.errorCode)
  );

  // check the error message size
  if (array.mid(12).size() != msgLen) {
    throw MalformedPacket(ErrorCode::CodingError, "Invalid Error Message Size");
  }

  // Set the Error Message
  packet.errorMessage = array.mid(12);

  // check packet type
  if (packet.packetType != PacketType::RequestFailed) {
    throw types::except::NotThisPacket("Not InvalidRequest Packet");
  }

  // Return the InvalidRequest
  return packet;
}

/**
 * @brief Convert the InvalidRequest to QByteArray
 */
QByteArray InvalidRequest::toBytes(InvalidRequest packet) {
  // using the utility functions
  using utility::functions::toQByteArray;

  // Create the QByteArray
  QByteArray array;

  // Append the Packet Type
  array.append(toQByteArray(packet.packetLength));
  array.append(toQByteArray(packet.packetType));
  array.append(toQByteArray(packet.errorCode));
  array.append(packet.errorMessage);

  // Return the QByteArray
  return array;
}
}  // namespace srilakshmikanthanp::clipbirdesk::network::packets
