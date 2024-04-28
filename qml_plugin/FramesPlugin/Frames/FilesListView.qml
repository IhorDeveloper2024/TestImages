import QtQuick
import DataController 1.0

Item {
    property alias currentIndex: root.currentIndex
    property int imageIndex: 0
ListView {
    id: root
    anchors.fill: parent
    model: DataController.filesList
    delegate: FileDelegate {
        width:  root.width
        is_active: root.currentIndex === index
        text_info: name
        is_folder: is_dir
        percent: progress
        onFileLoaded: {
            imageIndex++;
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                DataController.selectImage(index);
                root.currentIndex = index;
                imageIndex++;
            }
            onDoubleClicked: {
                DataController.navigate(root.currentIndex);
            }
        }
    }

    focus: true
    onFocusChanged: {
        if (!focus)
            focus = true;
    }

    Keys.onUpPressed: {
        if (root.currentIndex > 0) {
            DataController.selectImage(root.currentIndex - 1);
            root.currentIndex--;
        }
        imageIndex++;
    }

    Keys.onDownPressed: {
        if (root.currentIndex + 1 < root.count) {
            DataController.selectImage(root.currentIndex + 1);
            root.currentIndex++;
        }
        imageIndex++;
    }

    Keys.onPressed: (event)=> {
        if (event.key === Qt.Key_Return || event.key === Qt.Key_Enter) {
            DataController.navigate(root.currentIndex);
            root.currentIndex = 0;
            event.accepted = true;
        }
    }
}
}
