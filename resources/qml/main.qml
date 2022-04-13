import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3

Window {
    id: win
    visible: true
    width: 640
    height: 640
    title: qsTr('vdh')
    minimumWidth: columnLayout.implicitWidth + 2 * 10


    ColumnLayout {
        id: columnLayout
        spacing: 10
        anchors.fill: parent
        anchors.margins: 10


        Block {
            Layout.alignment: Qt.AlignLeft
        }

        Block {
            Layout.alignment: Qt.AlignHCenter
        }

        Block {
            Layout.alignment: Qt.AlignRight
        }

        Block {
            Layout.fillWidth: true
            Layout.minimumWidth: 300
        }

//        Block {
//            width: 500
//        }

//        Item {
//            Layout.fillHeight: true
//        }
    }

    Rectangle {
        anchors.fill: columnLayout
        border.width: 4
        border.color: 'black'
        color: 'transparent'
    }


    property int sizeUnit: sizeSlider.value

    function updateColor() {
        win.color = Qt.rgba(Math.random(),Math.random(),Math.random());
    }

    Slider {
        id: sizeSlider
        visible: false
        orientation: Qt.Vertical
        height: 200
        anchors.right: parent.right
        anchors.margins: 30
        from: 50
        to: 200
        value: 100
    }

    Button {
        id: centralBtn
        visible: false
        text: 'Click'
        anchors.centerIn: parent

        onClicked: {
           updateColor();
        }
    }

    RoundButton {
        id: roundBtn
        visible: false
        text: 'Click 2'
        onClicked: {
           updateColor();
        }
    }

    TestItem {
        id: test
        visible: false
        size: sizeUnit
        text: 'Click circle'
        onClicked: {
           updateColor();
        }
    }
}
