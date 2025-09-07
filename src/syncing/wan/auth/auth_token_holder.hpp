#pragma once

#include "auth_token_dto.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::wan {
class AuthTokenHolder : public QObject {
 private:

  std::optional<AuthTokenDto> authToken;

 private:  // just for Qt

  Q_OBJECT

 private:  // disable copy and move

  Q_DISABLE_COPY_MOVE(AuthTokenHolder)

 signals:

  void authTokenChanged(std::optional<AuthTokenDto> token);

 public:

  AuthTokenHolder(QObject *parent = nullptr);

  virtual ~AuthTokenHolder();

  std::optional<AuthTokenDto> getAuthToken() const;
  AuthTokenDto getAuthTokenOrThrow() const;
  void setAuthToken(const std::optional<AuthTokenDto> &token);

  static AuthTokenHolder &instance();
};
}  // namespace srilakshmikanthanp::clipbirdesk::syncing::wan
