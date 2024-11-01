#include "device.hpp"

namespace srilakshmikanthanp::clipbirdesk::types::device {
/**
 * @brief operator overloading for ==
 */
bool Device::operator==(const Device& other) const {
  return ip == other.ip && port == other.port && name == other.name;
}

/**
 * @brief operator overloading for !=
 */
bool Device::operator!=(const Device& other) const {
  return !(*this == other);
}
}  // namespace srilakshmikanthanp::types::device
