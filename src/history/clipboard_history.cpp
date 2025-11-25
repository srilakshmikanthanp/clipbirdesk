#include "clipboard_history.hpp"

namespace srilakshmikanthanp::clipbirdesk::history {
ClipboardHistory::ClipboardHistory(QObject *parent) : QObject(parent) {}
ClipboardHistory::~ClipboardHistory() = default;

bool ClipboardHistory::isEqual(const QVector<QPair<QString, QByteArray>> &a, const QVector<QPair<QString, QByteArray>> &b) const {
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

void ClipboardHistory::addHistory(QVector<QPair<QString, QByteArray>> data) {
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

void ClipboardHistory::deleteHistoryAt(int index) {
  if (index < 0 || index >= m_history.size()) {
    throw std::runtime_error("Index out of range");
  }
  m_history.remove(index);
  emit OnHistoryChanged(m_history);
}

QVector<QVector<QPair<QString, QByteArray>>> ClipboardHistory::getHistory() const {
  return m_history;
}
}
