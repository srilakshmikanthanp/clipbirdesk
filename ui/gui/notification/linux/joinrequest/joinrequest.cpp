#ifdef __linux__ // Only for linux using libnotify see https://developer.gnome.org/libnotify/

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "joinrequest.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::notification {
/**
 * @brief Accept Impl
 */
void JoinRequest::acceptImpl() const {
  QMetaObject::invokeMethod(const_cast<JoinRequest*>(this), "onAccept", Qt::QueuedConnection);
}

/**
 * @brief Reject Impl
 */
void JoinRequest::rejectImpl() const {
  QMetaObject::invokeMethod(const_cast<JoinRequest*>(this), "onReject", Qt::QueuedConnection);
}

/**
 * @brief Construct a new Abstract object
 *
 * @param parent
 */
JoinRequest::JoinRequest(QObject *parent) : QObject(parent) {
  // Do nothing
}

/**
 * @brief Show the notification
 */
void JoinRequest::show(const types::device::Device &device) {

}
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::notification
#endif  // __linux__
