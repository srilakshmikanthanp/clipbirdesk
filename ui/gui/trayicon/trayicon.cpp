// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "trayIcon.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui {
/**
 * @brief Construct a new Sys Tray object
 *
 * @param parent
 */
TrayIcon::TrayIcon(QWidget* parent) : QSystemTrayIcon(parent) {
  // create the menu for the system tray
  this->menu = new QMenu(parent);

  // Add the actions to the menu
  this->menu->addAction("About", this, &TrayIcon::OnAboutClicked);
  this->menu->addAction("Issue", this, &TrayIcon::OnIssueClicked);
  this->menu->addSeparator();
  this->menu->addAction("Exit", this, &TrayIcon::OnExitClicked);

  // add the menu to the system tray
  this->setContextMenu(this->menu);
}
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui
