#include "auth_controller.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::wan {
AuthController::AuthController(QObject* parent): QObject(parent) {}
AuthController::~AuthController() = default;

QFuture<void> AuthController::signIn(QString email, QString password) {
  return authRepository->signIn({email, password})
  .then([=, this](const syncing::wan::AuthTokenDto& token) {
    QKeychain::WritePasswordJob *job = nullptr;
    bool status = QMetaObject::invokeMethod(
      &storage::SecureStorage::instance(),
      &storage::SecureStorage::setHubAuthTokenToken,
      Qt::BlockingQueuedConnection,
      qReturnArg(job),
      token
    );
    assert(status && job != nullptr);
    return utility::functions::toFuture(job).then([token, this]() {
      syncing::wan::AuthTokenHolder::instance().setAuthToken(token);
    });
  })
  .unwrap();
}

bool AuthController::isSignedIn() {
  return syncing::wan::AuthTokenHolder::instance().getAuthToken().has_value();
}

QFuture<void> AuthController::signOut() {
  return utility::functions::toFuture(storage::SecureStorage::instance().removeHubHostDevice()).then([=, this]() {
    return utility::functions::toFuture(storage::SecureStorage::instance().removeHubJwtToken());
  }).unwrap().then([=, this]() {
    syncing::wan::AuthTokenHolder::instance().setAuthToken(std::nullopt);
  });
}
}
