#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// project headers
#include "controller/controller.hpp"

namespace srilakshmikanthanp::clipbirdesk::history {
class HistoryController : public controller::Controller {
 private:  // just for Qt

  Q_OBJECT

 private:

  Q_DISABLE_COPY_MOVE(HistoryController)

 private:

  QVector<QVector<QPair<QString, QByteArray>>> m_history;

 public:  // Constructors and Destructors

  HistoryController(QObject *parent = nullptr);
  virtual ~HistoryController();

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
