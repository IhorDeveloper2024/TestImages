import QtQuick
import QtQuick.Controls 2.15
import QtQuick.Layouts
import DataController 1.0

Rectangle {
    id: root
    color: "lightgreen"
    InitAnimation {
        id: animationFrame
        anchors.top: root.top
        anchors.topMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter
        width: root.width * 0.8
        height: root.height * 0.8
    }
    Item {
        id: titleName
        width: root.width
        anchors.top: animationFrame.bottom
        anchors.topMargin: 10
        height: 20
        Text {
            anchors.centerIn: parent
            id: textWidget
            text: "Select widgets implementation: QML or Qt Widget:"
        }
    }
    Item {
        id: buttonsGroup
        anchors.top: titleName.bottom
        anchors.left: root.left
        anchors.right: root.right
        anchors.bottom: root.bottom
        RowLayout {
            anchors.centerIn: parent
            Button {
                id: buttonQML
                text: "QML"
                onClicked: {
                    DataController.select("QML")
                }
            }
            Button {
                id: buttonQtWidget
                text: "Qt  Widgets"
                onClicked: {
                    DataController.select("QtWidget")
                }
            }

        }
        Button {
            id: exitButton
            anchors.right: buttonsGroup.right
            anchors.rightMargin: 10
            anchors.verticalCenter: buttonsGroup.verticalCenter
            text: "Exit"
            onClicked: {
                DataController.select("exit")
            }
        }
    }
}
