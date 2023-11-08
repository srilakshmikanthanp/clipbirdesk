// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "constants.hpp"
#include <iostream>


namespace srilakshmikanthanp::clipbirdesk::constants {
/**
 * @brief Get the Application Version
 * @return std::string
 */
std::string getAppVersion() {
  return std::string(CLIPBIRDESK_VERSION);
}

/**
 * @brief Get the Application Name
 * @return std::string
 */
std::string getAppName() {
  return std::string(CLIPBIRDESK_NAME);
}

/**
 * @brief Get logo path
 * @return std::string
 */
std::string getAppLogo() {
  return std::string(CLIPBIRDESK_LOGO);
}

/**
 * @brief Get the Application QSS File
 * @return std::string
 */
std::string getAppQSSLight() {
  return std::string(APPLICATION_LIGHT_QSS_PATH);
}

/**
 * @brief Get the Application QSS File
 * @return std::string
 */
std::string getAppQSSDark() {
  return std::string(APPLICATION_DARK_QSS_PATH);
}

/**
 * @brief Get App Home Directory
 * @return std::string
 */
std::string getAppHome() {
  return (std::filesystem::path(QDir::homePath().toStdString()) / ("." + getAppName())).string();
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
  return QSize(380, 400);
}

/**
 * @brief Get the App Home Page
 *
 * @return std::string
 */
std::string getAppHomePage() {
  return std::string(CLIPBIRDESK_HOMEPAGE);
}

/**
 * @brief Get the App Issue Page
 *
 * @return std::string
 */
std::string getAppIssuePage() {
  return std::string(CLIPBIRDESK_ISSUEPAGE);
}

/**
 * @brief Get the App Donate Page
 * @return std::string
 */
std::string getAppDonatePage() {
  return std::string(CLIPBIRDESK_DONATE);
}

/**
 * @brief Get the MDns Service Name
 *
 * @return std::string
 */
std::string getMDnsServiceName() {
  return QSysInfo::machineHostName().toStdString();
}

/**
 * @brief Get the MDns Service Type
 *
 * @return std::string
 */
std::string getMDnsServiceType() {
  return std::string("_clipbird._tcp");
}

/**
 * @brief Get the Organization Name
 *
 * @return std::string
 */
std::string getAppOrgName() {
  return std::string(CLIPBIRDESK_ORG_NAME);
}
}  // namespace srilakshmikanthanp::clipbirdesk::config
