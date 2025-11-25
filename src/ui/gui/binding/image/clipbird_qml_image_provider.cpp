#include "clipbird_qml_image_provider.hpp"

#include <QBuffer>
#include <QImageReader>

namespace srilakshmikanthanp::clipbirdesk::ui::gui::qml {
// ClipbirdQmlImageProvider implementation
ClipbirdQmlImageProvider::ClipbirdQmlImageProvider(): QQuickImageProvider(QQuickImageProvider::Pixmap) {
}

QPixmap ClipbirdQmlImageProvider::requestPixmap(const QString& id, QSize* size, const QSize& requestedSize) {
  QByteArray data = s_imageCache.value(id);

  if (data.isEmpty()) {
    return QPixmap();
  }

  QImage image;
  image.loadFromData(data, "PNG");

  if (image.isNull()) {
    return QPixmap();
  }

  if (requestedSize.isValid() && requestedSize.width() > 0 && requestedSize.height() > 0) {
    image = image.scaled(requestedSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
  }

  if (size) {
    *size = image.size();
  }

  return QPixmap::fromImage(image);
}

QString ClipbirdQmlImageProvider::storeImage(const QByteArray& data) {
  QString id = QString::number(s_imageCounter++);
  s_imageCache[id] = data;
  return id;
}

}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::qml
