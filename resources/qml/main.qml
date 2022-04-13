import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12

Window {
    id: win
    visible: true
    width: 640
    height: 480
    title: qsTr('vdh')

    function updateColor() {
        win.color = Qt.rgba(Math.random(),Math.random(),Math.random());
    }

    Button {
        id: button1
        text: 'Click'
        anchors.centerIn: parent

        onClicked: {
           updateColor();
        }
    }

    RoundButton {
        id: root
        text: 'Click 2'
        onClicked: {
           updateColor();
        }
    }

    TestItem {
        id: test
        size: 200
        text: 'Click circle'
        onClicked: {
           updateColor();
        }
    }
}
