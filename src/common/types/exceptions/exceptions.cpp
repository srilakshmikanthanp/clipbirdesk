#include "exceptions.hpp"

namespace srilakshmikanthanp::clipbirdesk::common::types::exceptions {
//------------------------MalformedPacket------------------------//
// Constructor
MalformedPacket::MalformedPacket(std::uint8_t code, const std::string& message) noexcept
    : message(message), code(code) {}

// get message
const char* MalformedPacket::what() const noexcept {
  return (std::to_string(code) + ": " + message).c_str();
}

// get Code
std::uint8_t MalformedPacket::getCode() const noexcept {
  return code;
}

//------------------------NotThisPacket------------------------//

// Constructor
NotThisPacket::NotThisPacket(const std::string& message) noexcept : message(message) {}

// get message
const char* NotThisPacket::what() const noexcept {
  return message.c_str();
}

//------------------------NotSupported------------------------//

// Constructor
NotSupported::NotSupported(const std::string& message) noexcept : message(message) {}

// get message
const char* NotSupported::what() const noexcept {
  return message.c_str();
}

//------------------------ErrorCodeException-----------------//
// Constructor
ErrorCodeException::ErrorCodeException(int code, const std::string& message) noexcept
    : message(message), code(code) {}

// get message
const char* ErrorCodeException::what() const noexcept {
  return (std::to_string(code) + ": " + message).c_str();
}

// get Code
int ErrorCodeException::getCode() const noexcept {
  return code;
}
}  // namespace srilakshmikanthanp::clipbirdesk::common::types::exceptions
