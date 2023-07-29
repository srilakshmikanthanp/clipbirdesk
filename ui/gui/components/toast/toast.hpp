#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QApplication>
#include <QHBoxLayout>
#include <QLabel>
#include <QScreen>
#include <QTimer>
#include <QWidget>


namespace srilakshmikanthanp::clipbirdesk::ui::gui::components {
/**
 * @brief Toast class Much like in android
 */
class Toast : public QWidget {
 private:  // disable copy and move for this class

  Q_DISABLE_COPY_MOVE(Toast)

 private:  // just for Qt

  Q_OBJECT

 private:  // Member variable

  QTimer* timer   = new QTimer(this);
  QLabel* label   = new QLabel();
  quint32 timeout = 0;

 public:

  /**
   * @brief Construct a new Toast object
   * with parent as QWidget
   * @param parent parent object
   */
  explicit Toast(QWidget* parent = nullptr);

  /**
   * @brief Destroy the Toast object
   */
  virtual ~Toast() = default;

  /**
   * @brief set the timeout
   * @param val timeout
   */
  void setTimeout(quint32 val);

  /**
   * @brief get the timeout
   * @return quint32 value
   */
  quint32 getTimeout() const;

  /**
   * @brief set the Toast
   * @param val Toast
   */
  void setToast(const QString& val);

  /**
   * @brief get the Toast
   * @return QString value
   */
  QString getToast() const;

 protected:

  /**
   * @brief Override the setVisible
   * @param visible visible
   */
  void setVisible(bool visible) override;

  /**
   * @brief Override the EnterEvent
   * @param event event
   */
  void enterEvent(QEnterEvent* event) override;

 public:  // static functions

  /**
   * @brief show the Toast
   * @param val Toast
   * @param timeout timeout
   */
  static void toast(const QString& val, quint32 timeout = 2000, QWidget* parent = nullptr);
};
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::components
