// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "cliphist.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::components {
/**
 * @brief Construct a new Clip Hist object
 *
 * @param parent
 */
ClipHist::ClipHist(QWidget *parent) : QWidget(parent) {
  // TODO: implement
}

/**
 * @brief Set the History
 */
void ClipHist::setHistory(const QList<QVector<QPair<QString, QByteArray>>> &) {
  this->history = history;
}

/**
 * @brief Get the History
 */
const QList<QVector<QPair<QString, QByteArray>>> &ClipHist::getHistory() const {
  return history;
}

/**
 * @brief Set the Bucket
 */
void ClipHist::setBucket(size_t size) {
  this->bucket = size;
}

/**
 * @brief Get the Bucket
 *
 * @return size_t
 */
size_t ClipHist::getBucket() const {
  return bucket;
}

/**
 * @brief Clear the history
 *
 */
void ClipHist::clear() {
  history.clear();
}

/**
 * @brief Add clip to history
 */
void ClipHist::addClip(const QVector<QPair<QString, QByteArray>> &clip) {
  if (this->history.size() + 1 > this->bucket) {
    this->history.pop_back();
  }

  this->history.push_front(clip);
}
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::components
