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
      color: #ffffff;
    }

    QMenu::item {
      background-color: #2b2a3c;
      font-size: 12px;
      margin-left: 15px;
      margin-right: 15px;
      padding-left: 15px;
      padding-right: 15px;
      padding-top: 5px;
      padding-bottom: 5px;
    }

    QMenu::item:hover {
      background-color: #3e3e3e;
    }

    QMenu::separator {
      height: 1px;
    }

    QMenu::item:selected {
      background-color: #0375ef;
      border: 1px solid #0375ef;
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
