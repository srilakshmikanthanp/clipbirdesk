// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "register.hpp"

#ifdef Q_OS_WIN || Q_OS_MAC
#include <dns_sd.h>
#endif

namespace srilakshmikanthanp::clipbirdesk::network::service {
//---------------------  OS & Mac Windows Start  ----------------------//
#ifdef Q_OS_WIN
/**
 * @brief Construct a new Discovery Register object
 *
 * @param parent Parent object
 */
Register::Register(QObject* parent) : interfaces::ImDNSRegister(parent) {}

/**
 * @brief Register the service
 *
 * @param callback Callback function to be called
 * when service Registered
 */
void Register::registerServiceAsync() {

}

/**
 * @brief Stop the server
 */
void Register::unregisterService() {}
#endif
//------------------------ Os Windows & Mac End ----------------------//
}  // namespace srilakshmikanthanp::clipbirdesk::network::service
