#pragma once

#include <stdexcept>

namespace srilakshmikanthanp::clipbirdesk::syncing::wan {
struct AuthError: public std::runtime_error {
  explicit AuthError(const std::string &message): std::runtime_error(message) {}
};
}  // namespace srilakshmikanthanp::clipbirdesk::syncing::wan
