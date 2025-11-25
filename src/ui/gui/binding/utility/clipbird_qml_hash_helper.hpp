#include <QCryptographicHash>
#include <QObject>
#include <QString>
#include <QtQml/qqmlregistration.h>

namespace srilakshmikanthanp::clipbirdesk::ui::gui::qml {
class ClipbirdQmlHashHelper : public QObject {
  Q_OBJECT
  QML_ELEMENT
  QML_SINGLETON

 public:
  ClipbirdQmlHashHelper(QObject* parent = nullptr);
  Q_INVOKABLE QString sha256(const QString &data);
};
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::qml
