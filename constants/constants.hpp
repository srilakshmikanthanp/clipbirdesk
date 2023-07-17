#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

// Qt headers
#include <QSize>

// C++ headers
#include <filesystem>
#include <string>

// project headers
#include "config/config.hpp"

namespace srilakshmikanthanp::clipbirdesk::config {
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
 * @brief Get the Application Name
 * @return std::string
 */
std::string getAppName();

/**
 * @brief Get App Home Directory
 * @return std::string
 */
std::string getAppHome();

/**
 * @brief Get the App Window Ratio
 * @return QSize
 */
QSize getAppWindowRatio();
}  // namespace srilakshmikanthanp::clipbirdesk::config
