#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

// Qt Headers
#include <QAbstractNativeEventFilter>
#include <QApplication>
#include <QFile>
#include <QGraphicsDropShadowEffect>
#include <QGuiApplication>
#include <QHostInfo>
#include <QMessageBox>
#include <QNetworkInterface>
#include <QNetworkProxy>
#include <QStyleHints>
#include <QSystemTrayIcon>
#include <SingleApplication>

// C++ Headers
#include <csignal>

// Project Headers
#include "constants/constants.hpp"
#include "controller/clipbird/clipbird.hpp"
#include "ui/gui/utilities/functions/functions.hpp"
#include "ui/gui/traymenu/traymenu.hpp"
#include "ui/gui/modals/aboutus/aboutus.hpp"
#include "ui/gui/modals/connect/connect.hpp"
#include "ui/gui/modals/group/group.hpp"
#include "ui/gui/notify/joinrequest/joinrequest.hpp"
#include "ui/gui/widgets/clipbird/clipbird.hpp"
#include "ui/gui/widgets/history/history.hpp"
#include "ui/gui/widgets/settings/settings.hpp"
#include "utility/functions/sslcert/sslcert.hpp"
#include "utility/logging/logging.hpp"

namespace srilakshmikanthanp::clipbirdesk {
class Application : public SingleApplication {
  private:  // Member Functions

  /**
   * @brief Get the certificate from App Home
   */
  QSslConfiguration getOldSslConfiguration();

  /**
   * @brief Get the certificate by creating new one
   */
  QSslConfiguration getNewSslConfiguration();

  /**
   * @brief Get the certificate from App Home
   * or generate new one and store it
   */
  QSslConfiguration getSslConfiguration();

  /**
   * @brief handle onConnectionError
   */
  void handleConnectionError(QString error);

  /**
   * @brief On Tab Changed for Client
   */
  void handleTabChange(ui::gui::widgets::Clipbird::Tabs tab);

  /**
   * @brief Handle the Client auth Request
   */
  void handleAuthRequest(const types::device::Device& client);

  //----------------------------- slots for Tray ----------------------------//

  /**
   * @brief On Qr Code Clicked
   */
  void onQrCodeClicked();

  /**
   * @brief On Connect Clicked
   */
  void onConnectClicked();

  /**
   * @brief On About Clicked
   */
  void onAboutClicked();

  /**
   * @brief On Open App Clicked
   */
  void openClipbird();

  /**
   * @brief On Send Clicked
   */
  void onSendClicked();

  /**
   * @brief On Received Clicked
   */
  void onHistoryClicked();

  /**
   * @brief On Settings Clicked
   */
  void onSettingsClicked();

  /**
   * @brief On Reset Clicked
   */
  void onResetClicked();

  /**
   * @brief On Tray Icon Clicked
   */
  void onTrayIconClicked(QSystemTrayIcon::ActivationReason reason);

  /**
   * @brief Set the Qss File for the color scheme
   */
  void setQssFile(Qt::ColorScheme scheme);

 private:  // Member variable (Modals)

  ui::gui::modals::AboutUs *aboutUs = new ui::gui::modals::AboutUs();
  ui::gui::modals::Group *group = new ui::gui::modals::Group();
  ui::gui::modals::Connect *joiner = new ui::gui::modals::Connect();

 private:  //  Member Variables and Objects

  ui::gui::widgets::Clipbird *clipbird;
  ui::gui::widgets::History *history;
  ui::gui::widgets::Settings *settings;
  ui::gui::TrayMenu *trayMenu;
  QSystemTrayIcon *trayIcon;
  controller::ClipBird *controller;

 private:  // Disable Copy, Move and Assignment

  Q_DISABLE_COPY_MOVE(Application);

 public:  // Constructors and Destructors

  /**
   * @brief Construct a new Clipbird Application object
   *
   * @param argc argument count
   * @param argv argument vector
   */
  Application(int &argc, char **argv);

  /**
   * @brief Destroy the Clipbird Application
   * Object and it's members
   */
  virtual ~Application();

  /**
   * @brief get the controller
   */
  controller::ClipBird *getController() const;
};
}
