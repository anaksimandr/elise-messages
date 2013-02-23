import QtQuick 2.0

Rectangle {
    property string title: "Red"
    anchors.fill: parent
    color: "#e3e3e3"

    Rectangle {
        anchors.fill: parent; //anchors.margins: 20
        color: "#ff7f7f"
        Text {
            width: parent.width - 20
            anchors.centerIn: parent; horizontalAlignment: Qt.AlignHCenter
            text: "Roses are red"
            font.pixelSize: 20
            wrapMode: Text.WordWrap
        }
    }
}
