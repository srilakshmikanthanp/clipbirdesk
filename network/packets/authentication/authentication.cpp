// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "authentication.hpp"

namespace srilakshmikanthanp::clipbirdesk::network::packets {
/**
 * @brief Set the Packet Length object
 *
 * @param length
 */
void Authentication::setPacketLength(quint32 length) {
  this->packetLength = length;
}

/**
 * @brief Get the Packet Length object
 *
 * @return qint32
 */
quint32 Authentication::getPacketLength() const noexcept {
  return this->packetLength;
}

/**
 * @brief Set the Packet Type object
 *
 * @param type
 */
void Authentication::setPacketType(quint8 type) {
  if (type != PacketType::AuthStatus) {
    throw std::invalid_argument("Invalid Packet Type");
  }

  this->packetType = type;
}

/**
 * @brief Get the Packet Type object
 *
 * @return quint8
 */
quint8 Authentication::getPacketType() const noexcept {
  return this->packetType;
}

/**
 * @brief Set the Auth Type object
 *
 * @param type
 */
void Authentication::setAuthType(quint8 type) {
  if (type == types::enums::AuthType::AuthReq) {
    this->authType = type;
    return;
  }

  if (type == types::enums::AuthType::AuthRes) {
    this->authType = type;
    return;
  }

  throw std::invalid_argument("Invalid Auth Type");
}

/**
 * @brief Get the Auth Type object
 *
 * @return quint8
 */
quint8 Authentication::getAuthType() const noexcept {
  return this->authType;
}

/**
 * @brief Set the Auth Status object
 *
 * @param status
 */
void Authentication::setAuthStatus(quint8 status) {
  if (status == types::enums::AuthStatus::AuthFailed) {
    this->authStatus = status;
    return;
  }

  if (status == types::enums::AuthStatus::AuthSuccess) {
    this->authStatus = status;
    return;
  }

  throw std::invalid_argument("Invalid Auth Status");
}

/**
 * @brief Get the Auth Status object
 *
 * @return quint8
 */
quint8 Authentication::getAuthStatus() const noexcept {
  return this->authStatus;
}

/**
 * @brief Get the Size of the Packet
 *
 * @return qint32
 */
quint32 Authentication::size() const noexcept {
  return qint32(
    sizeof(this->packetLength) +
    sizeof(this->packetType) +
    sizeof(this->authStatus)
  );
}

/**
 * @brief Input stream operator for QDataStream
 *
 * @param stream
 * @param packet
 * @return QDataStream&
 */
QDataStream& operator<<(QDataStream& stream, const Authentication packet) {
  stream << packet.packetLength << packet.packetType << packet.authStatus;
  return stream;
}

/**
 * @brief Output stream operator for QDataStream
 *
 * @param stream
 * @param packet
 * @return QDataStream&
 */
QDataStream& operator>>(QDataStream& stream, Authentication& packet) {
  // Read All the data from the stream
  stream >> packet.packetLength >> packet.packetType >> packet.authStatus;

  // using some types
  using types::enums::AuthStatus;
  using types::enums::ErrorCode;

  // Check the packet type
  if (packet.packetType != Authentication::PacketType::AuthStatus) {
    throw types::except::NotThisPacket("Not Authentication Packet");
  }

  // using malformed packet exception
  using types::except::MalformedPacket;

  // auth status
  const auto authStatus = packet.authStatus;

  // Check the auth status
  if (authStatus != AuthStatus::AuthFailed && authStatus != AuthStatus::AuthSuccess) {
    throw MalformedPacket(ErrorCode::CodingError, "Invalid Auth Status");
  }

  // return the stream
  return stream;
}
}  // namespace srilakshmikanthanp::clipbirdesk::network::packets
