#include "clipbird_qml_application_clipboard.hpp"

#include "clipboard/application_clipboard_factory.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::qml {

ClipbirdQmlApplicationClipboard::ClipbirdQmlApplicationClipboard(clipboard::ApplicationClipboard* applicationClipboard, QObject* parent): QObject(parent), m_applicationClipboard(applicationClipboard) {
  connect(
    m_applicationClipboard,
    &clipboard::ApplicationClipboard::OnClipboardChange,
    this,
    &ClipbirdQmlApplicationClipboard::clipboardChanged
  );
}

ClipbirdQmlApplicationClipboard::~ClipbirdQmlApplicationClipboard() = default;

ClipbirdQmlApplicationClipboard* ClipbirdQmlApplicationClipboard::create(QQmlEngine* engine, QJSEngine* scriptEngine) {
  static ClipbirdQmlApplicationClipboard* instance = new ClipbirdQmlApplicationClipboard(clipboard::ApplicationClipboardFactory::getApplicationClipboard());
  Q_UNUSED(engine);
  Q_UNUSED(scriptEngine);
  return instance;
}

void ClipbirdQmlApplicationClipboard::setClipboard(const QVariantList& items) {
  QVector<QPair<QString, QByteArray>> clipboardData;

  for (const auto& item : items) {
    QVariantMap itemMap = item.toMap();
    QString mimeType = itemMap[MIME_DATA_KEY].toString();
    QByteArray data = itemMap[DATA_KEY].toByteArray();
    clipboardData.append(qMakePair(mimeType, data));
  }

  m_applicationClipboard->set(clipboardData);
}

QVariantList ClipbirdQmlApplicationClipboard::getClipboard() const {
  QFuture<QVector<QPair<QString, QByteArray>>> futureClipboardData = m_applicationClipboard->get();
  QVector<QPair<QString, QByteArray>> clipboardData = futureClipboardData.result();
  QVariantList result;

  for (const auto& [mimeType, data] : clipboardData) {
    QVariantMap itemMap;
    itemMap[MIME_DATA_KEY] = mimeType;
    itemMap[DATA_KEY] = data;
    result.append(itemMap);
  }

  return result;
}

void ClipbirdQmlApplicationClipboard::clearClipboard() {
  m_applicationClipboard->clear();
}

}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::qml
