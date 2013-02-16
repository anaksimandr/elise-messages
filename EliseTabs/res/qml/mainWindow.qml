import QtQuick 2.0

import "border"

Rectangle {
    width: 100
    height: 100
    color: "#00000000"
    //color: "#FFFFFFFF"
    //opacity: 0.1
    smooth: true

    BorderImage {
        source: "../img/border.png"
        anchors.fill: parent
        border { left: 5; top: 5; right: 5; bottom: 5; }
        horizontalTileMode: BorderImage.Stretch
        verticalTileMode: BorderImage.Stretch
    }

    MouseArea {
        id: moveArea
        anchors.fill: parent
        property point clickPos: "1,1"

        onPressed: clickPos  = Qt.point(mouse.x, mouse.y)

        onPositionChanged:
        {
            var delta = Qt.point(mouse.x - clickPos.x, mouse.y - clickPos.y)
            _view.setX(_view.x + delta.x)
            _view.setY(_view.y + delta.y)
        }
    }

    //-- Border
    LeftBorder { }
    LeftTopBorder { }
    TopBorder { }
    TopRightBorder { }
    RightBorder { }
    RightBottomBorder { }
    BottomBorder { }
    BottomLeftBorder { }

}
