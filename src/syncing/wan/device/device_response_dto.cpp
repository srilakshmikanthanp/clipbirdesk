#include "device_response_dto.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::wan {
bool DeviceResponseDto::operator==(const DeviceResponseDto& other) const {
  return id == other.id;
}
}  // namespace srilakshmikanthanp::clipbirdesk::syncing::wan
