#include "invalidrequest.hpp"

namespace srilakshmikanthanp::clipbirdesk::packets {
/**
 * @brief Get the Packet Length object
 *
 * @return qint32
 */
quint32 InvalidRequest::getPacketLength() const noexcept {
  return quint32(
    sizeof(decltype(std::declval<InvalidRequest>().getPacketType())) +
    sizeof(packetType) +
    sizeof(errorCode) +
    errorMessage.size()
  );
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
  if (code == common::types::enums::ErrorCode::InvalidPacket) {
    this->errorCode = code;
    return;
  }

  if (code == common::types::enums::ErrorCode::CodingError) {
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
 * @brief Convert the InvalidRequest to QByteArray
 */
QByteArray InvalidRequest::toBytes() const {
  // create the stream
  auto byteArr = QByteArray();
  auto stream  = QDataStream(&byteArr, QIODevice::WriteOnly);

  // set the byte order
  stream.setByteOrder(QDataStream::BigEndian);

  // Write the fields
  stream << this->getPacketLength();
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
  using common::types::exceptions::MalformedPacket;
  using common::types::enums::ErrorCode;

  // Create the InvalidRequest
  InvalidRequest packet;

  // set the byte order
  stream.setByteOrder(QDataStream::BigEndian);

  quint32 packetLength;
  quint32 packetType;
  quint32 errorCode;

  // Read the fields
  stream >> packetLength;
  stream >> packetType;
  stream >> errorCode;

  QByteArray errorMessage;

  // Read the message
  errorMessage.resize(packetLength - (
    sizeof(packetType) +
    sizeof(packetLength) +
    sizeof(errorCode)
  ));

  // Read the message
  stream.readRawData(packet.errorMessage.data(), packet.errorMessage.size());

  // check packet type
  if (packetType != PacketType::INVALID_REQUEST) {
    throw common::types::exceptions::NotThisPacket("Not InvalidRequest Packet");
  }

  // if stream is not good
  if (stream.status() != QDataStream::Ok) {
    throw MalformedPacket(ErrorCode::CodingError, "InvalidRequest");
  }

  // check error code
  auto allowed = QVector<quint32>{
    common::types::enums::ErrorCode::InvalidPacket,
    common::types::enums::ErrorCode::CodingError
  };

  if (!allowed.contains(errorCode)) {
    throw std::invalid_argument("Invalid Error Code");
  }

  packet.packetType    = packetType;
  packet.errorCode     = errorCode;
  packet.errorMessage  = errorMessage;

  // Return the InvalidRequest
  return packet;
}
}  // namespace srilakshmikanthanp::clipbirdesk::packets
