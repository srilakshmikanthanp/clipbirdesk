#pragma once // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <string>

#include "../../types/types.hpp"

namespace srilakshmikanthanp::clipbirdesk::utility::typeconv::varies {
  /**
   * @brief convert to string
   */
  template<typename T>
  T from_varies(const types::varies_t &obj) {
    return T(obj.begin(), obj.end());
  }

  /**
   * @brief convert to varies
   */
  template<typename T>
  types::varies_t to_varies(const T &obj) {
    return types::varies_t(obj.begin(), obj.end());
  }
}
