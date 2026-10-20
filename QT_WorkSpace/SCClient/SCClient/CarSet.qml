import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    RowLayout{
        anchors.fill: parent
        spacing: 10
        CarGroup {
            carId: 0
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
        CarGroup {
            carId: 1
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
        CarGroup {
            carId: 2
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
    }
}
