#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

// Qt header
#include <QDialog>
#include <QLabel>
#include <QPainterPath>
#include <QPainter>
#include <QScreen>
#include <QGuiApplication>
#include <QPushButton>
#include <QStyleHints>
#include <QVBoxLayout>

namespace srilakshmikanthanp::clipbirdesk::ui::gui::modals {
class Request : public QDialog {
 private:  // disable copy and move for this class

  Q_DISABLE_COPY_MOVE(Request)

 private:  // just for Qt

  Q_OBJECT

 private:

  QLabel *prompt      = new QLabel(this);
  QLabel *host        = new QLabel(this);
  QPushButton *accept = new QPushButton(this);
  QPushButton *reject = new QPushButton(this);

 signals:  // signals
  // called when the clip is copied
  void onAccept();

 signals:  // signals
  // called when the clip is deleted
  void onReject();

 private:  // Member Functions

  /**
   * @brief Function used to set up all text in the label, etc..
   */
  void setUpLanguage();

 public:

  /**
   * @brief Construct a new Abstract object
   *
   * @param parent
   */
  explicit Request(QWidget * parent = nullptr);

  /**
   * @brief Destroy the Status object
   */
  virtual ~Request() = default;

  /**
   * @brief Set the Host Text object
   *
   * @param text
   */
  void setHost(const QString &text);

  /**
   * @brief get the Host Text object
   *
   * @return QString
   */
  QString getHost() const;

  /**
   * @brief override set visible
   */
  void setVisible(bool visible) override;

  /**
   * @brief Override paint event
   */
  void paintEvent(QPaintEvent* event) override;
};
}
