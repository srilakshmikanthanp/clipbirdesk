// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "logging.hpp"

namespace srilakshmikanthanp::clipbirdesk::logging {
/**
 * @brief Custom Logger message handler for Qt
 */
void Logger::handler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
  auto *function = context.function ? context.function : "";
  auto *file     = context.file ? context.file : "";
  logs->write(QObject::tr("%1 | %2 | at line : %3 : %4 : %5()-> %6\n")
                  .arg(QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss"))
                  .arg(Logger::contextNames.value(type))
                  .arg(context.line)
                  .arg(QFileInfo(QString(file)).baseName())
                  .arg(QString(context.function))
                  .arg(msg)
                  .toLocal8Bit());
}

/**
 * @brief Set the log file
 */
void Logger::setLogFile(QFile *file) {
  Logger::logs = file;
}
}  // namespace srilakshmikanthanp::clipbirdesk::logging
