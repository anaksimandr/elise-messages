import QtQuick 2.0

Rectangle {
    width: 100
    height: 100
    color: "#00000000"
    //color: "#FFFFFFFF"
    //opacity: 0.1

    MouseArea {
        id: mouseRegion
        anchors.fill: parent
        property point clickPos: "1,1"

        onPressed: {
            clickPos  = Qt.point(mouse.x, mouse.y)
        }

        onPositionChanged: {
            var delta = Qt.point(mouse.x - clickPos.x, mouse.y - clickPos.y)

            /*_mainWindow.pos = Qt.point(_mainWindow.pos.x + delta.x,
                                       _mainWindow.pos.y + delta.y)*/
            _mainWindow.setX(_mainWindow.x + delta.x)
            _mainWindow.setY(_mainWindow.y + delta.y)
        }
    }

    Rectangle {
        id: child
        width: 50
        height: 50
        color: "Red"
        anchors.centerIn: parent

        MouseArea {
            anchors.fill: parent
            property point clickPos: "1,1"

            onPressed: {
                clickPos = Qt.point(mouse.x, mouse.y)
            }

            onPositionChanged: {
                var delta = Qt.point(mouse.x - clickPos.x, mouse.y - clickPos.y)
                _mainWindow.setWidth(_mainWindow.width + delta.x)
                clickPos = Qt.point(mouse.x, mouse.y)
                //_mainWindow.setHeight(_mainWindow.height + delta.y)
            }
        }
    }
}
