pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls.Material
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: root
    width: parent.width
    height: content.implicitHeight + 20
    color: Material.backgroundColor

    signal sendClicked

    RowLayout {
        id: content
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        Text {
            text: qsTr("Send clipboard content to connected devices")
            font.pixelSize: 14
            color: Material.primaryTextColor
            Layout.fillWidth: true
            wrapMode: Text.WordWrap
        }

        Button {
            text: qsTr("Send")
            flat: false
            Material.background: Material.accentColor
            onClicked: root.sendClicked()
            hoverEnabled: false

            HoverHandler {
                cursorShape: Qt.PointingHandCursor
            }
        }
    }

    layer.enabled: true
    layer.effect: ShaderEffect {
        property real shadowOpacity: 0.1
    }
}
