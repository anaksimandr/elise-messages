import QtQuick 2.0
import QtDesktop 1.0

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

    TextArea {
        id: inputArea;
        height: 55;
        anchors { left: parent.left; right: parent.right; bottom: parent.bottom; }
    }

    Gradient {
        id: gradientStandart
        GradientStop { position: 0.0; color: "#86bbf7" }
        GradientStop { position: 1.0; color: "#6996c8" }
    }
}
