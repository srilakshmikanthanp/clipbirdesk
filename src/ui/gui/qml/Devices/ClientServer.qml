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

    required property ClipbirdQmlClientServer server
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
            color: Material.color(Material.Blue, Material.Shade200)
        }

        ColumnLayout {
            Layout.fillWidth: true
            spacing: 2

            Text {
                text: root.server.name
                font.pixelSize: 16
                font.bold: true
                color: Material.primaryTextColor
                Layout.fillWidth: true
            }

            Text {
                text: qsTr("Available")
                font.pixelSize: 13
                color: Material.secondaryTextColor
                Layout.fillWidth: true
            }
        }

        Button {
            text: qsTr("Join")
            hoverEnabled: false
            flat: true
            Material.foreground: Material.accentColor
            onClicked: root.joinClicked()

            HoverHandler {
                cursorShape: Qt.PointingHandCursor
            }
        }
    }
}
