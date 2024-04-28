import QtQuick
import QtQuick.Controls 2.15
import DataController 1.0

Item {
    id: root

    Loader {
        id: loader
        anchors.fill: parent
        sourceComponent: DataController.sourceComponent === "startScreen" ? startScreen : imageScreen
        visible: imageScreen.status === Component.Ready
    }

    Component {
        id: startScreen
        StartScreen { }
    }

    Component {
        id: imageScreen
        ImageScreen { }
    }
}
