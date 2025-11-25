import QtQuick
import QtQuick.Controls.Material
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root
    width: parent.width

    signal copyClicked
    signal deleteClicked

    RowLayout {
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.margins: 6
        spacing: 20

        Button {
            icon.source: "qrc:/images/copy.png"
            icon.width: 20
            icon.height: 20
            flat: true
            ToolTip.text: qsTr("Copy")
            ToolTip.visible: hovered
            onClicked: root.copyClicked()

            HoverHandler {
                cursorShape: Qt.PointingHandCursor
            }
        }

        Button {
            icon.source: "qrc:/images/delete.png"
            icon.width: 20
            icon.height: 20
            flat: true
            ToolTip.text: qsTr("Delete")
            ToolTip.visible: hovered
            onClicked: root.deleteClicked()

            HoverHandler {
                cursorShape: Qt.PointingHandCursor
            }
        }
    }
}
