#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

// Qt headers
#include <QMenu>
#include <QSystemTrayIcon>

namespace srilakshmikanthanp::clipbirdesk::ui::gui {
class TrayIcon : public QSystemTrayIcon {
 private:  // Disable copy constructor and copy assignment operator

  Q_DISABLE_COPY_MOVE(TrayIcon)

 private:  // Member variable

  QMenu* menu;

 private:  // just for Qt Object Macro

  Q_OBJECT

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
  explicit TrayIcon(QWidget* parent = nullptr);

  /**
   * @brief Destroy the Sys Tray object
   */
  virtual ~TrayIcon() = default;
};
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui
