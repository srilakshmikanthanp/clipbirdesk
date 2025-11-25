#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Qt headers
#include <QSysInfo>
#include <QSize>

// C++ headers
#include <filesystem>
#include <QDir>
#include <string>

// project headers
#include "config/config.hpp"

namespace srilakshmikanthanp::clipbirdesk::constants {
/**
 * @brief Get the Application Version
 * @return const char*
 */
const char* getAppVersion();

/**
 * @brief Get the App UUID
 */
const char* getAppUUID();

/**
 * @brief App Max History Size
 * @return int
 */
int getAppMaxHistorySize();

/**
 * @brief Get the Application Name
 * @return const char*
 */
const char* getAppName();

/**
 * @brief Get logo path
 * @return const char*
 */
const char* getAppLogo();

/**
 * @brief Get the Application QSS File
 * @return const char*
 */
const char* getAppQSSLight();

/**
 * @brief Get the Application QSS File
 * @return const char*
 */
const char* getAppQSSDark();

/**
 * @brief Get App Home Directory
 * @return const char*
 */
std::string getAppHome();

/**
 * @brief Get App Log File
 */
std::string getAppLogFile();

/**
 * @brief Get the App Home Page
 *
 * @return const char*
 */
const char* getAppHomePage();

/**
 * @brief Get the App Issue Page
 *
 * @return const char*
 */
const char* getAppIssuePage();

/**
 * @brief Get the App Donate Page
 * @return const char*
 */
const char* getAppDonatePage();

/**
 * @brief Get the App Window Ratio
 * @return QSize
 */
QSize getAppWindowSize();

/**
 * @brief Get the MDns Service Name
 *
 * @return const char*
 */
QString getMDnsServiceName();

/**
 * @brief Get the MDns Service Type
 *
 * @return const char*
 */
QString getMDnsServiceType();

/**
 * @brief Get the App Org Name object
 *
 * @return const char*
 */
const char* getAppOrgName();

/**
 * @brief Used to get the certificate expiry time should not be less than 2 months
 */
long long getAppCertExpiryInterval();

/**
 *  @brief Used to get the max read idle time
 */
long long getAppMaxReadIdleTime();

/**
 * @brief Used to get the max write idle time
 */
long long getAppMaxWriteIdleTime();

/**
 * @brief Used to get Keyboard shortcut for Clipbird history
 */
const char* getAppHistoryShortcut();
}  // namespace srilakshmikanthanp::clipbirdesk::config
