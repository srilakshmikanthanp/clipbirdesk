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
  QAction hub       =   QAction();
  QAction account   =   QAction();
  QAction exitApp   =   QAction();

 private:

  bool m_isSignedIn = false;
  bool m_isJoinedToHub = false;

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

 signals:  // signals
  void OnAccountClicked();

 signals:  // signals
  void OnHubClicked();

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
   * @brief set Received Enabled or Disabled
   */
  void setHistoryEnabled(bool);

  /**
   * @brief Is account In Enabled
   */
  void setAccoundEnabled(bool);

  /**
   * @brief set Hub Enabled or Disabled
   */
  void setHubEnabled(bool);

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
   * @brief Is Received Enabled
   */
  bool isHistoryEnabled() const;

  /**
   * @brief Is Account Enabled
   */
  bool isAccountEnabled() const;

  /**
   * @brief Is Hub Enabled
   */
  bool isHubEnabled() const;

  /**
   * @brief Is Signed In
   */
  void setSignedIn(bool);

  /**
   * @brief set joined to hub
   */
  void setJoinedToHub(bool isjoined);

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
