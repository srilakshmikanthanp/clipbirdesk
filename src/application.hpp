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
#include <QNetworkInformation>
#include <QQmlApplicationEngine>

// C++ Headers
#include <csignal>
#include <functional>
#include <QSystemTrayIcon>

// Project Headers
#include "constants/constants.hpp"
#include "service/clipbird_service_factory.hpp"
#include "service/clipbird_service.hpp"
#include "clipboard/applicationclipboard.hpp"
#include "ui/gui/utilities/functions/functions.hpp"
#include "ui/gui/traymenu/traymenu.hpp"
#include "utility/functions/ssl/ssl.hpp"
#include "utility/functions/crypto/crypto.hpp"
#include "utility/logging/logging.hpp"
#include "utility/powerhandler/powerhandler.hpp"

namespace srilakshmikanthanp::clipbirdesk {
class Application : public SingleApplication {
 private:  // Disable Copy, Move and Assignment

  Q_DISABLE_COPY_MOVE(Application);

 private:
  service::ClipbirdService* clipbirdService = service::ClipbirdServiceFactory::getClipbirdService();
  QQmlApplicationEngine engine = QQmlApplicationEngine(this);
  QSystemTrayIcon trayIcon = QSystemTrayIcon(this);
  ui::gui::TrayMenu trayMenu = ui::gui::TrayMenu();
  utility::PowerHandler powerHandler = utility::PowerHandler(this);
  ApplicatiionState *applicationState = ApplicationFactory::getApplicationState();

 private:
  void handleOpenApplication();
  void handleSleepEvent();
  void handleWakeUpEvent();
  void handleExit();

 public:  // Constructors and Destructors

  Application(int &argc, char **argv);
  virtual ~Application();

  utility::PowerHandler* getPowerHandler();
};
}
