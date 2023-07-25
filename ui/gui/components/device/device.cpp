// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "device.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::components {
/**
 * @brief Called when host is resolved
 *
 * @param info
 */
void Device::onHostResolved(const QHostInfo &info) {
  if (info.error() != QHostInfo::NoError) {
    this->hostName->setText("Unknown");
  }

  this->hostName->setText(info.hostName());
}

/**
 * @brief Construct a new Device object
 * with parent as QWidget
 * @param parent parent object
 */
Device::Device(QWidget *parent) : QWidget(parent) {
  // connect the button signal to this signal
  QObject::connect(actBtn, &QPushButton::clicked, [this]() {
    emit onAction({address, port, action});
  });

  // vertical alignment of the labels as center
  this->hostName->setAlignment(Qt::AlignVCenter);

  // create a layout to align the widgets
  QHBoxLayout *layout = new QHBoxLayout();

  // Ad the host name to left
  layout->addWidget(hostName, 0, Qt::AlignLeft);

  // add the button to right
  layout->addWidget(actBtn, 0, Qt::AlignRight);

  // set the layout
  this->setLayout(layout);

  // set id for styling
  this->setObjectName("host");
}

/**
 * @brief Set the Device
 *
 * @param QPair<QHostAddress, quint16>
 */
void Device::setHost(Device::Value host) {
  // set the address and port
  this->address   = std::get<0>(host);
  this->port      = std::get<1>(host);
  this->action    = std::get<2>(host);

  // set the host name
  this->hostName->setText("...");

  // action text to set
  const auto a = action == Action::Disconnect ? disconnect : connect;

  // set the action
  this->actBtn->setText(a);

  // set the object name to identify
  this->actBtn->setObjectName(a);

  // lookup the host name and change
  QHostInfo::lookupHost(address.toString(), this, &Device::onHostResolved);
}

/**
 * @brief Get the Device
 *
 * @return QPair<QHostAddress, quint16>
 */
Device::Value Device::getHost() const {
  return std::make_tuple(address, port, action);
}
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::components
