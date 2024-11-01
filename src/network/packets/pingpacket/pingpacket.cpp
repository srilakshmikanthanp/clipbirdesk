#include "pingpacket.hpp"

namespace srilakshmikanthanp::clipbirdesk::network::packets {
/**
 * @brief Set the Packet Length object
 *
 * @param length
 */
void PingPacket::setPacketLength(quint32 length) {
  packetLength = length;
}

/**
 * @brief Get the Packet Length object
 *
 * @return qint32
 */
quint32 PingPacket::getPacketLength() const noexcept {
  return packetLength;
}

/**
 * @brief Set the Packet Type object
 *
 * @param type
 */
void PingPacket::setPacketType(quint32 type) {
  packetType = type;
}

/**
 * @brief Get the Packet Type object
 *
 * @return quint32
 */
quint32 PingPacket::getPacketType() const noexcept {
  return packetType;
}

/**
 * @brief Set the Ping Type object
 *
 * @param type
 */
void PingPacket::setPingType(quint32 type) {
  if (type != types::enums::Ping && type != types::enums::Pong) {
    throw std::invalid_argument("Invalid Type");
  }

  pingType = type;
}

/**
 * @brief Get the Ping Type object
 *
 * @return quint32
 */
quint32 PingPacket::getPingType() const noexcept {
  return pingType;
}

/**
 * @brief Get the size of the packet
 *
 * @return size_t
 */
quint32 PingPacket::size() const noexcept {
  return sizeof(packetLength) + sizeof(packetType) + sizeof(pingType);
}

/**
 * @brief to Bytes
 */
QByteArray PingPacket::toBytes() const {
  auto byteArr = QByteArray();
  auto stream  = QDataStream(&byteArr, QIODevice::WriteOnly);

  stream.setByteOrder(QDataStream::BigEndian);

  stream << this->packetLength;
  stream << this->packetType;
  stream << this->pingType;

  return byteArr;
}

/**
 * @brief From Bytes
 */
PingPacket PingPacket::fromBytes(const QByteArray &array) {
  // allowed ping type
  auto allowedPingType = QList<quint32>{ types::enums::Ping, types::enums::Pong };
  auto stream = QDataStream(array);

  using types::except::MalformedPacket;
  using types::enums::ErrorCode;

  PingPacket packet;

  stream.setByteOrder(QDataStream::BigEndian);

  stream >> packet.packetLength;
  stream >> packet.packetType;
  stream >> packet.pingType;

  if (stream.status() != QDataStream::Ok) {
    throw MalformedPacket(ErrorCode::CodingError, "PingPacket");
  }

  if (packet.packetType != PacketType::PingPong) {
    throw types::except::NotThisPacket("Not PingPacket");
  }

  // check for valid ping type
  if (!allowedPingType.contains(packet.pingType)) {
    throw std::invalid_argument("Invalid Ping Type");
  }

  return packet;
}
}  // namespace srilakshmikanthanp::clipbirdesk::network::packets
