#include "clipbird_qml_hash_helper.hpp"

#include <QSslCertificate>

namespace srilakshmikanthanp::clipbirdesk::ui::gui::qml {
ClipbirdQmlHashHelper::ClipbirdQmlHashHelper(QObject* parent): QObject(parent) {}

QString ClipbirdQmlHashHelper::sha256(const QString& data) {
  QByteArray der = QByteArray::fromBase64(data.toUtf8());
  QByteArray hash = QCryptographicHash::hash(der, QCryptographicHash::Sha256);
  return hash.toHex(':').toUpper();
}
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::qml
