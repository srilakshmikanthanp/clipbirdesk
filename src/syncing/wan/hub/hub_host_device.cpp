#include "syncing/wan/hub/hub_host_device.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::wan {
HubHostDevice HubHostDevice::fromJson(const QString &text) {
  nlohmann::json j = nlohmann::json::parse(text.toStdString());
  return j.get<HubHostDevice>();
}
}  // namespace srilakshmikanthanp::clipbirdesk::syncing::wan
