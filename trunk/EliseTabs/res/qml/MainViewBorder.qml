import QtQuick 2.0

import "border"

Rectangle {
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

    MainView {
        anchors {
            fill: parent;
            margins: 5;
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

    //property int buttonPosStep: 40
    //property int buttonPosBegin: 15

    Button {
        id: buttonClose
        anchors {
            top: parent.top;
            right: parent.right;
            rightMargin: 5;
        }

        label: "close"

        onButtonClick: _window.close()
    }

    Button {
        id: buttonMinimize
        anchors {
            top: parent.top;
            right: buttonClose.left;
            rightMargin: 0;
        }

        label: "min"

        onButtonClick: _window.minimize()
    }

}
