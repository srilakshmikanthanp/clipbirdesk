#pragma once

#include <QObject>
#include <QString>
#include <QFuture>

#include "constants/constants.hpp"
#include "auth_token_dto.hpp"
#include "basic_auth_request_dto.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::wan {
class AuthRepository : public QObject {
 private:  // just for Qt

  /// @brief Qt meta object
  Q_OBJECT

 private:  // disable copy and move

  Q_DISABLE_COPY_MOVE(AuthRepository)

 public:

  /**
   * @brief Construct a new AuthRepository object
   *
   * @param parent Parent object
   */
  explicit AuthRepository(QObject* parent = nullptr);

  /**
   * @brief Authenticate user and get auth token
   */
  virtual QFuture<AuthTokenDto> signIn(BasicAuthRequestDto dto) = 0;

  /**
   * @brief Destroy the AuthRepository object
   */
  virtual ~AuthRepository();
};
}
