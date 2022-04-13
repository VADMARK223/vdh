import QtQuick 2.12 as QQ2
import QtQuick.Window 2.12
import QtQuick.Controls 2.12

QQ2.Item {
    id: root
    property int size: 200
    property color color: "green"
    property alias text: caption.text
    
    width: size
    height: size

    signal clicked()
    
    QQ2.Rectangle {
        anchors.fill: parent
        radius: size/2
        color: mouseArea.containsPress ? Qt.darker(root.color, 1.2) : root.color
        
        QQ2.Text {
            id: caption
            anchors.centerIn: parent
            text: qsTr("Default text")
        }
        QQ2.MouseArea {
            id: mouseArea
            anchors.fill: parent
            onClicked: {
                root.clicked();
            }
        }
    }
}
