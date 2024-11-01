#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Qt header files
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QHash>
#include <QMutex>
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
  inline static QTextStream *logs = Q_NULLPTR;

  /// @brief mutex to lock the file
  inline static QMutex mutex;

 public:

  /**
   * @brief Custom Logger message handler for Qt
   */
  static void handler(
    QtMsgType type,                      // type of the message
    const QMessageLogContext &context,   // context of the message
    const QString &msg                   // message
  );

  /**
   * @brief Set the log file
   */
  static void setLogStream(QTextStream *file);
};
}  // namespace srilakshmikanthanp::clipbirdesk::logging
