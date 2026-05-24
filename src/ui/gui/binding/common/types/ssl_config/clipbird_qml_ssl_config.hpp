#pragma once

#include <QJSEngine>
#include <QObject>
#include <QQmlEngine>
#include <QString>
#include <QVariantMap>
#include <QtQml/qqmlregistration.h>

#include "common/types/ssl_config/ssl_config_factory.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::qml {

class ClipbirdQmlSslConfig : public QObject {
  Q_OBJECT
  QML_ELEMENT
  QML_SINGLETON

 private:
  static constexpr const char* privateKeyKey = "privateKey";
  static constexpr const char* certificateKey = "certificate";

 public:
  explicit ClipbirdQmlSslConfig(QObject* parent = nullptr);
  virtual ~ClipbirdQmlSslConfig();

  static ClipbirdQmlSslConfig* create(QQmlEngine* engine, QJSEngine* scriptEngine);

  Q_INVOKABLE QVariantMap getHostSslConfig() const;
};

}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::qml
