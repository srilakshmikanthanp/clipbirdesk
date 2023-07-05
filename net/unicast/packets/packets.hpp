// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <cinttypes>
#include <vector>
#include <stdexcept>

#include "../../../utility/functions/functions.hpp"

namespace srilakshmikanthanp::clipbirdesk::net::unicast::packets {
/**
 * @brief Clipbird Sync Request
 */
class ClipbirdSyncPacket {
 private:
  std::uint8_t packetType = 0x03;
  std::uint32_t packetLength;
  std::uint32_t typeLength;
  std::vector<std::uint8_t> type;
  std::uint32_t dataLength;
  std::vector<std::uint8_t> data;
 public:
  /**
   * @brief Set the Packet Type object
   *
   * @param type
   */
  void setPacketType(std::uint8_t type) noexcept {
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
   * @brief Set the Type Length object
   *
   * @param length
   */
  void setTypeLength(std::uint32_t length) noexcept {
    this->typeLength = length;
  }

  /**
   * @brief Get the Type Length object
   *
   * @return std::uint32_t
   */
  std::uint32_t getTypeLength() const noexcept {
    return this->typeLength;
  }

  /**
   * @brief Set the Type object
   *
   * @param type
   */
  void setType(std::vector<std::uint8_t> type) noexcept {
    this->type = type;
  }

  /**
   * @brief Get the Type object
   *
   * @return std::vector<std::uint8_t>
   */
  std::vector<std::uint8_t> getType() const noexcept {
    return this->type;
  }

  /**
   * @brief Set the Data Length object
   *
   * @param length
   */
  void setDataLength(std::uint32_t length) noexcept {
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
  void setData(std::vector<std::uint8_t> data) noexcept {
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
    using namespace utility::functions;

    // create a vector to store the packet
    std::vector<std::uint8_t> packet;

    // push the packet type
    packet.push_back(this->packetType);

    // push the packet length
    for (const auto &i : toBytes(hostToNet(this->packetLength))) {
      packet.push_back(i);
    }

    // push the type length
    for (const auto &i : toBytes(hostToNet(this->typeLength))) {
      packet.push_back(i);
    }

    // push the type
    for (const auto &i : this->type) {
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
    using namespace utility::functions;

    // check the packet type
    if (bytes[0] != 0x03) throw std::invalid_argument("Invalid Packet Type");

    // remove the packet type
    bytes.erase(bytes.begin());

    // get the packet length
    this->packetLength = netToHost(fromBytes<std::uint32_t>(bytes));

    // remove the packet length
    bytes.erase(bytes.begin(), bytes.begin() + 4);

    // get the type length
    this->typeLength = netToHost(fromBytes<std::uint32_t>(bytes));

    // remove the type length
    bytes.erase(bytes.begin(), bytes.begin() + 4);

    // get the type
    for (std::uint32_t i = 0; i < this->typeLength; i++) {
      this->type.push_back(bytes[i]);
    }

    // remove the type
    bytes.erase(bytes.begin(), bytes.begin() + this->typeLength);

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
}  // namespace srilakshmikanthanp::clipbirdesk
