#include "auth_token_holder.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::wan {
AuthTokenHolder::AuthTokenHolder(QObject *parent) : QObject(parent) {}

AuthTokenHolder::~AuthTokenHolder() = default;

std::optional<AuthTokenDto> AuthTokenHolder::getAuthToken() const {
  return authToken;
}

AuthTokenDto AuthTokenHolder::getAuthTokenOrThrow() const {
  if (!authToken.has_value()) {
    throw std::runtime_error("Auth token not set");
  } else {
    return authToken.value();
  }
}

void AuthTokenHolder::setAuthToken(const std::optional<AuthTokenDto> &token) {
  emit authTokenChanged(authToken = token);
}

AuthTokenHolder& AuthTokenHolder::instance() {
  static AuthTokenHolder instance;
  return instance;
}
}  // namespace srilakshmikanthanp::clipbirdesk::syncing::wan
