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
void InvalidRequest::setPacketType(quint8 type) {
  if (type != PacketType::RequestFailed) {
    throw std::invalid_argument("Invalid Packet Type");
  }
}

/**
 * @brief Get the Packet Type object
 *
 * @return quint8
 */
quint8 InvalidRequest::getPacketType() const noexcept {
  return this->packetType;
}

/**
 * @brief Set the Error Code object
 *
 * @param code
 */
void InvalidRequest::setErrorCode(quint8 code) {
  if (code == types::enums::ErrorCode::InvalidPacket) {
    this->errorCode = code;
    return;
  }

  if (code == types::enums::ErrorCode::SSLError) {
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
 * @return quint8
 */
quint8 InvalidRequest::getErrorCode() const noexcept {
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
 * @brief Input stream operator for QDataStream
 *
 * @param stream
 * @param packet
 * @return QDataStream&
 */
QDataStream& operator<<(QDataStream& stream, const InvalidRequest packet) {
  // write the packet length
  stream << packet.packetLength;

  // write the packet type
  stream << packet.packetType;

  // write the error code
  stream << packet.errorCode;

  // Msg Len
  auto msgLen = packet.packetLength - (
    sizeof(packet.packetType) +
    sizeof(packet.packetLength) +
    sizeof(packet.errorCode)
  );

  // check the error message size
  if (packet.errorMessage.size() != msgLen) {
    throw std::invalid_argument("Invalid Error Message");
  }

  // write the error message
  stream.writeRawData(packet.errorMessage.data(), packet.errorMessage.size());

  // return the stream
  return stream;
}

/**
 * @brief Output stream operator for QDataStream
 *
 * @param stream
 * @param packet
 * @return QDataStream&
 */
QDataStream& operator>>(QDataStream& stream, InvalidRequest& packet) {
  // using malformed packet exception
  using types::except::MalformedPacket;

  // read the packet length
  stream >> packet.packetLength;

  // is the stream status is bad
  if (stream.status() != QDataStream::Ok) {
    throw MalformedPacket(types::enums::CodingError, "Invalid Packet Length");
  }

  // read the packet type
  stream >> packet.packetType;

  // is the stream status is bad
  if (stream.status() != QDataStream::Ok) {
    throw MalformedPacket(types::enums::CodingError, "Invalid Packet Type");
  }

  // check the packet type
  if (packet.packetType != InvalidRequest::PacketType::RequestFailed) {
    throw types::except::NotThisPacket("Not Authentication Packet");
  }

  // read the error code
  stream >> packet.errorCode;

  // is the stream status is bad
  if (stream.status() != QDataStream::Ok) {
    throw MalformedPacket(types::enums::CodingError, "Invalid Error Code");
  }

  // using some types
  using types::enums::ErrorCode;

  // allowed
  const auto allowed = std::vector<int>{
    ErrorCode::InvalidPacket,
    ErrorCode::SSLError,
    ErrorCode::CodingError
  };

  // check the error code
  if (std::find(allowed.begin(), allowed.end(), packet.errorCode) == allowed.end()) {
    throw MalformedPacket(types::enums::CodingError, "Invalid Error Code");
  }

  // Extract the error message length
  auto msgLen = packet.packetLength - (
    sizeof(packet.packetType) +
    sizeof(packet.packetLength) +
    sizeof(packet.errorCode)
  );

  // resize the error message
  packet.errorMessage.resize(msgLen);

  // read the error message
  stream.readRawData(packet.errorMessage.data(), packet.errorMessage.size());

  // is the stream status is bad
  if (stream.status() != QDataStream::Ok) {
    throw MalformedPacket(types::enums::CodingError, "Invalid Error Message");
  }

  // return the stream
  return stream;
}
}  // namespace srilakshmikanthanp::clipbirdesk::network::packets
