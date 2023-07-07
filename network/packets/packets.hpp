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
#include "../../types/types.hpp"

namespace srilakshmikanthanp::clipbirdesk::network::packets {
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
  void setIpType(types::IPType type) {
    // check the ip type
    if (type != types::IPType::IPv4 && type != types::IPType::IPv6) {
      throw std::invalid_argument("Invalid ip type");
    }

    // set the ip type
    this->ipType = type == types::IPType::IPv4 ? 0x04 : 0x06;
  }

  /**
   * @brief Get the Ip Type object
   *
   * @return quint8
   */
  types::IPType getIpType() const noexcept {
    if (this->ipType == 0x04) {
      return types::IPType::IPv4;
    } else {
      return types::IPType::IPv6;
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

    // check the packet type
    if (packet.packetType != 0x01 && packet.packetType != 0x02) {
      throw std::runtime_error("Invalid Packet Type");
    }

    // read the packet length
    stream >> packet.packetLength;

    // read the ip type
    stream >> packet.ipType;

    // check the ip type
    if (packet.ipType != 0x04 && packet.ipType != 0x06) {
      throw std::runtime_error("Invalid ip type");
    }

    // resize the ip
    packet.hostIp.resize(packet.ipType == 0x04 ? 4 : 16);

    // read the ip
    packet.hostIp = stream.device()->read(packet.hostIp.size());

    // read the port
    stream >> packet.hostPort;

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

    // check the packet type
    if (packet.packetType != 0x03) {
      throw std::runtime_error("Invalid Packet Type");
    }

    // read the packet length
    stream >> packet.packetLength;

    // read the data type length
    stream >> packet.dataTypeLength;

    // resize the data type
    packet.dataType.resize(packet.dataTypeLength);

    // read the data type
    packet.dataType = stream.device()->read(packet.dataType.size());

    // read the data length
    stream >> packet.dataLength;

    // resize the data
    packet.data.resize(packet.dataLength);

    // read the data
    packet.data = stream.device()->read(packet.data.size());

    // return the stream
    return stream;
  }
};
} // namespace srilakshmikanthanp::clipbirdesk::network::packets
