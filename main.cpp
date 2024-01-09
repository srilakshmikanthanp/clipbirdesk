// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

// Qt Headers
#include <QAbstractNativeEventFilter>
#include <QApplication>
#include <QFile>
#include <QGraphicsDropShadowEffect>
#include <QHostInfo>
#include <QMessageBox>
#include <QNetworkProxy>
#include <QStyleHints>
#include <QSystemTrayIcon>
#include <SingleApplication>

// C++ Headers
#include <csignal>

// Windows Headers
#if defined(_WIN32) || defined(_WIN64)
  #include <wintoastlib.h>
#endif

// Project Headers
#include "constants/constants.hpp"
#include "controller/clipbird/clipbird.hpp"
#include "ui/gui/container/container.hpp"
#include "ui/gui/screen/clipbird/clipbird.hpp"
#include "ui/gui/utilities/functions/functions.hpp"
#include "utility/functions/sslcert/sslcert.hpp"
#include "utility/logging/logging.hpp"

namespace srilakshmikanthanp::clipbirdesk {
/**
 * @brief Single Application class specialization
 * for ClipBird Application
 */
class ClipbirdApplication : public SingleApplication {
 private:  // Member Functions

  /**
   * @brief Get the certificate from App Home
   */
  QSslConfiguration getOldSslConfiguration() {
    auto two_months      = std::chrono::milliseconds(constants::getAppCertExpiryInterval());
    auto &storage        = storage::Storage::instance();

    // read the certificate and key
    QSslCertificate cert = QSslCertificate(storage.getHostCert(), QSsl::Pem);
    QSslKey key          = QSslKey(storage.getHostKey(), QSsl::Rsa);
    QSslConfiguration sslConfig;

    // get cert name
    auto name = cert.subjectInfo(QSslCertificate::CommonName);

    // Name is updated
    if (name != QString::fromStdString(constants::getAppName())) {
      return getNewSslConfiguration();
    }

    // set the certificate and key
    sslConfig.setLocalCertificate(cert);
    sslConfig.setPrivateKey(key);

    // if the configuration is null
    if (cert.isNull() || key.isNull() || sslConfig.isNull()) {
      throw std::runtime_error("Can't Create QSslConfiguration");
    }

    // if the certificate is going to expiry
    if (cert.expiryDate() - QDateTime::currentDateTime() < two_months) {
      return getNewSslConfiguration();
    }

    // set peer verify
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyPeer);

    // return the configuration
    return sslConfig;
  }

  /**
   * @brief Get the certificate by creating new one
   */
  QSslConfiguration getNewSslConfiguration() {
    // generate the certificate and key
    auto sslConfig = utility::functions::getQSslConfiguration();

    // write the certificate and key
    auto &storage  = storage::Storage::instance();
    storage.setHostCert(sslConfig.localCertificate().toPem());
    storage.setHostKey(sslConfig.privateKey().toPem());

    // return the configuration
    return sslConfig;
  }

  /**
   * @brief Get the certificate from App Home
   * or generate new one and store it
   */
  QSslConfiguration getSslConfiguration() {
    auto &storage = storage::Storage::instance();
    QSslConfiguration config;

    if (!storage.hasHostCert() || !storage.hasHostKey()) {
      config = getNewSslConfiguration();
    } else {
      config = getOldSslConfiguration();
    }

    // log the certificate and key
    qInfo("Certificate: \n" + config.localCertificate().toPem());
    qInfo("Key: \n" + config.privateKey().toPem());

    // return the configuration
    return config;
  }

  /**
   * @brief On Tray Icon Clicked
   */
  void onTrayIconClicked(QSystemTrayIcon::ActivationReason reason) {
    if (reason == QSystemTrayIcon::ActivationReason::Trigger) {
      window->isVisible() ? window->hide() : window->show();
    }
  }

  /**
   * @brief Set the Qss File for the color scheme
   */
  void setQssFile(Qt::ColorScheme scheme) {
    // detect system theme is dark or light
    bool isDark     = scheme == Qt::ColorScheme::Dark;

    // qss
    std::string qss = isDark ? constants::getAppQSSDark() : constants::getAppQSSLight();

    // QFile to read the qss file
    QFile qssFile(QString::fromStdString(qss));

    // open the qss file
    qssFile.open(QFile::ReadOnly);

    // set the style sheet
    qApp->setStyleSheet(qssFile.readAll());
  }

 private:  //  Member Variables and Objects

