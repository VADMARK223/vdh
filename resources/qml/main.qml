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
    readonly property int defMargin: 10
    readonly property color panelColor: '#17212B'
    readonly property color bubbleColor: '#2b5278'
    readonly property color bgColor: '#0E1621'
    readonly property color textColor: 'white'
//    minimumWidth: columnLayout.implicitWidth + 2 * defMargin

    MessageEditor {
        id: editor
    }

    Page {
        id: page
        anchors.fill: parent

        footer: MessageEditor {
            onNewMessage: {
                var newMsg = {};
                newMsg.text = msg;
                newMsg.time = Qt.formatDateTime(new Date(), 'hh:mm');
                listModel.append(newMsg);
            }
        }

        background: Rectangle {
            color: bgColor
        }

        ListView {
            id: listView
            anchors.fill: parent
            spacing: defMargin
            ScrollBar.vertical: ScrollBar {}

            /*model: [
                'first',
                'second',
                'third',
                'fourth'
            ]*/

            model: listModel

            delegate: MessageItem {
                height: 60
//                width: parent.width
//                width: ListView.view.width
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.margins: 2*defMargin
//                backgroundColor: 'green'
                text: model.text
                time: model.time

            }
        }

        ListModel {
            id: listModel
            ListElement {
                text: 'first'
                time: '10:34'
            }
            ListElement {
                text: 'second'
                time: '10:35'
            }
            ListElement {
                text: 'third'
                time: '10:36'
            }
            ListElement {
                text: 'fourth'
                time: '10:37'
            }

        }
    }



    ColumnLayout {
        id: columnLayout
        visible: false
        spacing: defMargin
        anchors.fill: parent
        anchors.margins: defMargin


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
        visible: false
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
