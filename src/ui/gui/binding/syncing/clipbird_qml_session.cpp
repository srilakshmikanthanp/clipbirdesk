#include "clipbird_qml_session.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::qml {

void ClipbirdQmlSession::handleTrustedStateChanged(bool isTrusted) {
  emit trustedStateChanged(isTrusted);
}

ClipbirdQmlSession::ClipbirdQmlSession(syncing::Session* session, QObject* parent): QObject(parent), m_session(session) {
  connect(m_session, &syncing::Session::onTrustedStateChanged, this, &ClipbirdQmlSession::handleTrustedStateChanged);
}

ClipbirdQmlSession::~ClipbirdQmlSession() = default;

QString ClipbirdQmlSession::getName() const {
  return m_session->getName();
}

QString ClipbirdQmlSession::getCertificate() const {
  return m_session->getCertificate().toBase64();
}

bool ClipbirdQmlSession::isTrusted() const {
  return m_session->isTrusted();
}

void ClipbirdQmlSession::disconnect() {
  m_session->disconnect();
}

syncing::Session* ClipbirdQmlSession::getSession() const {
  return m_session;
}

}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::qml
