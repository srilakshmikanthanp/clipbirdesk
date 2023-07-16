#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

// Standard header files
#include <stdexcept>

// Qt header files
#include <QByteArray>
#include <QDataStream>
#include <QIODevice>
#include <QtTypes>

// Local header files
#include "types/enums/enums.hpp"
#include "types/except/except.hpp"

namespace srilakshmikanthanp::clipbirdesk::network::packets {
/**
 * @brief Server Discovery Packet
 */
class DiscoveryPacket {
 private:
  quint8 packetType;
  qint32 packetLength;
  quint8 ipType;
  QByteArray hostIp;
  qint16 hostPort;

 public:
  /// @brief Allowed Packet Types
  enum PacketType : quint8 { Request = 0x01, Response = 0x02 };

 public:
  /**
   * @brief Set the Packet Type object
   *
   * @param type
   */
  void setPacketType(quint8 type);

  /**
   * @brief Get the Packet Type object
   *
   * @return quint8
   */
  quint8 getPacketType() const noexcept;

  /**
   * @brief Set the Packet Length object
   *
   * @param length
   */
  void setPacketLength(qint32 length);

  /**
   * @brief Get the Packet Length object
   *
   * @return qint32
   */
  qint32 getPacketLength() const noexcept;

  /**
   * @brief Set the Ip Type object
   *
   * @param type
   */
  void setIpType(types::enums::IPType type);

  /**
   * @brief Get the Ip Type object
   *
   * @return quint8
   */
  types::enums::IPType getIpType() const noexcept;

  /**
   * @brief Set the Client Ip object
   *
   * @param ip
   */
  void setHostIp(QByteArray ip);

  /**
   * @brief Get the Client Ip object
   *
   * @return const quint8*
   */
  QByteArray getHostIp() const noexcept;

  /**
   * @brief Set the Client Port object
   *
   * @param port
   */
  void setHostPort(qint16 port);

  /**
   * @brief Get the Client Port object
   *
   * @return qint16
   */
  qint16 getHostPort() const noexcept;

  /**
   * @brief Get the Size of the Packet
   *
   * @return std::size_t
   */
  std::size_t size() const noexcept;

  /**
   * @brief Input stream operator for QDataStream
   *
   * @param stream
   * @param packet
   * @return QDataStream&
   */
  friend QDataStream& operator<<(QDataStream& stream, const DiscoveryPacket& packet);

  /**
   * @brief Output stream operator for QDataStream
   *
   * @param stream
   * @param packet
   * @return QDataStream&
   */
  friend QDataStream& operator>>(QDataStream& stream, DiscoveryPacket& packet);
};
}  // namespace srilakshmikanthanp::clipbirdesk::network::packets
