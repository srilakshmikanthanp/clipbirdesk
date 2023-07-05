// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

namespace srilakshmikanthanp::clipbirdesk::interface {
/**
 * @brief Interface for network packet that can
 *       send and receive from network in bytes
 */
class INetworkPacket {
 public:
  /**
   * @brief Make Network packet from bytes that
   *        received from network
   * @param bytes
   * @throws std::runtime_error
   */
  void fromNetBytes(std::vector<std::uint8_t>) = 0;

  /**
   * @brief Make Network packet to bytes that
   *        send to network
   * @throws std::runtime_error
   * @return std::vector<std::uint8_t>
   */
  std::vector<std::uint8_t> toNetBytes() const = 0;
}
}  // namespace srilakshmikanthanp::clipbirdesk::interface
