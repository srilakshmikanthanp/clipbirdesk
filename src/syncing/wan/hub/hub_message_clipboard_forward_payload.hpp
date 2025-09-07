#pragma once

#include <QString>

#include "common/nlohmann/adl_serializer.hpp"
#include "nlohmann/json.hpp"
#include "types/encrypted_bytes.hpp"
#include "hub_message_type.hpp"
#include "hub_message_type_of.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::wan {
struct HubMessageClipboardForwardPayload {
  QString toDeviceId;
  QVector<QPair<QString, types::EncryptedBytes>> clipboard;
};

template<>
struct HubMessageTypeOf<HubMessageClipboardForwardPayload> {
  static constexpr HubMessageType value = HubMessageType::CLIPBOARD_FORWARD;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(HubMessageClipboardForwardPayload,
  toDeviceId,
  clipboard
);
}  // namespace srilakshmikanthanp::clipbirdesk::syncing::wan
