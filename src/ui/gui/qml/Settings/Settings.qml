import QtQuick
import QtQuick.Controls.Material
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window
import Clipbird.Qml

Page {
    id: root
    title: qsTr("Settings")

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 24
        spacing: 0

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: bluetoothRow.implicitHeight + 16
            color: "transparent"

            RowLayout {
                id: bluetoothRow
                anchors.fill: parent
                anchors.margins: 8
                spacing: 16

                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 4

                    Label {
                        text: qsTr("Use Bluetooth")
                        font.pixelSize: 16
                        font.bold: true
                        Layout.fillWidth: true
                    }

                    Label {
                        text: qsTr("Use Bluetooth for device synchronization")
                        font.pixelSize: 13
                        wrapMode: Text.WordWrap
                        Layout.fillWidth: true
                    }
                }

                Switch {
                    id: bluetoothSwitch
                    checked: ClipbirdQmlApplicationState.useBluetooth
                    Layout.alignment: Qt.AlignVCenter

                    onToggled: {
                        ClipbirdQmlApplicationState.setUseBluetooth(checked);
                    }

                    Connections {
                        target: ClipbirdQmlApplicationState
                        function onUseBluetoothChanged(useBluetooth) {
                            bluetoothSwitch.checked = useBluetooth;
                        }
                    }
                }
            }
        }

        Item {
            Layout.fillHeight: true
        }
    }
}
