// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <cinttypes>
#include <vector>
#include <stdexcept>

namespace srilakshmikanthanp::clipbirdesk::net::unicast::packets {
/**
 * @brief Clipbird Sync Request
 */
class ClipbirdSyncRequest {
 private:
  std::uint8_t packetType = 0x03;
  std::uint32_t packetLength;
  std::uint32_t typeLength;
  std::vector<std::uint8_t> type;
  std::uint32_t dataLength;
  std::vector<std::uint8_t> data;
 public:
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
  std::vector<std::uint8_t> toBigEndian() const {
    // TODO: complete this function
  }

  /**
   * @brief Set the binary data of the packet
   *
   * @param packet
   */
  void fromBigEndian(std::vector<std::uint8_t> packet) {
    // TODO: complete this function
  }
};
}  // namespace srilakshmikanthanp::clipbirdesk
