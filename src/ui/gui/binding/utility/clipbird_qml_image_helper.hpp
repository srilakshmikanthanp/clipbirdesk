#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Qt headers
#include <QObject>
#include <QImage>
#include <QtQml/qqmlregistration.h>
#include <QQmlEngine>
#include <QJSEngine>

namespace srilakshmikanthanp::clipbirdesk::ui::gui::qml {

/**
 * @brief QML binding helper for image operations
 */
class ClipbirdQmlImageHelper : public QObject {
  Q_OBJECT
  QML_ELEMENT
  QML_SINGLETON

 public:
  /**
   * @brief Construct a new ClipbirdQmlImageHelper object
   * @param parent Parent QObject
   */
  explicit ClipbirdQmlImageHelper(QObject* parent = nullptr);

  /**
   * @brief Destroy the ClipbirdQmlImageHelper object
   */
  virtual ~ClipbirdQmlImageHelper();

  /**
   * @brief Convert byte array to image URL that QML can use
   * @param data Byte array containing PNG image data
   * @return QString URL in format "image://clipbird/id"
   */
  Q_INVOKABLE QString imageUrl(const QByteArray& data) const;

  /**
   * @brief QML singleton factory function
   * @param engine QML engine
   * @param scriptEngine JS engine
   * @return ClipbirdQmlImageHelper* Singleton instance
   */
  static ClipbirdQmlImageHelper* create(QQmlEngine* engine, QJSEngine* scriptEngine);
};

}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::qml
