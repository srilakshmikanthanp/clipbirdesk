#pragma once // Header guard see https://en.wikipedia.org/wiki/Include_guard

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
 * @brief Invalid Packet used to indicate the error
 * on the client side
 */
class InvalidPacket {
 private:
  quint8     packetType = 0x00;
  qint32     packetLength;
  quint8     errorCode;
  QByteArray errorMessage;

 public:
  /// @brief Allowed Packet Types
  enum PacketType : quint8 { MalformedPacket = 0x00 };

 public:
  /**
   * @brief Set the Packet Type object
   *
   * @param type
   */
  void setPacketType(quint8 type) {
    if (type != PacketType::MalformedPacket) {
      throw std::invalid_argument("Invalid Packet Type");
    }
  }

  /**
   * @brief Get the Packet Type object
   *
   * @return quint8
   */
  quint8 getPacketType() const noexcept {
    return this->packetType;
  }

  /**
   * @brief Set the Packet Length object
   *
   * @param length
   */
  void setPacketLength(qint32 length) {
    this->packetLength = length;
  }

  /**
   * @brief Get the Packet Length object
   *
   * @return qint32
   */
  qint32 getPacketLength() const noexcept {
    return this->packetLength;
  }

  /**
   * @brief Set the Error Code object
   *
   * @param code
   */
  void setErrorCode(quint8 code) {
    if (code != types::enums::ErrorCode::CodingError) {
      throw std::invalid_argument("Invalid Error Code");
    }

    this->errorCode = code;
  }

  /**
   * @brief Get the Error Code object
   *
   * @return quint8
   */
  quint8 getErrorCode() const noexcept {
    return this->errorCode;
  }

  /**
   * @brief Set the Error Message object
   *
   * @param message
   */
  void setErrorMessage(QByteArray message) {
    this->errorMessage = message;
  }

  /**
   * @brief Get the Error Message object
   *
   * @return QByteArray
   */
  QByteArray getErrorMessage() const noexcept {
    return this->errorMessage;
  }

  /**
   * @brief Get the Size of the Packet
   *
   * @return std::size_t
   */
  std::size_t size() const noexcept {
    return (
      sizeof(packetType) +
      sizeof(packetLength) +
      sizeof(errorCode) +
      errorMessage.size()
    );
  }

  /**
   * @brief Input stream operator for QDataStream
   *
   * @param stream
   * @param packet
   * @return QDataStream&
   */
  friend QDataStream& operator<<(QDataStream& stream, const InvalidPacket packet) {
    // write the packet type
    stream << packet.packetType;

    // write the packet length
    stream << packet.packetLength;

    // write the error code
    stream << packet.errorCode;

    // write the error message
    stream.writeRawData(packet.errorMessage.data(), packet.errorMessage.size());

    // return the stream
    return stream;
  }

  /**
   * @brief Output stream operator for QDataStream
   *
   * @param stream
   * @param packet
   * @return QDataStream&
   */
  friend QDataStream& operator>>(QDataStream& stream, InvalidPacket& packet) {
    // read the packet type
    stream >> packet.packetType;

    // is the stream status is bad
    if (stream.status() != QDataStream::Ok) {
      throw types::except::MalformedPacket(
        types::enums::CodingError, "Invalid Packet Type"
      );
    }

    // check the packet type
    if (packet.packetType != 0x00) {
      throw types::except::MalformedPacket(
        types::enums::CodingError, "Invalid Packet Type"
      );
    }

    // read the packet length
    stream >> packet.packetLength;

    // is the stream status is bad
    if (stream.status() != QDataStream::Ok) {
      throw types::except::MalformedPacket(
        types::enums::CodingError, "Invalid Packet Length"
      );
    }

    // read the error code
    stream >> packet.errorCode;

    // is the stream status is bad
    if (stream.status() != QDataStream::Ok) {
      throw types::except::MalformedPacket(
        types::enums::CodingError, "Invalid Error Code"
      );
    }

    // check the error code
    if (packet.errorCode != 0x01) {
      throw types::except::MalformedPacket(
        types::enums::CodingError, "Invalid Error Code"
      );
    }

    // Extract the error message length
    auto msgLen = packet.packetLength - (
      sizeof(packet.packetType) +
      sizeof(packet.packetLength) +
      sizeof(packet.errorCode)
    );

    // resize the error message
    packet.errorMessage.resize(msgLen);

    // read the error message
    stream.readRawData(packet.errorMessage.data(), packet.errorMessage.size());

    // is the stream status is bad
    if (stream.status() != QDataStream::Ok) {
      throw types::except::MalformedPacket(
        types::enums::CodingError, "Invalid Error Message"
      );
    }

    // return the stream
    return stream;
  }
};
} // namespace srilakshmikanthanp::clipbirdesk::network::packets
