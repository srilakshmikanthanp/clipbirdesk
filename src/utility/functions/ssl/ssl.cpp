#include <openssl/rand.h>

#include "ssl.hpp"

namespace srilakshmikanthanp::clipbirdesk::utility::functions::internal {
/**
 *  @brief generates a EVP_PEY using RSA_generate_key_ex
 *  @returns EVP_PKEY* - shared pointer to the key
 */
std::shared_ptr<EVP_PKEY> generateRSAKey(int bits) {
  // create a shared pointer to handle the memory
  std::shared_ptr<EVP_PKEY> pkey(EVP_PKEY_new(), EVP_PKEY_free);

  // if pkey is null then throw an error
  if (pkey.get() == NULL) {
    throw std::runtime_error("Can't Create EVP_PKEY");
  }

  // rsa_key = RSA_new();
  std::shared_ptr<RSA> rsa(RSA_new(), RSA_free);

  // if rsa is null then throw an error
  if (rsa.get() == NULL) {
    throw std::runtime_error("Can't Create RSA");
  }

  // BIGNUM *bn_exp = BN_new();
  std::shared_ptr<BIGNUM> bn_exp(BN_new(), BN_free);

  // if bn_exp is null then throw an error
  if (bn_exp.get() == NULL) {
    throw std::runtime_error("Can't Create BIGNUM");
  }

  // Set the exponent to F4
  if (!BN_set_word(bn_exp.get(), RSA_F4)) {
    throw std::runtime_error("Can't Set BIGNUM");
  }

  // Try to generate the key
  if (!RSA_generate_key_ex(rsa.get(), bits, bn_exp.get(), NULL)) {
    throw std::runtime_error("Can't Generate RSA Key");
  }

  // Set the rsa key to the pkey
  if (!EVP_PKEY_set1_RSA(pkey.get(), rsa.get())) {
    throw std::runtime_error("Can't Assign RSA to EVP_PKEY");
  }

  // The key has been generated, return it
  return pkey;
}

/**
 * @brief Generates a self-signed x509 certificate
 *
 * @param pkey an EVP_PKEY pointer
 *
 * @return X509* - shared pointer to certificate
 */
std::shared_ptr<X509> generateX509(std::shared_ptr<EVP_PKEY> pkey) {
  // Allocate memory for the X509 structure & BIGNUM
  std::unique_ptr<BIGNUM, decltype(&::BN_free)> sn(BN_new(), ::BN_free);
  std::shared_ptr<X509> x509(X509_new(), X509_free);

  // Expiry time
  constexpr long expiry = 60 * 60 * 24 * 365;

  // if x509 is null then throw an error
  if (x509.get() == NULL) {
    throw std::runtime_error("Can't Create X509");
  }

  // if sn is null then throw an error
  if (sn.get() == NULL) {
    throw std::runtime_error("Can't Create BIGNUM");
  }

  // Set the version to 2
  if (!X509_set_version(x509.get(), 2)) {
    throw std::runtime_error("Can't Set Version");
  }

  // Generate random number
  if (!BN_rand(sn.get(), 160, -1, 0)) {
    throw std::runtime_error("Can't Generate Random Number");
  }

  // Set the serial number
  if (!BN_to_ASN1_INTEGER(sn.get(), X509_get_serialNumber(x509.get()))) {
    throw std::runtime_error("Can't Set Serial Number");
  }

  // get local host name
  auto cname = constants::getMDnsServiceName().toLocal8Bit();
  auto org   = constants::getAppOrgName();
  auto unit  = constants::getAppName();

  // get the subject name
  X509_NAME *name = X509_get_subject_name(x509.get());

  // set the common name
  if(!X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_ASC, (const unsigned char *)cname.data(), -1, -1, 0)) {
    throw std::runtime_error("Can't Set Subject Name");
  }

  // set the organization name
  if(!X509_NAME_add_entry_by_txt(name, "O", MBSTRING_ASC, (const unsigned char *)org, -1, -1, 0)) {
    throw std::runtime_error("Can't Set Subject Name");
  }

  // set the unit name
  if(!X509_NAME_add_entry_by_txt(name, "OU", MBSTRING_ASC, (const unsigned char *)unit, -1, -1, 0)) {
    throw std::runtime_error("Can't Set Subject Name");
  }

  // try to set the subject name
  if (!X509_set_subject_name(x509.get(), name)) {
    throw std::runtime_error("Can't Set Subject Name");
  }

  // Set the issuer name
  if (!X509_set_issuer_name(x509.get(), name)) {
    throw std::runtime_error("Can't Set Issuer Name");
  }

  // set Expiry date to 1 year
  X509_gmtime_adj(X509_get_notBefore(x509.get()), 0);
  X509_gmtime_adj(X509_get_notAfter(x509.get()), expiry);

  // Set the public key for our certificate.
  X509_set_pubkey(x509.get(), pkey.get());

  // Actually sign the certificate with our key.
  if (!X509_sign(x509.get(), pkey.get(), EVP_sha1())) {
    throw std::runtime_error("Can't Sign X509");
  }

  // return the certificate
  return x509;
}

/**
 * @brief Get the OpenSSL error as string
 * @return std::string - OpenSSL error
 */
std::string getOpenSSLError() {
  std::shared_ptr<BIO> bio(BIO_new(BIO_s_mem()), BIO_free_all);

  if (!bio) {
    return "Unknown OpenSSL error (BIO_new failed)";
  }

  ERR_print_errors(bio.get());
  char *buf;
  long len = BIO_get_mem_data(bio.get(), &buf);
  std::string msg(buf, static_cast<size_t>(len));

  if (msg.empty()) {
    return "Unknown OpenSSL error";
  }

  return msg;
}
}  // namespace srilakshmikanthanp::clipbirdesk::utility::functions::internal

