#include "auth_api_repository.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::wan {
/**
 * @brief Construct a new AuthRepository object
 *
 * @param parent Parent object
 */
AuthApiRepository::AuthApiRepository(AuthApiClient* apiClient, QObject* parent)
    : AuthRepository(parent), m_apiClient(apiClient) {}

/**
 * @brief Authenticate user and get auth token
 */
QFuture<AuthTokenDto> AuthApiRepository::signIn(BasicAuthRequestDto dto) {
  return m_apiClient->signIn(dto);
}

/**
 * @brief Destroy the AuthRepository object
 */
AuthApiRepository::~AuthApiRepository() = default;
}  // namespace srilakshmikanthanp::clipbirdesk::syncing::wan
