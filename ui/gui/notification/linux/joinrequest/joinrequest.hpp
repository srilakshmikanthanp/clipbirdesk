#ifdef __linux__  // Only for linux using libnotify see https://developer.gnome.org/libnotify/
#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QObject>
#include <QMetaObject>

#include "constants/constants.hpp"
#include "types/device/device.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::notification {
/**
 * @brief Notification class
 */
class JoinRequest : public QObject {
 private:  // disable copy and move for this class

  Q_DISABLE_COPY_MOVE(JoinRequest)

 private:  // just for Qt

  Q_OBJECT

 signals:  // signals
  // called when user Accepted
  void onAccept() const;

 signals:  // signals
  // called when user Rejected
  void onReject() const;

 private:  // Member Functions

  /**
   * @brief Accept Impl
   */
  void acceptImpl() const;

  /**
   * @brief Reject Impl
   */
  void rejectImpl() const;

 public:

  /**
   * @brief Construct a new Abstract object
   *
   * @param parent
   */
  explicit JoinRequest(QObject *parent = nullptr);

  /**
   * @brief Show the notification
   */
  void show(const types::device::Device &device);

  /**
   * @brief Destroy the Status object
   *
   */
  virtual ~JoinRequest() = default;
};
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::notification
#endif  // __linux__
