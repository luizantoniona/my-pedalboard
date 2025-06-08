import QtQuick
import QtQuick.Layouts
import Atoms
import Organisms

Item {
    id: root

    ColumnLayout {
        anchors.fill: parent

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true

            AudioInputPanel {
                Layout.preferredWidth: 300
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: Colors.color.background2
                Text {
                    anchors.centerIn: parent
                    text: "Aqui será colocado os pedais. No momento deixar vazio"
                }
            }

            AudioOutputPanel {
                Layout.preferredWidth: 300
            }
        }
    }
}
