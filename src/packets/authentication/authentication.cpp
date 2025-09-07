#include "authentication.hpp"

namespace srilakshmikanthanp::clipbirdesk::packets {
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
 * @brief Convert Authentication Packet to Bytes BigEndian
 */
QByteArray Authentication::toBytes() const {
  // create a ostream
  auto byteArr = QByteArray();
  auto stream  = QDataStream(&byteArr, QIODevice::WriteOnly);

  // set the byte order
  stream.setByteOrder(QDataStream::BigEndian);

  // write the fields
  stream << this->packetLength;
  stream << this->packetType;
  stream << this->authStatus;

  // return QByteArray
  return byteArr;
}

/**
 * @brief Create Authentication Packet from Bytes BigEndian
 */
Authentication Authentication::fromBytes(const QByteArray &array) {
  // create binary iostream with byte array
  auto stream = QDataStream(array);

  // set the byte order
  stream.setByteOrder(QDataStream::BigEndian);

  // Using Utility Functions
  using types::except::MalformedPacket;
  using types::enums::ErrorCode;

  // Create Packet
  Authentication packet;

  // read the Packet
  stream >> packet.packetLength;
  stream >> packet.packetType;
  stream >> packet.authStatus;

  // if stream is not in good state
  if (stream.status() != QDataStream::Ok) {
    throw MalformedPacket(ErrorCode::CodingError, "Invalid Packet");
  }

  // check packet type
  if (packet.packetType != PacketType::AuthStatus) {
    throw types::except::NotThisPacket("Not Authentication Packet");
  }

  // allowed status
  const QList<int> allowedStatus = {
    types::enums::AuthStatus::AuthOkay,
    types::enums::AuthStatus::AuthFail,
  };

  // Check the auth status
  if (!allowedStatus.contains(packet.authStatus)) {
    throw MalformedPacket(ErrorCode::CodingError, "Invalid Auth Status");
  }

  // return packet
  return packet;
}
}  // namespace srilakshmikanthanp::clipbirdesk::packets
