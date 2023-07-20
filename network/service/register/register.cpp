// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "register.hpp"

namespace srilakshmikanthanp::clipbirdesk::network::service {

/**
 * @brief Construct a new Discovery Register object
 *
 * @param parent Parent object
 */
Register::Register(QObject* parent) : QObject(parent) {
  this->service = new KDNSSD::PublicService();
  this->service->setParent(this);
}

/**
 * @brief Start the server
 */
void Register::registerService() {
  // Set the service name & other details
  this->service->setServiceName(constants::getMDnsServiceName().c_str());
  this->service->setType(constants::getMDnsServiceType().c_str());
  this->service->setPort(this->getPort());
  this->service->setDomain("local");

  // Try to publish the service
  if (!this->service->publish()) {
    throw std::runtime_error("Unable to publish service");
  }
}

/**
 * @brief Stop the server
 */
void Register::unregisterService() {
  this->service->stop();
}
}  // namespace srilakshmikanthanp::clipbirdesk::network::discovery
