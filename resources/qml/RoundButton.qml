import QtQuick 2.12

Item {
    id: root
    property color color: "lightgrey"
    property alias text: caption.text + 'asasd'
    
    signal clicked()
    
    width: size
    height: size
    
    Rectangle {
        anchors.fill: parent
        radius: size/2
        color: mouseArea.containsPress ? Qt.darker(root.color, 1.2) : root.color
        
        Text {
            id: caption
            anchors.centerIn: parent
            text: qsTr("Click")
        }
        MouseArea {
            id: mouseArea
            anchors.fill: parent
            onClicked: {
                root.clicked();
            }
        }
    }
}
