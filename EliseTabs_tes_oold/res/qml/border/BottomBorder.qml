import QtQuick 2.0

MouseArea {
    height: 5
    anchors {
        left: parent.left
        leftMargin: 5
        right: parent.right;
        rightMargin: 5
        bottom: parent.bottom;
    }
    hoverEnabled: true
    property point clickPos

    onEntered: _window.setCursorShape(Qt.SizeVerCursor)
    onExited: _window.setCursorShape(Qt.ArrowCursor)

    onPressed: clickPos = Qt.point(mouse.x, mouse.y)

    onPositionChanged:
    {
        if (pressedButtons & Qt.LeftButton) {
            _window.moveBottom(Qt.point(mouse.x, mouse.y), clickPos)
        }
    }

}
