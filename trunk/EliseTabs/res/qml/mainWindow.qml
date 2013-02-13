import QtQuick 2.0

Rectangle {
    id: main
    width: 200
    height: 200
    color: "#e8b4b4"

    Flipable {
        id: flipable1
        x: 0
        y: 0
        width: 200
        height: 200
        opacity: 0.81
    }
    Image {
        id: image1
        x: 60
        y: 23
        width: 100
        height: 131
        source: "qrc:/img/main.png"
    }
}
