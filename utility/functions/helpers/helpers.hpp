// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QByteArray>
#include <QtEndian>
#include <QtTypes>

namespace srilakshmikanthanp::clipbirdesk::utility::functions {
/**
 * @brief Convert Type to QByteArray in Big Endian
 */
template<class T>
QByteArray toQByteArray(T value) {
  QByteArray array(sizeof(T), 0);
  qToBigEndian<T>(value, array.data());
  return array;
}

/**
 * @brief Convert QByteArray to Type T
 */
template <class T>
T fromQByteArray(QByteArray array) {
  return qFromBigEndian<T>(array.data());
}
}
