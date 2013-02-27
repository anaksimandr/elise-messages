import QtQuick 2.0

Rectangle {
    id: tabWidget

    // Setting the default property to stack.children means any child items
    // of the TabWidget are actually added to the 'stack' item's children.
    // See the "Property Binding"
    // documentation for details on default properties.
    default property alias content: stack.children

    property int current: 0

    onCurrentChanged: setOpacities()
    Component.onCompleted: setOpacities()

    function setOpacities() {
        for (var i = 0; i < stack.children.length; ++i) {
            stack.children[i].opacity = (i == current ? 1 : 0)
        }
    }

    Row {
        id: header
        anchors.bottom: parent.bottom;

        Repeater {
            model: stack.children.length
            delegate: Rectangle {
                width: tabWidget.width / stack.children.length;
                height: 25

                Rectangle {
                    width: parent.width; height: 1
                    anchors { bottom: parent.bottom; bottomMargin: 1 }
                    color: "red"
                }
                BorderImage {
                    anchors { fill: parent; leftMargin: 2; topMargin: 5; rightMargin: 1 }
                    border { left: 7; right: 7 }
                    //source: "tab.png"
                    visible: tabWidget.current == index
                }
                Text {
                    horizontalAlignment: Qt.AlignHCenter; verticalAlignment: Qt.AlignVCenter
                    anchors.fill: parent
                    text: stack.children[index].title
                    elide: Text.ElideRight
                    font.bold: tabWidget.current == index
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: tabWidget.current = index
                }
            }
        }
    }

    Item {
        id: stack
        width: parent.width;
        anchors.top: parent.top;
        anchors.bottom: header.top;
    }

    function createTabObject() {
        var component;
        var sprite;
        component = Qt.createComponent("Tab.qml");
        sprite = component.createObject(stack, { "title": "test" });
        if (sprite === null) {
            // Error Handling
            console.log("Error creating object");
        }
    }

}
