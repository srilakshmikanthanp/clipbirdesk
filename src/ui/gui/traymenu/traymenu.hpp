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

  QAction connect   =   QAction();
  QAction qrCode    =   QAction();
  QAction reset     =   QAction();
  QAction openApp   =   QAction();
  QAction history   =   QAction();
  QAction about     =   QAction();
  QAction exitApp   =   QAction();

 signals:  // signals
  void OnConnectClicked();

 signals:  // signals
  void OnQrCodeClicked();

 signals:  // signals
  void OnResetClicked();

 signals:  // signals
  void OnOpenAppClicked();

 signals:  // signals
  void OnHistoryClicked();

 signals:  // signals
  void OnAboutClicked();

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
   * @brief set Qr Code Enabled or Disabled
   */
  void setQrCodeEnabled(bool);

  /**
   * @brief set Connect Enabled or Disabled
   */
  void setConnectEnabled(bool);

  /**
   * @brief set About Enabled or Disabled
   */
  void setAboutEnabled(bool);

  /**
   * @brief set Reset Enabled or Disabled
   */
  void setResetEnabled(bool);

  /**
   * @brief set Exit Enabled or Disabled
   */
  void setExitEnabled(bool);

  /**
   * @brief set Open App Enabled or Disabled
   */
  void setOpenAppEnabled(bool);

  /**
   * @brief set Send Enabled or Disabled
   */
  void setSendEnabled(bool);

  /**
   * @brief set Received Enabled or Disabled
   */
  void setHistoryEnabled(bool);

  /**
   * @brief Is Qr Code Enabled
   */
  bool isQrCodeEnabled() const;

  /**
   * @brief Is Connect Enabled
   */
  bool isConnectEnabled() const;

  /**
   * @brief Is About Enabled
   */
  bool isAboutEnabled() const;

  /**
   * @brief Is Reset Enabled
   */
  bool isResetEnabled() const;

  /**
   * @brief Is Exit Enabled
   */
  bool isExitEnabled() const;

  /**
   * @brief is Open App Enabled
   */
  bool isOpenAppEnabled() const;

  /**
   * @brief Is Send Enabled
   */
  bool isSendEnabled() const;

  /**
   * @brief Is Received Enabled
   */
  bool isHistoryEnabled() const;

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
