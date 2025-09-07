#pragma once

#include <QDateTime>
#include <QString>

#include "common/nlohmann/adl_serializer.hpp"
#include "nlohmann/json.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::wan {
struct AuthTokenDto {
  QDateTime issuedAt;
  QString token;
  QDateTime expiry;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(AuthTokenDto, issuedAt, token, expiry);
}  // namespace srilakshmikanthanp::clipbirdesk::syncing::wan
