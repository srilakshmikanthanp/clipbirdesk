// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <bit>
#include <cinttypes>
#include <vector>

namespace srilakshmikanthanp::clipbirdesk::utility::functions {
/**
 * @brief convert from host to network byte order
 *
 * @tparam T Type to convert
 * @param value value to convert
 * @return T converted value
 */
template <typename T>
T hostToNet(T value) noexcept {
  if constexpr (std::endian::native == std::endian::big) {
    return value;
  }

  T temp = 0;
  for (std::size_t i = 0; i < sizeof(T); i++) {
    temp |= (value & 0xFF) << (8 * (sizeof(T) - i - 1));
    value >>= 8;
  }
  return temp;
}

/**
 * @brief convert from network to host byte order
 *
 * @tparam T Type to convert
 * @param value value to convert
 * @return T converted value
 */
template <typename T>
T netToHost(T value) noexcept {
  if constexpr (std::endian::native == std::endian::big) {
    return value;
  }

  T temp = 0;
  for (std::size_t i = 0; i < sizeof(T); i++) {
    temp |= (value & 0xFF) << (8 * (sizeof(T) - i - 1));
    value >>= 8;
  }
  return temp;
}

/**
 * @brief convert Type T to std::verctor<std::uint8_t>
 *
 * @tparam T
 * @param value
 * @return std::vector<std::uint8_t>
 */
template <typename T>
std::vector<std::uint8_t> toBytes(T value) noexcept {
  const auto array = reinterpret_cast<std::uint8_t *>(&value);
  return std::vector<std::uint8_t>(array, array + sizeof(T));
}

/**
 * @brief convert std::vector<std::uint8_t> to Type T
 *
 * @tparam T
 * @param bytes
 * @return T
 */
template <typename T>
T fromBytes(const std::vector<std::uint8_t> &bytes) noexcept {
  return *reinterpret_cast<const T *>(bytes.data());
}
}  // namespace srilakshmikanthanp::clipbirdesk::utility::functions
