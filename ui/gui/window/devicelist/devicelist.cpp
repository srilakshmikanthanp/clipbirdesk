// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "devicelist.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::window {
DeviceList::DeviceList(QWidget* parent) : QWidget(parent) {
  // set the root layout for this widget
  this->setLayout(verticalLayout);

  for ( int i = 0 ; i < 5 ; i++ ) {
    this->addHost({QHostAddress::LocalHost, 45, Action::Disconnect});
  }

  // set the style
  this->setStyleSheet(style);
}

/**
 * @brief Set the Hosts to the list
 */
void DeviceList::setHosts(QList<components::Host::Value> hosts) {
  // remove & add all the hosts
  removeHosts();
  for (auto host : hosts) addHost(host);
}

/**
 * @brief Get the All Hosts from the list
 */
QList<components::Host::Value> DeviceList::getHosts() {
  // cast the widget to host view
  #define CAST(x) dynamic_cast<components::Host*>(x)

  // create a list of hosts
  QList<components::Host::Value> hosts;

  // iterate over the layout
  for (int i = 0; i < verticalLayout->count(); i++) {
    auto currItem = verticalLayout->itemAt(i)->widget();
    auto hostView = CAST(currItem);
    hosts.append(hostView->getHost());
  }

  // return the list of hosts
  return hosts;

  // undefine the cast macro
  #undef CAST
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

  // slot
  const auto slot = [&]() { emit onAction(host); };

  // connect the host view signal to this signal
  QObject::connect(hostView, &components::Host::onAction, slot);

  // add the host view to the layout
  verticalLayout->addWidget(hostView);

  // Redraw the widget
  this->update();
}

/**
 * @brief Remove a Host from the list
 */
void DeviceList::removeHost(components::Host::Value host) {
  // cast the widget to host view
  #define CAST(x) dynamic_cast<components::Host*>(x)

  // iterate over the layout
  for (int i = 0; i < verticalLayout->count(); i++) {
    auto currItem = verticalLayout->itemAt(i);
    auto hostView = CAST(currItem->widget());
    if (hostView->getHost() == host) {
      verticalLayout->removeItem(currItem);
      delete currItem->widget();
      delete currItem;
    }
  }

  // undefine the cast macro
  #undef CAST
}
} // namespace srilakshmikanthanp::clipbirdesk::ui::gui::window
