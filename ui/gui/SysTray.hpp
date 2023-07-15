#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

// Qt headers
#include <QSystemTrayIcon>
#include <QMenu>

namespace srilakshmikanthanp::clipbirdesk::ui::gui {
class SysTray : public QSystemTrayIcon {
 private: // Disable copy constructor and copy assignment operator
  Q_DISABLE_COPY_MOVE(SysTray)

 private: // Member variable
  QMenu* menu;

 private: // just for Qt Object Macro
  Q_OBJECT

 signals: // signals
  void OnAboutClicked();

 signals: // signals
  void OnIssueClicked();

 signals: // signals
  void OnExitClicked();

 public:  // Member function

  /**
   * @brief Construct a new Sys Tray object
   *
   * @param parent
   */
  explicit SysTray(QWidget* parent = nullptr) : QSystemTrayIcon(parent) {
    // create the menu for the system tray
    this->menu = new QMenu(parent);

    // Add the actions to the menu
    this->menu->addAction("About", this, &SysTray::OnAboutClicked);
    this->menu->addAction("Issue", this, &SysTray::OnIssueClicked);
    this->menu->addSeparator();
    this->menu->addAction("Exit", this, &SysTray::OnExitClicked);

    // add the menu to the system tray
    this->setContextMenu(this->menu);
  }

  /**
   * @brief Destroy the Sys Tray object
   */
  virtual ~SysTray() = default;
};
} // namespace srilakshmikanthanp::clipbirdesk::ui::gui
