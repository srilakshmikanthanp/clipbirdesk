#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Qt headers
#include <QObject>
#include <QString>
#include <QVariantList>
#include <QtQml/qqmlregistration.h>
#include <QQmlEngine>
#include <QJSEngine>

// project headers
#include "clipboard/applicationclipboard.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::qml {

/**
 * @brief QML binding for ApplicationClipboard that exposes clipboard functionality to QML
 * This wraps the ApplicationClipboard and makes it accessible from QML as a singleton
 */
class ClipbirdQmlApplicationClipboard : public QObject {
  Q_OBJECT
  QML_ELEMENT
  QML_SINGLETON

 public:
  static constexpr const char* MIME_DATA_KEY = "mimeType";
  static constexpr const char* DATA_KEY = "data";

 private:
  clipboard::ApplicationClipboard* m_applicationClipboard = nullptr;

 signals:
  /**
   * @brief Signal emitted when clipboard content changes
   */
  void clipboardChanged();

 public:
  /**
   * @brief Construct a new ClipbirdQmlApplicationClipboard object
   * @param applicationClipboard Pointer to ApplicationClipboard implementation
   * @param parent Parent QObject
   */
  explicit ClipbirdQmlApplicationClipboard(clipboard::ApplicationClipboard* applicationClipboard, QObject* parent = nullptr);

  /**
   * @brief Destroy the ClipbirdQmlApplicationClipboard object
   */
  virtual ~ClipbirdQmlApplicationClipboard();

  /**
   * @brief Set clipboard content from QML
   * @param items QVariantList containing QVariantMaps with "mimeType" and "data" keys
   */
  Q_INVOKABLE void setClipboard(const QVariantList& items);

  /**
   * @brief Get clipboard content as QVariantList
   * Each item is a QVariantMap containing "mimeType" and "data" keys
   * @return QVariantList Clipboard content
   */
  Q_INVOKABLE QVariantList getClipboard() const;

  /**
   * @brief Clear clipboard content
   */
  Q_INVOKABLE void clearClipboard();

  /**
   * @brief QML singleton factory function
   * @param engine QML engine
   * @param scriptEngine JS engine
   * @return ClipbirdQmlApplicationClipboard* Singleton instance
   */
  static ClipbirdQmlApplicationClipboard* create(QQmlEngine* engine, QJSEngine* scriptEngine);
};

}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::qml
