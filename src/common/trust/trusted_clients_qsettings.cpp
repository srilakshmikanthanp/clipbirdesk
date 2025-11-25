#include "trusted_clients_qsettings.hpp"

namespace srilakshmikanthanp::clipbirdesk::common::trust {
TrustedClientsQSettings::TrustedClientsQSettings(QObject* parent): TrustedClients(parent) {}
TrustedClientsQSettings::~TrustedClientsQSettings() {}

QMap<QString, QByteArray> TrustedClientsQSettings::getTrustedClients() {
  QMap<QString, QByteArray> clients;
  settings->beginGroup(trustedClientsGroup);
  QStringList keys = settings->allKeys();
  for (const QString& key : keys) {
    clients.insert(key, settings->value(key).toByteArray());
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

bool TrustedClientsQSettings::isTrustedClient(const QString& name, const QByteArray& certificate) {
  settings->beginGroup(trustedClientsGroup);
  QByteArray storedCert = settings->value(name).toByteArray();
  settings->endGroup();
  return storedCert == certificate;
}

void TrustedClientsQSettings::addTrustedClient(const QString& name, const QByteArray& certificate) {
  settings->beginGroup(trustedClientsGroup);
  settings->setValue(name, certificate);
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
