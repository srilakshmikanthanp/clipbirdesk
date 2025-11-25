import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Material
import QtQuick.Controls
import Clipbird.Qml

Rectangle {
    id: root
    width: parent.width
    height: content.implicitHeight + 32
    color: Material.backgroundColor

    property string name
    property string certificate
    signal remove

    RowLayout {
        id: content
        anchors.fill: parent
        anchors.margins: 16

        ColumnLayout {
            Layout.fillWidth: true
            spacing: 2

            Text {
                text: root.name
                font.pixelSize: 17
                font.bold: true
                Layout.fillWidth: true
                color: Material.primaryTextColor
            }

            Text {
                text: ClipbirdQmlHashHelper.sha256(root.certificateBase64)
                font.pixelSize: 13
                wrapMode: Text.WrapAnywhere
                color: Material.secondaryTextColor
                Layout.fillWidth: true
            }
        }

        Button {
            id: deleteButton
            text: qsTr("Delete")
            hoverEnabled: false
            flat: true
            Material.foreground: Material.accentColor
            onClicked: root.remove()

            HoverHandler {
                cursorShape: Qt.PointingHandCursor
            }
        }
    }
}
