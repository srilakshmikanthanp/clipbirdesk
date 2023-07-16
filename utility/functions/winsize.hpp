#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

// Qt headers
#include <QGuiApplication>
#include <QScreen>

namespace srilakshmikanthanp::clipbirdesk::utility::functions {
/**
 * @brief Get the Size of Application Window
 *
 * @param xRatio Ratio of width to screen
 * @param yRatio Ratio of height to screen
 *
 * @return QSize
 */
inline auto clipbirdSize(int xRatio, int yRatio ) -> QSize {
  // get the screen to which the window belongs
  auto screen = QGuiApplication::primaryScreen();

  // get the screen size
  auto size = screen->size();

  // New size
  auto newSize = QSize();

  // set the new size
  newSize.setWidth(size.width() * xRatio);

  // set the new size
  newSize.setHeight(size.height() * yRatio);

  // return the size
  return newSize;
}
}
