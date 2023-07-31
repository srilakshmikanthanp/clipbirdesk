// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "browser.hpp"

namespace srilakshmikanthanp::clipbirdesk::network::service::dnsd {
/**
 * @brief Callback function for DNSServiceBrowse function
 */
void Browser::browseCallback(
    DNSServiceRef serviceRef,       // DNSServiceRef
    DNSServiceFlags flags,          // DNSServiceFlags
    uint32_t interfaceIndex,        // InterfaceIndex
    DNSServiceErrorType errorCode,  // DNSServiceErrorType
    const char* serviceName,        // serviceName
    const char* regtype,            // regtype
    const char* domain,             // domain
    void* context                   // context
) {
  // convert context to Register object
  auto browserObj = static_cast<Browser*>(context);

  // Avoid warning of unused variables
  Q_UNUSED(interfaceIndex);
  Q_UNUSED(domain);

  // check for Error
  if (errorCode != kDNSServiceErr_NoError) {
    throw std::runtime_error("DNSServiceBrowse failed");
  }

  // service name
  auto name = QString::fromUtf8(serviceName);
  auto type = QString::fromUtf8(regtype);

  // check for flags for add or remove
  if (flags & kDNSServiceFlagsAdd) {

  } else {

  }
}

/**
 * @brief Process Activated
 */
void Browser::processActivated() {
  if (DNSServiceProcessResult(this->m_serviceRef) != kDNSServiceErr_NoError) {
    throw std::runtime_error("DNSServiceProcessResult failed");
  }
}

/**
 * @brief Construct a new Discovery Browser object
 *
 * @param parent Parent object
 */
Browser::Browser(QObject* parent) : interfaces::ImDNSBrowser(parent) {}

/**
 * @brief Starts the discovery client by sending the
 * broadcast message
 *
 * @param interval Interval between each broadcast
 */
void Browser::startBrowsing() {
  // Start to browse for the service
  auto errorType = DNSServiceBrowse(
      &this->m_serviceRef,                      // DNSServiceRef
      0,                                        // DNSServiceFlags
      kDNSServiceInterfaceIndexAny,             // InterfaceIndex
      constants::getMDnsServiceType().c_str(),  // regtype
      NULL,                                     // domain
      browseCallback,                           // callback
      this                                      // context
  );

  // check for error
  if (errorType != kDNSServiceErr_NoError) {
    throw std::runtime_error("DNSServiceBrowse failed");
  }

  // create socket notifier
  this->m_notifier = new QSocketNotifier(
      DNSServiceRefSockFD(this->m_serviceRef),  // socket
      QSocketNotifier::Read,                    // type
      this                                      // parent
  );

  // connect the socket notifier to slot
  const auto signal = &QSocketNotifier::activated;
  const auto slot   = &Browser::processActivated;
  connect(this->m_notifier, signal, this, slot);
}

/**
 * @brief Stops the discovery client by sending the
 * broadcast message
 *
 * @param interval Interval between each broadcast
 */
void Browser::stopBrowsing() {
  // check for service ref & notifier
  if (this->m_serviceRef == nullptr || this->m_notifier == nullptr) {
    return;
  }

  // stop the socket notifier
  this->m_notifier->setEnabled(false);

  // stop the service
  DNSServiceRefDeallocate(this->m_serviceRef);

  // delete the notifier
  delete this->m_notifier;

  // set the notifier to nullptr
  this->m_notifier = nullptr;

  // set the service ref to nullptr
  this->m_serviceRef = nullptr;
}
}  // namespace srilakshmikanthanp::clipbirdesk::network::service::dnsd
