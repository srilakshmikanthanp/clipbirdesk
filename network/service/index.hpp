// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QObject>

#if defined _WIN32 || defined __APPLE__  // for windows and macos using bonjour

#include "network/service/mdns/browser/browser.hpp"
#include "network/service/mdns/register/register.hpp"

namespace srilakshmikanthanp::clipbirdesk::network::service {
using mdnsRegister = mdns::Register;
using mdnsBrowser  = mdns::Browser;
}  // namespace srilakshmikanthanp::clipbirdesk::network::service

#else  // any other platforms

#Error "Currently We don't Support Any platform other than Apple and windows"

#endif
