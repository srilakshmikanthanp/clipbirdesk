#pragma once

#include <QString>

#include "common/nlohmann/adl_serializer.hpp"
#include "nlohmann/json.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::wan {
struct DeviceRequestDto {
  QString publicKey;
  QString name;
  QString type;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(DeviceRequestDto, publicKey, name, type);
}  // namespace srilakshmikanthanp::clipbirdesk::syncing::wan
