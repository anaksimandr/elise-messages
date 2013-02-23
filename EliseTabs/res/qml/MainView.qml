import QtQuick 2.0

Rectangle {

    TabbedUI {
        id: tabs
        anchors.fill: parent;
    }

    function createTabObject() {
        tabs.createTabObject();
    }

}
