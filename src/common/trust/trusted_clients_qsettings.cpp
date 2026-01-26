#include "trusted_clients_qsettings.hpp"

namespace srilakshmikanthanp::clipbirdesk::common::trust {
TrustedClientsQSettings::TrustedClientsQSettings(QObject* parent): TrustedClients(parent) {}
TrustedClientsQSettings::~TrustedClientsQSettings() {}

QList<TrustedClient> TrustedClientsQSettings::getTrustedClients() {
  QList<TrustedClient> clients;
  settings->beginGroup(trustedClientsGroup);
  QStringList keys = settings->allKeys();
  for (const QString& key : keys) {
    clients.append(TrustedClient{key, settings->value(key).toByteArray()});
  }
  settings->endGroup();
  return clients;
}

bool TrustedClientsQSettings::hasTrustedClient(const QString& name) {
  settings->beginGroup(trustedClientsGroup);
  bool exists = settings->contains(name);
  settings->endGroup();
  return exists;
}

bool TrustedClientsQSettings::isTrustedClient(const TrustedClient& client) {
  settings->beginGroup(trustedClientsGroup);
  QByteArray storedCert = settings->value(client.name).toByteArray();
  settings->endGroup();
  return storedCert == client.certificate;
}

void TrustedClientsQSettings::addTrustedClient(const TrustedClient& client) {
  settings->beginGroup(trustedClientsGroup);
  settings->setValue(client.name, client.certificate);
  settings->endGroup();
  emit trustedClientsChanged(getTrustedClients());
}

void TrustedClientsQSettings::removeTrustedClient(const QString& name) {
  settings->beginGroup(trustedClientsGroup);
  settings->remove(name);
  settings->endGroup();
  emit trustedClientsChanged(getTrustedClients());
}
}
