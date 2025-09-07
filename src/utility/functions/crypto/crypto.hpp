#include <QByteArray>
#include "utility/functions/ssl/ssl.hpp"

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
QByteArray encrypt(const QByteArray& data, const QByteArray& key);

/**
 * @brief Sign data using RSA-PSS with SHA-256
 * @param data - Data to be signed
 * @param key - RSA Private Key in PEM format
 * @return QByteArray - Signature
 */
QByteArray sign(const QByteArray& data, const QByteArray& key);

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
 * |Size of Ciphertext (4 bytes) in big-endian|
 * |Ciphertext|
 */
QByteArray decrypt(const QByteArray& data, const QByteArray& key);
}
