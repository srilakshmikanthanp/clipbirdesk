#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

// Qt headers
#include <QMenu>
#include <QSystemTrayIcon>

namespace srilakshmikanthanp::clipbirdesk::ui::gui::content {
class TrayMenu : public QMenu {
 private:  // Disable copy constructor and copy assignment operator

  Q_DISABLE_COPY_MOVE(TrayMenu)

 private:  // just for Qt Object Macro

  Q_OBJECT

 private:  // Actions

  QAction connect   =   QAction("Join to Group");
  QAction qrCode    =   QAction("Group QrCode");
  QAction reset     =   QAction("Reset Devices");
  QAction openApp   =   QAction("ClipBird");
  QAction send      =   QAction("Send");
  QAction history  =   QAction("History");
  QAction about     =   QAction("About");
  QAction issue     =   QAction("Report Issue");
  QAction exitApp   =   QAction("Exit");

 signals:  // signals
  void OnConnectClicked();

 signals:  // signals
  void OnQrCodeClicked();

 signals:  // signals
  void OnResetClicked();

 signals:  // signals
  void OnOpenAppClicked();

 signals:  // signals
  void OnSendClicked();

 signals:  // signals
  void OnHistoryClicked();

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
   * @brief set Issue Enabled or Disabled
   */
  void setIssueEnabled(bool);

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
   * @brief Is Issue Enabled
   */
  bool isIssueEnabled() const;

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
};
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui
