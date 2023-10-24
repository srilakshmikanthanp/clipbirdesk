#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QStyle>
#include <QStyleOption>

namespace srilakshmikanthanp::clipbirdesk::ui::gui::components {
class ClipHist : public QWidget {
 private:  // disable copy and move for this class

  Q_DISABLE_COPY_MOVE(ClipHist)

 private:  // just for Qt

  Q_OBJECT

 private:  // Member variable

  QList<QVector<QPair<QString, QByteArray>>> history;
};
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::components
