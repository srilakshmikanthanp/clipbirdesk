#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// for windows and macos using bonjour & avahi compact
#if defined _WIN32 || defined __APPLE__ || __linux__

#include "dnssd_register/dnssd_register.hpp"
#include "dnssd_browser/dnssd_browser.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::network {
using MdnsRegister = network::DnssdRegister;
using MdnsBrowser  = network::DnssdBrowser;
}  // namespace srilakshmikanthanp::clipbirdesk::network::service

#else  // any other platforms

#Error "This platform is Not Supported for mDns"

#endif
