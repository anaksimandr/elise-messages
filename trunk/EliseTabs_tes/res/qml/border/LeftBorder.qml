import QtQuick 2.0

MouseArea {
    width: 5
    anchors {
        left: parent.left
        top: parent.top;
        topMargin: 5;
        bottom: parent.bottom;
        bottomMargin: 5;
    }
    hoverEnabled: true
    property point clickPos

    onEntered: _window.setCursorShape(Qt.SizeHorCursor)
    onExited: _window.setCursorShape(Qt.ArrowCursor)

    onPressed: clickPos = Qt.point(mouse.x, mouse.y)

    onPositionChanged:
    {
        if (pressedButtons & Qt.LeftButton) {
            _window.moveLeft(Qt.point(mouse.x, mouse.y), clickPos)
        }
    }

}
