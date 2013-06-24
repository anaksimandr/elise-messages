import QtQuick 2.0

Rectangle {
    id: button
    //x: 10
    //y: 200
    width: 30
    height: 15
    smooth: true
    visible: true
    clip: true
    radius: 1
    transformOrigin: Item.Center
    border.color: "#57a2f4"
    opacity: 1.0
    color: "white"

    property string label: "Button"

    signal buttonClick()

    Behavior {
        NumberAnimation { property: "color"; easing.type: Easing.OutExpo }
    }

    Text {
        id: buttonLabel
        anchors.centerIn: parent
        font.bold: true
        color: "white"
        text: button.label
    }

    Gradient {
        id: gradientEntered
        GradientStop { position: 0.0; color: "#c1dcf9" }
        GradientStop { position: 1.0; color: "#78a9e0" }
    }

    Gradient {
        id: gradientStandart
        GradientStop { position: 0.0; color: "#86bbf7" }
        GradientStop { position: 1.0; color: "#6996c8" }
    }

    gradient: gradientStandart

    states: [
        State {
            name: "BUTTON_ENTERED"
            PropertyChanges { target: button; gradient: gradientEntered}
        },
        State {
            name: "BUTTON_EXITED"
            PropertyChanges { target: button; gradient: gradientStandart}
        }
    ]

    MouseArea {
        anchors.fill: parent
        onClicked: buttonClick()
        hoverEnabled: true
        onEntered: button.state = "BUTTON_ENTERED"
        onExited:  button.state = "BUTTON_EXITED"
    }
}
