#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

// Qt header files
#include <QtLogging>

namespace srilakshmikanthanp::clipbirdesk::logging {
/**
 * @brief Create Qt Logging Message Handler for Logging thar
 * Redirects to Qt Logging to Given File
 *
 * @param file
 *
 * @return message handler
 */
void createFileMessageHandler(const QString &file);
}  // namespace srilakshmikanthanp::clipbirdesk::logging
