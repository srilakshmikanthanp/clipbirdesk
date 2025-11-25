#include "clipbird_qml_history.hpp"

#include "ui/gui/binding/clipboard/clipbird_qml_application_clipboard.hpp"
#include "history/clipboard_history_factory.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::qml {
QVariantList ClipbirdQmlHistory::toQVariantList(const QVector<QVector<QPair<QString, QByteArray>>>& history) const {
  QVariantList result;
  for (const auto& items : history) {
    QVariantList itemList;
    for (const auto& [mimeType, data] : items) {
      QVariantMap itemMap;
      itemMap[ClipbirdQmlApplicationClipboard::MIME_DATA_KEY] = mimeType;
      itemMap[ClipbirdQmlApplicationClipboard::DATA_KEY] = data;
      itemList.append(itemMap);
    }
    result.append(QVariant::fromValue(itemList));
  }
  return result;
}

void ClipbirdQmlHistory::handleHistoryChanged(QVector<QVector<QPair<QString, QByteArray>>> history) {
  emit historyChanged(toQVariantList(history));
}

ClipbirdQmlHistory::ClipbirdQmlHistory(history::ClipboardHistory* clipboardHistory, QObject* parent): QObject(parent), m_clipboardHistory(clipboardHistory) {
  connect(
    m_clipboardHistory,
    &history::ClipboardHistory::OnHistoryChanged,
    this,
    &ClipbirdQmlHistory::handleHistoryChanged
  );
}

ClipbirdQmlHistory::~ClipbirdQmlHistory() = default;

ClipbirdQmlHistory* ClipbirdQmlHistory::create(QQmlEngine* engine, QJSEngine* scriptEngine) {
  static ClipbirdQmlHistory* instance = new ClipbirdQmlHistory(history::ClipboardHistoryFactory::getClipboardHistory());
  Q_UNUSED(engine);
  Q_UNUSED(scriptEngine);
  return instance;
}

QVariantList ClipbirdQmlHistory::getHistory() const {
  return toQVariantList(m_clipboardHistory->getHistory());
}

void ClipbirdQmlHistory::deleteHistoryAt(int index) {
  m_clipboardHistory->deleteHistoryAt(index);
}

}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::qml
