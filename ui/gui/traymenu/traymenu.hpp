#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

// Qt headers
#include <QMenu>
#include <QSystemTrayIcon>

namespace srilakshmikanthanp::clipbirdesk::ui::gui {
class TrayMenu : public QMenu {
 private:  // Disable copy constructor and copy assignment operator

  Q_DISABLE_COPY_MOVE(TrayMenu)

 private:  // just for Qt Object Macro

  Q_OBJECT

 private:  // Constants for style sheet

  static constexpr auto styleSheet = R"(
    QMenu {
      background-color: #2b2a3c;
      border: 1px solid #2b2a3c;
      border-radius: 2px;
      color: #ffffff;
    }

    QMenu::item {
      background-color: #2b2a3c;
      color: #ffffff;
      padding : 5px;
      font-size: 15px;
    }

    QMenu::item:hover {
      background-color: #3e3e3e;
      border: 1px solid #3e3e3e;
    }
  )";

 signals:  // signals
  void OnAboutClicked();

 signals:  // signals
  void OnIssueClicked();

 signals:  // signals
  void OnExitClicked();

 public:  // Member function
  /**
   * @brief Construct a new Sys Tray object
   *
   * @param parent
   */
  TrayMenu(QWidget* parent = nullptr);

  /**
   * @brief Destroy the Sys Tray object
   */
  virtual ~TrayMenu() = default;
};
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui
