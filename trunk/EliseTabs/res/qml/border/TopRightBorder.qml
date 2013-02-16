import QtQuick 2.0

MouseArea {
    width: 5
    height: 5
    anchors {
        top: parent.top;
        right: parent.right
    }
    hoverEnabled: true
    property point clickPos

    onEntered: _window.setCursorShape(Qt.SizeBDiagCursor)
    onExited: _window.setCursorShape(Qt.ArrowCursor)

    onPressed: clickPos = Qt.point(mouse.x, mouse.y)

    onPositionChanged:
    {
        if (pressedButtons & Qt.LeftButton) {
            _window.moveTopRight(Qt.point(mouse.x, mouse.y), clickPos)
        }
    }

}
