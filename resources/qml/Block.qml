import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3

Rectangle {
    implicitWidth: 200
//    width: 200
    implicitHeight: 100
    color: Qt.rgba(Math.random(), Math.random(), Math.random())
    
    Text {
        anchors.centerIn: parent
        font.pointSize: 16
        text: 'Width: ' + parent.width + '\nHeight: ' + parent.height
    }
}
