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
class InvalidPacket : public std::exception {
 private:
  std::string message;
  std::uint8_t code;

 public:
 // Constructor
  InvalidPacket(std::uint8_t code, const std::string& message) noexcept
      : message(message), code(code) {}

  // get message
  const char* what() const noexcept override {
    return (std::to_string(code) + ": " + message).c_str();
  }

  // get Code
  std::uint8_t getCode() const noexcept {
    return code;
  }

  // Virtual destructor for inheritance
  virtual ~InvalidPacket() noexcept = default;
};
}  // namespace srilakshmikanthanp::clipbirdesk::types::except
