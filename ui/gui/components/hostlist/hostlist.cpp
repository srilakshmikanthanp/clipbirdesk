// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "hostlist.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::content {
HostList::HostList(QWidget* parent) : QWidget(parent) {
  // set alignment from start and center
  verticalLayout->setAlignment(Qt::AlignTop);

  // create a label
  QLabel *label = new QLabel("No Hosts");

  // set alignment as center
  label->setAlignment(Qt::AlignCenter);

  // set the style sheet
  label->setStyleSheet("QLabel { color : gray; }");

  // add the layout to the stack layout
  this->stackLayout->addWidget(label);

  // set the alignment
  this->stackLayout->setAlignment(label, Qt::AlignCenter);

  // add the layout to the stack layout
  this->stackLayout->addWidget(verticalWidget);

  // set the layout
  this->setLayout(this->stackLayout);
}

/**
 * @brief Set the Hosts to the list
 */
void HostList::setHosts(QList<components::Host::Value> hosts) {
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
  this->repaint();
}

/**
 * @brief Get the All Hosts from the list
 */
QList<components::Host::Value> HostList::getHosts() {
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
void HostList::removeHosts() {
  // iterate over the layout
  QLayoutItem* item;
  while ((item = verticalLayout->takeAt(0)) != nullptr) {
    verticalLayout->removeItem(item);
    delete item->widget();
    delete item;
  }

  // Redraw the widget
  this->repaint();
}

/**
 * @brief Add Device to the list
 */
void HostList::addHost(components::Host::Value host) {
  // create a new host view
  auto hostView = new components::Host();

  // set the host
  hostView->setHost(host);

  // connect the host view signal to this signal
  const auto signal = &components::Host::onAction;
  const auto slot   = [&](auto h) { emit onAction(h); };
  QObject::connect(hostView, signal, slot);

  // add the host view to the layout
  verticalLayout->addWidget(hostView);

  // Redraw the widget
  this->repaint();
}

/**
 * @brief Remove All the Device as same as the given host
 */
void HostList::removeHost(components::Host::Value host) {
  // iterate over the layout and remove all the host as same as the given host
  for (int i = 0; i < verticalLayout->count(); i++) {
    auto hostView = dynamic_cast<components::Host*>(verticalLayout->itemAt(i)->widget());
    if (hostView->getHost() == host) {
      verticalLayout->removeWidget(hostView);
      delete hostView;
    }
  }

  // Redraw the widget
  this->repaint();
}

/**
 * @brief Override the showEvent
 */
void HostList::paintEvent(QPaintEvent *event) {
  // if the vertical layout is empty then add a label
  if (verticalLayout->count() == 0) {
    this->stackLayout->setCurrentIndex(0);
  } else {
    this->stackLayout->setCurrentIndex(1);
  }

  // call the base class
  QWidget::paintEvent(event);
}
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::window
