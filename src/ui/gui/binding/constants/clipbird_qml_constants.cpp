#include "clipbird_qml_constants.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::qml {

/**
 * @brief Construct a new QmlConstants object
 */
ClipbirdQmlConstants::ClipbirdQmlConstants(QObject* parent) : QObject(parent) {}

/**
 * @brief Get the Application Version
 */
QString ClipbirdQmlConstants::getAppVersion() const {
  return QString(constants::getAppVersion());
}

/**
 * @brief Get the App UUID
 */
QString ClipbirdQmlConstants::getAppUUID() const {
  return QString(constants::getAppUUID());
}

/**
 * @brief App Max History Size
 */
int ClipbirdQmlConstants::getAppMaxHistorySize() const {
  return constants::getAppMaxHistorySize();
}

/**
 * @brief Get the Application Name
 */
QString ClipbirdQmlConstants::getAppName() const {
  return QString(constants::getAppName());
}

/**
 * @brief Get logo path
 */
QString ClipbirdQmlConstants::getAppLogo() const {
  return QString(constants::getAppLogo());
}

/**
 * @brief Get the Application QSS File (Light)
 */
QString ClipbirdQmlConstants::getAppQSSLight() const {
  return QString(constants::getAppQSSLight());
}

/**
 * @brief Get the Application QSS File (Dark)
 */
QString ClipbirdQmlConstants::getAppQSSDark() const {
  return QString(constants::getAppQSSDark());
}

/**
 * @brief Get App Home Directory
 */
QString ClipbirdQmlConstants::getAppHome() const {
  return QString::fromStdString(constants::getAppHome());
}

/**
 * @brief Get App Log File
 */
QString ClipbirdQmlConstants::getAppLogFile() const {
  return QString::fromStdString(constants::getAppLogFile());
}

/**
 * @brief Get the App Home Page
 */
QString ClipbirdQmlConstants::getAppHomePage() const {
  return QString(constants::getAppHomePage());
}

/**
 * @brief Get the App Issue Page
 */
QString ClipbirdQmlConstants::getAppIssuePage() const {
  return QString(constants::getAppIssuePage());
}

/**
 * @brief Get the App Donate Page
 */
QString ClipbirdQmlConstants::getAppDonatePage() const {
  return QString(constants::getAppDonatePage());
}

/**
 * @brief Get the App Window Size
 */
QSize ClipbirdQmlConstants::getAppWindowSize() const {
  return constants::getAppWindowSize();
}

/**
 * @brief Get the MDns Service Name
 */
QString ClipbirdQmlConstants::getMDnsServiceName() const {
  return constants::getMDnsServiceName();
}

/**
 * @brief Get the MDns Service Type
 */
QString ClipbirdQmlConstants::getMDnsServiceType() const {
  return constants::getMDnsServiceType();
}

/**
 * @brief Get the App Org Name
 */
QString ClipbirdQmlConstants::getAppOrgName() const {
  return QString(constants::getAppOrgName());
}

/**
 * @brief Used to get the certificate expiry time (should not be less than 2 months)
 */
qint64 ClipbirdQmlConstants::getAppCertExpiryInterval() const {
  return constants::getAppCertExpiryInterval();
}

/**
 * @brief Used to get the max read idle time
 */
qint64 ClipbirdQmlConstants::getAppMaxReadIdleTime() const {
  return constants::getAppMaxReadIdleTime();
}

/**
 * @brief Used to get the max write idle time
 */
qint64 ClipbirdQmlConstants::getAppMaxWriteIdleTime() const {
  return constants::getAppMaxWriteIdleTime();
}

/**
 * @brief Used to get Keyboard shortcut for Clipbird history
 */
QString ClipbirdQmlConstants::getAppHistoryShortcut() const {
  return QString(constants::getAppHistoryShortcut());
}

}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::qml
