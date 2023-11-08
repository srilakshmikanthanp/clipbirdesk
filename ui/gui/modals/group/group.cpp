// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "group.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::modals {
/**
 * @brief Set up the qr code color
 *
 */
void Group::setUpQrCodeColor(Qt::ColorScheme scheme) {
  if (scheme == Qt::ColorScheme::Dark) {
    qrcode->setColor(Qt::white);
  } else {
    qrcode->setColor(Qt::black);
  }
}

/**
 * @brief Construct a new Modal object
 *
 * @param parent
 */
Group::Group(QWidget *parent) : Modal(parent) {
  // create layout
  auto root = new QVBoxLayout(this);

  // set the size
  qrcode->setFixedSize(200, 200);

  // set center alignment
  port->setAlignment(Qt::AlignCenter);

  // add the ip and port input to layout
  root->addWidget(qrcode);
  root->addWidget(port);

  // size policy
  root->setSizeConstraint(QLayout::SetFixedSize);

  // center alignment of qrcode
  root->setAlignment(qrcode, Qt::AlignCenter);

  // set the layout
  this->setLayout(root);

  // initial theme
  setUpQrCodeColor(QGuiApplication::styleHints()->colorScheme());

  // detect the system theme
  const auto styleHints = QGuiApplication::styleHints();
  const auto signal = &QStyleHints::colorSchemeChanged;
  const auto slot = &Group::setUpQrCodeColor;
  QObject::connect(styleHints, signal, this, slot);
}

/**
 * @brief set the qr code
 */
void Group::setQrCode(const QString &qrcode) {
  this->qrcode->setText(qrcode);
  this->update();
}

/**
 * @brief get the qr code
 */
QString Group::getQrCode() const {
  return this->qrcode->getText();
}

/**
 * @brief set the port
 */
void Group::setPort(const QString &port) {
  this->port->setText(port);
  this->update();
}

/**
 * @brief get the port
 */
QString Group::getPort() const {
  return this->port->text();
}
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::modals
