// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "device.hpp"

namespace srilakshmikanthanp::clipbirdesk::types::device {
/**
 * @brief operator overloading for ==
 */
bool Device::operator==(const Device& other) const {
  return ip == other.ip && port == other.port && name == other.name && cert == other.cert;
}

/**
 * @brief operator overloading for !=
 */
bool Device::operator!=(const Device& other) const {
  return !(*this == other);
}
}  // namespace srilakshmikanthanp::types::device
