import QtQuick 2.0

Rectangle {

    VisualItemModel {
        id: tabsModel
        Tab {
            name: "Tab 1"
            icon: "pics/tab0.png"

            color: "yellow"
            Text {
                anchors.centerIn: parent
                text: "This is page 1"
            }
        }
        Tab {
            name: "Tab 2"
            icon: "pics/tab1.png"

            color: "green"
            Text {
                anchors.centerIn: parent
                text: "This is page 2"
            }
        }
        Tab {
            name: "Tab 3"
            icon: "pics/tab2.png"

            color: "red"
            Text {
                anchors.centerIn: parent
                text: "This is page 3"
            }
        }
    }

    TabbedUI {
        tabsHeight: 72
        tabIndex: 1
        tabsModel: tabsModel
    }

    function createTabObject() {
        var component;
        var sprite;
        //console.log("1");
        component = Qt.createComponent("Tab.qml");
        //console.log("2");
        sprite = component.createObject(tabsModel, {"x": 0, "y": 0, "name": "test", "icon": ".png"});
        //console.log("3");
        if (sprite === null) {
            // Error Handling
            console.log("Error creating object");
        }
    }
}
