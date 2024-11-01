#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

#include <QEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPainter>
#include <QStyle>
#include <QPushButton>
#include <QStackedLayout>
#include <QStyleOption>

#include "constants/constants.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::components {

class ClipSend : public QWidget {
 private:  // disable copy and move for this class

  Q_DISABLE_COPY_MOVE(ClipSend)

 private:  // just for Qt

  Q_OBJECT

 private:  // Member variable

  // create a label
  QLabel *item = new QLabel(this);

 signals:  // signals

  // called when the clip is copied
  void onClipSend();

 private:  // Member Functions

  /**
   * @brief Function used to set up all text in the label, etc..
   */
  void setUpLanguage();

 public:  // Constructor and Destructor

  /**
   * @brief Construct a new Clip Tile object
   *
   * @param parent
   */
  ClipSend(QWidget *parent = nullptr);

  /**
   * @brief Destroy the Clip Tile object
   */
  ~ClipSend() = default;

 protected:  // protected member function

  // paint event
  void paintEvent(QPaintEvent *) override;

  // change event
  void changeEvent(QEvent *) override;
};
} // namespace srilakshmikanthanp::clipbirdesk::ui::gui::components
