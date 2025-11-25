#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Qt headers
#include <QObject>
#include <QString>
#include <QQmlEngine>
#include <QtQml/qqmlregistration.h>

// project headers
#include "syncing/session.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::qml {

/**
 * @brief QML binding for Session that exposes session information to QML
 * This wraps the Session interface and makes it accessible from QML
 */
class ClipbirdQmlSession : public QObject {
  Q_OBJECT
  QML_ELEMENT
  QML_UNCREATABLE("ClipbirdQmlSession objects are created by C++")

  Q_PROPERTY(QString name READ getName CONSTANT)
  Q_PROPERTY(QString certificate READ getCertificate CONSTANT)
  Q_PROPERTY(bool isTrusted READ isTrusted NOTIFY trustedStateChanged)

 private:
  syncing::Session* m_session = nullptr;

 private:
  void handleTrustedStateChanged(bool isTrusted);

 signals:
  void trustedStateChanged(bool isTrusted);

 public:
  explicit ClipbirdQmlSession(syncing::Session* session, QObject* parent = nullptr);
  virtual ~ClipbirdQmlSession();

  /**
   * @brief Get session name
   * @return QString Session name
   */
  Q_INVOKABLE QString getName() const;

  /**
   * @brief Get session certificate as base64
   * @return QString Certificate in base64 format
   */
  Q_INVOKABLE QString getCertificate() const;

  /**
   * @brief Check if session is trusted
   * @return bool true if trusted, false otherwise
   */
  Q_INVOKABLE bool isTrusted() const;

  /**
   * @brief Disconnect the session
   */
  Q_INVOKABLE void disconnect();

  /**
   * @brief Get the underlying session pointer
   * @return syncing::Session* Session pointer
   */
  syncing::Session* getSession() const;
};

}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::qml
