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
/**
 * @brief Invalid Packet used to indicate the error
 * on the client side
 */
class InvalidRequest: public NetworkPacket {
 private:

  quint32 packetType = PacketType::INVALID_REQUEST;
  quint32 errorCode;
  QByteArray errorMessage;

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
   * @brief Convert the InvalidRequest to QByteArray
   */
  QByteArray toBytes() const override;

  /**
   * @brief Convert the QByteArray to InvalidRequest
   */
  static InvalidRequest fromBytes(const QByteArray &array);
};
}  // namespace srilakshmikanthanp::clipbirdesk::packets
