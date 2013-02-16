import QtQuick 2.0

Rectangle {
    width: 100
    height: 100
    color: "#00000000"
    //color: "#FFFFFFFF"
    //opacity: 0.1
    border.width: 3
    border.color: "blue"

    MouseArea {
        id: mouseRegion
        anchors.fill: parent
        property point clickPos: "1,1"

        onPressed: {
            clickPos  = Qt.point(mouse.x, mouse.y)
        }

        onPositionChanged: {
            var delta = Qt.point(mouse.x - clickPos.x, mouse.y - clickPos.y)

            _view.setX(_view.x + delta.x)
            _view.setY(_view.y + delta.y)
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
            //hoverEnabled: true
            property point clickPos: "1,1"

            onEntered: _window.setCursorShape(Qt.SizeHorCursor)
            onExited: _window.setCursorShape(Qt.ArrowCursor)

            onPressed: {
                clickPos = Qt.point(mouse.x, mouse.y)
            }

            onPositionChanged: {
                var delta = Qt.point(mouse.x - clickPos.x, mouse.y - clickPos.y)
                _view.setWidth(_view.width + delta.x)
                clickPos = Qt.point(mouse.x, mouse.y)
                //_mainWindow.setHeight(_mainWindow.height + delta.y)
            }

        }
    }
}
