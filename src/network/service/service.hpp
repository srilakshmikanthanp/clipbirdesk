#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

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
