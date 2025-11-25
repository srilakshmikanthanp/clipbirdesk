pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls.Material
import QtQuick.Controls
import QtQuick.Layouts
import Clipbird.Qml

Rectangle {
    id: root
    width: parent.width
    height: content.implicitHeight + 16
    color: "transparent"

    required property ClipbirdQmlSession session
    signal disconnectClicked
    signal addToTrustClicked
    signal joinClicked

    RowLayout {
        id: content
        anchors.fill: parent
        anchors.margins: 8
        spacing: 12

        Rectangle {
            width: 30
            height: 30
            radius: 15
            color: Material.color(Material.Green, Material.Shade200)

            Text {
                anchors.centerIn: parent
                text: "✓"
                font.pixelSize: 16
                color: Material.color(Material.Green)
            }
        }

        ColumnLayout {
            Layout.fillWidth: true
            spacing: 2

            Text {
                text: root.session.name
                font.pixelSize: 16
                font.bold: true
                color: Material.primaryTextColor
                Layout.fillWidth: true
            }

            Text {
                text: qsTr("Joined")
                font.pixelSize: 13
                color: Material.secondaryTextColor
                Layout.fillWidth: true
            }
        }

        RowLayout {
            spacing: 4

            Button {
                text: qsTr("Disconnect")
                hoverEnabled: false
                flat: true
                Material.foreground: Material.accentColor
                onClicked: root.disconnectClicked()

                HoverHandler {
                    cursorShape: Qt.PointingHandCursor
                }
            }

            Button {
                visible: !root.session.isTrusted
                text: "+"
                hoverEnabled: false
                flat: true
                Material.foreground: Material.accentColor
                onClicked: root.addToTrustClicked()

                HoverHandler {
                    cursorShape: Qt.PointingHandCursor
                }
            }
        }
    }
}
