#pragma once

#include <exception>
#include "invalidrequest.hpp"

namespace srilakshmikanthanp::clipbirdesk::packets {
class InvalidRequestException: public std::exception {
private:
  InvalidRequest invalidRequestPacket;
public:
  explicit InvalidRequestException(const InvalidRequest& packet): invalidRequestPacket(packet) {}

  InvalidRequest getInvalidRequestPacket() const noexcept {
    return invalidRequestPacket;
  }

  const char* what() const noexcept override {
    return "Invalid Request Exception occurred.";
  }
};
}  // namespace srilakshmikanthanp::clipbirdesk::packets
