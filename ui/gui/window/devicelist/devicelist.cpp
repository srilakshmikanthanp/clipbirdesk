// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "devicelist.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::window {
DeviceList::DeviceList(QWidget* parent) : QWidget(parent) {
  // set the root layout for this widget
  this->setLayout(verticalLayout);

  // add some dummy widgets
  for (int i = 0; i < 5; i++) {
    addHost({QHostAddress::LocalHost, 8080, Action::Connect});
  }

  // set the style
  this->setStyleSheet(style);
}

/**
 * @brief Set the Hosts to the list
 */
void DeviceList::setHosts(QList<components::Host::Value> hosts) {
  // get All Hosts from the list and compare with the given list
  auto currHosts = getHosts();

  // remove the hosts which are not in the list
  for (auto host : currHosts) {
    if (!hosts.contains(host)) {
      removeHost(host);
    }
  }

  // add the hosts which are not in the list
  for (auto host : hosts) {
    if (!currHosts.contains(host)) {
      addHost(host);
    }
  }

  // Redraw the widget
  this->update();
}

/**
 * @brief Get the All Hosts from the list
 */
QList<components::Host::Value> DeviceList::getHosts() {
  // create a list of hosts
  QList<components::Host::Value> hosts;

  // iterate over the layout
  for (int i = 0; i < verticalLayout->count(); i++) {
    hosts.append(dynamic_cast<components::Host*>(verticalLayout->itemAt(i)->widget())->getHost());
  }

  // return the list of hosts
  return hosts;
}

/**
 * @brief Remove all Hosts from the list
 */
void DeviceList::removeHosts() {
  // iterate over the layout
  QLayoutItem* item;
  while ((item = verticalLayout->takeAt(0)) != nullptr) {
    verticalLayout->removeItem(item);
    delete item->widget();
    delete item;
  }

  // Redraw the widget
  this->update();
}

/**
 * @brief Add Host to the list
 */
void DeviceList::addHost(components::Host::Value host) {
  // create a new host view
  auto hostView = new components::Host();

  // set the host
  hostView->setHost(host);

  // connect the host view signal to this signal
  const auto signal = &components::Host::onAction;
  const auto slot   = [&]() { emit onAction(host); };
  QObject::connect(hostView, signal, slot);

  // add the host view to the layout
  verticalLayout->addWidget(hostView);

  // Redraw the widget
  this->update();
}

/**
 * @brief Remove All the Host as same as the given host
 */
void DeviceList::removeHost(components::Host::Value host) {
  // iterate over the layout and remove all the host as same as the given host
  for (int i = 0; i < verticalLayout->count(); i++) {
    auto hostView = dynamic_cast<components::Host*>(verticalLayout->itemAt(i)->widget());
    if (hostView->getHost() == host) {
      verticalLayout->removeWidget(hostView);
      delete hostView;
    }
  }
}
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::window
