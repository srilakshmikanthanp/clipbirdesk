#include "generic.hpp"

#ifdef __linux__  // Only for linux to set the socket as non blocking
#include <fcntl.h>
#include <unistd.h>
#endif

namespace srilakshmikanthanp::clipbirdesk::utility::functions::platform {
/**
 * @brief Set socket as non blocking
 */
#ifdef __linux__  // Only for linux to set the socket as non blocking
int setSocketNonBlocking(int fd) {
  int flags;
  /* If they have O_NONBLOCK, use the Posix way to do it */
#if defined(O_NONBLOCK)
  /* Fixme: O_NONBLOCK is defined but broken on SunOS 4.1.x and AIX 3.2.5. */
  if (-1 == (flags = fcntl(fd, F_GETFL, 0))) flags = 0;
  return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
#else
  /* Otherwise, use the old way of doing it */
  flags = 1;
  return ioctl(fd, FIOBIO, &flags);
#endif
}
#endif  // __linux__
}  // namespace srilakshmikanthanp::clipbirdesk::utility::functions::platform

namespace srilakshmikanthanp::clipbirdesk::utility::functions {
QString decodeOctalEscapes(const QString& input) {
  QString result;
  QRegularExpression octalRegex(R"(\\([0-7]{1,3}))");
  QRegularExpressionMatchIterator it = octalRegex.globalMatch(input);

  int lastIndex                      = 0;
  while (it.hasNext()) {
    QRegularExpressionMatch match = it.next();
    int start                     = match.capturedStart(0);
    int end                       = match.capturedEnd(0);
    QString octalStr              = match.captured(1);
    bool ok;
    ushort asciiCode = octalStr.toUShort(&ok, 8);

    if (ok) {
      result    += input.mid(lastIndex, start - lastIndex);
      result    += QChar(asciiCode);
      lastIndex  = end;
    }
  }

  result += input.mid(lastIndex);
  return result;
}
}  // namespace srilakshmikanthanp::clipbirdesk::utility::functions
