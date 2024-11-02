#include "constants.hpp"
#include <iostream>

namespace srilakshmikanthanp::clipbirdesk::constants {
/**
 * @brief Get the Application Version
 * @return const char*
 */
const char* getAppVersion() {
  return CLIPBIRDESK_VERSION;
}

/**
 * @brief Get the App UUID
 */
const char* getAppUUID() {
  return CLIPBIRDESK_UUID;
}

/**
 * @brief App Max History Size
 * @return int
 */
int getAppMaxHistorySize() {
  return 20;
}

/**
 * @brief Get the Application Name
 * @return const char*
 */
const char* getAppName() {
  return CLIPBIRDESK_NAME;
}

/**
 * @brief Get logo path
 * @return const char*
 */
const char* getAppLogo() {
  return CLIPBIRDESK_LOGO;
}

/**
 * @brief Get the Application QSS File
 * @return const char*
 */
const char* getAppQSSLight() {
  return APPLICATION_LIGHT_QSS_PATH;
}

/**
 * @brief Get the Application QSS File
 * @return const char*
 */
const char* getAppQSSDark() {
  return APPLICATION_DARK_QSS_PATH;
}

/**
 * @brief Get App Home Directory
 * @return const char*
 */
std::string getAppHome() {
  return (std::filesystem::path(QDir::homePath().toStdString()) / (std::string(".") + getAppName())).string();
}

/**
 * @brief Get App Log File
 */
std::string getAppLogFile() {
  return (std::filesystem::path(getAppHome()) / "clipbird.log").string();
}

/**
 * @brief Get the App Window Size
 * @return QSize
 */
QSize getAppWindowSize() {
  return QSize(350, 400);
}

/**
 * @brief Get the App Home Page
 *
 * @return const char*
 */
const char* getAppHomePage() {
  return CLIPBIRDESK_HOMEPAGE;
}

/**
 * @brief Get the App Issue Page
 *
 * @return const char*
 */
const char* getAppIssuePage() {
  return CLIPBIRDESK_ISSUEPAGE;
}

/**
 * @brief Get the App Donate Page
 * @return const char*
 */
const char* getAppDonatePage() {
  return CLIPBIRDESK_DONATE;
}

/**
 * @brief Get the MDns Service Name
 *
 * @return const char*
 */
std::string getMDnsServiceName() {
  return QSysInfo::machineHostName().toStdString();
}

/**
 * @brief Get the MDns Service Type
 *
 * @return const char*
 */
const char* getMDnsServiceType() {
  return "_clipbird._tcp";
}

/**
 * @brief Get the Organization Name
 *
 * @return const char*
 */
const char* getAppOrgName() {
  return CLIPBIRDESK_ORG_NAME;
}

/**
 * @brief Used to get the certificate expiry time should not be less than 2 months
 */
long long getAppCertExpiryInterval() {
  return 60LL * 60LL * 24LL * 60LL * 1000LL;
}

/**
 *  @brief Used to get the max read idle time
 */
long long getAppMaxReadIdleTime() {
  return 60 * 1000;
}

/**
 * @brief Used to get the max write idle time
 */
long long getAppMaxWriteIdleTime() {
  return 10 * 1000;
}

/**
 * @brief Used to get Keyboard shortcut for Clipbird history
 */
const char* getAppHistoryShortcut()  {
  return "Ctrl+Alt+C";
}
}  // namespace srilakshmikanthanp::clipbirdesk::config
