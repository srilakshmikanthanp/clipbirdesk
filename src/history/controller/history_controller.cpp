#include "history_controller.hpp"

namespace srilakshmikanthanp::clipbirdesk::controller {
HistoryController::HistoryController(QObject *parent) : Controller(parent) {}
HistoryController::~HistoryController() = default;

void HistoryController::addHistory(QVector<QPair<QString, QByteArray>> data) {
  if (this->m_history.size() + 1 > constants::getAppMaxHistorySize()) {
    this->m_history.pop_back();
  }
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
