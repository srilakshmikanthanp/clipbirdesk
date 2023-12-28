#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

// for windows and macos using bonjour & avahi compact
#if defined _WIN32 || defined __APPLE__ || __linux__

#include "network/service/mdns/browser/browser.hpp"
#include "network/service/mdns/register/register.hpp"

namespace srilakshmikanthanp::clipbirdesk::network::service {
using mdnsRegister = mdns::Register;
using mdnsBrowser  = mdns::Browser;
}  // namespace srilakshmikanthanp::clipbirdesk::network::service

#else  // any other platforms

#Error "This platform is Not Supported for mDns"

#endif
