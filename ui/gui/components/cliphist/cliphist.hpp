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
  qsizetype bucket = 30;

 signals:

  void onClipSelected(const QVector<QPair<QString, QByteArray>> &);

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
  ~ClipHist() = default;

 public:  // public member function

  /**
   * @brief Set the History
   */
  void setHistory(const QList<QVector<QPair<QString, QByteArray>>> &);

  /**
   * @brief Get the History
   */
  const QList<QVector<QPair<QString, QByteArray>>> &getHistory() const;

  /**
   * @brief Set the Bucket
   */
  void setBucket(size_t);

  /**
   * @brief Get the Bucket
   *
   * @return size_t
   */
  size_t getBucket() const;

  /**
   * @brief Clear the history
   *
   */
  void clear();

  /**
   * @brief Add clip to history
   */
  void addClip(const QVector<QPair<QString, QByteArray>> &);
};
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::components
