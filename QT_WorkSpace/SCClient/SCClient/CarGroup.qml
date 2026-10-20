import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

GroupBox {
    id: root
    title: "车辆1"
    property int carId: 0

    // 状态显示辅助函数
    function refreshStatus() {
        var status = dc.getCarStatus(carId)
        lightStatusText.text = status.light ? "开" : "关"
        alarmStatusText.text = status.alarm ? "开" : "关"
        runStatusText.text = status.running ? "启动" : "停止"
        lockStatusText.text = status.locked ? "锁定" : "解锁"
        speedText.text = status.speed + " KM/H"
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

        // 第一行：密钥标签和输入框
        Text {
            text: "密钥"
            Layout.fillWidth: true
            horizontalAlignment: Qt.AlignHCenter
        }
        TextInput {
            id: carKey
            text: "ADMIN123"
            horizontalAlignment: Qt.AlignHCenter
            Layout.fillWidth: true
        }

        // 控制按钮区（两列）
        Button { text: "打开大灯";Layout.fillWidth: true;Layout.fillHeight: true; onClicked: { nc.sendCarLightSw(root.carId, true, carKey.text); dc.setCarLight(root.carId, true) } }
        Button { text: "关闭大灯";Layout.fillWidth: true;Layout.fillHeight: true; onClicked: { nc.sendCarLightSw(root.carId, false, carKey.text); dc.setCarLight(root.carId, false) } }
        Button { text: "打开双闪";Layout.fillWidth: true;Layout.fillHeight: true; onClicked: { nc.sendCarAlarmLightSw(root.carId, true, carKey.text); dc.setCarAlarm(root.carId, true) } }
        Button { text: "关闭双闪";Layout.fillWidth: true;Layout.fillHeight: true; onClicked: { nc.sendCarAlarmLightSw(root.carId, false, carKey.text); dc.setCarAlarm(root.carId, false) } }
        Button { text: "启动";Layout.fillWidth: true;Layout.fillHeight: true; onClicked: { nc.sendCarRunSw(root.carId, true, carKey.text); dc.setCarRunning(root.carId, true) } }
        Button { text: "停止";Layout.fillWidth: true;Layout.fillHeight: true; onClicked: { nc.sendCarRunSw(root.carId, false, carKey.text); dc.setCarRunning(root.carId, false) } }
        Button { text: "解锁";Layout.fillWidth: true;Layout.fillHeight: true; onClicked: { nc.sendCarLockSw(root.carId, false, carKey.text); dc.setCarLock(root.carId, false) } }
        Button { text: "锁定";Layout.fillWidth: true;Layout.fillHeight: true; onClicked: { nc.sendCarLockSw(root.carId, true, carKey.text); dc.setCarLock(root.carId, true) } }

        // 状态显示区
        Text { text: "大灯状态"; Layout.fillWidth: true; Layout.fillHeight: true; horizontalAlignment: Qt.AlignHCenter; verticalAlignment: Qt.AlignVCenter }
        Text { id: lightStatusText; text: "--"; Layout.fillWidth: true; Layout.fillHeight: true; horizontalAlignment: Qt.AlignHCenter; verticalAlignment: Qt.AlignVCenter }

        Text { text: "双闪状态"; Layout.fillWidth: true; Layout.fillHeight: true; horizontalAlignment: Qt.AlignHCenter; verticalAlignment: Qt.AlignVCenter }
        Text { id: alarmStatusText; text: "--"; Layout.fillWidth: true; Layout.fillHeight: true; horizontalAlignment: Qt.AlignHCenter; verticalAlignment: Qt.AlignVCenter }

        Text { text: "启动状态"; Layout.fillWidth: true; Layout.fillHeight: true; horizontalAlignment: Qt.AlignHCenter; verticalAlignment: Qt.AlignVCenter }
        Text { id: runStatusText; text: "--"; Layout.fillWidth: true; Layout.fillHeight: true; horizontalAlignment: Qt.AlignHCenter; verticalAlignment: Qt.AlignVCenter }

        Text { text: "锁定状态"; Layout.fillWidth: true; Layout.fillHeight: true; horizontalAlignment: Qt.AlignHCenter; verticalAlignment: Qt.AlignVCenter }
        Text { id: lockStatusText; text: "--"; Layout.fillWidth: true; Layout.fillHeight: true; horizontalAlignment: Qt.AlignHCenter; verticalAlignment: Qt.AlignVCenter }

        Text { text: "续航"; Layout.fillWidth: true; Layout.fillHeight: true; horizontalAlignment: Qt.AlignHCenter; verticalAlignment: Qt.AlignVCenter }
        Text { id: rangeText; text: "-- KM"; Layout.fillWidth: true; Layout.fillHeight: true; horizontalAlignment: Qt.AlignHCenter; verticalAlignment: Qt.AlignVCenter }

        Text { text: "里程"; Layout.fillWidth: true; Layout.fillHeight: true; horizontalAlignment: Qt.AlignHCenter; verticalAlignment: Qt.AlignVCenter }
        Text { id: mileageText; text: "-- KM"; Layout.fillWidth: true; Layout.fillHeight: true; horizontalAlignment: Qt.AlignHCenter; verticalAlignment: Qt.AlignVCenter }

        Text { text: "速度"; Layout.fillWidth: true; Layout.fillHeight: true; horizontalAlignment: Qt.AlignHCenter; verticalAlignment: Qt.AlignVCenter }
        Text { id: speedText; text: "-- KM/H"; Layout.fillWidth: true; Layout.fillHeight: true; horizontalAlignment: Qt.AlignHCenter; verticalAlignment: Qt.AlignVCenter }

        Text { text: "警告"; Layout.fillWidth: true; Layout.fillHeight: true; horizontalAlignment: Qt.AlignHCenter; verticalAlignment: Qt.AlignVCenter }
        Text { id: warningText; text: "--"; Layout.fillWidth: true; Layout.fillHeight: true; horizontalAlignment: Qt.AlignHCenter; verticalAlignment: Qt.AlignVCenter; color: "red" }



        Item { Layout.columnSpan: 2; Layout.fillHeight: true }
    }

    onCarIdChanged: {
        title = '车辆' + String(carId + 1)
        refreshStatus()
    }
}