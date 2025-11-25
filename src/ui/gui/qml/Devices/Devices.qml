pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls.Material
import QtQuick.Controls
import QtQuick.Layouts
import Clipbird.Qml

Item {
    id: root
    width: parent.width
    height: parent.height

    property bool isServer: ClipbirdQmlApplicationState.isServer

    Connections {
        target: ClipbirdQmlApplicationState
        function onIsServerChanged() {
            root.isServer = ClipbirdQmlApplicationState.isServer;
        }
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        TabBar {
            id: tabBar
            Layout.fillWidth: true
            currentIndex: root.isServer ? 0 : 1
            focusPolicy: Qt.NoFocus

            TabButton {
                text: qsTr("Create Group")
                width: implicitWidth
                background: Rectangle {
                    color: "transparent"
                }
                onClicked: ClipbirdQmlApplicationState.setIsServer(true)
                HoverHandler {
                    cursorShape: Qt.PointingHandCursor
                }
            }

            TabButton {
                text: qsTr("Join  Group")
                width: implicitWidth
                background: Rectangle {
                    color: "transparent"
                }
                onClicked: ClipbirdQmlApplicationState.setIsServer(false)
                HoverHandler {
                    cursorShape: Qt.PointingHandCursor
                }
            }
        }

        StackLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            currentIndex: tabBar.currentIndex
            ServerGroup {}
            ClientGroup {}
        }
    }
}
