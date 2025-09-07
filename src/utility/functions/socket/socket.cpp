#include "socket.hpp"

#ifdef __linux__  // Only for linux to set the socket as non blocking
#include <fcntl.h>
#include <unistd.h>
#endif

namespace srilakshmikanthanp::clipbirdesk::utility::functions {
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
