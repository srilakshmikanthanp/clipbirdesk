// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "hostslist.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::components {
HostsList::HostsList(QWidget *parent) : QWidget(parent) {
  // set the root layout for this widget
  this->setLayout(verticalLayout);

  // set the style
  this->setStyleSheet(style);
}

/**
 * @brief Set the Hosts to the list
 */
void HostsList::setHosts(QList<Host::Value> hosts) {
  // remove & add all the hosts
  removeAllHosts();
  for (auto host : hosts) addHost(host);
}

/**
 * @brief Get the All Hosts from the list
 */
QList<components::Host::Value> HostsList::getAllHosts() {
// cast the widget to host view
#define CAST(x) dynamic_cast<components::Host *>(x)

  // create a list of hosts
  QList<Host::Value> hosts;

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
 * @brief Add Host to the list
 */
void HostsList::addHost(Host::Value host) {
  // create a new host view
  auto hostView = new components::Host(this);

  // set the host
  hostView->setHost(host);

  // connect the host view signal to this signal
  QObject::connect(hostView, &Host::onAction, [&](auto h) { emit onAction(h); });

  // add the host view to the layout
  verticalLayout->addWidget(hostView);
}

/**
 * @brief Remove a Host from the list
 */
void HostsList::removeHost(Host::Value host) {
// cast the widget to host view
#define CAST(x) dynamic_cast<components::Host *>(x)

  // iterate over the layout
  for (int i = 0; i < verticalLayout->count(); i++) {
    auto currItem = verticalLayout->itemAt(i)->widget();
    auto hostView = CAST(currItem);
    if (hostView->getHost() != host) continue;
    verticalLayout->removeWidget(hostView);
  }

// undefine the cast macro
#undef CAST
}

/**
 * @brief Remove all Hosts from the list
 */
void HostsList::removeAllHosts() {
// cast the widget to host view
#define CAST(x) dynamic_cast<components::Host *>(x)

  // iterate over the layout
  for (int i = 0; i < verticalLayout->count(); i++) {
    auto currItem = verticalLayout->itemAt(i)->widget();
    auto hostView = CAST(currItem);
    verticalLayout->removeWidget(hostView);
  }

// undefine the cast macro
#undef CAST
}
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::components
