// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "traymenu.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui {
/**
 * @brief Construct a new Sys Tray object
 *
 * @param parent
 */
TrayMenu::TrayMenu(QWidget* parent) : QMenu(parent) {
  // connect signals
  QObject::connect(&qrCode, &QAction::triggered, this, &TrayMenu::OnQrCodeClicked);
  QObject::connect(&connect, &QAction::triggered, this, &TrayMenu::OnConnectClicked);
  QObject::connect(&about, &QAction::triggered, this, &TrayMenu::OnAboutClicked);
  QObject::connect(&reset, &QAction::triggered, this, &TrayMenu::OnResetClicked);
  QObject::connect(&exitApp, &QAction::triggered, this, &TrayMenu::OnExitClicked);
  QObject::connect(&openApp, &QAction::triggered, this, &TrayMenu::OnOpenAppClicked);
  QObject::connect(&send, &QAction::triggered, this, &TrayMenu::OnSendClicked);
  QObject::connect(&history, &QAction::triggered, this, &TrayMenu::OnHistoryClicked);

  // set the Menu Items
  this->addAction(&qrCode);
  this->addAction(&connect);
  this->addAction(&reset);
  this->addSeparator();
  this->addAction(&openApp);
  this->addAction(&send);
  this->addAction(&history);
  this->addSeparator();
  this->addAction(&about);
  this->addSeparator();
  this->addAction(&exitApp);

  // set up the initial language
  this->setUpLanguage();
}

/**
 * @brief Function used to set up all text in the label, etc..
 */
void TrayMenu::setUpLanguage() {
  connect.setText(QObject::tr("Join to Group"));
  qrCode.setText(QObject::tr("Group QrCode"));
  reset.setText(QObject::tr("Reset Devices"));
  openApp.setText(constants::getAppName().c_str());
  send.setText(QObject::tr("Send"));
  history.setText(QObject::tr("History"));
  about.setText(QObject::tr("About"));
  exitApp.setText(QObject::tr("Exit"));
}

/**
 * @brief set Connect Enabled or Disabled
 */
void TrayMenu::setConnectEnabled(bool isenabled) {
  connect.setEnabled(isenabled);
}

/**
 * @brief set Qr Code Enabled or Disabled
 */
void TrayMenu::setQrCodeEnabled(bool isenabled) {
  qrCode.setEnabled(isenabled);
}

/**
 * @brief set About Enabled or Disabled
 */
void TrayMenu::setAboutEnabled(bool isenabled) {
  about.setEnabled(isenabled);
}

/**
 * @brief set Reset Enabled or Disabled
 */
void TrayMenu::setResetEnabled(bool isenabled) {
  reset.setEnabled(isenabled);
}

/**
 * @brief set Exit Enabled or Disabled
 */
void TrayMenu::setExitEnabled(bool isenabled) {
  exitApp.setEnabled(isenabled);
}

/**
 * @brief set open app Enabled or Disabled
 */
void TrayMenu::setOpenAppEnabled(bool isenabled) {
  openApp.setEnabled(isenabled);
}

/**
 * @brief set Send Enabled or Disabled
 */
void TrayMenu::setSendEnabled(bool isenabled) {
  send.setEnabled(isenabled);
}

/**
 * @brief set Received Enabled or Disabled
 */
void TrayMenu::setHistoryEnabled(bool isenabled) {
  history.setEnabled(isenabled);
}

/**
 * @brief Is Connect Enabled
 */
bool TrayMenu::isConnectEnabled() const {
  return connect.isEnabled();
}

/**
 * @brief Is Qr Code Enabled
 */
bool TrayMenu::isQrCodeEnabled() const {
  return qrCode.isEnabled();
}

/**
 * @brief Is About Enabled
 */
bool TrayMenu::isAboutEnabled() const {
  return about.isEnabled();
}

/**
 * @brief Is Reset Enabled
 */
bool TrayMenu::isResetEnabled() const {
  return reset.isEnabled();
}

/**
 * @brief Is Exit Enabled
 */
bool TrayMenu::isExitEnabled() const {
  return exitApp.isEnabled();
}

/**
 * @brief Is Open App Enabled
 */
bool TrayMenu::isOpenAppEnabled() const {
  return openApp.isEnabled();
}

/**
 * @brief is Send Enabled
 */
bool TrayMenu::isSendEnabled() const {
  return send.isEnabled();
}

/**
 * @brief is Received Enabled
 */
bool TrayMenu::isHistoryEnabled() const {
  return history.isEnabled();
}

/**
 * @brief change event
 */
void TrayMenu::changeEvent(QEvent *event) {
  if (event->type() == QEvent::LanguageChange) {
    this->setUpLanguage();
  }

  QWidget::changeEvent(event);
}

/**
 * @brief Override paint event
 */
void TrayMenu::paintEvent(QPaintEvent* event) {
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);

  QPainterPath path;
  QRectF rect = QRectF(this->rect()).adjusted(2.0, 2.0, -2.0, -2.0);
  path.addRoundedRect(rect, 7, 7);
  QRegion region = QRegion(path.toFillPolygon().toPolygon());
  this->setMask(region);

  QMenu::paintEvent(event);
}
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui
