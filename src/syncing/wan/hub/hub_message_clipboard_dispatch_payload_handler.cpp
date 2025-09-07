#include "hub_message_clipboard_dispatch_payload_handler.hpp"
#include "utility/functions/crypto/crypto.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::wan {
HubMessageClipboardDispatchPayloadHandler::HubMessageClipboardDispatchPayloadHandler(QObject* parent) : HubMessagePayloadHandler<HubMessageClipboardDispatchPayload>(parent) {}

void HubMessageClipboardDispatchPayloadHandler::handle(Hub *hub, const HubMessageClipboardDispatchPayload& payload) {
  using namespace srilakshmikanthanp::clipbirdesk::utility::functions;
  QVector<QPair<QString, QByteArray>> items;
  for (const auto& item : payload.clipboard) items.push_back({ item.first, decrypt(item.second, hub->getHubHostDevice().privateKey.toUtf8()) });
  emit hub->OnSyncRequest(items);
}
}  // namespace srilakshmikanthanp::clipbirdesk::syncing::wan
