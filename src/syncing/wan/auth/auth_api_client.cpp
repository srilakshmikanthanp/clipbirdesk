#include "auth_api_client.hpp"
#include "cpr/cpr.h"
#include "nlohmann/json.hpp"
#include <QtConcurrent>
#include <format>

namespace srilakshmikanthanp::clipbirdesk::syncing::wan {
AuthApiClient::AuthApiClient(QObject* parent) : QObject(parent) {}
AuthApiClient::~AuthApiClient() = default;

QFuture<AuthTokenDto> AuthApiClient::signIn(BasicAuthRequestDto dto) {
  return QtConcurrent::run([=]() {
    cpr::Response res = cpr::Post(
        cpr::Url{std::format("{}/{}/signin", constants::getClipbirdApiUrl(), BASE_URL)},
        cpr::Body{nlohmann::json(dto).dump()},
        cpr::Header{{"Content-Type", "application/json"}}
    );
    if (res.error && res.status_code == 401) {
      throw AuthError("Invalid username or password");
    }
    if (res.error) {
      throw std::runtime_error(res.error.message);
    }
    return nlohmann::json::parse(res.text).get<AuthTokenDto>();
  });
}
}  // namespace srilakshmikanthanp::clipbirdesk::syncing::wan
