#include "crypto.hpp"

#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rand.h>
#include <openssl/rsa.h>

#include <QtEndian>
#include <memory>
#include <stdexcept>

namespace srilakshmikanthanp::clipbirdesk::utility::functions::internal {
QByteArray encryptAESKey(const QByteArray& aesKey, const QByteArray& rsaPublickKey) {
  std::shared_ptr<BIO> bio(BIO_new_mem_buf(rsaPublickKey.data(), rsaPublickKey.size()), BIO_free_all);

  if (!bio) {
    throw std::runtime_error("BIO_new_mem_buf failed: " + internal::getOpenSSLError());
  }

  std::shared_ptr<RSA> rsaKey(PEM_read_bio_RSA_PUBKEY(bio.get(), nullptr, nullptr, nullptr), RSA_free);

  if (!rsaKey) {
    throw std::runtime_error("PEM_read_bio_RSA_PUBKEY failed: " + internal::getOpenSSLError());
  }

  QByteArray encryptedKey;
  encryptedKey.resize(RSA_size(rsaKey.get()));
  int result = RSA_public_encrypt(
    aesKey.size(),
    reinterpret_cast<const unsigned char*>(aesKey.data()),
    reinterpret_cast<unsigned char*>(encryptedKey.data()),
    rsaKey.get(),
    RSA_PKCS1_OAEP_PADDING
  );

  if (result == -1) {
    throw std::runtime_error("RSA_public_encrypt failed: " + internal::getOpenSSLError());
  }

  encryptedKey.resize(result);
  return encryptedKey;
}

QByteArray decryptAESKey(const QByteArray& encryptedAesKey, const QByteArray& rsaPrivateKey) {
  std::shared_ptr<BIO> bio(BIO_new_mem_buf(rsaPrivateKey.data(), rsaPrivateKey.size()), BIO_free_all);

  if (!bio) {
    throw std::runtime_error("BIO_new_mem_buf failed: " + internal::getOpenSSLError());
  }

  std::shared_ptr<RSA> rsaKey(PEM_read_bio_RSAPrivateKey(bio.get(), nullptr, nullptr, nullptr), RSA_free);

    if (!rsaKey) {
      throw std::runtime_error("PEM_read_bio_RSAPrivateKey failed: " + internal::getOpenSSLError());
    }

  QByteArray decryptedKey;
  decryptedKey.resize(RSA_size(rsaKey.get()));
  int result = RSA_private_decrypt(
    encryptedAesKey.size(),
    reinterpret_cast<const unsigned char*>(encryptedAesKey.data()),
    reinterpret_cast<unsigned char*>(decryptedKey.data()),
    rsaKey.get(),
    RSA_PKCS1_OAEP_PADDING
  );

    if (result == -1) {
      throw std::runtime_error("RSA_private_decrypt failed: " + internal::getOpenSSLError());
    }

  decryptedKey.resize(result);
  return decryptedKey;
}

class BlockReader {
public:
  BlockReader(const QByteArray &src) : data(src), offset(0) {}

  void nextBlock(quint32 &size, QByteArray &block) {
    if (offset + 4 > data.size()) {
      throw std::runtime_error("Invalid data format (missing size field)");
    }

    size = qFromBigEndian<quint32>(reinterpret_cast<const uchar*>(data.constData() + offset));
    offset += 4;

    if (offset + size > data.size()) {
      throw std::runtime_error("Invalid data format (block truncated)");
    }

    block = data.mid(offset, size);
    offset += size;
  }

  bool hasMore() const { return offset < data.size(); }

private:
  const QByteArray &data;
  int offset;
};
}

