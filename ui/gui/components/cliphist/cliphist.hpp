#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

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
   * @brief Clear the clip
   */
  void clearClip();

  /**
   * @brief Has Clip
   */
  bool hasClip();

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

class ClipHist : public QWidget {
 private:  // disable copy and move for this class

  Q_DISABLE_COPY_MOVE(ClipHist)

 private:  // just for Qt

  Q_OBJECT

 private:  // Member variable

  QStackedLayout* stackLayout = new QStackedLayout(this);
  QWidget* verticalWidget     = new QWidget(this);
  QVBoxLayout* verticalLayout = new QVBoxLayout(verticalWidget);
  QList<ClipTile*> list   = QList<ClipTile*>();

 private:  // Member variable (With Text Info)

  QLabel *label = new QLabel(this);

 signals:  // signals

  // called when the clip is copied
  void onClipSelected(int index);

  // called when the clip is deleted
  void onClipDelete(int index);

 private:  // Member Functions

  /**
   * @brief Function used to set up all text in the label, etc..
   */
  void setUpLanguage();

 public:  // Constructor and Destructor

  /**
   * @brief Construct a new Clip Hist object
   *
   * @param parent
   */
  ClipHist(QWidget *parent = nullptr);

  /**
   * @brief Destroy the Clip Hist object
   */
  ~ClipHist();

 public:  // public member function

  /**
   * @brief Set the History
   */
  void setHistory(const QList<QVector<QPair<QString, QByteArray>>> &);

  /**
   * @brief Clear the History
   */
  void clearHistory();

  /**
   * @brief get the History
   */
  QList<QVector<QPair<QString, QByteArray>>> getHistory();

 protected:  // protected member function

  // paint event
  void paintEvent(QPaintEvent *) override;

  // change event
  void changeEvent(QEvent *) override;
};
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::components
