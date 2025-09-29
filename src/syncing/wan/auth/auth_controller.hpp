#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

#include <QObject>
#include <QFuture>
#include <QPromise>

#include "store/secure_storage.hpp"

// project headers
#include "syncing/wan/auth/auth_repository.hpp"
#include "syncing/wan/auth/auth_api_repository.hpp"
#include "syncing/wan/auth/auth_api_client.hpp"
#include "syncing/wan/auth/auth_token_holder.hpp"
#include "utility/functions/crypto/crypto.hpp"
#include "utility/functions/keychain/keychain.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::wan {
class AuthController : public QObject {
 private:  // just for Qt

  Q_OBJECT

 private:

  Q_DISABLE_COPY_MOVE(AuthController)

 private:

  AuthRepository *authRepository = new AuthApiRepository(new AuthApiClient(this), this);

 public:  // Constructors and Destructors

  AuthController(QObject *parent = nullptr);
  virtual ~AuthController();

  QFuture<void> signIn(QString email, QString password);
  bool isSignedIn();
  QFuture<void> signOut();
};
}
