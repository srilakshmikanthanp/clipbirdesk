#pragma once // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

// Standard header files
#include <stdexcept>

// Qt header files
#include <QByteArray>

namespace srilakshmikanthanp::clipbirdesk::interface {
/**
 * @brief Interface for network packet that can
 *        send and receive from network in bytes
 */
class INetworkPacket {
 public:
  /**
   * @brief   Make Network packet from bytes that
   *          received from network
   * @param   bytes
   * @throws  std::runtime_error
   */
  virtual void fromNetBytes(QByteArray) = 0;

  /**
   * @brief   Get the size of the packet
   * @return  size_t
   */
  virtual std::size_t size() const = 0;

  /**
   * @brief   Make Network packet to bytes that
   *          send to network
   * @throws  std::runtime_error
   * @return  QByteArray
   */
  virtual QByteArray toNetBytes() const = 0;
};
}  // namespace srilakshmikanthanp::clipbirdesk::interface
