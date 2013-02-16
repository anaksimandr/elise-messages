import QtQuick 2.0

MouseArea {
    width: 5
    height: 5
    anchors {
        right: parent.right;
        bottom: parent.bottom;
    }
    hoverEnabled: true
    property point clickPos

    onEntered: _window.setCursorShape(Qt.SizeFDiagCursor)
    onExited: _window.setCursorShape(Qt.ArrowCursor)

    onPressed: clickPos = Qt.point(mouse.x, mouse.y)

    onPositionChanged:
    {
        if (pressedButtons & Qt.LeftButton) {
            _window.moveRightBottom(Qt.point(mouse.x, mouse.y), clickPos)
        }
    }

}
