// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "qrcode.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::modals {
/**
 * @brief Construct a new Modal object
 *
 * @param parent
 */
QrCode::QrCode(QWidget *parent) : Modal(parent) {
  // create layout
  auto root = new QVBoxLayout();

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
  if (QGuiApplication::styleHints()->colorScheme() == Qt::ColorScheme::Dark) {
    qrcode->setColor(Qt::white);
  } else {
    qrcode->setColor(Qt::black);
  }

  // detect the system theme
  const auto signal = &QStyleHints::colorSchemeChanged;
  QObject::connect(QGuiApplication::styleHints(), signal, [=](auto scheme) {
    if (scheme == Qt::ColorScheme::Dark) {
      qrcode->setColor(Qt::white);
    } else {
      qrcode->setColor(Qt::black);
    }
  });
}

/**
 * @brief set the qr code
 */
void QrCode::setQrCode(const QString &qrcode) {
  this->qrcode->setText(qrcode);
  this->update();
}

/**
 * @brief get the qr code
 */
QString QrCode::getQrCode() const {
  return this->qrcode->getText();
}

/**
 * @brief set the port
 */
void QrCode::setPort(const QString &port) {
  this->port->setText(port);
  this->update();
}

/**
 * @brief get the port
 */
QString QrCode::getPort() const {
  return this->port->text();
}
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::modals
