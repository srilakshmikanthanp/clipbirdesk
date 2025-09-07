#pragma once

#include <QString>
#include "common/nlohmann/adl_serializer.hpp"
#include "nlohmann/json.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::wan {
struct HubHostDevice {
  QString id;
  QString name;
  QString type;
  QString publicKey;
  QString privateKey;

  static HubHostDevice fromJson(const QString &text);
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(HubHostDevice,
  id,
  name,
  type,
  publicKey,
  privateKey
);
}  // namespace srilakshmikanthanp::clipbirdesk::syncing::wan
