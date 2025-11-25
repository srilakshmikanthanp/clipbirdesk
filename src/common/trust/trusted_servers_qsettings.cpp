#include "trusted_servers_qsettings.hpp"

namespace srilakshmikanthanp::clipbirdesk::common::trust {
TrustedServersQSettings::TrustedServersQSettings(QObject* parent): TrustedServers(parent) {}
TrustedServersQSettings::~TrustedServersQSettings() {}

QMap<QString, QByteArray> TrustedServersQSettings::getTrustedServers() {
  QMap<QString, QByteArray> servers;
  settings->beginGroup(trustedServersGroup);
  QStringList keys = settings->allKeys();
  for (const QString& key : keys) {
    servers.insert(key, settings->value(key).toByteArray());
  }
  settings->endGroup();
  return servers;
}

bool TrustedServersQSettings::hasTrustedServer(const QString& name) {
  settings->beginGroup(trustedServersGroup);
  bool exists = settings->contains(name);
  settings->endGroup();
  return exists;
}

bool TrustedServersQSettings::isTrustedServer(const QString& name, const QByteArray& certificate) {
  settings->beginGroup(trustedServersGroup);
  QByteArray storedCert = settings->value(name).toByteArray();
  settings->endGroup();
  return storedCert == certificate;
}

void TrustedServersQSettings::addTrustedServer(const QString& name, const QByteArray& certificate) {
  settings->beginGroup(trustedServersGroup);
  settings->setValue(name, certificate);
  settings->endGroup();
  emit trustedServersChanged(getTrustedServers());
}

void TrustedServersQSettings::removeTrustedServer(const QString& name) {
  settings->beginGroup(trustedServersGroup);
  settings->remove(name);
  settings->endGroup();
  emit trustedServersChanged(getTrustedServers());
}
}
