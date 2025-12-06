#include "certificate_exchange_packet.hpp"

namespace srilakshmikanthanp::clipbirdesk::packets {
/**
 * @brief Get the Packet Length
 *
 * @return qint32
 */
quint32 CertificateExchangePacket::getPacketLength() const noexcept {
  return quint32(
    (sizeof(decltype(std::declval<CertificateExchangePacket>().getPacketLength()))) +
    sizeof(packetType) +
    certificateBytes.size()
  );
}

/**
 * @brief Get the Packet Type
 *
 * @return quint32
 */
quint32 CertificateExchangePacket::getPacketType() const noexcept {
  return this->packetType;
}

/**
 * @brief Get the Certificate size
 *
 * @return quint32
 */
quint32 CertificateExchangePacket::getCertificateSize() const noexcept {
  return quint32(this->certificateBytes.size());
}

/**
 * @brief Get the Certificate object
 *
 * @return QByteArray
 */
QByteArray CertificateExchangePacket::getCertificate() const noexcept {
  return this->certificateBytes;
}

/**
 * @brief Set the Certificate object
 *
 * @param bytes
 */
void CertificateExchangePacket::setCertificate(QByteArray bytes) {
  this->certificateBytes = bytes;
}

/**
 * @brief Convert the CertificateExchangePacket to QByteArray
 */
QByteArray CertificateExchangePacket::toBytes() const {
  auto byteArray = QByteArray();
  auto stream  = QDataStream(&byteArray, QIODevice::WriteOnly);

  stream.setByteOrder(QDataStream::BigEndian);

  stream << this->getPacketLength();
  stream << this->packetType;
  stream.writeRawData(this->certificateBytes.data(), this->certificateBytes.size());

  return byteArray;
}

/**
 * @brief Convert the QByteArray to CertificateExchangePacket
 */
CertificateExchangePacket CertificateExchangePacket::fromBytes(const QByteArray &array) {
  auto stream = QDataStream(array);

  stream.setByteOrder(QDataStream::BigEndian);

  quint32 packetLength;
  quint32 packetType;

  stream >> packetLength;
  stream >> packetType;

  quint32 certificateSize = packetLength - sizeof(packetType) - sizeof(packetLength);
  QByteArray certificateBytes;
  certificateBytes.resize(certificateSize);
  stream.readRawData(certificateBytes.data(), certificateSize);

  // check the packet type
  if (packetType != PacketType::CERTIFICATE_EXCHANGE) {
    throw common::types::exceptions::NotThisPacket("Not SyncingPacket");
  }

  using common::types::exceptions::MalformedPacket;
  using common::types::enums::ErrorCode;

  // if the stream is not good
  if (stream.status() != QDataStream::Ok) {
    throw MalformedPacket(ErrorCode::CodingError, "SyncingPacket");
  }

  auto packet = CertificateExchangePacket();
  packet.setCertificate(certificateBytes);
  return packet;
}
}
