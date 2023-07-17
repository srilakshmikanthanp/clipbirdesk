#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QHostAddress>
#include <QPair>
#include <QTtypes>

#include <functional>

namespace srilakshmikanthanp::clipbirdesk::types::callback {
/// @brief Authenticator for server to authenticate the client
using Authenticator = std::function<bool(QPair<QHostAddress, quint16>)>;
} // namespace srilakshmikanthanp::clipbirdesk::types::callback
