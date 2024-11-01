#pragma once // Header guard see https://en.wikipedia.org/wiki/Include_guard

namespace srilakshmikanthanp::clipbirdesk::utility::functions::platform {
/**
 * @brief Set socket as non blocking
 */
#ifdef __linux__ // Only for linux to set the socket as non blocking
int setSocketNonBlocking(int fd);
#endif  // __linux__
}  // namespace srilakshmikanthanp::clipbirdesk::utility::functions::linux
