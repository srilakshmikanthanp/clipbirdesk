#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

#include <QObject>

#include "constants/constants.hpp"

namespace srilakshmikanthanp::clipbirdesk::history {
class ClipboardHistory : public QObject {
 private:  // just for Qt

  Q_OBJECT

 private:

  Q_DISABLE_COPY_MOVE(ClipboardHistory)

 private:

  QVector<QVector<QPair<QString, QByteArray>>> m_history;

 public:  // Constructors and Destructors

  ClipboardHistory(QObject *parent = nullptr);
  virtual ~ClipboardHistory();

 signals:

  void OnHistoryChanged(QVector<QVector<QPair<QString, QByteArray>>>);
  void onClipboard(QVector<QPair<QString, QByteArray>>);

 private:
  bool isEqual(const QVector<QPair<QString, QByteArray>> &a, const QVector<QPair<QString, QByteArray>> &b) const;

 public:  // Member functions

  void addHistory(QVector<QPair<QString, QByteArray>> data);
  QVector<QVector<QPair<QString, QByteArray>>> getHistory() const;
  void deleteHistoryAt(int index);
};
}  // namespace srilakshmikanthanp::clipbirdesk::controller
