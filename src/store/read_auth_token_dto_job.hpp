#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

#include <QSslCertificate>
#include <QSslKey>
#include <QObject>
#include "qt6keychain/keychain.h"

#include <optional>

#include "constants/constants.hpp"
#include "syncing/wan/auth/auth_token_dto.hpp"

namespace srilakshmikanthanp::clipbirdesk::storage {
class ReadAuthTokenDtoJob : public QKeychain::ReadPasswordJob {
  Q_OBJECT
public:
  using QKeychain::ReadPasswordJob::ReadPasswordJob;

  syncing::wan::AuthTokenDto authToken() const;
};
}
