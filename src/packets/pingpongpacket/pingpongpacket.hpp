#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Standard header files
#include <iostream>
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
/**
 * @brief Clipboard Ping Packet's to check the connection
 */
class PingPongPacket: public NetworkPacket {
 private:  // private members

  quint32 packetType = PacketType::PING_PONG_PACKET;
  quint32 pingType;

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
   * @brief Set the Ping Type object
   *
   * @param type
   */
  void setPingType(quint32 type);

  /**
   * @brief Get the Ping Type object
   *
   * @return quint32
   */
  quint32 getPingType() const noexcept;

  /**
   * @brief to Bytes
   */
  QByteArray toBytes() const override;

  /**
   * @brief From Bytes
   */
  static PingPongPacket fromBytes(const QByteArray &array);
};
}
