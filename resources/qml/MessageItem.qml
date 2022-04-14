import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3

Item {
    id: root
    property alias text: lblText.text
    property alias time: lblTime.text
    property alias backgroundColor: rectBubble.color
    property alias textColor: lblText.color
    property alias font: lblText.font

    height: lblText.contentHeight + 2 * defMargin + lblTime.height

    Rectangle {
        id: rectBubble
        color: win.bubbleColor
        anchors.fill: parent
        radius: 5
//                width: parent.width
//                width: ListView.view.width
//        anchors.left: parent.left
//        anchors.right: parent.right
//        anchors.margins: 2*defMargin
//        border.color: 'red'//backgroundColor
//        radius: 5

        Text {
            id: lblText
//                    anchors.centerIn: parent
//                    text: modelData
            anchors.fill: parent
            anchors.margins: defMargin
            text: model.text
        }

        Text {
            id: lblTime
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.margins: defMargin/2
            text: model.time
        }
    }
}
