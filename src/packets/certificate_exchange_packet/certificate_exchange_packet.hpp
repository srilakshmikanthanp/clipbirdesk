#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Standard header files
#include <stdexcept>

// Qt header files
#include <QByteArray>
#include <QDataStream>
#include <QIODevice>
#include <QtTypes>

// Local header files
#include "packets/network_packet.hpp"
#include "packets/packet_type.hpp"
#include "common/types/enums/enums.hpp"
#include "common/types/exceptions/exceptions.hpp"

namespace srilakshmikanthanp::clipbirdesk::packets {
class CertificateExchangePacket : public NetworkPacket {
 private:

  quint32 packetType = PacketType::CERTIFICATE_EXCHANGE;
  QByteArray certificateBytes;

 public:

  /**
   * @brief Get the Packet Length object
   *
   * @return qint32
   */
  quint32 getPacketLength() const noexcept;

  /**
   * @brief Get the Packet Type object
   *
   * @return quint32
   */
  quint32 getPacketType() const noexcept;

  /**
   * @brief Get the Certificate size
   *
   * @return quint32
   */
  quint32 getCertificateSize() const noexcept;

  /**
   * @brief Get the Certificate object
   *
   * @return QByteArray
   */
  QByteArray getCertificate() const noexcept;

  /**
   * @brief Set the Certificate object
   *
   * @param bytes
   */
  void setCertificate(QByteArray bytes);

  /**
   * @brief Convert the CertificateExchangePacket to QByteArray
   */
  QByteArray toBytes() const override;

  /**
   * @brief Convert the QByteArray to CertificateExchangePacket
   */
  static CertificateExchangePacket fromBytes(const QByteArray &array);
};
}  // namespace srilakshmikanthanp::clipbirdesk::packets
