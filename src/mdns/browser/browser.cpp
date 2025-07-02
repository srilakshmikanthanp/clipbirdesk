#if defined(_WIN32) || defined(__APPLE__) || defined(__linux__)  // Only for Operating System that supports bonjour or compatible to it

#include "browser.hpp"

namespace srilakshmikanthanp::clipbirdesk::network::service::mdns {
/**
 * @brief Callback for QHostInfo Address Resolve Function
 *
 * @param isAdded
 * @param const QHostInfo& info
 */
void Browser::onHostResolved(quint16 port, QString srvName, const QHostInfo& info) {
  // check for error
  if (info.error() != QHostInfo::NoError || info.addresses().isEmpty()) {
    qWarning() << "Unable to resolve service"; return;
  }

  // remove the service type from service name
  srvName.remove("." + QString::fromStdString(constants::getMDnsServiceType() + std::string(".local.")));

  // Replace the \xxx ascii code to char
  QRegularExpression regex("\\\\([0-9]{3})");
  QString result;
  QRegularExpressionMatchIterator it = regex.globalMatch(srvName);

  int lastIndex                      = 0;
  while (it.hasNext()) {
    QRegularExpressionMatch match = it.next();
    int start                     = match.capturedStart(0);
    int end                       = match.capturedEnd(0);
    QString str                   = match.captured(1);
    bool ok;
    ushort asciiCode = str.toUShort(&ok, 10);

    if (ok) {
      result    += srvName.mid(lastIndex, start - lastIndex);
      result    += QChar(asciiCode);
      lastIndex  = end;
    }
  }

  if (result.isEmpty()) {
    qWarning() << "Service name is empty after removing service type"; return;
  }

  if (serviceMap.contains(result)) {
    qWarning() << "Service already exists in the map, ignoring"; return;
  }

  // get the ip address
  auto ip = info.addresses().first();

  // add to map
  this->serviceMap[result] = {ip, port};

  // emit the signal
  emit onServiceAdded({ip, port, result});
}

/**
 * @brief Callback function for DNSServiceBrowse function
 */
void Browser::browseCallback(
    DNSServiceRef serviceRef,                        // DNSServiceRef
    DNSServiceFlags flags,                           // DNSServiceFlags
    uint32_t interfaceIndex,                         // InterfaceIndex
    DNSServiceErrorType errorCode,                   // DNSServiceErrorType
    const char* serviceName,                         // serviceName
    const char* regtype,                             // regtype
    const char* domain,                              // domain
    void* context                                    // context
) {
  // if the service name is same as our service name
  if (constants::getMDnsServiceName() == std::string(serviceName)) {
    return;
  }

  // convert context to Register object
  auto browserObj = static_cast<Browser*>(context);

  // Avoid warning of unused variables
  Q_UNUSED(interfaceIndex);
  Q_UNUSED(domain);

  // check for Error
  if (errorCode != kDNSServiceErr_NoError) {
    qWarning() << "DNSServiceBrowse failed"; return;
  }

  // create context with service name and this
  auto contextObj = new std::pair(new std::string(serviceName), browserObj);

  // if removed
  if (!(flags & kDNSServiceFlagsAdd)) {
    return browserObj->removeCallback(QString(serviceName));
  }

  // resolve the service
  auto errorType = DNSServiceResolve(
      &browserObj->m_res_ref,                        // DNSServiceRef
      0,                                             // DNSServiceFlags
      interfaceIndex,                                // InterfaceIndex
      serviceName,                                   // serviceName
      regtype,                                       // regtype
      domain,                                        // domain
      addedCallback,                                 // callback
      browserObj                                     // context
  );

  // check for error
  if (errorType != kDNSServiceErr_NoError) {
    qWarning() << "DNSServiceResolve failed"; return;
  }

  // set as non blocking
#ifdef __linux__
  utility::functions::platform::setSocketNonBlocking(DNSServiceRefSockFD(browserObj->m_res_ref));
#endif

  // create socket notifier
  browserObj->m_res_notify = new QSocketNotifier(
      DNSServiceRefSockFD(browserObj->m_res_ref),    // socket
      QSocketNotifier::Read,                         // type
      browserObj                                     // parent
  );

  // process resolve socket
  const auto processResSock = [=] {
    if (DNSServiceProcessResult(browserObj->m_res_ref) != kDNSServiceErr_NoError) {
      // No action needed, even logging cause huge log spam
    }
  };

  // connect the socket notifier to slot
  connect(
    browserObj->m_res_notify,  &QSocketNotifier::activated,
    processResSock
  );
}

/**
 * @brief Callback function for DNSServiceResolve function
 */
void Browser::addedCallback(
    DNSServiceRef serviceRef,                        // DNSServiceRef
    DNSServiceFlags flags,                           // DNSServiceFlags
    uint32_t interfaceIndex,                         // InterfaceIndex
    DNSServiceErrorType errorCode,                   // DNSServiceErrorType
    const char* fullname,                            // fullname
    const char* hosttarget,                          // hosttarget
    uint16_t port,                                   // port
    uint16_t txtLen,                                 // txtLen
    const unsigned char* txtRecord,                  // txtRecord
    void* context                                    // context
) {
  // convert context to Register object
  auto browserObj = static_cast<Browser*>(context);

  // Avoid warning of unused variables
  Q_UNUSED(interfaceIndex);
  Q_UNUSED(txtLen);
  Q_UNUSED(txtRecord);

  // service name
  auto serviceName = QString::fromUtf8(fullname);

  // check for Error
  if (errorCode != kDNSServiceErr_NoError) {
    qWarning() << "DNSServiceResolve failed"; return;
  }

  // service name
  auto hostname   = QString::fromUtf8(hosttarget);

  // bind the iAdded and port
  auto callback = [=](const QHostInfo& hostInfo) {
    browserObj->onHostResolved(ntohs(port), serviceName, hostInfo);
  };

  // Resolve the ip address from hostname
  QHostInfo::lookupHost(hostname, callback);
}

/**
 * @brief Callback function for DNSServiceResolve function
 */
void Browser::removeCallback(QString serviceName) {
  // remove the service name from service map
  auto service = this->serviceMap.find(serviceName);
  auto device = types::Device();

  // if not found
  if (service == this->serviceMap.end()) {
    return;
  }

  // get the QHostAddress
  device.port = service->second;
  device.name = serviceName;
  device.ip = service->first;

  // emit the signal
  emit onServiceRemoved(device);

  // remove the service from map
  this->serviceMap.erase(service);
}

/**
 * @brief Construct a new Discovery Browser object
 *
 * @param parent Parent object
 */
Browser::Browser(QObject* parent) : QObject(parent) {
  // Empty Constructor just calls the parent constructor
}

/**
 * @brief Starts the discovery client by sending the
 * broadcast message
 *
 * @param interval Interval between each broadcast
 */
void Browser::startBrowsing() {
  // Start to browse for the service
  auto errorType = DNSServiceBrowse(
      &this->m_browse_ref,                           // DNSServiceRef
      0,                                             // DNSServiceFlags
      kDNSServiceInterfaceIndexAny,                  // InterfaceIndex
      constants::getMDnsServiceType(),               // regtype
      NULL,                                          // domain
      browseCallback,                                // callback
      this                                           // context
  );

  // check for error
  if (errorType != kDNSServiceErr_NoError) {
    qWarning() << "DNSServiceBrowse failed"; return;
  }

  // create socket notifier
  this->m_browse_notify = new QSocketNotifier(
      DNSServiceRefSockFD(this->m_browse_ref),       // socket
      QSocketNotifier::Read,                         // type
      this                                           // parent
  );

  // process register socket
  const auto processBrowseSock = [=] {
    if (DNSServiceProcessResult(this->m_browse_ref) != kDNSServiceErr_NoError) {
      qWarning() << "DNSServiceProcessResult failed";
    }
  };

  // connect the socket notifier to slot
  connect(
    this->m_browse_notify, &QSocketNotifier::activated,
    processBrowseSock
  );
}

/**
 * @brief Stops the mDNS Browser by stopping the
 * socket notifier and service
 */
void Browser::stopBrowsing() {
  const auto deleter = [=](auto& serviceRef, auto*& notifier) {
    // check for service ref & notifier
    if (serviceRef == nullptr || notifier == nullptr) {
      return;
    }

    // stop the socket notifier
    notifier->setEnabled(false);

    // stop the service
    DNSServiceRefDeallocate(serviceRef);

    // delete the notifier
    delete notifier;

    // set the service ref to nullptr
    serviceRef = nullptr;

    // set the notifier to nullptr
    notifier = nullptr;
  };

  // delete the service ref & socket notifier for resolve
  deleter(this->m_res_ref, this->m_res_notify);

  // delete the service ref & socket notifier for browse
  deleter(this->m_browse_ref, this->m_browse_notify);
}

/**
 * @brief Destroy the Discovery Browser object
 */
Browser::~Browser() {
  this->stopBrowsing();
}
}  // namespace srilakshmikanthanp::clipbirdesk::network::service::dnsd
#endif
