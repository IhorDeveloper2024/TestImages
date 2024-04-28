import QtQuick
import QtQuick.Controls 2.15
import QtQuick.Layouts
import DataController 1.0

Rectangle {
    id: root
    property int percent: -1
    property bool is_active: true
    property alias text_info: info.text
    property bool is_folder: false
    signal fileLoaded()

    color: is_active ? "gray" : "transparent"
    border.width: 3
    border.color: is_active ? "black" : "transparent"
    radius: 5
    height: 40
    Rectangle {
        id: type
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: is_folder ? height / 10 : 0
        anchors.left: parent.left
        anchors.leftMargin: 20
        height: is_folder ? root.height / 2.5 : root.height / 1.5
        width: is_folder ? root.height / 1.5 : height / 1.2
        border.width: 1
        border.color: is_folder ? "orange" : "gray"
        radius: 2
        color: is_folder ? "yellow" : "white"
        Canvas {
            id: drawingCanvas
            anchors.fill: parent
            onPaint: {
                var ctx = getContext("2d")
                if (is_dir || is_valid) {
                    ctx.beginPath();
                    ctx.reset();
                    drawingCanvas.requestPaint();
                    return;
                }
                ctx.lineWidth = 2;
                ctx.strokeStyle = "red"
                ctx.beginPath()
                ctx.moveTo(0, 0)
                ctx.lineTo(drawingCanvas.width, drawingCanvas.height)
                ctx.moveTo(drawingCanvas.width, 0)
                ctx.lineTo(0 , drawingCanvas.height)
                ctx.closePath()
                ctx.stroke()
            }
        }
    }
    onFileLoaded: {
        drawingCanvas.requestPaint();
    }

    Rectangle {
        id: subType
        anchors.bottom: type.top
        anchors.left: type.left
        visible: is_folder
        border.width: 1
        border.color: "orange"
        height: type.height / 5
        width: type.width / 2
        color: "yellow"
    }

    Text {
        id: info
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: type.right
        anchors.right: indicator.left
        anchors.leftMargin: 20
        anchors.rightMargin: 20
        color: is_active ? "white" : "gray"
        elide: Text.AlignRight
        clip: true
    }

    Rectangle {
        id: indicator
        visible: !is_folder && percent>= 0 && percent < 100
        onVisibleChanged: {
            root.fileLoaded()
        }

        anchors.verticalCenter: parent.verticalCenter
        anchors.right: root.right
        anchors.rightMargin: 10
        height:  root.height / 2
        width: root.width / 4
        border.color: "gray"
        border.width: 1
        color: "white"
        radius: 3

        Rectangle {
            id: line
            color: "green"
            anchors.fill: parent
            anchors.rightMargin: indicator.width * (100 - percent) / 100
        }

        Text {
            anchors.centerIn: parent
            text: percent + " %"
        }
    }
}


