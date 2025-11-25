import QtQuick
import QtQuick.Controls.Material
import QtQuick.Controls
import QtQuick.Layouts
import Clipbird.Qml

ApplicationWindow {
    id: window
    minimumWidth: 700
    minimumHeight: 500
    width: 900
    height: 600
    visible: false
    title: "ClipBird"
    Material.theme: Application.styleHints.colorScheme === Qt.Dark ? Material.Dark : Material.Light
    Material.accent: Material.Blue
    Material.primary: Material.DeepPurple

    ListModel {
        id: menuModel
        ListElement {
            name: "Devices"
            page: "Devices/Devices.qml"
        }
        ListElement {
            name: "History"
            page: "History/History.qml"
        }
        ListElement {
            name: "Trust"
            page: "Trust/Trust.qml"
        }
        ListElement {
            name: "Settings"
            page: "Settings/Settings.qml"
        }
        ListElement {
            name: "About Us"
            page: "AboutUs/AboutUs.qml"
        }
    }

    property int selectedIndex: 0

    RowLayout {
        anchors.fill: parent

        NavigationMenu {
            id: sideMenu
            Layout.preferredWidth: 200
            Layout.fillHeight: true
            model: menuModel
            selectedIndex: window.selectedIndex

            onMenuClicked: index => {
                window.selectedIndex = index;
            }
        }

        Loader {
            id: pageLoader
            Layout.fillWidth: true
            Layout.fillHeight: true
            source: menuModel.get(window.selectedIndex).page
        }
    }
}
