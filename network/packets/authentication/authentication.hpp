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
#include <QtEndian>
#include <QIODevice>
#include <QtTypes>

// Local header files
#include "types/enums/enums.hpp"
#include "types/except/except.hpp"
#include "utility/functions/helpers/helpers.hpp"

namespace srilakshmikanthanp::clipbirdesk::network::packets {
/**
 * @brief Authentication Packet used to indicate the status
 * of the authentication process
 */
class Authentication {
 private:

  quint32 packetLength;
  quint32 packetType = 0x01;
  quint32 authStatus;

 public:

  /// @brief Allowed Packet Types
  enum PacketType : quint8 { AuthStatus = 0x01 };

 public:

  /**
   * @brief Set the Packet Length object
   *
   * @param length
   */
  void setPacketLength(quint32 length);

  /**
   * @brief Get the Packet Length object
   *
   * @return qint32
   */
  quint32 getPacketLength() const noexcept;

  /**
   * @brief Set the Packet Type object
   *
   * @param type
   */
  void setPacketType(quint32 type);

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
   * @brief Get the Size of the Packet
   *
   * @return qint32
   */
  quint32 size() const noexcept;

  /**
   * @brief Create Authentication Packet from Bytes
   */
  static Authentication fromBytes(const QByteArray &array);

  /**
   * @brief Convert Authentication Packet to Bytes
   */
  static QByteArray toBytes(const Authentication& packet);
};
} // namespace srilakshmikanthanp::clipbirdesk::network::packets
