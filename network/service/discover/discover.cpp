// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "discover.hpp"

namespace srilakshmikanthanp::clipbirdesk::network::service {
/**
 * @brief Construct a new Discovery Discover object
 *
 * @param parent Parent object
 */
Discover::Discover(QObject* parent) : QObject(parent) {

}

/**
 * @brief Destroy the Discovery Discover object
 */
Discover::~Discover() {
  this->stopDiscovery();
}

/**
 * @brief Starts the discovery client by sending the
 * broadcast message
 *
 * @param interval Interval between each broadcast
 */
void Discover::startDiscovery() {

}

/**
 * @brief Stops the discovery client
 */
void Discover::stopDiscovery() {

}
}  // namespace srilakshmikanthanp::clipbirdesk::network::discovery
