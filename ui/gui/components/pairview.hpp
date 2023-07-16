#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QFormLayout>
#include <QWidget>

namespace srilakshmikanthanp::clipbirdesk::ui::gui::components {
class PairView : public QWidget {
 private:  // Constants for style
  static constexpr const char *const style = R"(
  )";

 private:  // Member variable
  QWidget *key;
  QWidget *val;

 private:  // just for Qt
  Q_OBJECT

 private:  // disable copy and move
  Q_DISABLE_COPY_MOVE(PairView)

 public:
  /**
   * @brief Construct a new PairView object
   * with parent as QWidget
   * @param parent parent object
   */
  explicit PairView(QWidget* parent = nullptr);

  /**
   * @brief Destroy the PairView object
   */
  virtual ~PairView() = default;

  /**
   * @brief Set as a pair
   *
   * @param key key
   * @param val value
   */
  void set(QWidget *key, QWidget* val);

  /**
   * @brief get the key and value as pair
   *
   * @return std::pair<QString, QString> key and value
   */
  QPair<QWidget*, QWidget*> get() const;
};
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::components::core
