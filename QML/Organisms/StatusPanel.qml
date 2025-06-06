import QtQuick

StatusPanelDesign {
    id: root

    navigationBar {
        onNavigate: function (page) {
            window.updatePage(page);
        }
    }
}
