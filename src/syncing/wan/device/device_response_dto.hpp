#pragma once

#include <QString>

#include "common/nlohmann/adl_serializer.hpp"
#include "nlohmann/json.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::wan {
struct DeviceResponseDto {
  QString id;
  QString name;
  QString type;
  QString publicKey;
  bool isOnline;
  QString userId;
  QString createdAt;
  QString updatedAt;

  bool operator==(const DeviceResponseDto& other) const;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(DeviceResponseDto, id, name, type, publicKey, isOnline, userId, createdAt, updatedAt);
}  // namespace srilakshmikanthanp::clipbirdesk::syncing::wan

namespace std {
template <>
struct hash<srilakshmikanthanp::clipbirdesk::syncing::wan::DeviceResponseDto> {
  size_t operator()(const srilakshmikanthanp::clipbirdesk::syncing::wan::DeviceResponseDto& d) const noexcept {
    return qHash(d.id);
  }
};
}
