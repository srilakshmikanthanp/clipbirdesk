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
 * @brief Create Authentication Packet from Bytes
 */
Authentication Authentication::fromBytes(const QByteArray &array) {
  // using Authentication packet from protobuf
  using ProtoPacket = srilakshmikanthanp::clipbirdesk::proto::authentication::Authentication;
  using ProtoStatus = srilakshmikanthanp::clipbirdesk::proto::authentication::AuthStatus;

  // create the Authentication Packet
  auto packet = ProtoPacket();

  // parse the packet
  if(!packet.ParseFromString(array.toStdString())) {
    throw types::except::MalformedPacket(types::enums::ErrorCode::CodingError, "Invalid Packet");
  }

  // check the packet type
  if (packet.packet_type() != PacketType::AuthStatus) {
    throw types::except::MalformedPacket(types::enums::ErrorCode::CodingError, "Invalid Packet");
  }

  // create the Authentication Packet
  Authentication authPacket;

  // set the packet length
  authPacket.setPacketLength(packet.packet_length());

  // set the packet type
  authPacket.setPacketType(packet.packet_type());

  // set the auth status
  if (packet.status() == ProtoStatus::AUTH_FAIL) {
    authPacket.setAuthStatus(types::enums::AuthStatus::AuthFail);
  } else {
    authPacket.setAuthStatus(types::enums::AuthStatus::AuthOkay);
  }

  // return the packet
  return authPacket;
}

/**
 * @brief Convert Authentication Packet to Bytes
 */
QByteArray Authentication::toBytes(const Authentication& packet) {
  // using Authentication packet from protobuf
  using ProtoPacket = srilakshmikanthanp::clipbirdesk::proto::authentication::Authentication;
  using ProtoStatus = srilakshmikanthanp::clipbirdesk::proto::authentication::AuthStatus;

  // create the Authentication Packet
  auto authPacket = ProtoPacket();

  // set the packet length
  authPacket.set_packet_length(packet.getPacketLength());

  // set the packet type
  authPacket.set_packet_type(packet.getPacketType());

  // set the auth status
  if (packet.getAuthStatus() == types::enums::AuthStatus::AuthFail) {
    authPacket.set_status(ProtoStatus::AUTH_FAIL);
  } else {
    authPacket.set_status(ProtoStatus::AUTH_OKAY);
  }

  // create the byte array
  std::string array;

  // serialize the packet
  if (!authPacket.SerializeToString(&array)) {
    throw std::runtime_error("Failed to serialize the packet");
  }

  // return the byte array
  return QByteArray::fromStdString(array);
}
}  // namespace srilakshmikanthanp::clipbirdesk::network::packets
