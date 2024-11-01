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

class ClipTile : public QWidget {
 private:  // disable copy and move for this class

  Q_DISABLE_COPY_MOVE(ClipTile)

 private:  // just for Qt

  Q_OBJECT

 private:  // Member variable

  const QString MIME_TYPE_TEXT  = "text/plain";
  const QString MIME_TYPE_PNG   = "image/png";
  const QString MIME_TYPE_HTML  = "text/html";

  const int IMG_SIZE = 3145728; // 3 MB
  const int TXT_SIZE = 200;

 private:  // Member variable

  QVector<QPair<QString, QByteArray>> clip;

 private:  // Member variable

  // create a label
  QLabel *item = new QLabel(this);

 signals:  // signals

  // called when the clip is deleted
  void onClipDelete();

  // called when the clip is copied
  void onClipCopy();

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
  ClipTile(QWidget *parent = nullptr);

  /**
   * @brief Destroy the Clip Tile object
   */
  ~ClipTile() = default;

  /**
   * @brief set the clip
   */
  void setClip(const QVector<QPair<QString, QByteArray>> &clip);

  /**
   * @brief get the Clip
   */
  QVector<QPair<QString, QByteArray>> getClip();

 protected:  // protected member function

  // paint event
  void paintEvent(QPaintEvent *) override;

  // change event
  void changeEvent(QEvent *) override;
};
} // namespace srilakshmikanthanp::clipbirdesk::ui::gui::components
