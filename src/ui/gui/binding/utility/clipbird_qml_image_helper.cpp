#include "clipbird_qml_image_helper.hpp"
#include "ui/gui/binding/image/clipbird_qml_image_provider.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::qml {

// ClipbirdQmlImageHelper implementation
ClipbirdQmlImageHelper::ClipbirdQmlImageHelper(QObject* parent)
  : QObject(parent) {
}

ClipbirdQmlImageHelper::~ClipbirdQmlImageHelper() = default;

QString ClipbirdQmlImageHelper::imageUrl(const QByteArray& data) const {
  return QString("image://clipbird/%1").arg(ClipbirdQmlImageProvider::storeImage(data));
}

ClipbirdQmlImageHelper* ClipbirdQmlImageHelper::create(QQmlEngine* engine, QJSEngine* scriptEngine) {
  static ClipbirdQmlImageHelper* instance = new ClipbirdQmlImageHelper();
  Q_UNUSED(engine);
  Q_UNUSED(scriptEngine);
  return instance;
}

}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::qml
