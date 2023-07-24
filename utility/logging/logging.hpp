#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

// Qt header files
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QHash>
#include <QObject>
#include <QtLogging>

namespace srilakshmikanthanp::clipbirdesk::logging {
class Logger {
 private:

  /// @brief Context names for the logger
  inline static QHash<QtMsgType, QString> contextNames{
      {   QtMsgType::QtDebugMsg, "Debug    "},
      {    QtMsgType::QtInfoMsg, "Info     "},
      { QtMsgType::QtWarningMsg, "Warning  "},
      {QtMsgType::QtCriticalMsg, "Critical "},
      {   QtMsgType::QtFatalMsg, "Fatal    "}
  };

  /// @brief log file to log the messages
  inline static QFile *logs   = Q_NULLPTR;

 public:
  /**
   * @brief Custom Logger message handler for Qt
   */
  static void handler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

  /**
   * @brief Set the log file
   */
  static void setLogFile(QFile *file);
};
}  // namespace srilakshmikanthanp::clipbirdesk::logging
