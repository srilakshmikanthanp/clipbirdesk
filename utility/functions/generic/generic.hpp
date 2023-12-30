#pragma once // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

namespace srilakshmikanthanp::clipbirdesk::utility::functions::platform {
/**
 * @brief Set socket as non blocking
 */
#ifdef __linux__ // Only for linux to set the socket as non blocking
int setSocketNonBlocking(int fd);
#endif  // __linux__
}  // namespace srilakshmikanthanp::clipbirdesk::utility::functions::linux
