#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Standard header files
#include <stdexcept>

// Qt header files
#include <QByteArray>
#include <QDataStream>
#include <QtEndian>
#include <QIODevice>
#include <QtTypes>

// Local header files
#include "packets/network_packet.hpp"
#include "packets/packet_type.hpp"
#include "common/types/enums/enums.hpp"
#include "common/types/exceptions/exceptions.hpp"

namespace srilakshmikanthanp::clipbirdesk::packets {
/**
 * @brief Authentication Packet used to indicate the status
 * of the authentication process
 */
class Authentication : public NetworkPacket {
 private:

  quint32 packetType = PacketType::AUTHENTICATION_PACKET;
  quint32 authStatus;

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
   * @brief Set the Auth Status object
   *
   * @param status
   */
  void setAuthStatus(quint32 status);

  /**
   * @brief Get the Auth Status object
   *
   * @return quint32
   */
  quint32 getAuthStatus() const noexcept;

  /**
   * @brief Convert Authentication Packet to Bytes
   */
  QByteArray toBytes() const override;

  /**
   * @brief Create Authentication Packet from Bytes
   */
  static Authentication fromBytes(const QByteArray &array);
};
} // namespace srilakshmikanthanp::clipbirdesk::packets
