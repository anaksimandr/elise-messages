import QtQuick 2.0

import ProxyWidgets 1.0

Rectangle {
    property string title: "Red"
    anchors.fill: parent

    Rectangle {
        id: view;
        anchors { left: parent.left; top: parent.top; right: parent.right; bottom: buttons.top }
        color: "red"
    }

    Rectangle {
        id: buttons;
        height: 25;
        anchors { left: parent.left; right: parent.right; bottom: inputArea.top; }
        gradient: gradientStandart
    }

    /*TextEdit {
        id: inputArea;
        height: 55;
        anchors { left: parent.left; right: parent.right; bottom: parent.bottom; }
        //color: "blue"
    }*/

    /*Flickable {
        id: inputArea;
        height: 55;
        anchors { left: parent.left; right: parent.right; bottom: parent.bottom; }
        contentWidth: edit.paintedWidth
        contentHeight: edit.paintedHeight
        //clip: true

        function ensureVisible(r)
        {
            if (contentX >= r.x)
                contentX = r.x;
            else if (contentX+width <= r.x+r.width)
                contentX = r.x+r.width-width;
            if (contentY >= r.y)
                contentY = r.y;
            else if (contentY+height <= r.y+r.height)
                contentY = r.y+r.height-height;
        }

        TextEdit {
            id: edit
            anchors.fill: parent;
            focus: true
            activeFocusOnPress: true;
            wrapMode: TextEdit.Wrap
            onCursorRectangleChanged: parent.ensureVisible(cursorRectangle)
        }
    }*/
    ProxyTextEdit {
            id: proxyTextEdit
            width: 300
            height: 500
            //text: "This is QTextEdit"
        }


    Gradient {
        id: gradientStandart
        GradientStop { position: 0.0; color: "#86bbf7" }
        GradientStop { position: 1.0; color: "#6996c8" }
    }
}
