#include "read_hub_host_device_job.hpp"

namespace srilakshmikanthanp::clipbirdesk::storage {
syncing::wan::HubHostDevice ReadHubHostDeviceJob::device() const {
  return syncing::wan::HubHostDevice::fromJson(this->textData());
}
}
