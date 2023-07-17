#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QInputDialog>
#include <QPair>
#include <QHostAddress>

namespace srilakshmikanthanp::clipbirdesk::ui::gui {
/**
 * @brief Authenticator for server to authenticate the client
 * when client connects to server, This Authenticator just
 * shows a info of the client and asks for permission to
 * connect or not.
 *
 * @param host <QHostAddress, quint16>
 *
 * @return true if user accepts the connection
 * @return false if user rejects the connection
 */
bool authenticator(QPair<QHostAddress, quint16> host);
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::utility
