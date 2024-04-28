import QtQuick
import QtQuick.Controls 2.15
import DataController 1.0
import FramesPlugin 1.0

Window {
    id: root
    width: 640
    height: 480
    visible: DataController.isVisible
    title: qsTr("Image Compression Application")

    FramesPlugin {
    anchors.fill: parent
    }
}
