import QtQuick
import Atoms

ButtonIconDesign {
    id: root

    signal clicked

    property string vIconName: ""
    property real vSize: 40
    property color vColorBackground: Colors.color.background2
    property color vColorIcon: Colors.color.background0

    button.onClicked: function (){
        root.clicked();
    }
}
