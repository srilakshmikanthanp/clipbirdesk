// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "register.hpp"

namespace srilakshmikanthanp::clipbirdesk::network::service {

/**
 * @brief Construct a new Discovery Register object
 *
 * @param parent Parent object
 */
Register::Register(QObject* parent) : QObject(parent) {

}

/**
 * @brief Start the server
 */
void Register::registerService() {

}

/**
 * @brief Stop the server
 */
void Register::unregisterService() {

}
}  // namespace srilakshmikanthanp::clipbirdesk::network::discovery
