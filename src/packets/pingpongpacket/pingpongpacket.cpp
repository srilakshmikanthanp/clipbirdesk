#include "pingpongpacket.hpp"

namespace srilakshmikanthanp::clipbirdesk::packets {
/**
 * @brief Get the Packet Length object
 *
 * @return qint32
 */
quint32 PingPongPacket::getPacketLength() const noexcept {
  return sizeof(decltype(std::declval<PingPongPacket>().getPacketType())) + sizeof(packetType) + sizeof(pingType);
}

/**
 * @brief Get the Packet Type object
 *
 * @return quint32
 */
quint32 PingPongPacket::getPacketType() const noexcept {
  return packetType;
}

/**
 * @brief Set the Ping Type object
 *
 * @param type
 */
void PingPongPacket::setPingType(quint32 type) {
  if (type != common::types::enums::Ping && type != common::types::enums::Pong) {
    throw std::invalid_argument("Invalid Type");
  }

  pingType = type;
}

/**
 * @brief Get the Ping Type object
 *
 * @return quint32
 */
quint32 PingPongPacket::getPingType() const noexcept {
  return pingType;
}

/**
 * @brief to Bytes
 */
QByteArray PingPongPacket::toBytes() const {
  auto byteArr = QByteArray();
  auto stream  = QDataStream(&byteArr, QIODevice::WriteOnly);

  stream.setByteOrder(QDataStream::BigEndian);

  stream << this->getPacketLength();
  stream << this->packetType;
  stream << this->pingType;

  return byteArr;
}

/**
 * @brief From Bytes
 */
PingPongPacket PingPongPacket::fromBytes(const QByteArray &array) {
  // allowed ping type
  auto allowedPingType = QList<quint32>{ common::types::enums::Ping, common::types::enums::Pong };
  auto stream = QDataStream(array);

  using common::types::exceptions::MalformedPacket;
  using common::types::enums::ErrorCode;

  PingPongPacket packet;

  stream.setByteOrder(QDataStream::BigEndian);

  quint32 packetLength;
  quint32 packetType;
  quint32 pingType;

  stream >> packetLength;
  stream >> packetType;
  stream >> pingType;

  if (packetType != PacketType::PING_PONG_PACKET) {
    throw common::types::exceptions::NotThisPacket("Not PingPongPacket");
  }

  if (stream.status() != QDataStream::Ok) {
    throw MalformedPacket(ErrorCode::CodingError, "PingPongPacket");
  }

  // check for valid ping type
  if (!allowedPingType.contains(pingType)) {
    throw std::invalid_argument("Invalid Ping Type");
  }

  packet.setPingType(pingType);

  return packet;
}
}  // namespace srilakshmikanthanp::clipbirdesk::packets
