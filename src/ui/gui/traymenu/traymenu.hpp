#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Qt headers
#include <QEvent>
#include <QMenu>
#include <QPainter>
#include <QPainterPath>
#include <QSystemTrayIcon>

// local headers
#include "constants/constants.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui {
class TrayMenu : public QMenu {
 private:  // Disable copy constructor and copy assignment operator

  Q_DISABLE_COPY_MOVE(TrayMenu)

 private:  // just for Qt Object Macro

  Q_OBJECT

 private:  // Member Functions

  /**
   * @brief Function used to set up all text in the label, etc..
   */
  void setUpLanguage();

 private:  // Actions

  QAction openApp   =   QAction();
  QAction exitApp   =   QAction();

 signals:  // signals
  void OnOpenAppClicked();

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

  /**
   * @brief set Exit Enabled or Disabled
   */
  void setExitEnabled(bool);

  /**
   * @brief set Open App Enabled or Disabled
   */
  void setOpenAppEnabled(bool);

  /**
   * @brief Is Exit Enabled
   */
  bool isExitEnabled() const;

  /**
   * @brief is Open App Enabled
   */
  bool isOpenAppEnabled() const;

 protected:

  /**
   * @brief Override paint event
   */
  void paintEvent(QPaintEvent* event) override;

  /**
   * @brief Override change event
   */
  void changeEvent(QEvent *) override;
};
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui
