// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "discover.hpp"
#include <iostream>

namespace srilakshmikanthanp::clipbirdesk::network::service {
/**
 * @brief Construct a new Discovery Discover object
 *
 * @param parent Parent object
 */
Discover::Discover(QObject* parent) : QObject(parent) {
  this->m_browser = new KDNSSD::ServiceBrowser(constants::getMDnsServiceType().c_str());
  this->m_browser->setParent(this);
}

/// @brief On Service Found
void Discover::OnServiceFound(KDNSSD::RemoteService::Ptr service) {
  // get the name of the device & lambda callback
  const auto myDevice = QString::fromStdString(constants::getMDnsServiceName());
  const auto callback = [&, service](const QHostInfo& info) {
    if (info.error() != QHostInfo::NoError || info.addresses().isEmpty()) {
      emit this->OnErrorOccurred(LOG("Unable to resolve service"));
      return;
    }

    auto host = info.addresses().first();
    auto port = quint16(service->port());
    this->onServerAdded({host, port});
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
void Discover::OnServiceRemoved(KDNSSD::RemoteService::Ptr service) {
  // get the name of the device & lambda callback
  const auto myDevice = QString::fromStdString(constants::getMDnsServiceName());
  const auto callback = [&, service](const QHostInfo& info) {
    if (info.error() != QHostInfo::NoError || info.addresses().isEmpty()) {
      emit this->OnErrorOccurred(LOG("Unable to resolve service"));
      return;
    }

    auto host = info.addresses().first();
    auto port = quint16(service->port());
    this->onServerRemoved({host, port});
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
 * @brief Starts the discovery client by sending the
 * broadcast message
 *
 * @param interval Interval between each broadcast
 */
void Discover::startDiscovery() {
  // connect the browser signal to the slot of this class
  const auto signal_r = &KDNSSD::ServiceBrowser::serviceRemoved;
  const auto slot_r   = &Discover::OnServiceRemoved;
  connect(this->m_browser, signal_r, this, slot_r);

  // connect the browser signal to the slot of this class
  const auto signal_a = &KDNSSD::ServiceBrowser::serviceAdded;
  const auto slot_a   = &Discover::OnServiceFound;
  connect(this->m_browser, signal_a, this, slot_a);

  // start the browser
  this->m_browser->startBrowse();
}
}  // namespace srilakshmikanthanp::clipbirdesk::network::service
