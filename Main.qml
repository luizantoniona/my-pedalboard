import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window

// import Organisms
// import Pages
Window {
    visible: true
    width: 1280
    height: 720
    title: "My Pedalboard"

    property int currentPage: 0

    enum Pages {
        Home,
        Settings,
        About
    }

    // function updatePage(page) {
    //     stack.clear()
    //     switch (page) {
    //     case Pages.Home:
    //         stack.push(Qt.createComponent("qrc:/qt/qml/Pages/HomePage.qml"))
    //         break
    //     case Pages.Settings:
    //         stack.push(Qt.createComponent("qrc:/qt/qml/Pages/SettingsPage.qml"))
    //         break
    //     case Pages.About:
    //         stack.push(Qt.createComponent("qrc:/qt/qml/Pages/AboutPage.qml"))
    //         break
    //     }
    // }
    ColumnLayout {
        anchors.fill: parent

        // TopBar {
        //     Layout.fillWidth: true
        //     onNavigate: page => currentPage = page
        // }
        StackView {
            id: stack
            Layout.fillWidth: true
            Layout.fillHeight: true

            // initialItem: HomePage {}
        }
    }
}
