import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

GroupBox{
    title: "路口1"
    property int crossingId: 0
    GridLayout{
        anchors.fill: parent
        columns: 2
        columnSpacing: 10
        rowSpacing: 2
        Text {
            id: nowMode
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.preferredHeight: 2
            Layout.columnSpan: 2
            text: "--"
            font.pixelSize: parent.height * 0.05
            horizontalAlignment: Qt.AlignHCenter
            verticalAlignment: Qt.AlignVCenter
        }
        Button {
            text: "自动模式"
            Layout.preferredHeight: 2
            Layout.fillHeight: true
            Layout.fillWidth: true
            onClicked: {
                nc.sendCrossingMode(crossingId, 0)
            }
        }
        Button {
            text: "水平通行模式"
            Layout.preferredHeight: 2
            Layout.fillHeight: true
            Layout.fillWidth: true
            onClicked: {
                nc.sendCrossingMode(crossingId, 1)
            }
        }
        Button {
            text: "垂直通行模式"
            Layout.preferredHeight: 2
            Layout.fillHeight: true
            Layout.fillWidth: true
            onClicked: {
                nc.sendCrossingMode(crossingId, 2)
            }
        }
        Button {
            text: "夜间模式"
            Layout.preferredHeight: 2
            Layout.fillHeight: true
            Layout.fillWidth: true
            onClicked: {
                nc.sendCrossingMode(crossingId, 4)
            }
        }
        Button {
            Layout.preferredHeight: 2
            text: "禁止通行模式"
            Layout.fillHeight: true
            Layout.fillWidth: true
            onClicked: {
                nc.sendCrossingMode(crossingId, 3)
            }
        }
        Item{
            Layout.preferredHeight: 10
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.columnSpan: 2
        }
    }
    onCrossingIdChanged: {
        title = '路口' + String(crossingId+1)
    }
    Component.onCompleted: {
        nc.crossingModeChanged.connect(crossingModeChangedSlot)
    }
    function crossingModeChangedSlot(id, mode){
        if(id !== crossingId)
            return
        nowMode.text = getModeText(mode)
    }
    function getModeText(mode){
        if(mode === 0)
            return '自动模式'
        else if(mode === 1)
            return '水平通行模式'
        else if(mode === 2)
            return '垂直通行模式'
        else if(mode === 3)
            return '禁止通行模式'
        else if(mode === 4)
            return '夜间模式'
        return '--'
    }
}
