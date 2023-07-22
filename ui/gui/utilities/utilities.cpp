// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "utilities.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui {
/**
 * @brief Authenticator for server to authenticate the client
 * when client connects to server, This Authenticator just
 * shows a info of the client and asks for permission to
 * connect or not.
 *
 * @param host <QHostAddress, quint16>
 *
 * @return true if user accepts the connection
 * @return false if user rejects the connection
 */
bool authenticator(QPair<QHostAddress, quint16> host) {
  // get the message to show
  auto message = QString(
    "A client is trying to connect to the server\n"
    "Host: %1:%2\n"
    "Accept the connection?"
  ).arg(
    host.first.toString(), QString::number(host.second)
  );

  // get the user input
  auto dialog = QMessageBox();

  // set the icon
  dialog.setWindowIcon(QIcon(constants::getAppLogo().c_str()));

  // set the title
  dialog.setWindowTitle("Clipbird");

  // set the message
  dialog.setText(message);

  // set the buttons
  dialog.setStandardButtons(QMessageBox::Yes | QMessageBox::No);

  // set the default button
  dialog.setDefaultButton(QMessageBox::No);

  // show the dialog
  dialog.exec();

  // return the result
  return dialog.result() == QMessageBox::Yes;
}
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::utility
