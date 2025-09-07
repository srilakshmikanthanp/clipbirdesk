#pragma once

#include <QObject>
#include <QString>
#include <QFuture>

#include "constants/constants.hpp"
#include "auth_token_dto.hpp"
#include "basic_auth_request_dto.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::wan {
class AuthApiClient : public QObject {
 private:  // just for Qt

  /// @brief Qt meta object
  Q_OBJECT

 private:  // disable copy and move

  Q_DISABLE_COPY_MOVE(AuthApiClient)

 private:

  static constexpr const char* BASE_URL = "auth";

 public:  // constructor and destructor

  /**
   * @brief Construct a new AuthApiClient object
   *
   * @param parent Parent object
   */
  explicit AuthApiClient(QObject* parent = nullptr);

  /**
   * @brief Destroy the AuthApiClient object
   */
  virtual ~AuthApiClient();

 public: // methods

  /**
   * @brief Authenticate user and get auth token
   */
  QFuture<AuthTokenDto> signIn(BasicAuthRequestDto dto);
};
} // namespace srilakshmikanthanp::clipbirdesk::syncing::wan
