// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "browser.hpp"

namespace srilakshmikanthanp::clipbirdesk::network::service::bonjour {
/**
 * @brief Construct a new Discovery Browser object
 *
 * @param parent Parent object
 */
Browser::Browser(QObject* parent) : interfaces::ImDNSBrowser(parent) {

}

/**
 * @brief Starts the discovery client by sending the
 * broadcast message
 *
 * @param interval Interval between each broadcast
 */
void Browser::startBrowsing() {

}

/**
 * @brief Stops the discovery client by sending the
 * broadcast message
 *
 * @param interval Interval between each broadcast
 */
void Browser::stopBrowsing() {

}
}  // namespace srilakshmikanthanp::clipbirdesk::network::service
