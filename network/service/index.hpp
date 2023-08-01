// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#if defined Q_OS_WIN || defined Q_OS_MACOS // for windows and macos using bonjour

#include "network/service/dnsd/browser/browser.hpp"
#include "network/service/dnsd/register/register.hpp"

#define mDNSRegister srilakshmikanthanp::clipbirdesk::network::service::dnsd::Register
#define mDNSBrowser  srilakshmikanthanp::clipbirdesk::network::service::dnsd::Browser

#endif

#ifdef Q_OS_LINUX // for linux using avahi

#include "network/service/avahi/browser/browser.hpp"
#include "network/service/avahi/register/register.hpp"

#define mDNSRegister srilakshmikanthanp::clipbirdesk::network::service::avahi::Register
#define mDNSBrowser  srilakshmikanthanp::clipbirdesk::network::service::avahi::Browser

#endif
