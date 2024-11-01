#include "logging.hpp"

namespace srilakshmikanthanp::clipbirdesk::logging {
/**
 * @brief Custom Logger message handler for Qt
 */
void Logger::handler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
  // if log file is not set
  if (!logs) throw std::runtime_error("Log file is not set");

  auto *function = context.function ? context.function : "";
  auto *file     = context.file ? context.file : "";
  auto message   = QObject::tr("%1 | %2 | at line : %3 : %4 : %5()-> %6\n")
                     .arg(QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss"))
                     .arg(Logger::contextNames.value(type))
                     .arg(context.line)
                     .arg(QFileInfo(QString(file)).baseName())
                     .arg(QString(function))
                     .arg(msg)
                     .toLocal8Bit();

  mutex.lock();                     // lock the file so another thread can't write to it
  (*logs) << message << Qt::endl;   // write the message to the log file
  mutex.unlock();                   // unlock the file so another thread can write to it
}

/**
 * @brief Set the log file
 */
void Logger::setLogStream(QTextStream *file) {
  Logger::logs = file;
}
}  // namespace srilakshmikanthanp::clipbirdesk::logging
