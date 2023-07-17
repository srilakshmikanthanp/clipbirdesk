#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <exception>
#include <string>

namespace srilakshmikanthanp::clipbirdesk::types::except {
/**
 * @brief Exception for Invalid Packet
 */
class MalformedPacket : public std::exception {
 private:

  std::string message;
  std::uint8_t code;

 public:

  // Constructor
  MalformedPacket(std::uint8_t code, const std::string& message) noexcept;

  // get message
  const char* what() const noexcept override;

  // get Code
  std::uint8_t getCode() const noexcept;

  // Virtual destructor for inheritance
  virtual ~MalformedPacket() noexcept = default;
};

/**
 * @brief Exception for Not supported type
 */
class NotSupported : public std::exception {
 private:

  std::string message;

 public:

  // Constructor
  NotSupported(const std::string& message) noexcept;

  // get message
  const char* what() const noexcept override;
};
}  // namespace srilakshmikanthanp::clipbirdesk::types::except