  controller::ClipBird *controller;
  ui::gui::Clipbird *content;
  ui::gui::Container *window;
  QSystemTrayIcon *trayIcon;

 private:  // Disable Copy, Move and Assignment

  Q_DISABLE_COPY_MOVE(ClipbirdApplication);

 public:  // Constructors and Destructors

  /**
   * @brief Construct a new Clipbird Application object
   *
   * @param argc argument count
   * @param argv argument vector
   */
  ClipbirdApplication(int &argc, char **argv) : SingleApplication(argc, argv) {
    // create the objects of the class
    controller = new controller::ClipBird(this->getSslConfiguration());
    content    = new ui::gui::Clipbird(controller);
    window     = new ui::gui::Container();
    trayIcon   = new QSystemTrayIcon(this);

    // set the signal handler for all os
    signal(SIGTERM, [](int sig) { qApp->quit(); });
    signal(SIGINT, [](int sig) { qApp->quit(); });
    signal(SIGABRT, [](int sig) { qApp->quit(); });

    // set initial theme
    setQssFile(QGuiApplication::styleHints()->colorScheme());

    // set ToolTip
    trayIcon->setToolTip(QString::fromStdString(constants::getAppName()));

    // set not to quit on last content closed
    qApp->setQuitOnLastWindowClosed(false);

    // set the QSystemTrayIcon
    content->setTrayIcon(trayIcon);

    // set the content Ratio
    window->setFixedSize(constants::getAppWindowSize());

    // set the content
    window->setContent(content);

    // set the icon to content
    window->setWindowIcon(QIcon(constants::getAppLogo()));

    // using some classes
    using ui::gui::Clipbird;

    // tray icon click from content
    const auto signal_tic = &QSystemTrayIcon::activated;
    const auto slot_tic   = &ClipbirdApplication::onTrayIconClicked;
    QObject::connect(trayIcon, signal_tic, this, slot_tic);

    // set the signal for instance Started
    const auto signal_is = &SingleApplication::instanceStarted;
    const auto slot_is   = &Clipbird::show;
    QObject::connect(this, signal_is, window, slot_is);

    // detect the system theme
    const auto signal = &QStyleHints::colorSchemeChanged;
    const auto slot   = &ClipbirdApplication::setQssFile;
    QObject::connect(QGuiApplication::styleHints(), signal, this, slot);
  }

  /**
   * @brief Destroy the Clipbird Application
   * Object and it's members
   */
  virtual ~ClipbirdApplication() {
    delete controller;
    delete content;
    delete window;
  }

  /**
   * @brief get the controller
   */
  controller::ClipBird *getController() const {
    return controller;
  }
};

/**
 * @brief Custom event filter for the application that
 * captures window shown event and if the window is decorated
 * apply some attributes to the window
 */
class ClipbirdEventFilter : public QObject {
  virtual bool eventFilter(QObject *o, QEvent *e) {
    if (e->type() == QEvent::WindowActivate) {
      handleWindowShownEvent(dynamic_cast<QWidget *>(o));
    }

    return QObject::eventFilter(o, e);
  }

  void handleWindowShownEvent(QWidget *window) {
    if (!(window->windowFlags() & Qt::FramelessWindowHint)) {
      ui::gui::utilities::setPlatformAttributes(window);
    }
  }
};

/**
 * @brief Custom event filter for the application that
 * captures Native event and if the device is going to sleep
 * or wake up disconnect
 */
class ClipbirdNativeEventFilter : public QAbstractNativeEventFilter {
 private:

  bool nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result) override {
    constexpr const char *WIN_MSG = "windows_generic_MSG";

#if defined(_WIN32) || defined(_WIN64)
    if (eventType == WIN_MSG) {
      handleWindowsGenericMessage(static_cast<MSG *>(message));
    }
#endif

    return false;
  }

#if defined(_WIN32) || defined(_WIN64)
  void handleWindowsGenericMessage(MSG *msg) {
    if (msg->message == WM_POWERBROADCAST) {
      switch (msg->wParam) {
      case PBT_APMRESUMESUSPEND:
        this->handleWakeUpEvent();
        break;

      case PBT_APMSUSPEND:
        this->handleSleepEvent();
        break;
      }
    }
  }
