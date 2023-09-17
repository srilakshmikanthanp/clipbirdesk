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
 * @brief Convert the QByteArray to InvalidRequest
 */
InvalidRequest InvalidRequest::fromByteArray(QByteArray data) {
  // using InvalidRequest from Protobuf
  using ProtoPacket = srilakshmikanthanp::clipbirdesk::proto::invalidrequest::InvalidRequest;

  // create the protobuf packet
  ProtoPacket protoPacket;

  // parse the packet
  if(!protoPacket.ParseFromString(data.toStdString())) {
    throw types::except::MalformedPacket(types::enums::CodingError, "Invalid Packet");
  }

  // create the InvalidRequest packet
  InvalidRequest packet;

  // set the packet length
  packet.setPacketLength(protoPacket.packet_length());

  // set the packet type
  packet.setPacketType(protoPacket.packet_type());

  // set the error code
  packet.setErrorCode(protoPacket.error_code());

  // set the error message
  packet.setErrorMessage(protoPacket.error_message());

  // return the packet
  return packet;
}

/**
 * @brief Convert the InvalidRequest to QByteArray
 */
QByteArray InvalidRequest::toByteArray(InvalidRequest packet) {
  // using InvalidRequest from Protobuf
  using ProtoPacket = srilakshmikanthanp::clipbirdesk::proto::invalidrequest::InvalidRequest;

  // create the protobuf packet
  ProtoPacket protoPacket;

  // set the packet length
  protoPacket.set_packet_length(packet.getPacketLength());

  // set the packet type
  protoPacket.set_packet_type(packet.getPacketType());

  // set the error code
  protoPacket.set_error_code(packet.getErrorCode());

  // set the error message
  protoPacket.set_error_message(packet.getErrorMessage().toStdString());

  // create the string
  std::string buffer;

  // serialize the packet
  if (!protoPacket.SerializeToString(&buffer)) {
    throw throw std::runtime_error("Failed to serialize the packet");
  }

  // return the QByteArray
  return QByteArray::fromStdString(buffer);
}
}  // namespace srilakshmikanthanp::clipbirdesk::network::packets
