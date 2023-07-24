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
std::string getAppMajorVersion() {
  return std::string(CLIPBIRDESK_VERSION_MAJOR);
}

/**
 * @brief Get the Application Version
 * @return std::string
 */
std::string getAppMinorVersion() {
  return std::string(CLIPBIRDESK_VERSION_MINOR);
}

/**
 * @brief Get the Application Version
 * @return std::string
 */
std::string getAppPatchVersion() {
  return std::string(CLIPBIRDESK_VERSION_PATCH);
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
std::string getAppQSS() {
  return std::string(CLIPBIRDESK_QSS);
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
 * @brief Get the App Window Ratio
 * @return QSize
 */
auto getAppWindowRatio() -> QSize {
  return QSize(4, 2);
}

/**
 * @brief Get the MDns Service Name
 *
 * @return std::string
 */
std::string getMDnsServiceName() {
  std::cout << QSysInfo::machineHostName().toStdString() << std::endl;
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
}  // namespace srilakshmikanthanp::clipbirdesk::config
