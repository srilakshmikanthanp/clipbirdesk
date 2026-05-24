#include "clipbird_qml_ssl_config.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::qml {

ClipbirdQmlSslConfig::ClipbirdQmlSslConfig(QObject* parent) : QObject(parent) {}

ClipbirdQmlSslConfig::~ClipbirdQmlSslConfig() = default;

ClipbirdQmlSslConfig* ClipbirdQmlSslConfig::create(QQmlEngine* engine, QJSEngine* scriptEngine) {
  static ClipbirdQmlSslConfig* instance = new ClipbirdQmlSslConfig();
  Q_UNUSED(engine);
  Q_UNUSED(scriptEngine);
  return instance;
}

QVariantMap ClipbirdQmlSslConfig::getHostSslConfig() const {
  const auto config = common::types::SslConfigFactory::getHostSslConfig();
  QVariantMap result;
  result[privateKeyKey] = QString::fromUtf8(config.privateKey.toBase64());
  result[certificateKey] = QString::fromUtf8(config.certificate.toBase64());
  return result;
}

}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::qml
