import QtQuick
import QtQuick.Controls

Rectangle {
    color: "white"
    radius: 8

    Grid {
        anchors.centerIn: parent
        columns: 2
        spacing: 30
        Repeater {
            model: dc.getLightStatusList ? dc.getLightStatusList() : []
            Rectangle {
                width: 100; height: 100
                radius: 10
                color: "#f0f0f0"
                border.color: "gray"
                Column {
                    anchors.centerIn: parent
                    spacing: 10
                    Text { text: "灯"+modelData.id; font.bold: true }
                    Rectangle {
                        width: 40; height: 40
                        radius: 20
                        color: modelData.color === 0 ? "green" : (modelData.color === 1 ? "yellow" : "red")
                        border.color: "black"
                    }
                    Text { text: modelData.colorText }
                }
            }
        }
    }
}