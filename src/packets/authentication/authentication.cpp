#include "authentication.hpp"

namespace srilakshmikanthanp::clipbirdesk::packets {
/**
 * @brief Get the Packet Length object
 *
 * @return qint32
 */
quint32 Authentication::getPacketLength() const noexcept {
  return qint32(
    (sizeof(decltype(std::declval<Authentication>().getPacketLength()))) +
    sizeof(this->packetType) +
    sizeof(this->authStatus)
  );
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
  if (status == common::types::enums::AuthStatus::AuthFail) {
    this->authStatus = status;
    return;
  }

  if (status == common::types::enums::AuthStatus::AuthOkay) {
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
 * @brief Convert Authentication Packet to Bytes BigEndian
 */
QByteArray Authentication::toBytes() const {
  // create a ostream
  auto byteArr = QByteArray();
  auto stream  = QDataStream(&byteArr, QIODevice::WriteOnly);

  // set the byte order
  stream.setByteOrder(QDataStream::BigEndian);

  // write the fields
  stream << this->getPacketLength();
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
  using common::types::exceptions::MalformedPacket;
  using common::types::enums::ErrorCode;

  // Create Packet
  Authentication packet;

  quint32 packetLength;
  quint32 packetType;
  quint32 authStatus;

  // read the Packet
  stream >> packetLength;
  stream >> packetType;
  stream >> authStatus;

  // if stream is not in good state
  if (stream.status() != QDataStream::Ok) {
    throw MalformedPacket(ErrorCode::CodingError, "Invalid Packet");
  }

  // check packet type
  if (packetType != PacketType::AUTHENTICATION_PACKET) {
    throw common::types::exceptions::NotThisPacket("Not Authentication Packet");
  }

  // allowed status
  const QList<int> allowedStatus = {
    common::types::enums::AuthStatus::AuthOkay,
    common::types::enums::AuthStatus::AuthFail,
  };

  // Check the auth status
  if (!allowedStatus.contains(authStatus)) {
    throw MalformedPacket(ErrorCode::CodingError, "Invalid Auth Status");
  }

  packet.packetType   = packetType;
  packet.authStatus   = authStatus;

  // return packet
  return packet;
}
}  // namespace srilakshmikanthanp::clipbirdesk::packets
