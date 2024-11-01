#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

#ifdef __linux__
  #include "ui/gui/notify/joinrequest/linux/joinrequest/joinrequest.hpp"
#elif _WIN32
  #include "ui/gui/notify/joinrequest/win/joinrequest/joinrequest.hpp"
#endif
