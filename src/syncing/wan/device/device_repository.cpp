#include "device_repository.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::wan {
/**
 * @brief Construct a new DeviceRepository object
 *
 * @param parent Parent object
 */
DeviceRepository::DeviceRepository(QObject* parent) : QObject(parent) {}

/**
 * @brief Destroy the DeviceRepository object
 */
DeviceRepository::~DeviceRepository() = default;
}  // namespace srilakshmikanthanp::clipbirdesk::syncing::wan
