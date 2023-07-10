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
 * @brief Server Discovery Packet
 */
class DiscoveryPacket {
 private:
  quint8     packetType;
  qint32     packetLength;
  quint8     ipType;
  QByteArray hostIp;
  qint16     hostPort;

 public:
  /// @brief Allowed Packet Types
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
    if (type != PacketType::Request && type != PacketType::Response) {
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
   * @brief Get the Size of the Packet
   *
   * @return std::size_t
   */
  std::size_t size() const noexcept {
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
  friend QDataStream& operator<<(QDataStream& stream, const DiscoveryPacket& packet) {
    // write the packet type
    stream << packet.packetType;

    // write the packet length
    stream << packet.packetLength;

    // write the ip type
    stream << packet.ipType;

    // check the ip value
    if (packet.hostIp.size() != (packet.ipType == 0x04 ? 4 : 16)) {
      throw std::invalid_argument("Invalid ip length");
    }

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
  friend QDataStream& operator>>(QDataStream& stream, DiscoveryPacket& packet) {
    // read the packet type
    stream >> packet.packetType;

    // is the stream status is bad
    if (stream.status() != QDataStream::Ok) {
      throw types::except::MalformedPacket(
        types::enums::CodingError, "Invalid Packet Type"
      );
    }

    // check the packet type
    if (packet.packetType != 0x01 && packet.packetType != 0x02) {
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

    // read the ip type
    stream >> packet.ipType;

    // is the stream status is bad
    if (stream.status() != QDataStream::Ok) {
      throw types::except::MalformedPacket(
        types::enums::CodingError, "Invalid IP Type"
      );
    }

    // check the ip type
    if (packet.ipType != 0x04 && packet.ipType != 0x06) {
      throw types::except::MalformedPacket(
        types::enums::CodingError, "Invalid IP Type"
      );
    }

    // resize the ip
    packet.hostIp.resize(packet.ipType == 0x04 ? 4 : 16);

    // read the ip
    stream.readRawData(packet.hostIp.data(), packet.hostIp.size());

    // is the stream status is bad
    if (stream.status() != QDataStream::Ok) {
      throw types::except::MalformedPacket(
        types::enums::CodingError, "Invalid IP"
      );
    }

    // read the port
    stream >> packet.hostPort;

    // is the stream status is bad
    if (stream.status() != QDataStream::Ok) {
      throw types::except::MalformedPacket(
        types::enums::CodingError, "Invalid Port"
      );
    }

    // return the stream
    return stream;
  }
};
} // namespace srilakshmikanthanp::clipbirdesk::network::packets
