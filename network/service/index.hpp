// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QObject>

#if defined _WIN32 || defined __APPLE__ // for windows and macos using bonjour

#include "network/service/dnsd/browser/browser.hpp"
#include "network/service/dnsd/register/register.hpp"

#define mDNSRegister srilakshmikanthanp::clipbirdesk::network::service::dnsd::Register
#define mDNSBrowser  srilakshmikanthanp::clipbirdesk::network::service::dnsd::Browser

#else // any other platforms

#Error "Currently We don't Support Any platform other than Apple and windows"

#endif
