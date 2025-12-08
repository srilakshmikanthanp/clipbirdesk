#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Qt headers
#include <QObject>
#include <QString>
#include <QQmlEngine>
#include <QtQml/qqmlregistration.h>

// project headers
#include "syncing/client_server.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::qml {

/**
 * @brief QML binding for ClientServer that exposes server information to QML
 * This wraps the ClientServer interface and makes it accessible from QML
 */
class ClipbirdQmlClientServer : public QObject {
  Q_OBJECT
  QML_ELEMENT
  QML_UNCREATABLE("ClipbirdQmlClientServer objects are created by C++")

  Q_PROPERTY(QString name READ getName CONSTANT)

 private:
  syncing::ClientServer* m_clientServer = nullptr;

 public:
  explicit ClipbirdQmlClientServer(syncing::ClientServer* clientServer, QObject* parent = nullptr);
  virtual ~ClipbirdQmlClientServer();

  /**
   * @brief Get server name
   * @return QString Server name
   */
  Q_INVOKABLE QString getName() const;

  /**
   * @brief Get the underlying ClientServer pointer
   * @return syncing::ClientServer* ClientServer pointer
   */
  syncing::ClientServer* getClientServer() const;
};

}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::qml
