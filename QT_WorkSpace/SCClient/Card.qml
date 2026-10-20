import QtQuick

Rectangle {
    property string title: ""
    property var value: 0
    property color cardColor: "#3498db"
    implicitHeight: 100
    radius: 8
    color: Qt.lighter(cardColor, 1.2)
    border.color: cardColor
    Column {
        anchors.centerIn: parent
        spacing: 5
        Text {
            text: title
            font.pixelSize: 14
            color: "#2c3e50"
            anchors.horizontalCenter: parent.horizontalCenter
        }
        Text {
            text: value
            font.pixelSize: 32
            font.bold: true
            color: cardColor
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }
}
