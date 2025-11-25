pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls.Material
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root
    width: parent.width
    height: parent.height

    property string emptyMessage: ""
    property var devices: ({})
    signal removeClicked(string name)

    StackLayout {
        anchors.fill: parent
        currentIndex: root.devices.length === 0 ? 0 : 1

        Rectangle {
            color: "transparent"

            Text {
                anchors.centerIn: parent
                text: root.emptyMessage
                font.pixelSize: 14
                color: Material.secondaryTextColor
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.WordWrap
            }
        }

        ListView {
            model: root.devices
            spacing: 10
            clip: true

            delegate: TrustedDevice {
                required property string deviceName
                required property string certificate
                name: deviceName
                certificate: certificate
                onRemove: root.removeClicked(deviceName)
            }
        }
    }
}
