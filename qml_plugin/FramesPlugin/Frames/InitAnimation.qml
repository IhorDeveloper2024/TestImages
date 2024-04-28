import QtQuick

Item {
    id: root

    Item {
        id: book
        anchors.verticalCenter: root.verticalCenter
        anchors.left: root.left
        anchors.leftMargin: book.width / 10
        width: root.width / 5
        height: width * 1.5

        Text {
            id: info
            anchors.top: book.top
            anchors.horizontalCenter: book.horizontalCenter
            text: "BMP"
        }

        Image {
            id: bookImage
            anchors.top: info.bottom
            anchors.horizontalCenter: book.horizontalCenter
            anchors.topMargin: book.width / 20
            anchors.bottom: book.bottom

            source: "qrc:/images/book.jpg"
            width: book.width
        }
    }

    Image {
        id: left_arrow
        anchors.left: book.right
        anchors.verticalCenter: root.verticalCenter
        source: "qrc:/images/arrow.png"
        rotation: 180
        width: root.width / 15
        height: width
    }
    Image {
        id: compressor
        anchors.left: left_arrow.right
        anchors.right: right_arrow.left
        anchors.verticalCenter: left_arrow.verticalCenter
        source: "qrc:/images/Compressor.jpg"

        height: compressor.width * 0.7
    }
    Image {
        id: right_arrow
        anchors.right: rightBook.left
        anchors.verticalCenter: rightBook.verticalCenter
        source: "qrc:/images/arrow.png"
        rotation: 180
        width: root.width / 15
        height: width
    }

    Item {
        id: rightBook
        anchors.verticalCenter: root.verticalCenter
        anchors.right: root.right
        anchors.rightMargin: book.width / 10
        width: book.width / 3
        height: rightBook.width * 1.5

        Text {
            id: rightInfo
            anchors.top: rightBook.top
            anchors.horizontalCenter: rightBook.horizontalCenter
            text: "BRANCH"
        }

        Image {
            id: bookBranch
            anchors.top: rightInfo.bottom
            anchors.horizontalCenter: rightBook.horizontalCenter
            anchors.topMargin: rightBook.width / 20
            anchors.bottom: rightBook.bottom

            source: "qrc:/images/book.jpg"
            width: rightBook.width
        }
    }
}
