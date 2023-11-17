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
void Group::setUpQrCode(Qt::ColorScheme scheme) {
  using utility::functions::getQrCode;

  if (scheme == Qt::ColorScheme::Dark) {
    this->qrcode->setPixmap(QPixmap::fromImage(getQrCode(text, Qt::white)));
  } else {
    this->qrcode->setPixmap(QPixmap::fromImage(getQrCode(text, Qt::black)));
  }

  this->update();
}

/**
 * @brief Construct a new Abstract object
 *
 * @param parent
 */
Group::Group(QWidget *parent) : QDialog(parent) {
  // create layout
  auto root = new QVBoxLayout();

  // set center alignment
  qrcode->setAlignment(Qt::AlignCenter);
  port->setAlignment(Qt::AlignCenter);

  // add the ip and port input to layout
  root->addWidget(qrcode);
  root->addWidget(port);

  // center alignment of qrcode
  root->setAlignment(qrcode, Qt::AlignCenter);
  root->setAlignment(port, Qt::AlignCenter);

  // set layout
  this->setLayout(root);

  this->setUpLanguage();

  // detect the system theme
  const auto styleHints = QGuiApplication::styleHints();
  const auto signal = &QStyleHints::colorSchemeChanged;
  const auto slot = &Group::setUpQrCode;
  QObject::connect(styleHints, signal, this, slot);
}

/**
 * @brief set the qr code
 */
void Group::setQrCode(const QString &qrcode) {
  auto styleHints = QGuiApplication::styleHints();
  this->text = qrcode;
  this->setUpQrCode(styleHints->colorScheme());
  this->update();
}

/**
 * @brief get the qr code
 */
QString Group::getQrCode() const {
  return this->text;
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

/**
 * @brief Function used to set up all text in the label, etc..
 */
void Group::setUpLanguage() {
  // Nothing to do
}

/**
 * @brief change event
 */
void Group::changeEvent(QEvent *event) {
  if (event->type() == QEvent::LanguageChange) {
    this->setUpLanguage();
  }

  QWidget::changeEvent(event);
}
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::modals
