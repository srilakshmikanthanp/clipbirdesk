// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "hostview.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::components {
/**
 * @brief Construct a new HostView object
 * with parent as QWidget
 * @param parent parent object
 */
HostView::HostView(QWidget *parent = nullptr) : QWidget(parent) {
  // connect the button signal to this signal
  QObject::connect(actBtn, &core::Button::clicked,
                   [this]() { emit onAction(this->getHost()); });

  // create a layout to align the widgets
  QHBoxLayout *layout = new QHBoxLayout();

  // create a vertical layout
  QVBoxLayout *left = new QVBoxLayout();

  // add the hostname and ip
  left->addWidget(this->hostName);
  left->addWidget(this->ip);

  // create a vertical layout
  QVBoxLayout *right = new QVBoxLayout();

  // add the action to the right layout
  right->addWidget(this->actBtn);

  // add the left and right layout
  layout->addLayout(left);
  layout->addLayout(right);

  // align the left and right layout
  layout->setAlignment(left, Qt::AlignLeft);
  layout->setAlignment(right, Qt::AlignRight);

  this->setStyleSheet(style);
}

/**
 * @brief Set the Host
 *
 * @param QPair<QHostAddress, quint16>
 */
void HostView::setHost(std::tuple<QHostAddress, quint16, Action> host) {
  // set the address and port
  this->address = std::get<0>(host);
  this->port = std::get<1>(host);
  this->action = std::get<2>(host);

  // get the host info
  const auto info = QHostInfo::fromName(address.toString());

  // set the host name
  this->hostName->setText(info.hostName());

  // set the ip
  this->ip->setText(address.toString());

  // action text to set
  const auto a = action == Action::Disconnect ? disconnect : connect;

  // set the action
  this->actBtn->setText(a);
}

/**
 * @brief Get the Host
 *
 * @return QPair<QHostAddress, quint16>
 */
std::tuple<QHostAddress, quint16, HostView::Action> HostView::getHost() const {
  return std::make_tuple(address, port, action);
}
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::components