namespace srilakshmikanthanp::clipbirdesk::utility::functions {
/**
 * @brief Encrypt data using AES-256-GCM and RSA-OAEP for key encryption
 * @param data - Data to be encrypted
 * @param key - RSA Public Key in PEM format
 * @return QByteArray - Encrypted data
 * |Size of encrypted AES key(4 bytes) in big-endian|
 * |Encrypted AES key|
 * |Size of IV(4 bytes) in big-endian|
 * |IV|
 * |Size of TAG(4 bytes) in big-endian|
 * |TAG|
 * |Size of Ciphertext(4 bytes) (4 bytes) in big-endian|
 * |Ciphertext|
 */
QByteArray encrypt(const QByteArray& data, const QByteArray& key) {
  QByteArray aesKey = generateAESKey(256), iv = generateIV(96);

  std::shared_ptr<EVP_CIPHER_CTX> ctx(EVP_CIPHER_CTX_new(), EVP_CIPHER_CTX_free);

  if (!ctx) {
    throw std::runtime_error("EVP_CIPHER_CTX_new failed: " + internal::getOpenSSLError());
  }

  if (EVP_EncryptInit_ex(ctx.get(), EVP_aes_256_gcm(), nullptr, nullptr, nullptr) != 1) {
    throw std::runtime_error("EncryptInit_ex failed: " + internal::getOpenSSLError());
  }

  if (!EVP_CIPHER_CTX_ctrl(ctx.get(), EVP_CTRL_GCM_SET_IVLEN, iv.size(), nullptr)) {
    throw std::runtime_error("Set IV length failed: " + internal::getOpenSSLError());
  }

  if (EVP_EncryptInit_ex(ctx.get(), nullptr, nullptr, reinterpret_cast<unsigned char*>(aesKey.data()), reinterpret_cast<unsigned char*>(iv.data())) != 1) {
    throw std::runtime_error("EncryptInit_ex (key/iv) failed: " + internal::getOpenSSLError());
  }

  QByteArray ciphertext;
  ciphertext.resize(data.size() + EVP_CIPHER_block_size(EVP_aes_256_gcm()));

  int total = 0, outLen = 0;
  if (EVP_EncryptUpdate(ctx.get(), reinterpret_cast<unsigned char*>(ciphertext.data()), &outLen, reinterpret_cast<const unsigned char*>(data.data()), data.size()) != 1) {
    throw std::runtime_error("EncryptUpdate failed: " + internal::getOpenSSLError());
  }

  total += outLen;

  if (EVP_EncryptFinal_ex(ctx.get(), reinterpret_cast<unsigned char*>(ciphertext.data()) + total, &outLen) != 1) {
    throw std::runtime_error("EncryptFinal_ex failed: " + internal::getOpenSSLError());
  }

  total += outLen;
  ciphertext.resize(total);

  QByteArray tag;
  tag.resize(16);

  if (EVP_CIPHER_CTX_ctrl(ctx.get(), EVP_CTRL_GCM_GET_TAG, 16, reinterpret_cast<unsigned char*>(tag.data())) != 1) {
    throw std::runtime_error("Get TAG failed: " + internal::getOpenSSLError());
  }

  QByteArray encryptedAesKey = internal::encryptAESKey(aesKey, key);

  QByteArray result;
  QDataStream stream(&result, QIODevice::WriteOnly);
  stream.setByteOrder(QDataStream::BigEndian);

  stream << static_cast<quint32>(encryptedAesKey.size());
  stream.writeRawData(encryptedAesKey.constData(), encryptedAesKey.size());
  stream << static_cast<quint32>(iv.size());
  stream.writeRawData(iv.constData(), iv.size());
  stream << static_cast<quint32>(tag.size());
  stream.writeRawData(tag.constData(), tag.size());
  stream << static_cast<quint32>(ciphertext.size());
  stream.writeRawData(ciphertext.constData(), ciphertext.size());

  return result;
}

/**
 * @brief Sign data using RSA-PSS with SHA-256
 * @param data - Data to be signed
 * @param key - RSA Private Key in PEM format
 * @return QByteArray - Signature
 */
QByteArray sign(const QByteArray& data, const QByteArray& key) {
  std::shared_ptr<BIO> bio(BIO_new_mem_buf(key.data(), key.size()), BIO_free_all);

  if (!bio) {
    throw std::runtime_error("Failed to create BIO for private key: " + internal::getOpenSSLError());
  }

  std::shared_ptr<EVP_PKEY> pkey(PEM_read_bio_PrivateKey(bio.get(), nullptr, nullptr, nullptr), EVP_PKEY_free);

  if (!pkey) {
    throw std::runtime_error("Failed to load private key from PEM: " + internal::getOpenSSLError());
  }

  std::shared_ptr<EVP_MD_CTX> ctx(EVP_MD_CTX_new(), EVP_MD_CTX_free);

  if (!ctx) {
    throw std::runtime_error("Failed to create EVP_MD_CTX: " + internal::getOpenSSLError());
  }

  if (EVP_DigestSignInit(ctx.get(), nullptr, EVP_sha256(), nullptr, pkey.get()) <= 0) {
    throw std::runtime_error("EVP_DigestSignInit failed: " + internal::getOpenSSLError());
  }

  if (EVP_DigestSignUpdate(ctx.get(), data.data(), data.size()) <= 0) {
    throw std::runtime_error("EVP_DigestSignUpdate failed: " + internal::getOpenSSLError());
  }

  size_t sigLen = 0;

  if (EVP_DigestSignFinal(ctx.get(), nullptr, &sigLen) <= 0) {
    throw std::runtime_error("EVP_DigestSignFinal (size) failed: " + internal::getOpenSSLError());
  }

  QByteArray signature;
  signature.resize(static_cast<int>(sigLen));

  if (EVP_DigestSignFinal(ctx.get(), reinterpret_cast<unsigned char *>(signature.data()), &sigLen) <= 0) {
    throw std::runtime_error("EVP_DigestSignFinal failed: " + internal::getOpenSSLError());
  }

  signature.resize(sigLen);
  return signature;
}

/**
 * @brief Decrypt data using AES-256-GCM and RSA-OAEP for key decryption
 * @param data - Data to be decrypted
 * @param key - RSA Private Key in PEM format
 * @return QByteArray - Decrypted data
 * |Size of encrypted AES key(4 bytes) in big-endian|
 * |Encrypted AES key|
 * |Size of IV(4 bytes) in big-endian|
 * |IV|
 * |Size of TAG(4 bytes) in big-endian|
 * |TAG|
 * |Size of Ciphertext(4 bytes) in big-endian|
 * |Ciphertext|
 */
QByteArray decrypt(const QByteArray& data, const QByteArray& key) {
  quint32 encryptedAesKeySize, ivSize, tagSize, ciphertextSize;
  QByteArray encryptedAesKey, iv, tag, ciphertext;

  internal::BlockReader reader(data);

  reader.nextBlock(encryptedAesKeySize, encryptedAesKey);
  reader.nextBlock(ivSize, iv);
  reader.nextBlock(tagSize, tag);
  reader.nextBlock(ciphertextSize, ciphertext);

  if (reader.hasMore()) {
    throw std::runtime_error("Invalid data format (extra data found)");
  }

  QByteArray aesKey = internal::decryptAESKey(encryptedAesKey, key);

  std::shared_ptr<EVP_CIPHER_CTX> ctx(EVP_CIPHER_CTX_new(), EVP_CIPHER_CTX_free);

  if (!ctx) {
    throw std::runtime_error("EVP_CIPHER_CTX_new failed: " + internal::getOpenSSLError());
  }

  if (EVP_DecryptInit_ex(ctx.get(), EVP_aes_256_gcm(), nullptr, nullptr, nullptr) != 1) {
    throw std::runtime_error("DecryptInit_ex failed: " + internal::getOpenSSLError());
  }

  if (!EVP_CIPHER_CTX_ctrl(ctx.get(), EVP_CTRL_GCM_SET_IVLEN, iv.size(), nullptr)) {
    throw std::runtime_error("Set IV length failed: " + internal::getOpenSSLError());
  }

  if (EVP_DecryptInit_ex(ctx.get(), nullptr, nullptr, reinterpret_cast<unsigned char*>(aesKey.data()), reinterpret_cast<unsigned char*>(iv.data())) != 1) {
    throw std::runtime_error("DecryptInit_ex (key/iv) failed: " + internal::getOpenSSLError());
  }

  if (!EVP_CIPHER_CTX_ctrl(ctx.get(), EVP_CTRL_GCM_SET_TAG, tag.size(), reinterpret_cast<unsigned char*>(tag.data()))) {
    throw std::runtime_error("Set TAG failed: " + internal::getOpenSSLError());
  }

  QByteArray plaintext;
  plaintext.resize(ciphertext.size());

  int total = 0, outLen = 0;
  if (EVP_DecryptUpdate(ctx.get(), reinterpret_cast<unsigned char*>(plaintext.data()), &outLen, reinterpret_cast<const unsigned char*>(ciphertext.data()), ciphertext.size()) != 1) {
    throw std::runtime_error("DecryptUpdate failed: " + internal::getOpenSSLError());
  }

  total += outLen;

  if (EVP_DecryptFinal_ex(ctx.get(), reinterpret_cast<unsigned char*>(plaintext.data()) + total, &outLen) != 1) {
    throw std::runtime_error("DecryptFinal_ex failed (data may be tampered or key is incorrect): " + internal::getOpenSSLError());
  }

  total += outLen;

  plaintext.resize(total);
  return plaintext;
}
}  // namespace srilakshmikanthanp::clipbirdesk::utility::functions
