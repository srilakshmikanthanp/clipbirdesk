#pragma once

#include <QString>

#include "common/nlohmann/adl_serializer.hpp"
#include "nlohmann/json.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::wan {
struct BasicAuthRequestDto {
  QString userName;
  QString password;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(BasicAuthRequestDto, userName, password);
}  // namespace srilakshmikanthanp::clipbirdesk::syncing::wan
