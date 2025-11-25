import QtQuick
import QtQuick.Controls.Material
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window
import Clipbird.Qml

Page {
    id: root
    title: qsTr("About Us")
    height: column.implicitHeight

    ColumnLayout {
        id: column
        anchors.centerIn: parent
        spacing: 10

        Image {
            id: logo
            source: "qrc:/images/logo.png"
            Layout.preferredWidth: 128
            Layout.preferredHeight: 128
            fillMode: Image.PreserveAspectFit
            Layout.alignment: Qt.AlignHCenter
        }

        Label {
            id: versionLabel
            text: qsTr("Version: ") + ClipbirdQmlConstants.getAppVersion()
            font.pixelSize: 14
            horizontalAlignment: Text.AlignHCenter
            Layout.alignment: Qt.AlignHCenter
        }

        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            spacing: 12

            Button {
                id: webButton
                text: qsTr("Website")
                icon.source: "qrc:/images/web.png"
                onClicked: Qt.openUrlExternally(ClipbirdQmlConstants.getAppHomePage())
            }

            Button {
                id: bugButton
                text: qsTr("Report Bug")
                icon.source: "qrc:/images/bug.png"
                onClicked: Qt.openUrlExternally(ClipbirdQmlConstants.getAppIssuePage())
            }

            Button {
                id: donateButton
                text: qsTr("Donate")
                icon.source: "qrc:/images/dollar.png"
                onClicked: Qt.openUrlExternally(ClipbirdQmlConstants.getAppDonatePage())
            }
        }
    }
}
