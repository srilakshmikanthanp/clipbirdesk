#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QList>
#include <QObject>
#include <QSslConfiguration>
#include <QSslServer>

#include "discovery/DiscoveryClient.hpp"
#include "types/enums/enums.hpp"
#include "utility/functions/ipconv.hpp"

namespace srilakshmikanthanp::clipbirdesk::network::syncing {
/**
 * @brief Syncing client that syncs the clipboard data between
 * client and server
 */
class SyncingClient : public QObject {};
}  // namespace srilakshmikanthanp::clipbirdesk::network::syncing
