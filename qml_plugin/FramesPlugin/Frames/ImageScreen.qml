import QtQuick.Controls
import QtQuick
import QtQuick.Layouts
import DataController 1.0
import QtQuick.Dialogs

Rectangle {
    id: root
    color: "lightblue"
    Rectangle {
        id: textWidget
        color: "green"
        anchors.top: root.top
        width: root.width
        height: 40
        Text {
            anchors.centerIn: parent
            text: "IMAGE SCREEN"
        }
    }

    Rectangle {
        id: pathWidget
        color: "lightBlue"
        anchors.top: textWidget.top
        width: root.width
        height: 40
        Text {
            anchors.left: parent.left
            anchors.leftMargin: parent.width / 20
            anchors.verticalCenter: parent.verticalCenter
            text: " Path: " + DataController.path
        }
        z: 100
    }

    Component {
    id: highlight
    Rectangle {
        width: 180; height: 40
        color: "lightsteelblue"; radius: 5
        Behavior on y {
            SpringAnimation {
                spring: 3
                damping: 0.2
            }
        }
      }
    }
    FilesListView {
        id: view
        anchors.top: pathWidget.bottom
        anchors.left: root.left
        width: root.width / 2
        height: image.height
        MessageDialog {
            id: dialog
            title: "Error: Can't write"
            onAccepted: {
                close()
                parent.focus = true
            }
        }
    }
    Image {
        id: image
        anchors.left: view.right
        anchors.right: root.right
        anchors.top: pathWidget.bottom
        anchors.bottom: rectang.top
        source: "image://ImagePic/" + view.imageIndex
        fillMode: Image.PreserveAspectFit
        height: view.height
    }
    Rectangle {
        id: rectang
        color: "yellow"
        anchors.bottom: root.bottom
        width: root.width
        height: root.height / 7
        RowLayout {
            id: botomRow
            anchors.centerIn: parent
            height: buttonQtWidget.height
            Button {
                id: buttonQtWidget
                text: "SAVE FILE"
                onClicked: {
                    var result = DataController.save();
                    if (result === "") {
                        view.currentIndex = 0;
                        DataController.selectImage(0);
                    }
                    else {
                        dialog.text = result;
                        dialog.open();
                    }
                }
            }
            Item {width: root.width / 3}
            Button {
                id: prevScreen
                text: "Previous screen"
                onClicked: {
                    DataController.select("previous")
                }
            }
        }
    }
}
