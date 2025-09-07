#pragma once // Header guard see https://en.wikipedia.org/wiki/Include_guard

#include <QRegularExpression>
#include <QRegularExpressionMatchIterator>
#include <QString>

namespace srilakshmikanthanp::clipbirdesk::utility::functions {
/**
 * @brief Set socket as non blocking
 */
#ifdef __linux__ // Only for linux to set the socket as non blocking
int setSocketNonBlocking(int fd);
#endif  // __linux__
}  // namespace srilakshmikanthanp::clipbirdesk::utility::functions::linux
