#include "history_controller.hpp"

namespace srilakshmikanthanp::clipbirdesk::history {
HistoryController::HistoryController(QObject *parent) : controller::Controller(parent) {}
HistoryController::~HistoryController() = default;

bool HistoryController::isEqual(const QVector<QPair<QString, QByteArray>> &a, const QVector<QPair<QString, QByteArray>> &b) const {
  if (a.size() != b.size()) {
    return false;
  }
  for (int i = 0; i < a.size(); i++) {
    if (a[i].first != b[i].first || a[i].second != b[i].second) {
      return false;
    }
  }
  return true;
}

void HistoryController::addHistory(QVector<QPair<QString, QByteArray>> data) {
  if (!m_history.isEmpty() && isEqual(m_history.first(), data)) {
    return;
  }
  if (this->m_history.size() + 1 > constants::getAppMaxHistorySize()) {
    this->m_history.pop_back();
  }
  emit onClipboard(data);
  this->m_history.push_front(data);
  emit OnHistoryChanged(m_history);
}

void HistoryController::deleteHistoryAt(int index) {
  if (index < 0 || index >= m_history.size()) {
    throw std::runtime_error("Index out of range");
  }
  m_history.remove(index);
  emit OnHistoryChanged(m_history);
}

QVector<QVector<QPair<QString, QByteArray>>> HistoryController::getHistory() const {
  return m_history;
}
}
