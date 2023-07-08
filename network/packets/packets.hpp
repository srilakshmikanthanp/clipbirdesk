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
#include "../../interface/INetworkPacket.hpp"
#include "../../types/enums/enums.hpp"
#include "../../types/except/except.hpp"

namespace srilakshmikanthanp::clipbirdesk::network::packets {
/**
 * @brief Invalid Packet used to indicate the error
 * on the client side
 */
class MalformedPacket : public interface::INetworkPacket {
 private:
  quint8     packetType = 0x00;
  qint32     packetLength;
  quint8     errorCode;
  QByteArray errorMessage;

 public:
  enum PacketType : quint8 {
    InvalidPacket = 0x00
  };

  enum ErrorCode : quint8 {
    CodingError = 0x01,
  };

 public:
  /**
   * @brief Set the Packet Type object
   *
   * @param type
   */
  void setPacketType(quint8 type) {
    if (type != 0x00) {
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
    if (code != 0x01) {
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
   * @brief Get the binary data of the packet
   *
   * @return QByteArray
   */
  QByteArray toNetBytes() const override {
    // Create a QByteArray & QDataStream to store the bytes
    QByteArray bytes; QDataStream stream(&bytes, QIODevice::WriteOnly);

    // write the object
    stream << *this;

    // return the bytes
    return bytes;
  }

  /**
   * @brief Set the binary data of the packet
   *
   * @param bytes
   */
  void fromNetBytes(QByteArray bytes) override {
    // create a data stream
    QDataStream stream(bytes);

    // Read the packet
    stream >> *this;
  }

  /**
   * @brief Get the Size of the Packet
   *
   * @return std::size_t
   */
  std::size_t size() const noexcept override {
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
  friend QDataStream& operator<<(QDataStream& stream, const MalformedPacket packet) {
    // set the byte order
    stream.setByteOrder(QDataStream::BigEndian);

    // set the version
    stream.setVersion(QDataStream::Qt_5_15);

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
  friend QDataStream& operator>>(QDataStream& stream, MalformedPacket& packet) {
    // set the byte order
    stream.setByteOrder(QDataStream::BigEndian);

    // set the version
    stream.setVersion(QDataStream::Qt_5_15);

    // read the packet type
    stream >> packet.packetType;

    // is the stream status is bad
    if (stream.status() != QDataStream::Ok) {
      throw types::except::InvalidPacket(
        MalformedPacket::ErrorCode::CodingError, "Invalid Packet Type"
      );
    }

    // check the packet type
    if (packet.packetType != 0x00) {
      throw types::except::InvalidPacket(
        MalformedPacket::ErrorCode::CodingError, "Invalid Packet Type"
      );
    }

    // read the packet length
    stream >> packet.packetLength;

    // is the stream status is bad
    if (stream.status() != QDataStream::Ok) {
      throw types::except::InvalidPacket(
        MalformedPacket::ErrorCode::CodingError, "Invalid Packet Length"
      );
    }

    // read the error code
    stream >> packet.errorCode;

    // is the stream status is bad
    if (stream.status() != QDataStream::Ok) {
      throw types::except::InvalidPacket(
        MalformedPacket::ErrorCode::CodingError, "Invalid Error Code"
      );
    }

    // check the error code
    if (packet.errorCode != 0x01) {
      throw types::except::InvalidPacket(
        MalformedPacket::ErrorCode::CodingError, "Invalid Error Code"
      );
    }

    // Extract the error message length
    auto msgLen = packet.packetLength - (
      sizeof(packet.packetType) + sizeof(packet.packetLength) +
      sizeof(packet.errorCode)
    );

    // resize the error message
    packet.errorMessage.resize(msgLen);

    // read the error message
    stream.readRawData(packet.errorMessage.data(), packet.errorMessage.size());

    // is the stream status is bad
    if (stream.status() != QDataStream::Ok) {
      throw types::except::InvalidPacket(
        MalformedPacket::ErrorCode::CodingError, "Invalid Error Message"
      );
    }

    // return the stream
    return stream;
  }
};

/**
 * @brief Server Discovery Packet
 */
class ServiceDiscoveryPacket : public interface::INetworkPacket {
 private:
  quint8     packetType;
  qint32     packetLength;
  quint8     ipType;
  QByteArray hostIp;
  qint16     hostPort;

 public:
  /**
   * @brief Packet Type
   */
  enum PacketType : quint8 {
    Request = 0x01, Response = 0x02
  };

 public:
  /**
   * @brief Set the Packet Type object
   *
   * @param type
   */
  void setPacketType(quint8 type) {
    if (type != 0x01 && type != 0x02) {
      throw std::invalid_argument("Invalid Packet Type");
    }

    this->packetType = type;
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
   * @brief Set the Ip Type object
   *
   * @param type
   */
  void setIpType(types::enums::IPType type) {
    // check the ip type
    if (type != types::enums::IPType::IPv4 && type != types::enums::IPType::IPv6) {
      throw std::invalid_argument("Invalid ip type");
    }

    // set the ip type
    this->ipType = type == types::enums::IPType::IPv4 ? 0x04 : 0x06;
  }

  /**
   * @brief Get the Ip Type object
   *
   * @return quint8
   */
  types::enums::IPType getIpType() const noexcept {
    if (this->ipType == 0x04) {
      return types::enums::IPType::IPv4;
    } else {
      return types::enums::IPType::IPv6;
    }
  }

  /**
   * @brief Set the Client Ip object
   *
   * @param ip
   */
  void setHostIp(QByteArray ip) {
    // check the ip type is set or not
    if (ipType != 0x04 && ipType != 0x06) {
      throw std::runtime_error("Set the ip type first");
    }

    // check the ip length of ipv16
    if (ipType == 0x06 && ip.size() != 16) {
      throw std::runtime_error("Invalid ip length");
    }

    // check the ip length of ipv4
    if (ipType == 0x04 && ip.size() != 4) {
      throw std::runtime_error("Invalid ip length");
    }

    // set the ip if tests are passed
    this->hostIp = ip;
  }

  /**
   * @brief Get the Client Ip object
   *
   * @return const quint8*
   */
  QByteArray getHostIp() const noexcept {
    return this->hostIp;
  }

  /**
   * @brief Set the Client Port object
   *
   * @param port
   */
  void setHostPort(qint16 port) {
    this->hostPort = port;
  }

  /**
   * @brief Get the Client Port object
   *
   * @return qint16
   */
  qint16 getHostPort() const noexcept {
    return this->hostPort;
  }

  /**
   * @brief Get the binary data of the packet
   *
   * @return QByteArray
   */
  QByteArray toNetBytes() const {
    // Create a QByteArray & QDataStream to store the bytes
    QByteArray bytes; QDataStream stream(&bytes, QIODevice::WriteOnly);

    // write the object
    stream << *this;

    // return the bytes
    return bytes;
  }

  /**
   * @brief Set the binary data of the packet
   *
   * @param packet
   */
  void fromNetBytes(QByteArray bytes) {
    // create a data stream
    QDataStream stream(bytes);

    // read the object
    stream >> *this;
  }

  /**
   * @brief Get the Size of the Packet
   *
   * @return std::size_t
   */
  std::size_t size() const noexcept override {
    return (
      sizeof(packetType) +
      sizeof(packetLength) +
      sizeof(ipType) +
      sizeof(hostPort) +
      hostIp.size()
    );
  }

  /**
   * @brief Input stream operator for QDataStream
   *
   * @param stream
   * @param packet
   * @return QDataStream&
   */
  friend QDataStream& operator<<(QDataStream& stream, const ServiceDiscoveryPacket& packet) {
    // set the byte order
    stream.setByteOrder(QDataStream::BigEndian);

    // set the version
    stream.setVersion(QDataStream::Qt_5_15);

    // write the packet type
    stream << packet.packetType;

    // write the packet length
    stream << packet.packetLength;

    // write the ip type
    stream << packet.ipType;

    // write the ip
    stream.writeRawData(packet.hostIp.data(), packet.hostIp.size());

    // write the port
    stream << packet.hostPort;

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
  friend QDataStream& operator>>(QDataStream& stream, ServiceDiscoveryPacket& packet) {
    // set the byte order
    stream.setByteOrder(QDataStream::BigEndian);

    // set the version
    stream.setVersion(QDataStream::Qt_5_15);

    // read the packet type
    stream >> packet.packetType;

    // is the stream status is bad
    if (stream.status() != QDataStream::Ok) {
      throw types::except::InvalidPacket(
        MalformedPacket::ErrorCode::CodingError, "Invalid Packet Type"
      );
    }

    // check the packet type
    if (packet.packetType != 0x01 && packet.packetType != 0x02) {
      throw types::except::InvalidPacket(
        MalformedPacket::ErrorCode::CodingError, "Invalid Packet Type"
      );
    }

    // read the packet length
    stream >> packet.packetLength;

    // is the stream status is bad
    if (stream.status() != QDataStream::Ok) {
      throw types::except::InvalidPacket(
        MalformedPacket::ErrorCode::CodingError, "Invalid Packet Length"
      );
    }

    // read the ip type
    stream >> packet.ipType;

    // is the stream status is bad
    if (stream.status() != QDataStream::Ok) {
      throw types::except::InvalidPacket(
        MalformedPacket::ErrorCode::CodingError, "Invalid IP Type"
      );
    }

    // check the ip type
    if (packet.ipType != 0x04 && packet.ipType != 0x06) {
      throw types::except::InvalidPacket(
        MalformedPacket::ErrorCode::CodingError, "Invalid IP Type"
      );
    }

    // resize the ip
    packet.hostIp.resize(packet.ipType == 0x04 ? 4 : 16);

    // read the ip
    stream.readRawData(packet.hostIp.data(), packet.hostIp.size());

    // is the stream status is bad
    if (stream.status() != QDataStream::Ok) {
      throw types::except::InvalidPacket(
        MalformedPacket::ErrorCode::CodingError, "Invalid IP"
      );
    }

    // read the port
    stream >> packet.hostPort;

    // is the stream status is bad
    if (stream.status() != QDataStream::Ok) {
      throw types::except::InvalidPacket(
        MalformedPacket::ErrorCode::CodingError, "Invalid Port"
      );
    }

    // return the stream
    return stream;
  }
};

/**
 * @brief Clipbird Sync Request
 */
class ClipbirdSyncPacket : public interface::INetworkPacket {
 private:
  quint8     packetType = 0x03;
  qint32     packetLength;
  qint32     dataTypeLength;
  QByteArray dataType;
  qint32     dataLength;
  QByteArray data;

 public:
  enum PacketType : quint8 {
    SyncPacket = 0x03
  };

 public:
  /**
   * @brief Set the Packet Type object
   *
   * @param type
   */
  void setPacketType(quint8 type) {
    if (type != 0x03) {
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
   * @brief Set the Type Length object
   *
   * @param length
   */
  void setDataTypeLength(qint32 length) {
    this->dataTypeLength = length;
  }

  /**
   * @brief Get the Type Length object
   *
   * @return qint32
   */
  qint32 getDataTypeLength() const noexcept {
    return this->dataTypeLength;
  }

  /**
   * @brief Set the Type object
   *
   * @param type
   */
  void setDataType(QByteArray type) {
    this->dataType = type;
  }

  /**
   * @brief Get the Type object
   *
   * @return QByteArray
   */
  QByteArray getDataType() const noexcept {
    return this->dataType;
  }

  /**
   * @brief Set the Data Length object
   *
   * @param length
   */
  void setDataLength(qint32 length) {
    this->dataLength = length;
  }

  /**
   * @brief Get the Data Length object
   *
   * @return qint32
   */
  qint32 getDataLength() const noexcept {
    return this->dataLength;
  }

  /**
   * @brief Set the Data object
   *
   * @param data
   */
  void setData(QByteArray data) {
    this->data = data;
  }

  /**
   * @brief Get the Data object
   *
   * @return QByteArray
   */
  QByteArray getData() const noexcept {
    return this->data;
  }

  /**
   * @brief Get the binary data of the packet
   *
   * @return QByteArray
   */
  QByteArray toNetBytes() const {
    // Create a QByteArray & QDataStream to store the bytes
    QByteArray bytes; QDataStream stream(&bytes, QIODevice::WriteOnly);

    // write the object
    stream << *this;

    // return the bytes
    return bytes;
  }

  /**
   * @brief Set the binary data of the packet
   *
   * @param bytes
   */
  void fromNetBytes(QByteArray bytes) {
    // create a data stream
    QDataStream stream(bytes);

    // Read the packet
    stream >> *this;
  }

  /**
   * @brief Get the Size of the Packet
   *
   * @return std::size_t
   */
  std::size_t size() const noexcept override {
    return (
      sizeof(packetType) +
      sizeof(packetLength) +
      sizeof(dataTypeLength) +
      sizeof(dataLength) +
      dataType.size() +
      data.size()
    );
  }

  /**
   * @brief Input stream operator for QDataStream
   *
   * @param stream
   * @param packet
   * @return QDataStream&
   */
  friend QDataStream& operator<<(QDataStream& stream, const ClipbirdSyncPacket& packet) {
    // set the byte order
    stream.setByteOrder(QDataStream::BigEndian);

    // set the version
    stream.setVersion(QDataStream::Qt_5_15);

    // write the packet type
    stream << packet.packetType;

    // write the packet length
    stream << packet.packetLength;

    // write the data type length
    stream << packet.dataTypeLength;

    // write the data type
    stream.writeRawData(packet.dataType.data(), packet.dataType.size());

    // write the data length
    stream << packet.dataLength;

    // write the data
    stream.writeRawData(packet.data.data(), packet.data.size());

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
  friend QDataStream& operator>>(QDataStream& stream, ClipbirdSyncPacket& packet) {
    // set the byte order
    stream.setByteOrder(QDataStream::BigEndian);

    // set the version
    stream.setVersion(QDataStream::Qt_5_15);

    // read the packet type
    stream >> packet.packetType;

    // is the stream status is bad
    if (stream.status() != QDataStream::Ok) {
      throw types::except::InvalidPacket(
        MalformedPacket::ErrorCode::CodingError, "Invalid Packet Type"
      );
    }

    // check the packet type
    if (packet.packetType != 0x03) {
      throw types::except::InvalidPacket(
        MalformedPacket::ErrorCode::CodingError, "Invalid Packet Type"
      );
    }

    // read the packet length
    stream >> packet.packetLength;

    // is the stream status is bad
    if (stream.status() != QDataStream::Ok) {
      throw types::except::InvalidPacket(
        MalformedPacket::ErrorCode::CodingError, "Invalid Packet Length"
      );
    }

    // read the data type length
    stream >> packet.dataTypeLength;

    // is the stream status is bad
    if (stream.status() != QDataStream::Ok) {
      throw types::except::InvalidPacket(
        MalformedPacket::ErrorCode::CodingError, "Invalid Data Type Length"
      );
    }

    // resize the data type
    packet.dataType.resize(packet.dataTypeLength);

    // read the data type
    stream.readRawData(packet.dataType.data(), packet.dataType.size());

    // is the stream status is bad
    if (stream.status() != QDataStream::Ok) {
      throw types::except::InvalidPacket(
        MalformedPacket::ErrorCode::CodingError, "Invalid Data Type"
      );
    }

    // read the data length
    stream >> packet.dataLength;

    // is the stream status is bad
    if (stream.status() != QDataStream::Ok) {
      throw types::except::InvalidPacket(
        MalformedPacket::ErrorCode::CodingError, "Invalid Data Length"
      );
    }

    // resize the data
    packet.data.resize(packet.dataLength);

    // read the data
    stream.readRawData(packet.data.data(), packet.data.size());

    // is the stream status is bad
    if (stream.status() != QDataStream::Ok) {
      throw types::except::InvalidPacket(
        MalformedPacket::ErrorCode::CodingError, "Invalid Data"
      );
    }

    // return the stream
    return stream;
  }
};
} // namespace srilakshmikanthanp::clipbirdesk::network::packets
