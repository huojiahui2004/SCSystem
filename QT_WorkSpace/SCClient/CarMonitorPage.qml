import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    color: "white"
    radius: 8

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 15

        Text {
            text: "车辆监控"
            font.pixelSize: 28
            font.bold: true
            color: "#2c3e50"
            Layout.alignment: Qt.AlignHCenter
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 10

            CarMonitorGroup { carId: 0; Layout.fillHeight: true; Layout.fillWidth: true }
            CarMonitorGroup { carId: 1; Layout.fillHeight: true; Layout.fillWidth: true }
            CarMonitorGroup { carId: 2; Layout.fillHeight: true; Layout.fillWidth: true }
        }
    }
}