namespace srilakshmikanthanp::clipbirdesk::utility::functions {
/**
 * @brief Get the Q Ssl Configuration object
 * @param bits - RSA key size
 * @return QSslConfiguration
 */
QPair<QSslKey, QSslKey> generateQtKeyPair(int bits) {
  std::shared_ptr<EVP_PKEY> pkey = internal::generateRSAKey(bits);
  std::shared_ptr<BIO> privateBio(BIO_new(BIO_s_mem()), BIO_free_all);

  if (!privateBio) {
    throw std::runtime_error("Can't create BIO for private key");
  }

  if (!PEM_write_bio_PrivateKey(privateBio.get(), pkey.get(), nullptr, nullptr, 0, nullptr, nullptr)) {
    throw std::runtime_error("Can't write private key to PEM");
  }

  BUF_MEM *privateBufffer = nullptr;
  BIO_get_mem_ptr(privateBio.get(), &privateBufffer);
  QByteArray privatePem(privateBufffer->data, static_cast<int>(privateBufffer->length));
  QSslKey privateKey(privatePem, QSsl::Rsa, QSsl::Pem, QSsl::PrivateKey);

  std::shared_ptr<BIO> publicBio(BIO_new(BIO_s_mem()), BIO_free_all);

  if (!publicBio) {
    throw std::runtime_error("Can't create BIO for public key");
  }

  if (!PEM_write_bio_PUBKEY(publicBio.get(), pkey.get())) {
    throw std::runtime_error("Can't write public key to PEM");
  }

  BUF_MEM *publicBuffer = nullptr;
  BIO_get_mem_ptr(publicBio.get(), &publicBuffer);
  QByteArray publicPem(publicBuffer->data, static_cast<int>(publicBuffer->length));
  QSslKey publicKey(publicPem, QSsl::Rsa, QSsl::Pem, QSsl::PublicKey);

  return qMakePair(privateKey, publicKey);
}

/**
 * @brief Get the Q Ssl Configuration object
 * @param bits - RSA key size
 * @return QSslConfiguration
 */
common::types::SslConfig getQSslConfiguration(int bits) {
  // Generate the RSA key for the certificate
  std::shared_ptr<EVP_PKEY> pkey = internal::generateRSAKey(bits);

  // Generate the certificate
  std::shared_ptr<X509> x509     = internal::generateX509(pkey);

  // using the defer as in golang
  using Defer                    = std::shared_ptr<void>;

  // Write the key to buffer
  BIO *pkey_buffer               = BIO_new(BIO_s_mem());

  // Free the memory
  Defer defer_pkey_buffer(pkey_buffer, BIO_free_all);

  // Write the key to buffer
  PEM_write_bio_PrivateKey(pkey_buffer, pkey.get(), NULL, NULL, 0, NULL, NULL);

  // Get the key from buffer
  BUF_MEM *pkey_buffer_memory;

  // Get the key from buffer
  BIO_get_mem_ptr(pkey_buffer, &pkey_buffer_memory);

  // QByteArray for the Key
  QByteArray key(QByteArray(pkey_buffer_memory->data, pkey_buffer_memory->length));

  // Create the QSslKey
  QSslKey sslKey(key, QSsl::Rsa, QSsl::Pem, QSsl::PrivateKey);

  // Write the certificate to buffer
  BIO *x509_buffer = BIO_new(BIO_s_mem());

  // Free the memory
  Defer defer_x509_buffer(x509_buffer, BIO_free_all);

  // Write the certificate to buffer
  PEM_write_bio_X509(x509_buffer, x509.get());

  // Get the certificate from buffer
  BUF_MEM *x509_buffer_memory;

  // Get the certificate from buffer
  BIO_get_mem_ptr(x509_buffer, &x509_buffer_memory);

  // QByteArray for the Certificate
  QByteArray cert(QByteArray(x509_buffer_memory->data, x509_buffer_memory->length));

  // Create the QSslCertificate
  return common::types::SslConfig {
    .privateKey = key,
    .certificate = cert
  };
}

/**
 * @brief Generate AES Key
 * @param bits - AES key size
 * @return QByteArray - AES key
 */
QByteArray generateAESKey(int bits) {
  // Check if bits is valid
  if (bits % 8 != 0) {
    throw std::invalid_argument("Invalid AES key size");
  }

  // Calculate the number of bytes
  int num_bytes = bits / 8;

  // Create a QByteArray to hold the key
  QByteArray key;
  key.resize(num_bytes);

  // Generate random bytes
  if (RAND_bytes(reinterpret_cast<unsigned char *>(key.data()), num_bytes) != 1) {
    throw std::runtime_error("Can't generate random bytes for AES key");
  }

  return key;
}

/**
 * @brief Generate IV
 * @param bits - IV bits
 * @return QByteArray - IV
 */
QByteArray generateIV(int bits) {
  // Check if bits is valid
  if (bits % 8 != 0) {
    throw std::invalid_argument("Invalid IV size");
  }

  // Calculate the number of bytes
  int num_bytes = bits / 8;

  // Create a QByteArray to hold the IV
  QByteArray iv;
  iv.resize(num_bytes);

  // Generate random bytes
  if (RAND_bytes(reinterpret_cast<unsigned char *>(iv.data()), num_bytes) != 1) {
    throw std::runtime_error("Can't generate random bytes for AES IV");
  }

  return iv;
}
}  // namespace srilakshmikanthanp::clipbirdesk::utility::functions
