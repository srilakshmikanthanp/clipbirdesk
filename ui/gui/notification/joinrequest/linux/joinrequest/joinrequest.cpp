#ifdef __linux__ // Only for linux using libnotify see https://developer.gnome.org/libnotify/

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "joinrequest.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::notification {
/**
 * @brief Callback for the accept action
 */
void JoinRequest::onAcceptAction(NotifyNotification* notification, char* action, gpointer user_data) {
  auto joinRequest = static_cast<JoinRequest*>(user_data);
  if (joinRequest) {
    joinRequest->acceptImpl();
  }
}

/**
 * @brief Callback for the reject action
 */
void JoinRequest::onRejectAction(NotifyNotification* notification, char* action, gpointer user_data) {
  auto joinRequest = static_cast<JoinRequest*>(user_data);
  if (joinRequest) {
    joinRequest->rejectImpl();
  }
}

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
  // do nothing
}

/**
 * @brief Show the notification
 */
void JoinRequest::show(const types::device::Device &device) {
  auto body = QObject::tr("%1 wants to Join to your Group").arg(device.name).toStdString();
  auto icon = (qApp->applicationDirPath() + "/assets/images/logo.png").toStdString(); 

  NotifyNotification* notification = notify_notification_new(
    constants::getAppName(), body.c_str(), icon.c_str()
  );

  notify_notification_add_action(
    notification, 
    "accept", 
    "Accept", 
    NOTIFY_ACTION_CALLBACK(JoinRequest::onAcceptAction), 
    this, 
    nullptr);
  
  notify_notification_add_action(
    notification, 
    "reject", 
    "Reject", 
    NOTIFY_ACTION_CALLBACK(onRejectAction),
    this, 
    nullptr);

  if (!notify_notification_show(notification, nullptr)) {
    qErrnoWarning("Failed to show notification");
  }

  // Unref the notification object to avoid memory leak
  g_object_unref(G_OBJECT(notification));
}
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::notification
#endif  // __linux__
