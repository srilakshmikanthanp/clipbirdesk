#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

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
 * @return std::string
 */
std::string getAppMajorVersion();

/**
 * @brief Get the Application Version
 * @return std::string
 */
std::string getAppMinorVersion();

/**
 * @brief Get the Application Version
 * @return std::string
 */
std::string getAppPatchVersion();

/**
 * @brief Get the Application Version
 * @return std::string
 */
std::string getAppVersion();

/**
 * @brief App Max History Size
 * @return int
 */
int getAppMaxHistorySize();

/**
 * @brief Get the Application Name
 * @return std::string
 */
std::string getAppName();

/**
 * @brief Get logo path
 * @return std::string
 */
std::string getAppLogo();

/**
 * @brief Get the Application QSS File
 * @return std::string
 */
std::string getAppQSSLight();

/**
 * @brief Get the Application QSS File
 * @return std::string
 */
std::string getAppQSSDark();

/**
 * @brief Get App Home Directory
 * @return std::string
 */
std::string getAppHome();

/**
 * @brief Get App Log File
 */
std::string getAppLogFile();

/**
 * @brief Get the App Home Page
 *
 * @return std::string
 */
std::string getAppHomePage();

/**
 * @brief Get the App Issue Page
 *
 * @return std::string
 */
std::string getAppIssuePage();

/**
 * @brief Get the App Donate Page
 * @return std::string
 */
std::string getAppDonatePage();

/**
 * @brief Get the App Window Ratio
 * @return QSize
 */
QSize getAppWindowSize();

/**
 * @brief Get the MDns Service Name
 *
 * @return std::string
 */
std::string getMDnsServiceName();

/**
 * @brief Get the MDns Service Type
 *
 * @return std::string
 */
std::string getMDnsServiceType();

/**
 * @brief Get the App Org Name object
 *
 * @return std::string
 */
std::string getAppOrgName();

/**
 * @brief Used to get the certificate expiry time should not be less than 2 months
 */
long long getAppCertExpiryInterval();
}  // namespace srilakshmikanthanp::clipbirdesk::config
