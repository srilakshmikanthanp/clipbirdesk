#pragma once // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <cinttypes>
#include <stdexcept>
#include <vector>

#include "../../interface/INetworkPacket.hpp"
#include "../../utility/functions/coding.hpp"

namespace srilakshmikanthanp::clipbirdesk::net::packets {
/**
 * @brief Server Discovery Packet
 */
class ServerDiscoveryPacket : public interface::INetworkPacket {
 private:
  std::uint8_t packetType;
  std::uint32_t packetLength;
  std::uint8_t ipType;
  std::vector<std::uint8_t> hostIp;
  std::uint16_t hostPort;

 public:
  /**
   * @brief Packet Type
   */
  enum PacketType : std::uint8_t {
    Request = 0x01, Response = 0x02
  };

  /**
   * @brief Ip Type
   */
  enum IpType : std::uint8_t {
    Ipv4 = 0x04, Ipv6 = 0x06
  };

 public:
  /**
   * @brief Set the Packet Type object
   *
   * @param type
   */
  void setPacketType(std::uint8_t type) {
    if (type != 0x01 && type != 0x02) {
      throw std::invalid_argument("Invalid Packet Type");
    }

    this->packetType = type;
  }

  /**
   * @brief Get the Packet Type object
   *
   * @return std::uint8_t
   */
  std::uint8_t getPacketType() const noexcept {
    return this->packetType;
  }

  /**
   * @brief Set the Packet Length object
   *
   * @param length
   */
  void setPacketLength(std::uint32_t length) {
    this->packetLength = length;
  }

  /**
   * @brief Get the Packet Length object
   *
   * @return std::uint32_t
   */
  std::uint32_t getPacketLength() const noexcept {
    return this->packetLength;
  }

  /**
   * @brief Set the Ip Type object
   *
   * @param type
   */
  void setIpType(std::uint8_t type) {
    // check the ip type
    if (type != 0x04 && type != 0x06) {
      throw std::runtime_error("Invalid ip type");
    }

    this->ipType = type;
  }

  /**
   * @brief Get the Ip Type object
   *
   * @return std::uint8_t
   */
  std::uint8_t getIpType() const noexcept {
    return this->ipType;
  }

  /**
   * @brief Set the Client Ip object
   *
   * @param ip
   */
  void setHostIp(std::vector<std::uint8_t> ip) {
    // check the ip type
    if (ipType == 0x06 && ip.size() != 16) {
      throw std::runtime_error("Invalid ip length");
    }

    if (ipType == 0x04 && ip.size() != 4) {
      throw std::runtime_error("Invalid ip length");
    }

    for (int i = 0; i < ip.size(); i++) {
      this->hostIp[i] = ip[i];
    }
  }

  /**
   * @brief Get the Client Ip object
   *
   * @return const std::uint8_t*
   */
  std::vector<std::uint8_t> getHostIp() const noexcept {
    return this->hostIp;
  }

  /**
   * @brief Set the Client Port object
   *
   * @param port
   */
  void setHostPort(std::uint16_t port) noexcept {
    this->hostPort = port;
  }

  /**
   * @brief Get the Client Port object
   *
   * @return std::uint16_t
   */
  std::uint16_t getHostPort() const noexcept {
    return this->hostPort;
  }

  /**
   * @brief Get the binary data of the packet
   *
   * @return std::vector<std::uint8_t>
   */
  std::vector<std::uint8_t> toNetBytes() const {
    // using utility::functions namespace
    using namespace utility::functions::coding;

    // Create a vector to store the packet
    std::vector<std::uint8_t> packet;

    // Push the packet length
    packet.push_back(this->packetType);

    // push packet length
    for (const auto &i : toBytes(hostToNet(this->packetLength))) {
      packet.push_back(i);
    }

    // Push the ip type
    packet.push_back(this->ipType);

    // length of the ip
    std::size_t ipLength = this->ipType == 0x04 ? 4 : 16;

    // Push the client ip
    for (std::size_t i = 0; i < ipLength; i++) {
      packet.push_back(this->hostIp[i]);
    }

    // Push the client port
    for (const auto &i : toBytes(hostToNet(this->hostPort))) {
      packet.push_back(i);
    }

    // return the packet
    return packet;
  }

  /**
   * @brief Set the binary data of the packet
   *
   * @param packet
   */
  void fromNetBytes(std::vector<std::uint8_t> bytes) {
    // using utility::functions namespace
    using namespace utility::functions::coding;

    // check the packet type
    if (bytes[0] != 0x01 && bytes[0] != 0x02) {
      throw std::runtime_error("Invalid packet type");
    }

    // get the packet type
    this->packetType = bytes[0];

    // remove the packet type
    bytes.erase(bytes.begin());

    // get the packet length
    this->packetLength = netToHost(fromBytes<std::uint32_t>(bytes));

    // remove the packet length
    bytes.erase(bytes.begin(), bytes.begin() + 4);

    // get the ip type
    this->ipType = bytes[5];

    // remove the ip type
    bytes.erase(bytes.begin());

    // check the ip type
    if (this->ipType != 0x04 && this->ipType != 0x06) {
      throw std::runtime_error("Invalid ip type");
    }

    // length of the ip
    std::size_t ipLength = this->ipType == 0x04 ? 4 : 16;

    // get the client ip
    for (std::size_t i = 0; i < ipLength; i++) {
      this->hostIp[i] = bytes[i];
    }

    // remove the client ip
    bytes.erase(bytes.begin(), bytes.begin() + ipLength);

    // get the client port
    this->hostPort = netToHost(fromBytes<std::uint16_t>(bytes));

    // remove the client port
    bytes.erase(bytes.begin(), bytes.begin() + 2);

    // check the packet length
    if (bytes.size() != 0) {
      throw std::runtime_error("Invalid packet length");
    }
  }
};

/**
 * @brief Clipbird Sync Request
 */
class ClipbirdSyncPacket : public interface::INetworkPacket {
 private:
  std::uint8_t packetType = 0x03;
  std::uint32_t packetLength;
  std::uint32_t dataTypeLength;
  std::vector<std::uint8_t> dataType;
  std::uint32_t dataLength;
  std::vector<std::uint8_t> data;

