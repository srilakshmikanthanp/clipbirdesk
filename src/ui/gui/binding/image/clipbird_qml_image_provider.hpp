#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Qt headers
#include <QQuickImageProvider>
#include <QPixmap>
#include <QImage>
#include <QSize>
#include <QString>
#include <QHash>
#include <QByteArray>

namespace srilakshmikanthanp::clipbirdesk::ui::gui::qml {

/**
 * @brief Image provider that converts byte arrays to QPixmap for QML
 * This allows QML to load images from byte arrays using image:// URL scheme
 */
class ClipbirdQmlImageProvider : public QQuickImageProvider {
 private:
  static inline QHash<QString, QByteArray> s_imageCache;
  static inline int s_imageCounter;

 public:
  ClipbirdQmlImageProvider();
  QPixmap requestPixmap(const QString& id, QSize* size, const QSize& requestedSize) override;
  static QString storeImage(const QByteArray& data);
};

}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::qml
