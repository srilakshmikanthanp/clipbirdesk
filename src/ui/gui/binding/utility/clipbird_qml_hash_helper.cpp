#include "clipbird_qml_hash_helper.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::qml {
ClipbirdQmlHashHelper::ClipbirdQmlHashHelper(QObject* parent): QObject(parent) {}

QString ClipbirdQmlHashHelper::sha256(const QString& data) {
  QByteArray hash = QCryptographicHash::hash(QByteArray::fromBase64(data.toUtf8()), QCryptographicHash::Sha256);
  QStringList hashParts;
  for (auto byte : hash) {
    hashParts.append(QString("%1").arg(static_cast<unsigned char>(byte), 2, 16, QChar('0')).toUpper());
  }
  return hashParts.join(":");
}
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::qml
