#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Qt headers
#include <QObject>
#include <QString>
#include <QSize>
#include <QtQml/qqmlregistration.h>

// project headers
#include "constants/constants.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::qml {

/**
 * @brief QML Constants class that provides application constants accessible from QML
 * This wraps the constants namespace functions and exposes them as QML-callable methods
 */
class ClipbirdQmlConstants : public QObject {
  Q_OBJECT
  QML_ELEMENT
  QML_SINGLETON

public:
  /**
   * @brief Construct a new QmlConstants object
   */
  explicit ClipbirdQmlConstants(QObject* parent = nullptr);

  /**
   * @brief Get the Application Version
   */
  Q_INVOKABLE QString getAppVersion() const;

  /**
   * @brief Get the App UUID
   */
  Q_INVOKABLE QString getAppUUID() const;

  /**
   * @brief App Max History Size
   */
  Q_INVOKABLE int getAppMaxHistorySize() const;

  /**
   * @brief Get the Application Name
   */
  Q_INVOKABLE QString getAppName() const;

  /**
   * @brief Get logo path
   */
  Q_INVOKABLE QString getAppLogo() const;

  /**
   * @brief Get the Application QSS File (Light)
   */
  Q_INVOKABLE QString getAppQSSLight() const;

  /**
   * @brief Get the Application QSS File (Dark)
   */
  Q_INVOKABLE QString getAppQSSDark() const;

  /**
   * @brief Get App Home Directory
   */
  Q_INVOKABLE QString getAppHome() const;

  /**
   * @brief Get App Log File
   */
  Q_INVOKABLE QString getAppLogFile() const;

  /**
   * @brief Get the App Home Page
   */
  Q_INVOKABLE QString getAppHomePage() const;

  /**
   * @brief Get the App Issue Page
   */
  Q_INVOKABLE QString getAppIssuePage() const;

  /**
   * @brief Get the App Donate Page
   */
  Q_INVOKABLE QString getAppDonatePage() const;

  /**
   * @brief Get the App Window Size
   */
  Q_INVOKABLE QSize getAppWindowSize() const;

  /**
   * @brief Get the MDns Service Name
   */
  Q_INVOKABLE QString getMDnsServiceName() const;

  /**
   * @brief Get the MDns Service Type
   */
  Q_INVOKABLE QString getMDnsServiceType() const;

  /**
   * @brief Get the App Org Name
   */
  Q_INVOKABLE QString getAppOrgName() const;

  /**
   * @brief Used to get the certificate expiry time (should not be less than 2 months)
   */
  Q_INVOKABLE qint64 getAppCertExpiryInterval() const;

  /**
   * @brief Used to get the max read idle time
   */
  Q_INVOKABLE qint64 getAppMaxReadIdleTime() const;

  /**
   * @brief Used to get the max write idle time
   */
  Q_INVOKABLE qint64 getAppMaxWriteIdleTime() const;

  /**
   * @brief Used to get Keyboard shortcut for Clipbird history
   */
  Q_INVOKABLE QString getAppHistoryShortcut() const;
};

}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::qml
