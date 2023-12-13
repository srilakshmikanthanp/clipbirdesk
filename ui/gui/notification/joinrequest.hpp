#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#ifdef _WIN32
  #include "ui/gui/notification/windows/joinrequest/joinrequest.hpp"
#else
  #Error "Not implemented for this platform"
#endif
