#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#ifdef __linux__
  #include "ui/gui/notify/joinrequest/linux/joinrequest/joinrequest.hpp"
#elif _WIN32
  #include "ui/gui/notify/joinrequest/win/joinrequest/joinrequest.hpp"
#endif