#endif

  void handleSleepEvent() {
    switch (controller->getHostType()) {
    case types::enums::HostType::CLIENT:
      controller->disposeClient();
      break;

    case types::enums::HostType::SERVER:
      controller->disposeServer();
      break;
    }
  }

  void handleWakeUpEvent() {
    if (controller->isLastlyHostIsServer()) {
      controller->setCurrentHostAsServer();
    } else {
      controller->setCurrentHostAsClient();
    }
  }

 private:

  controller::ClipBird *controller;

 public:

  /**
   * @brief Destroy the Clipbird Native Event Filter object
   */
  virtual ~ClipbirdNativeEventFilter() = default;

  /**
   * @brief Construct a new Clipbird Native Event Filter object
   */
  ClipbirdNativeEventFilter(controller::ClipBird *controller) : controller(controller) {}
};
}  // namespace srilakshmikanthanp::clipbirdesk

/**
 * @brief Global Error Handler that helps to log
 */
void globalErrorHandler() {
  try {
    std::rethrow_exception(std::current_exception());
  } catch (const std::exception &e) {
    qCritical() << e.what();
  } catch (...) {
    qCritical() << "Unknown Exception";
  }

  std::abort();
}

/**
 * @brief main function that starts the application
 * by ensuring that only one instance of the
 * application is running
 *
 * @param argc Unused
 * @param argv Unused
 *
 * @return int Status code
 */
auto main(int argc, char **argv) -> int {
  // using ClipbirdApplication class from namespace
  using srilakshmikanthanp::clipbirdesk::ClipbirdApplication;
  using srilakshmikanthanp::clipbirdesk::ClipbirdEventFilter;
  using srilakshmikanthanp::clipbirdesk::ClipbirdNativeEventFilter;
  using srilakshmikanthanp::clipbirdesk::constants::getAppHome;
  using srilakshmikanthanp::clipbirdesk::constants::getAppLogo;
  using srilakshmikanthanp::clipbirdesk::constants::getAppLogFile;
  using srilakshmikanthanp::clipbirdesk::constants::getAppName;
  using srilakshmikanthanp::clipbirdesk::constants::getAppOrgName;
  using srilakshmikanthanp::clipbirdesk::constants::getAppVersion;
  using srilakshmikanthanp::clipbirdesk::logging::Logger;

  // std::string to std::wstring
  const auto W = [](std::string str) -> std::wstring {
    return std::wstring(str.begin(), str.end());
  };

  // create the application
  ClipbirdApplication app(argc, argv);

  // native event filter
  auto filter = new ClipbirdNativeEventFilter(app.getController());

  // install native event filter
  app.installNativeEventFilter(filter);

  // install event filter
  app.installEventFilter(new ClipbirdEventFilter());

#if defined(_WIN32) || defined(_WIN64)
  // create AUMI
  auto appAumi = WinToastLib::WinToast::configureAUMI(
    W(getAppOrgName()), W(getAppName()), W(std::string()), W(getAppVersion())
  );

  // set up wintoast lib
  WinToastLib::WinToast::instance()->setAppName(W(getAppName()));
  WinToastLib::WinToast::instance()->setAppUserModelId(appAumi);

  // initialize
  if (!WinToastLib::WinToast::instance()->initialize()) {
    auto dialog = QMessageBox(nullptr);
    dialog.setText(QObject::tr("Can't Initialize WinToast"));
    dialog.setIcon(QMessageBox::Critical);
    dialog.setWindowTitle(getAppName());
    dialog.setWindowIcon(QIcon(getAppLogo()));
    dialog.exec();
    return EXIT_FAILURE;
  }
#endif

  // Home Directory of the application
  auto path = QString::fromStdString(getAppHome());

  // make app home directory if not exists
  if (!QDir(path).exists() && !QDir().mkdir(path)) {
    QMessageBox::critical(nullptr, "Error", "Can't Create App Home");
    return EXIT_FAILURE;
  }

  // set proxy for the application (No Proxy)
  QNetworkProxy::setApplicationProxy(QNetworkProxy::NoProxy);

#ifdef NDEBUG
  // log file to record the logs
  QFile logfile(QString::fromStdString(getAppLogFile()));

  // open the log file
  logfile.open(QIODevice::WriteOnly | QIODevice::Append);

  // open QStream to write the log file
  QTextStream logstream(&logfile);

  // Set the log file
  Logger::setLogStream(&logstream);
#else
  // open QStream to write the log file
  QTextStream logstream(stdout);

  // Set the log file as std::cout
  Logger::setLogStream(&logstream);
#endif

  // Set the global error handler
  std::set_terminate(globalErrorHandler);

  // Set the custom message handler
  qInstallMessageHandler(Logger::handler);

  // start application and return status code
  return app.exec();
}
