#ifdef __linux__  // Only for Linux Operating System that supports avahi & kdnssd

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "browser.hpp"

namespace srilakshmikanthanp::clipbirdesk::network::service::avahi {
/// @brief On Service Found
void Browser::OnServiceFound(KDNSSD::RemoteService::Ptr service) {
  // get the name of the device & lambda callback
  const auto myDevice = QString::fromStdString(constants::getMDnsServiceName());
  const auto callback = [&, service](const QHostInfo& info) {
    if (info.error() != QHostInfo::NoError || info.addresses().isEmpty()) {
      emit this->OnErrorOccurred(LOG("Unable to resolve service"));
      return;
    }

    auto host = info.addresses().first();
    auto port = quint16(service->port());
    this->onServiceAdded({host, port});
  };

  // resolve the service
  if (!service->resolve()) {
    emit this->OnErrorOccurred(LOG("Unable to resolve service"));
    return;
  }

  // check if the service is mine
  if (service->serviceName() == myDevice) {
    return;
  }

  // lookup the host
  QHostInfo::lookupHost(service->hostName(), callback);
}

/// @brief On Service Removed
void Browser::OnServiceRemoved(KDNSSD::RemoteService::Ptr service) {
  // get the name of the device & lambda callback
  const auto myDevice = QString::fromStdString(constants::getMDnsServiceName());
  const auto callback = [&, service](const QHostInfo& info) {
    if (info.error() != QHostInfo::NoError || info.addresses().isEmpty()) {
      emit this->OnErrorOccurred(LOG("Unable to resolve service"));
      return;
    }

    auto host = info.addresses().first();
    auto port = quint16(service->port());
    this->onServiceRemoved({host, port});
  };

  // resolve the service
  if (!service->resolve()) {
    emit this->OnErrorOccurred(LOG("Unable to resolve service"));
    return;
  }

  // check if the service is mine
  if (service->serviceName() == myDevice) {
    return;
  }

  // lookup the host
  QHostInfo::lookupHost(service->hostName(), callback);
}

/**
 * @brief Construct a new Discovery Browser object
 *
 * @param parent Parent object
 */
Browser::Browser(QObject* parent) : interfaces::ImDNSBrowser(parent) {
  // Empty Constructor just calls the parent constructor
}

/**
 * @brief Starts the discovery client by sending the
 * broadcast message
 *
 * @param interval Interval between each broadcast
 */
void Browser::startBrowsing() {
  // create the browser object and set the parent
  this->m_browser = new KDNSSD::ServiceBrowser(constants::getMDnsServiceType().c_str());
  this->m_browser->setParent(this);

  // connect the browser signal to the slot of this class
  const auto signal_r = &KDNSSD::ServiceBrowser::serviceRemoved;
  const auto slot_r   = &Browser::OnServiceRemoved;
  connect(this->m_browser, signal_r, this, slot_r);

  // connect the browser signal to the slot of this class
  const auto signal_a = &KDNSSD::ServiceBrowser::serviceAdded;
  const auto slot_a   = &Browser::OnServiceFound;
  connect(this->m_browser, signal_a, this, slot_a);

  // start the browser
  this->m_browser->startBrowse();
}

/**
 * @brief Stops the discovery client by sending the
 * broadcast message
 *
 * @param interval Interval between each broadcast
 */
void Browser::stopBrowsing() {
  delete this->m_browser;
}
}  // namespace srilakshmikanthanp::clipbirdesk::network::service
#endif  // Q_OS_LINUX
