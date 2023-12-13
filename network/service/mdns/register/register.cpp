#if defined(_WIN32) || defined(__APPLE__)   // Only for Linux Operating System that supports bonjour

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "register.hpp"

namespace srilakshmikanthanp::clipbirdesk::network::service::mdns {
/**
 * @brief Callback function for DNSServiceRegister function
 * This is called when the service is registered, Note
 * this is a static function so we need to pass the
 * Register object as context
 */
void Register::publishCallback(
    DNSServiceRef serviceRef,                   // DNSServiceRef
    DNSServiceFlags flags,                      // DNSServiceFlags
    DNSServiceErrorType errorCode,              // DNSServiceErrorType
    const char* name,                           // name of service
    const char* regtype,                        // regtype
    const char* domain,                         // domain
    void* context                               // context
) {
  // convert context to Register object
  auto registerObj = static_cast<Register*>(context);

  // Avoid warning of unused variables
  Q_UNUSED(flags);
  Q_UNUSED(name);
  Q_UNUSED(regtype);
  Q_UNUSED(domain);

  // check for Error
  if (errorCode != kDNSServiceErr_NoError) {
    throw std::runtime_error("DNSServiceRegister failed");
  }

  // emit the signal
  emit registerObj->OnServiceRegistered();
}

/**
 * @brief Process Activated
 */
void Register::processActivated() {
  if (DNSServiceProcessResult(this->m_serviceRef) != kDNSServiceErr_NoError) {
    throw std::runtime_error("DNSServiceProcessResult failed");
  }
}

/**
 * @brief Construct a new Discovery Register object
 *
 * @param parent Parent object
 */
Register::Register(QObject* parent) : QObject(parent) {
  // Empty Constructor just calls the parent constructor
}

/**
 * @brief Register the service
 *
 * @param callback Callback function to be called
 * when service Registered
 */
void Register::registerServiceAsync() {
  // register service for clipbird
  auto errorType = DNSServiceRegister(
      &this->m_serviceRef,                      // DNSServiceRef
      kDNSServiceInterfaceIndexAny,             // interfaceIndex
      0,                                        // flags
      constants::getMDnsServiceName().c_str(),  // server's name
      constants::getMDnsServiceType(),          // service type
      NULL,                                     // domain
      NULL,                                     // host
      htons(this->getPort()),                   // port
      0,                                        // txtLen
      0,                                        // txtRecord
      publishCallback,                          // callback
      this                                      // context
  );

  // check for error
  if (errorType != kDNSServiceErr_NoError) {
    throw std::runtime_error("DNSServiceRegister failed");
  }

  // create socket notifier
  this->m_notifier = new QSocketNotifier(
      DNSServiceRefSockFD(this->m_serviceRef),  // socket
      QSocketNotifier::Read,                    // type
      this                                      // parent
  );

  // connect the socket notifier to slot
  const auto signal = &QSocketNotifier::activated;
  const auto slot = &Register::processActivated;
  connect(this->m_notifier, signal, this, slot);
}

/**
 * @brief Stop the server
 */
void Register::unregisterService() {
  // check for service & notifier
  if (this->m_serviceRef == nullptr || this->m_notifier == nullptr) {
    return;
  }

  // stop the socket notifier
  this->m_notifier->setEnabled(false);

  // delete the socket notifier
  delete this->m_notifier;

  // stop the service
  DNSServiceRefDeallocate(this->m_serviceRef);

  // set the notifier to nullptr
  this->m_notifier = nullptr;

  // set the service ref to nullptr
  this->m_serviceRef = nullptr;
}

/**
 * @brief Destroy the Discovery Register object
 */
Register::~Register() {
  this->unregisterService();
}
}  // namespace srilakshmikanthanp::clipbirdesk::network::service::dnsd
#endif
