#include "auth_repository.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::wan {
/**
 * @brief Construct a new AuthRepository object
 *
 * @param parent Parent object
 */
AuthRepository::AuthRepository(QObject* parent) : QObject(parent) {}

/**
 * @brief Destroy the AuthRepository object
 */
AuthRepository::~AuthRepository() = default;
}  // namespace srilakshmikanthanp::clipbirdesk::syncing::wan
