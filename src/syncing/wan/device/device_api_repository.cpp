#include "device_api_repository.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::wan {
/**
 * @brief Construct a new DeviceRepository object
 *
 * @param parent Parent object
 */
DeviceApiRepository::DeviceApiRepository(DeviceApiClient* apiClient, QObject* parent)
    : DeviceRepository(parent), m_apiClient(apiClient) {}

/**
 * @brief Create a new device
 */
QFuture<DeviceResponseDto> DeviceApiRepository::createDevice(DeviceRequestDto dto) {
  return m_apiClient->createDevice(dto);
}

/**
 * @brief Update device by ID
 */
QFuture<DeviceResponseDto> DeviceApiRepository::updateDevice(
    const QString& id, DeviceRequestDto dto
) {
  return m_apiClient->updateDevice(id, dto);
}

/**
 * @brief Destroy the DeviceRepository object
 */
DeviceApiRepository::~DeviceApiRepository() = default;
}  // namespace srilakshmikanthanp::clipbirdesk::syncing::wan
