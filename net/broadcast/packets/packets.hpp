// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <cinttypes>
#include <vector>
#include <stdexcept>

#include "../../../utility/functions/functions.hpp"

namespace srilakshmikanthanp::clipbirdesk::net::broadcast::packets {
/**
 * @brief Server Discovery Packet
 */
class ServerDiscoveryPacket {
 private:
  std::uint8_t packetType;
  std::uint32_t packetLength;
  std::uint8_t ipType;
  std::uint8_t hostIp[16];
  std::uint16_t hostPort;

 public:
  /**
   * @brief Set the Packet Type object
   *
   * @param type
   */
  void setPacketType(std::uint8_t type) noexcept {
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
  void setPacketLength(std::uint32_t length) noexcept {
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
  void setIpType(std::uint8_t type) noexcept {
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
  void setClientIp(std::uint8_t ip[16]) noexcept {
    for (int i = 0; i < 16; i++) {
      this->hostIp[i] = ip[i];
    }
  }

  /**
   * @brief Get the Client Ip object
   *
   * @return const std::uint8_t*
   */
  const std::uint8_t* getClientIp() noexcept {
    return this->hostIp;
  }

  /**
   * @brief Set the Client Port object
   *
   * @param port
   */
  void setClientPort(std::uint16_t port) noexcept {
    this->hostPort = port;
  }

  /**
   * @brief Get the Client Port object
   *
   * @return std::uint16_t
   */
  std::uint16_t getClientPort() const noexcept {
    return this->hostPort;
  }

  /**
   * @brief Get the binary data of the packet
   *
   * @return std::vector<std::uint8_t>
   */
  std::vector<std::uint8_t> toNetBytes() const {
    // using utility::functions namespace
    using namespace utility::functions;

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
    using namespace utility::functions;

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
}  // namespace srilakshmikanthanp::clipbirdesk
