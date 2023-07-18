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
  // set the Menu Items
  this->addAction("About", this, &TrayMenu::OnAboutClicked);
  this->addAction("Issue", this, &TrayMenu::OnIssueClicked);
  this->addSeparator();
  this->addAction("Exit", this, &TrayMenu::OnExitClicked);

  // set the style sheet
  this->setStyleSheet(styleSheet);
}
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui
