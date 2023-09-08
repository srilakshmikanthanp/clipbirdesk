// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

// Qt Headers
#include <QApplication>
#include <QHostInfo>
#include <QFile>
#include <QNetworkProxy>
#include <QStyleHints>
#include <QGraphicsDropShadowEffect>
#include <QMessageBox>
#include <QSystemTrayIcon>
#include <SingleApplication>

// C++ Headers
#include <csignal>

// Project Headers
#include "constants/constants.hpp"
#include "controller/clipbird/clipbird.hpp"
#include "ui/gui/content/content.hpp"
#include "ui/gui/window/window.hpp"
#include "utility/functions/sslcert/sslcert.hpp"
#include "utility/logging/logging.hpp"

namespace srilakshmikanthanp::clipbirdesk {
/**
 * @brief Single Application class specialization
 * for ClipBird Application
 */
class ClipbirdApplication : public SingleApplication {
 private:  // Member Variables and Objects

  // file path to store the certificate and key
  std::string certPath = (std::filesystem::path(constants::getAppHome()) / "cert.pem").string();
  std::string keyPath  = (std::filesystem::path(constants::getAppHome()) / "key.pem").string();

 private:  // Member Functions

  /**
   * @brief Get the certificate from App Home
   */
  QSslConfiguration getOldSslConfiguration() {
    // QFile to read the certificate and key
    QFile certFile(certPath.c_str()), pkeyFile(keyPath.c_str());

    // open the certificate and key
    if(!certFile.open(QIODevice::ReadOnly) || !pkeyFile.open(QIODevice::ReadOnly)) {
      throw std::runtime_error("Can't Create Certificate");
    }

    // read the certificate and key
    QSslCertificate cert = QSslCertificate(certFile.readAll(), QSsl::Pem);
    QSslKey key = QSslKey(pkeyFile.readAll(), QSsl::Rsa);
    QSslConfiguration sslConfig;

    // set the certificate and key
    sslConfig.setLocalCertificate(cert);
    sslConfig.setPrivateKey(key);

    // if the configuration is null
    if (cert.isNull() || key.isNull() || sslConfig.isNull()) {
      throw std::runtime_error("Can't Create QSslConfiguration");
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
    // QFile to read the certificate and key
    QFile certFile(certPath.c_str()), pkeyFile(keyPath.c_str());

    // open the certificate and key
    if(!certFile.open(QIODevice::WriteOnly) || !pkeyFile.open(QIODevice::WriteOnly)) {
      throw std::runtime_error("Can't Create Certificate");
    }

    // generate the certificate and key
    auto sslConfig = utility::functions::getQSslConfiguration();

    // write the certificate and key
    certFile.write(sslConfig.localCertificate().toPem());
    pkeyFile.write(sslConfig.privateKey().toPem());

    // return the configuration
    return sslConfig;
  }

  /**
   * @brief Get the certificate from App Home
   * or generate new one and store it
   */
  QSslConfiguration getSslConfiguration() {
    if (!std::filesystem::exists(certPath) || !std::filesystem::exists(keyPath)) {
      return getNewSslConfiguration();
    } else {
      return getOldSslConfiguration();
    }
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
    bool isDark = scheme == Qt::ColorScheme::Dark;

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
  ui::gui::Content *content;
  ui::gui::Window *window;

 private:  // Disable Copy, Move and Assignment

  Q_DISABLE_COPY_MOVE(ClipbirdApplication);

 public:   // Constructors and Destructors

  /**
   * @brief Construct a new Clipbird Application object
   *
   * @param argc argument count   [unused]
   * @param argv argument vector  [unused]
   */
  ClipbirdApplication(int &argc, char **argv) : SingleApplication(argc, argv) {
    // create the objects of the class
    controller = new controller::ClipBird();

    // set the ssl configuration
    controller->setSslConfiguration(this->getSslConfiguration());

    // create the objects of the class
    content = new ui::gui::Content(controller);
    window  = new ui::gui::Window();

    // set the signal handler for all os
    signal(SIGTERM, [](int sig) { qApp->quit(); });
    signal(SIGINT, [](int sig) { qApp->quit(); });
    signal(SIGABRT, [](int sig) { qApp->quit(); });

    // set initial theme
    setQssFile(QGuiApplication::styleHints()->colorScheme());

    // set not to quit on last content closed
    qApp->setQuitOnLastWindowClosed(false);

    // BackDrop Shadow
    auto shadow = new QGraphicsDropShadowEffect();

    // set the shadow Properties
    shadow->setBlurRadius(10);
    shadow->setOffset(2, 2);
    shadow->setColor(QColor(0, 0, 0, 100));

    // set the shadow to content
    content->setGraphicsEffect(shadow);

    // set the content Ratio
    window->setFixedSize(constants::getAppWindowSize());

    // set the content
    window->setContent(content);

    // set the icon to content
    window->setWindowIcon(QIcon(constants::getAppLogo().c_str()));

    // using some classes
    using ui::gui::Content;

    // tray icon click from content
    const auto signal_tic = &QSystemTrayIcon::activated;
    const auto slot_tic   = &ClipbirdApplication::onTrayIconClicked;
    QObject::connect(content->getTrayIcon(), signal_tic, this, slot_tic);

    // set the signal for instance Started
    const auto signal_is = &SingleApplication::instanceStarted;
    const auto slot_is   = &Content::show;
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
};
}  // namespace srilakshmikanthanp::clipbirdesk

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
  using srilakshmikanthanp::clipbirdesk::constants::getAppHome;
  using srilakshmikanthanp::clipbirdesk::constants::getAppLogFile;
  using srilakshmikanthanp::clipbirdesk::logging::Logger;

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

  // Set the custom message handler
  qInstallMessageHandler(Logger::handler);

  // create the application
  ClipbirdApplication app(argc, argv);

  // start application and return status code
  return app.exec();
}
