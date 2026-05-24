#pragma once

#include <QDateTime>
#include <QSslCertificate>
#include <QByteArray>

namespace srilakshmikanthanp::clipbirdesk::common::types {
struct SslConfig {
  QByteArray privateKey;
  QByteArray certificate;

  bool isNull() const {
    return privateKey.isEmpty() || certificate.isEmpty();
  }

  bool isCertificateValid() const {
    QSslCertificate cert(certificate, QSsl::Der);
    return cert.expiryDate() > QDateTime::currentDateTime();
  }
};
}
