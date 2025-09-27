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
#include <QHotkey>

// C++ Headers
#include <csignal>

// Project Headers
#include "constants/constants.hpp"
#include "clipboard/controller/clipboard_controller.hpp"
#include "history/controller/history_controller.hpp"
#include "syncing/lan/controller/lan_controller.hpp"
#include "syncing/wan/controller/wan_controller.hpp"
#include "store/secure_storage.hpp"
#include "store/storage.hpp"
#include "syncing/wan/auth/auth_api_client.hpp"
#include "syncing/wan/auth/auth_api_repository.hpp"
#include "syncing/wan/auth/auth_token_holder.hpp"
#include "syncing/wan/device/device_api_client.hpp"
#include "syncing/wan/device/device_api_repository.hpp"
#include "syncing/wan/device/device_type.hpp"
#include "ui/gui/utilities/functions/functions.hpp"
#include "ui/gui/traymenu/traymenu.hpp"
#include "ui/gui/modals/aboutus/aboutus.hpp"
#include "ui/gui/modals/connect/connect.hpp"
#include "ui/gui/modals/group/group.hpp"
#include "ui/gui/modals/signin/signin.hpp"
#include "ui/gui/notification/joinrequest/joinrequest.hpp"
#include "ui/gui/widgets/clipbird/clipbird.hpp"
#include "ui/gui/widgets/history/history.hpp"
#include "utility/functions/ssl/ssl.hpp"
#include "utility/functions/keychain/keychain.hpp"
#include "utility/functions/crypto/crypto.hpp"
#include "utility/logging/logging.hpp"
#include "utility/powerhandler/powerhandler.hpp"

namespace srilakshmikanthanp::clipbirdesk {
class Application : public SingleApplication {
  private:

  QSslConfiguration getOldSslConfiguration();
  QSslConfiguration getNewSslConfiguration();
  QSslConfiguration getSslConfiguration();

  QFuture<void> setupHubConnection();

  void handleConnectionError(QString error);
  void handleTabChange(ui::gui::widgets::Clipbird::Tabs tab);
  void handleAuthRequest(const types::Device& client);
  void handleConnect(QString ip, QString port);
  void handleSignin(QString email, QString password);
  void handleHubConnect();
  void handleHubDisconnect();
  void handleHubErrorOccurred(QAbstractSocket::SocketError);
  void handleSleepEvent();
  void handleWakeUpEvent();
  void handleClientStateChanged(types::Device client, bool connected);
  void handleServerStatusChanged(bool status, types::Device host);
  void handleServerFound(types::Device server);
  void handleOnClipboard(QVector<QPair<QString, QByteArray>> data);
  void handleSyncRequest(QVector<QPair<QString, QByteArray>> data);

  void onTrayIconClicked(QSystemTrayIcon::ActivationReason reason);
  void openClipbird();
  void openQrCode();
  void openConnect();
  void openAbout();
  void sendClipboard();
  void openHistory();
  void resetDevices();
  void onAccountClicked();
  void onHubClicked();

  void setQssFile(Qt::ColorScheme scheme);

 private:

  ui::gui::modals::AboutUs *aboutUs = new ui::gui::modals::AboutUs();
  ui::gui::modals::Group *group = new ui::gui::modals::Group();
  ui::gui::modals::Connect *joiner = new ui::gui::modals::Connect();
  ui::gui::modals::SignIn *signin = new ui::gui::modals::SignIn();

 private:

  controller::ClipboardController *clipboardController;
  controller::HistoryController *historyController;
  controller::LanController *lanController;
  controller::WanController *wanController;
  ui::gui::widgets::Clipbird *clipbird;
  ui::gui::widgets::History *history;
  ui::gui::TrayMenu *trayMenu;
  QSystemTrayIcon *trayIcon;
  PowerHandler *powerHandler;

 private:

  QHotkey *hotkey;

 private:

  using DeviceRepository = syncing::wan::DeviceRepository;
  using AuthRepository = syncing::wan::AuthRepository;
  using DeviceApiClient = syncing::wan::DeviceApiClient;
  using AuthApiClient = syncing::wan::AuthApiClient;
  using DeviceApiRepository = syncing::wan::DeviceApiRepository;
  using AuthApiRepository = syncing::wan::AuthApiRepository;
  using Server = syncing::lan::Server;
  using Client = syncing::lan::Client;

 private:
  DeviceRepository *deviceRepository = new DeviceApiRepository(new DeviceApiClient(this), this);
  AuthRepository *authRepository = new AuthApiRepository(new AuthApiClient(this), this);

 private:  // Disable Copy, Move and Assignment

  Q_DISABLE_COPY_MOVE(Application);

 public:  // Constructors and Destructors

  Application(int &argc, char **argv);
  virtual ~Application();

  controller::ClipboardController* getClipboardController() const;
  controller::HistoryController* getHistoryController() const;
  controller::LanController* getLanController() const;
  controller::WanController* getWanController() const;
  PowerHandler* getPowerHandler() const;
};
}