 public:
  /**
   * @brief Set the Packet Type object
   *
   * @param type
   */
  void setPacketType(std::uint8_t type) {
    if (type != 0x03) {
      throw std::invalid_argument("Invalid Packet Type");
    }
  }

  /**
   * @brief Get the Packet Type object
   *
   * @return std::uint8_t
   */
  std::uint8_t getPacketType() const noexcept {
    return this->packetType;
  }

  /**
   * @brief Set the Packet Length object
   *
   * @param length
   */
  void setPacketLength(std::uint32_t length) {
    this->packetLength = length;
  }

  /**
   * @brief Get the Packet Length object
   *
   * @return std::uint32_t
   */
  std::uint32_t getPacketLength() const noexcept {
    return this->packetLength;
  }

  /**
   * @brief Set the Type Length object
   *
   * @param length
   */
  void setDataTypeLength(std::uint32_t length) {
    this->dataTypeLength = length;
  }

  /**
   * @brief Get the Type Length object
   *
   * @return std::uint32_t
   */
  std::uint32_t getDataTypeLength() const noexcept {
    return this->dataTypeLength;
  }

  /**
   * @brief Set the Type object
   *
   * @param type
   */
  void setDataType(std::vector<std::uint8_t> type) {
    this->dataType = type;
  }

  /**
   * @brief Get the Type object
   *
   * @return std::vector<std::uint8_t>
   */
  std::vector<std::uint8_t> getDataType() const noexcept {
    return this->dataType;
  }

  /**
   * @brief Set the Data Length object
   *
   * @param length
   */
  void setDataLength(std::uint32_t length) {
    this->dataLength = length;
  }

  /**
   * @brief Get the Data Length object
   *
   * @return std::uint32_t
   */
  std::uint32_t getDataLength() const noexcept {
    return this->dataLength;
  }

  /**
   * @brief Set the Data object
   *
   * @param data
   */
  void setData(std::vector<std::uint8_t> data) {
    this->data = data;
  }

  /**
   * @brief Get the Data object
   *
   * @return std::vector<std::uint8_t>
   */
  std::vector<std::uint8_t> getData() const noexcept {
    return this->data;
  }

  /**
   * @brief Get the binary data of the packet
   *
   * @return std::vector<std::uint8_t>
   */
  std::vector<std::uint8_t> toNetBytes() const {
    // using utility::functions namespace
    using namespace utility::functions::coding;

    // create a vector to store the packet
    std::vector<std::uint8_t> packet;

    // push the packet type
    packet.push_back(this->packetType);

    // push the packet length
    for (const auto &i : toBytes(hostToNet(this->packetLength))) {
      packet.push_back(i);
    }

    // push the type length
    for (const auto &i : toBytes(hostToNet(this->dataTypeLength))) {
      packet.push_back(i);
    }

    // push the type
    for (const auto &i : this->dataType) {
      packet.push_back(i);
    }

    // push the data length
    for (const auto &i : toBytes(hostToNet(this->dataLength))) {
      packet.push_back(i);
    }

    // push the data
    for (const auto &i : this->data) {
      packet.push_back(i);
    }

    // return the packet
    return packet;
  }

  /**
   * @brief Set the binary data of the packet
   *
   * @param bytes
   */
  void fromNetBytes(std::vector<std::uint8_t> bytes) {
    // using utility::functions namespace
    using namespace utility::functions::coding;

    // check the packet type
    if (bytes[0] != 0x03) throw std::invalid_argument("Invalid Packet Type");

    // remove the packet type
    bytes.erase(bytes.begin());

    // get the packet length
    this->packetLength = netToHost(fromBytes<std::uint32_t>(bytes));

    // remove the packet length
    bytes.erase(bytes.begin(), bytes.begin() + 4);

    // get the type length
    this->dataTypeLength = netToHost(fromBytes<std::uint32_t>(bytes));

    // remove the type length
    bytes.erase(bytes.begin(), bytes.begin() + 4);

    // get the type
    for (std::uint32_t i = 0; i < this->dataTypeLength; i++) {
      this->dataType.push_back(bytes[i]);
    }

    // remove the type
    bytes.erase(bytes.begin(), bytes.begin() + this->dataTypeLength);

    // get the data length
    this->dataLength = netToHost(fromBytes<std::uint32_t>(bytes));

    // remove the data length
    bytes.erase(bytes.begin(), bytes.begin() + 4);

    // get the data
    for (std::uint32_t i = 0; i < this->dataLength; i++) {
      this->data.push_back(bytes[i]);
    }

    // remove the data
    bytes.erase(bytes.begin(), bytes.begin() + this->dataLength);

    // check the packet length
    if (bytes.size() != 0) {
      throw std::invalid_argument("Invalid Packet Length");
    }
  }
};
}  // namespace srilakshmikanthanp::clipbirdesk::net::packets
