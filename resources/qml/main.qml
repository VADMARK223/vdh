import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12

Window {
    id: win
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    Button {
        id: button1
        text: "Click me"
        anchors.centerIn: parent

        onClicked: {
           win.color = Qt.rgba(Math.random(),Math.random(),Math.random())
        }
    }
}