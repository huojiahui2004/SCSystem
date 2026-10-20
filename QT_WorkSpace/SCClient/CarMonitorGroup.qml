import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

GroupBox {
    id: monitorRoot
    property int carId: 0
    title: "车辆" + (carId + 1)

    function refreshStatus() {
        var status = dc.getCarStatus(carId)
        lightText.text = status.light ? "开" : "关"
        alarmText.text = status.alarm ? "开" : "关"
        runText.text = status.running ? "启动" : "停止"
        lockText.text = status.locked ? "锁定" : "解锁"
        rangeText.text = status.range + " KM"
        mileageText.text = status.mileage + " KM"
        warningText.text = status.warning
    }

    Component.onCompleted: {
        refreshStatus()
        dc.dataChanged.connect(refreshStatus)
    }

    GridLayout {
        anchors.fill: parent
        columns: 2
        columnSpacing: 10
        rowSpacing: 5

        Text { text: "大灯：" }
        Text { id: lightText; font.bold: true }
        Text { text: "双闪：" }
        Text { id: alarmText; font.bold: true }
        Text { text: "启动：" }
        Text { id: runText; font.bold: true }
        Text { text: "锁定：" }
        Text { id: lockText; font.bold: true }
        Text { text: "续航：" }
        Text { id: rangeText }
        Text { text: "里程：" }
        Text { id: mileageText }
        Text { text: "警告：" }
        Text { id: warningText; color: "red" }

        Item { Layout.columnSpan: 2; Layout.fillHeight: true }
    }
}