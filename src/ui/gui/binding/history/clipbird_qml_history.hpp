#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Qt headers
#include <QObject>
#include <QString>
#include <QVariantList>
#include <QtQml/qqmlregistration.h>
#include <QQmlEngine>
#include <QJSEngine>

// project headers
#include "history/clipboard_history.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::qml {

/**
 * @brief QML binding for ClipboardHistory that exposes history functionality to QML
 * This wraps the ClipboardHistory and makes it accessible from QML as a singleton
 */
class ClipbirdQmlHistory : public QObject {
  Q_OBJECT
  QML_ELEMENT
  QML_SINGLETON

  Q_PROPERTY(QVariantList history READ getHistory NOTIFY historyChanged)

 private:
  history::ClipboardHistory* m_clipboardHistory = nullptr;

 private:
  QVariantList toQVariantList(const QVector<QVector<QPair<QString, QByteArray>>>& history) const;

 private:
  void handleHistoryChanged(QVector<QVector<QPair<QString, QByteArray>>> history);

 signals:
  void historyChanged(QVariantList);

 public:
  /**
   * @brief Construct a new ClipbirdQmlHistory object
   * @param clipboardHistory Pointer to ClipboardHistory implementation
   * @param parent Parent QObject
   */
  explicit ClipbirdQmlHistory(history::ClipboardHistory* clipboardHistory, QObject* parent = nullptr);

  /**
   * @brief Destroy the ClipbirdQmlHistory object
   */
  virtual ~ClipbirdQmlHistory();

  /**
   * @brief Get clipboard history as QVariantList
   * Each item is a QVariantList containing QVariantMaps with "mimeType" and "data" keys
   * @return QVariantList History items
   */
  Q_INVOKABLE QVariantList getHistory() const;

  /**
   * @brief Delete history item at specified index
   * @param index Index of item to delete
   */
  Q_INVOKABLE void deleteHistoryAt(int index);

  /**
   * @brief QML singleton factory function
   * @param engine QML engine
   * @param scriptEngine JS engine
   * @return ClipbirdQmlHistory* Singleton instance
   */
  static ClipbirdQmlHistory* create(QQmlEngine* engine, QJSEngine* scriptEngine);
};

}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::qml
