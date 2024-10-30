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

  g_object_unref(G_OBJECT(notification));
}

/**
 * @brief Callback for the reject action
 */
void JoinRequest::onRejectAction(NotifyNotification* notification, char* action, gpointer user_data) {
  auto joinRequest = static_cast<JoinRequest*>(user_data);
  
  if (joinRequest) {
    joinRequest->rejectImpl();
  }

  g_object_unref(G_OBJECT(notification));
}

/**
 * @brief Handle notification closure
 */
void JoinRequest::onClosed(NotifyNotification* notification, gpointer user_data) {
  auto joinRequest = static_cast<JoinRequest*>(user_data);
  
  if (joinRequest) {
    joinRequest->rejectImpl();
  }

  g_object_unref(G_OBJECT(notification));
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
  auto icon = QDir::tempPath() + QDir::separator() + QUuid::createUuid().toString(QUuid::StringFormat::Id128) + ".png";
  auto body = QObject::tr("%1 wants to Join to your Group").arg(device.name).toStdString();
  
  // Create a image for the notification
  QImage image(":/images/logo.png");
  QByteArray byteArray;
  QBuffer buffer(&byteArray);
  buffer.open(QIODevice::WriteOnly);
  image.save(&buffer, "PNG"); // Save as PNG format

  // Write the image to a temp file
  QFile tempFile(icon);
  if (tempFile.open(QIODevice::WriteOnly)) {
    tempFile.write(byteArray);
    tempFile.close();
  }

  auto path = icon.toLocal8Bit().data();

  NotifyNotification* notification = notify_notification_new(
    constants::getAppName(), body.c_str(), path
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

  g_signal_connect(
    notification, 
    "closed", 
    G_CALLBACK(onClosed), 
    this
  );

  if (!notify_notification_show(notification, nullptr)) {
    qErrnoWarning("Failed to show notification");
    g_object_unref(G_OBJECT(notification));
    return;
  }
}
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::notification
#endif  // __linux__
