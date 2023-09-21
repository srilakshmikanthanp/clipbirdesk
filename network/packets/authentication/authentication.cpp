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
void Authentication::setPacketType(quint32 type) {
  if (type != PacketType::AuthStatus) {
    throw std::invalid_argument("Invalid Packet Type");
  }

  this->packetType = type;
}

/**
 * @brief Get the Packet Type object
 *
 * @return quint32
 */
quint32 Authentication::getPacketType() const noexcept {
  return this->packetType;
}

/**
 * @brief Set the Auth Status object
 *
 * @param status
 */
void Authentication::setAuthStatus(quint32 status) {
  if (status == types::enums::AuthStatus::AuthFail) {
    this->authStatus = status;
    return;
  }

  if (status == types::enums::AuthStatus::AuthOkay) {
    this->authStatus = status;
    return;
  }

  throw std::invalid_argument("Invalid Auth Status");
}

/**
 * @brief Get the Auth Status object
 *
 * @return quint32
 */
quint32 Authentication::getAuthStatus() const noexcept {
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
 * @brief Create Authentication Packet from Bytes BigEndian
 */
Authentication Authentication::fromBytes(const QByteArray &array) {
  // Using Utility Functions
  using utility::functions::fromQByteArray;
  using types::except::MalformedPacket;
  using types::enums::ErrorCode;

  // check the array length
  if (array.size() != 12) {
    throw types::except::NotThisPacket("Not Authentication Packet");
  }

  // Create Packet
  Authentication packet;

  // init the packet
  packet.packetLength = fromQByteArray<quint32>(array.mid(0, 4));
  packet.packetType = fromQByteArray<quint32>(array.mid(4, 4));
  packet.authStatus = fromQByteArray<quint32>(array.mid(8, 4));

  // check packet type
  if (packet.packetType != PacketType::AuthStatus) {
    throw types::except::NotThisPacket("Not Authentication Packet");
  }

  // auth status
  const auto authStatus = packet.authStatus;

  // allowed status
  const QList<int> allowedStatus = {
    types::enums::AuthStatus::AuthOkay,
    types::enums::AuthStatus::AuthFail,
  };

  // Check the auth status
  if (!allowedStatus.contains(authStatus)) {
    throw MalformedPacket(ErrorCode::CodingError, "Invalid Auth Status");
  }

  // return packet
  return packet;
}

/**
 * @brief Convert Authentication Packet to Bytes BigEndian
 */
QByteArray Authentication::toBytes(const Authentication& packet) {
  // Using Utility Functions
  using utility::functions::toQByteArray;

  // Create Array
  QByteArray array;

  // append to array
  array.append(toQByteArray(packet.packetLength));
  array.append(toQByteArray(packet.packetType));
  array.append(toQByteArray(packet.authStatus));

  // return array
  return array;
}
}  // namespace srilakshmikanthanp::clipbirdesk::network::packets
