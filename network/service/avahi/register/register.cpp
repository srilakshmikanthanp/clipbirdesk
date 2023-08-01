// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "register.hpp"

namespace srilakshmikanthanp::clipbirdesk::network::service::avahi {
/**
 * @brief Construct a new Discovery Register object
 *
 * @param parent Parent object
 */
Register::Register(QObject* parent) : interfaces::ImDNSRegister(parent) {
  // create the service
  this->service = new KDNSSD::PublicService();
  this->service->setParent(this);

  // connect the signals to the slots
  const auto signal_r = &KDNSSD::PublicService::published;
  const auto slot_r   = &Register::OnServiceRegistered;
  connect(this->service, signal_r, this, slot_r);
}

/**
 * @brief Register the service
 *
 * @param callback Callback function to be called
 * when service Registered
 */
void Register::registerServiceAsync() {
  // Set the service name & other details
  this->service->setServiceName(constants::getMDnsServiceName().c_str());
  this->service->setType(constants::getMDnsServiceType().c_str());
  this->service->setPort(this->getPort());

  // publish the service Asynchronously
  this->service->publishAsync();
}

/**
 * @brief Stop the server
 */
void Register::unregisterService() {
  this->service->stop();
}
//------------------------ Os Windows & Mac End ----------------------//
}  // namespace srilakshmikanthanp::clipbirdesk::network::service
