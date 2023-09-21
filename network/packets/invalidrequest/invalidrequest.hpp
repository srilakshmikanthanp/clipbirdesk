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
#include "utility/functions/helpers/helpers.hpp"

namespace srilakshmikanthanp::clipbirdesk::network::packets {
/**
 * @brief Invalid Packet used to indicate the error
 * on the client side
 */
class InvalidRequest {
 private:

  quint32 packetLength;
  quint32 packetType = 0x00;
  quint32 errorCode;
  QByteArray errorMessage;

 public:

  /// @brief Allowed Packet Types
  enum PacketType : quint32 { RequestFailed = 0x00 };

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
   * @brief Set the Error Code object
   *
   * @param code
   */
  void setErrorCode(quint32 code);

  /**
   * @brief Get the Error Code object
   *
   * @return quint32
   */
  quint32 getErrorCode() const noexcept;

  /**
   * @brief Set the Error Message object
   *
   * @param message
   */
  void setErrorMessage(QByteArray message);

  /**
   * @brief Get the Error Message object
   *
   * @return QByteArray
   */
  QByteArray getErrorMessage() const noexcept;

  /**
   * @brief Get the Size of the Packet
   *
   * @return qint32
   */
  quint32 size() const noexcept;

  /**
   * @brief Convert the QByteArray to InvalidRequest
   */
  static InvalidRequest fromBytes(const QByteArray &array);

  /**
   * @brief Convert the InvalidRequest to QByteArray
   */
  static QByteArray toBytes(InvalidRequest packet);
};
}  // namespace srilakshmikanthanp::clipbirdesk::network::packets
