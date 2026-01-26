#include "trusted_servers_qsettings.hpp"

namespace srilakshmikanthanp::clipbirdesk::common::trust {
TrustedServersQSettings::TrustedServersQSettings(QObject* parent): TrustedServers(parent) {}
TrustedServersQSettings::~TrustedServersQSettings() {}

QList<TrustedServer> TrustedServersQSettings::getTrustedServers() {
  QList<TrustedServer> servers;
  settings->beginGroup(trustedServersGroup);
  QStringList keys = settings->allKeys();
  for (const QString& key : keys) {
    servers.append(TrustedServer{key, settings->value(key).toByteArray()});
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

bool TrustedServersQSettings::isTrustedServer(const TrustedServer& server) {
  settings->beginGroup(trustedServersGroup);
  QByteArray storedCert = settings->value(server.name).toByteArray();
  settings->endGroup();
  return storedCert == server.certificate;
}

void TrustedServersQSettings::addTrustedServer(const TrustedServer& server) {
  settings->beginGroup(trustedServersGroup);
  settings->setValue(server.name, server.certificate);
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
