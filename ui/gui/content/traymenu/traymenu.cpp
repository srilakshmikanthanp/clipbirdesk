// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "traymenu.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::content {
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
  QObject::connect(&issue, &QAction::triggered, this, &TrayMenu::OnIssueClicked);
  QObject::connect(&reset, &QAction::triggered, this, &TrayMenu::OnResetClicked);
  QObject::connect(&exitApp, &QAction::triggered, this, &TrayMenu::OnExitClicked);

  // set the Menu Items
  this->addAction(&connect);
  this->addAction(&qrCode);
  this->addAction(&reset);
  this->addSeparator();
  this->addAction(&about);
  this->addAction(&issue);
  this->addSeparator();
  this->addAction(&exitApp);
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
 * @brief set Issue Enabled or Disabled
 */
void TrayMenu::setIssueEnabled(bool isenabled) {
  issue.setEnabled(isenabled);
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
 * @brief Is Issue Enabled
 */
bool TrayMenu::isIssueEnabled() const {
  return issue.isEnabled();
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
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::content
