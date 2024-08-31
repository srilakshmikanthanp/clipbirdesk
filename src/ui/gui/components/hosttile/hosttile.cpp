// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "hosttile.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::components {
/**
 * @brief Construct a new Device object
 * with parent as QWidget
 * @param parent parent object
 */
HostTile::HostTile(QWidget *parent) : QWidget(parent) {
  // connect the button signal to this signal
  QObject::connect(actBtn, &QPushButton::clicked, [this]() {
    emit onAction({device, action});
  });

  // vertical alignment of the labels as center
  this->hostName->setAlignment(Qt::AlignVCenter);

  // create a layout to align the widgets
  QHBoxLayout *layout = new QHBoxLayout();

  // Ad the host name to left
  layout->addWidget(hostName, 0, Qt::AlignLeft);

  // add the button to right
  layout->addWidget(actBtn, 0, Qt::AlignRight);

  // ActBtn cursor as pointer
  actBtn->setCursor(Qt::PointingHandCursor);

  // set the layout
  this->setLayout(layout);

  // set id for styling
  this->setObjectName("Device");
}

/**
 * @brief Function used to set up all text in the label, etc..
 */
void HostTile::setUpLanguage() {
  // Nothing to do
}

/**
 * @brief Set the Device
 */
void HostTile::setHost(HostTile::Value host) {
  // set the address and port
  this->device  = std::get<0>(host);
  this->action  = std::get<1>(host);

  // set the host name
  this->hostName->setText(device.name);

  // action text to set
  const auto a = action == Action::Disconnect ? disconnect : connect;

  // set the action
  this->actBtn->setText(a);

  // set the object name to identify
  this->actBtn->setObjectName(a);
}

/**
 * @brief Get the Device
 */
HostTile::Value HostTile::getHost() const {
  return std::make_tuple(device, action);
}

/**
 * @brief Override paint for custom style
 */
void HostTile::paintEvent(QPaintEvent *event) {
  QStyleOption opt;
  opt.initFrom(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

/**
 * @brief change event
 */
void HostTile::changeEvent(QEvent *event) {
  if (event->type() == QEvent::LanguageChange) {
    this->setUpLanguage();
  }

  QWidget::changeEvent(event);
}
} // namespace srilakshmikanthanp::clipbirdesk::ui::gui::components
