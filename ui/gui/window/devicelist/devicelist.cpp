// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "devicelist.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::window {
DeviceList::DeviceList(QWidget* parent) : QWidget(parent) {
  // set alignment from start
  verticalLayout->setAlignment(Qt::AlignTop);

  // set alignment from start
  labelLayout->setAlignment(Qt::AlignTop);

  // create a label
  QLabel *label = new QLabel("No Hosts");

  // set alignment as center
  label->setAlignment(Qt::AlignCenter);

  // add label to labellayout
  labelLayout->addWidget(label);

  // set object name
  label->setObjectName("no-host-label");

  // set the object name
  this->setObjectName("devicelist");

  // A Widget for the label Layout
  QWidget* labelWidget = new QWidget();

  // set the layout
  labelWidget->setLayout(labelLayout);

  // add the layout to the stack layout
  this->stackLayout->addWidget(labelWidget);

  // A Widget for the vertical Layout
  QWidget* verticalWidget = new QWidget();

  // set the layout
  verticalWidget->setLayout(verticalLayout);

  // add the layout to the stack layout
  this->stackLayout->addWidget(verticalWidget);

  // set the layout
  this->setLayout(this->stackLayout);
}

/**
 * @brief Set the Hosts to the list
 */
void DeviceList::setHosts(QList<components::Device::Value> hosts) {
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
QList<components::Device::Value> DeviceList::getHosts() {
  // create a list of hosts
  QList<components::Device::Value> hosts;

  // iterate over the layout
  for (int i = 0; i < verticalLayout->count(); i++) {
    hosts.append(dynamic_cast<components::Device*>(verticalLayout->itemAt(i)->widget())->getHost());
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
  this->repaint();
}

/**
 * @brief Add Device to the list
 */
void DeviceList::addHost(components::Device::Value host) {
  // create a new host view
  auto hostView = new components::Device();

  // set the host
  hostView->setHost(host);

  // connect the host view signal to this signal
  const auto signal = &components::Device::onAction;
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
void DeviceList::removeHost(components::Device::Value host) {
  // iterate over the layout and remove all the host as same as the given host
  for (int i = 0; i < verticalLayout->count(); i++) {
    auto hostView = dynamic_cast<components::Device*>(verticalLayout->itemAt(i)->widget());
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
void DeviceList::paintEvent(QPaintEvent *event) {
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
