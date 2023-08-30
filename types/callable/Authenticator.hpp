#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "types/device/device.hpp"

namespace srilakshmikanthanp::clipbirdesk::types::callable {
/**
 * @brief Authenticator that takes type Device and return true or false
 */
using Authenticator = std::function<bool(const types::device::Device&)>;
}
