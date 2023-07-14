#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QWidget>

namespace srilakshmikanthanp::clipbirdesk::ui::gui::components::internal {
class Status : public QWidget {
 private:  // Member variable
 private:  // just for Qt
  Q_OBJECT

 public:
  /**
   * @brief Construct a new Status object
   * with parent as QWidget
   * @param parent parent object
   */
  explicit Status(QWidget* parent = nullptr) {}
};

class ServerIP : public QWidget {
 private:  // Member variable
 private:  // just for Qt
  Q_OBJECT

 public:
  /**
   * @brief Construct a new ServerIP object
   * with parent as QWidget
   * @param parent parent object
   */
  explicit ServerIP(QWidget* parent = nullptr) {}
};

class ServerName : public QWidget {
 private:  // Member variable
 private:  // just for Qt
  Q_OBJECT

 public:
  /**
   * @brief Construct a new ServerName object
   * with parent as QWidget
   * @param parent parent object
   */
  explicit ServerName(QWidget* parent = nullptr) {}
};

class Hosts : public QWidget {
 private:  // Member variable
 private:  // just for Qt
  Q_OBJECT

 public:
  /**
   * @brief Construct a new Hosts object
   * with parent as QWidget
   * @param parent parent object
   */
  explicit Hosts(QWidget* parent = nullptr) {}
};
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::components::internal

namespace srilakshmikanthanp::clipbirdesk::ui::gui::components {
class Clipbird : public QWidget {
 private:  // Member variable
 private:  // just for Qt
  Q_OBJECT

 public:
  /**
   * @brief Construct a new Clipbird object
   * with parent as QWidget
   * @param parent parent object
   */
  explicit Clipbird(QWidget* parent = nullptr) {}
};
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::components
