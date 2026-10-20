import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    RowLayout{
        anchors.fill: parent
        spacing: 10
        CrossingGroup{
            crossingId: 0
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
        CrossingGroup{
            crossingId: 1
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
    }
}
