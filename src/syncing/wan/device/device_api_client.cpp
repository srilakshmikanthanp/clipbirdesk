#include "syncing/wan/auth/auth_token_holder.hpp"
#include "cpr/cpr.h"
#include "device_api_client.hpp"
#include "nlohmann/json.hpp"
#include <QtConcurrent>
#include <format>

namespace srilakshmikanthanp::clipbirdesk::syncing::wan {
/**
 * @brief Construct a new DeviceApiClient object
 */
DeviceApiClient::DeviceApiClient(QObject* parent) : QObject(parent) {}

/**
 * @brief Destroy the DeviceApiClient object
 */
DeviceApiClient::~DeviceApiClient() {}

/**
 * @brief Create a new device
 */
QFuture<DeviceResponseDto> DeviceApiClient::createDevice(DeviceRequestDto dto) {
  return QtConcurrent::run([=]() {
    cpr::Response res = cpr::Post(
        cpr::Url{std::format("{}/{}", constants::getClipbirdApiUrl(), BASE_URL)},
        cpr::Body{nlohmann::json(dto).dump()},
        cpr::Header{
          {"Authorization", std::format("Bearer {}", AuthTokenHolder::instance().getAuthTokenOrThrow().token.toStdString())},
          {"Content-Type", "application/json"}
        }
    );
    if (res.error && res.status_code == 401) {
      throw AuthError("Invalid username or password");
    }
    if (res.error) {
      throw std::runtime_error(res.error.message);
    }
    return nlohmann::json::parse(res.text).get<DeviceResponseDto>();
  });
}

/**
 * @brief Update device by ID
 */
QFuture<DeviceResponseDto> DeviceApiClient::updateDevice(const QString& id, DeviceRequestDto dto) {
  return QtConcurrent::run([=]() {
    cpr::Response res = cpr::Patch(
        cpr::Url{std::format("{}/{}/{}", constants::getClipbirdApiUrl(), BASE_URL, id.toStdString())},
        cpr::Body{nlohmann::json(dto).dump()},
        cpr::Header{
          {"Authorization", std::format("Bearer {}", AuthTokenHolder::instance().getAuthTokenOrThrow().token.toStdString())},
          {"Content-Type", "application/json"}
        }
    );
    if (res.error && res.status_code == 401) {
      throw AuthError("Invalid username or password");
    }
    if (res.error) {
      throw std::runtime_error(res.error.message);
    }
    return nlohmann::json::parse(res.text).get<DeviceResponseDto>();
  });
}
}  // namespace srilakshmikanthanp::clipbirdesk::syncing::wan
