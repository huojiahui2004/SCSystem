import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Window {
    width: 1024
    height: 680
    visible: true
    title: qsTr("智慧交通管理系统")

    // ---------- 道路阻断相关属性 ----------
    property int blockedRoadId: -1
    property int countdown: 5

    RowLayout {
        anchors.fill: parent
        spacing: 0

        // ========== 左侧导航菜单 ==========
        Rectangle {
            Layout.preferredWidth: 200
            Layout.fillHeight: true
            color: "#2c3e50"

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 10
                spacing: 10

                // 标题
                Text {
                    text: "第八组"
                    font.pixelSize: 24
                    font.bold: true
                    color: "white"
                    Layout.alignment: Qt.AlignHCenter
                    Layout.bottomMargin: 20
                }

                // 菜单按钮列表
                Repeater {
                    model: [
                        "地图",
                        "车辆监控",
                        "路口监控",
                        "流量监控",
                        "车辆设置",
                        "路口设置",
                        "系统设置",
                    ]

                    delegate: Button {
                        text: modelData
                        flat: true
                        font.pixelSize: 16
                        contentItem: Text {
                            text: parent.text
                            color: parent.hovered ? "#3498db" : "white"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                        background: Rectangle {
                            color: parent.pressed ? "#1a252f" : "transparent"
                            radius: 4
                        }
                        Layout.fillWidth: true
                        Layout.preferredHeight: 45
                        onClicked: {
                            if(index < 4) {
                                swipeView.currentIndex = 0
                            } else {
                                swipeView.currentIndex = index - 3
                            }
                            switch(index) {
                            case 0:
                                dc.showRoad = true
                                dc.showLight = true
                                dc.showCar = true
                                break
                            case 1:
                                dc.showRoad = false
                                dc.showLight = false
                                dc.showCar = true
                                break
                            case 2:
                                dc.showRoad = false
                                dc.showLight = true
                                dc.showCar = false
                                break
                            case 3:
                                dc.showRoad = true
                                dc.showLight = false
                                dc.showCar = false
                                break
                            }
                        }
                    }
                }

                Item { Layout.fillHeight: true }
            }
        }

        // ========== 右侧滑动页面区域 ==========
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#ecf0f1"

            SwipeView {
                id: swipeView
                anchors.fill: parent
                anchors.margins: 10
                currentIndex: 0
                clip: true

                Map { }
                CarSet { }
                CrossingSet { }
                ServerSet { }
            }

            // 底部页面指示点
            PageIndicator {
                id: indicator
                anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                currentIndex: swipeView.currentIndex
                count: swipeView.count
                interactive: true
                spacing: 8
                delegate: Rectangle {
                    width: 8
                    height: 8
                    radius: 4
                    color: index === indicator.currentIndex ? "#3498db" : "#bdc3c7"
                }
            }
        }
    }

    // ---------- 10秒后随机阻断道路 ----------
    Timer {
        id: initBlockTimer
        interval: 10000
        repeat: false
        onTriggered: {
            var total = dc.getTotalRoadCount()
            var available = []
            for (var i = 0; i < total; i++) {
                if (!dc.isRoadBlocked(i)) available.push(i)
            }
            if (available.length === 0) return
            var randIndex = Math.floor(Math.random() * available.length)
            blockedRoadId = available[randIndex]
            dc.setRoadBlocked(blockedRoadId, true)
            countdown = 5
            repairPopup.open()
            countdownTimer.start()
        }
    }

    // 倒计时每秒更新
    Timer {
        id: countdownTimer
        interval: 1000
        repeat: true
        onTriggered: {
            countdown--
            repairText.text = "道路 " + (blockedRoadId + 1) + " 已阻断！请在 " + countdown + " 秒内修复。"
            if (countdown <= 0) {
                countdownTimer.stop()
                repairPopup.close()
                timeoutToast.visible = true
                timeoutToastTimer.start()
            }
        }
    }

    // 修复对话框
    Popup {
        id: repairPopup
        modal: true
        anchors.centerIn: parent
        width: 300
        height: 150
        closePolicy: Popup.NoAutoClose
        ColumnLayout {
            anchors.fill: parent
            spacing: 10
            Text {
                id: repairText
                text: "道路 " + (blockedRoadId + 1) + " 已阻断！请在 5 秒内修复。"
                wrapMode: Text.WordWrap
                Layout.fillWidth: true
            }
            Button {
                text: "修复"
                Layout.alignment: Qt.AlignHCenter
                onClicked: {
                    if (blockedRoadId >= 0) {
                        dc.setRoadBlocked(blockedRoadId, false)
                    }
                    countdownTimer.stop()
                    repairPopup.close()
                    repairSuccessToast.visible = true
                    repairSuccessToastTimer.start()
                }
            }
        }
    }

    // 修复超时提示
    Rectangle {
        id: timeoutToast
        anchors.centerIn: parent
        width: 200
        height: 50
        color: "#333"
        radius: 10
        visible: false
        Text {
            anchors.centerIn: parent
            text: "修复超时！"
            color: "white"
        }
        Timer {
            id: timeoutToastTimer
            interval: 2000
            onTriggered: timeoutToast.visible = false
        }
    }

    // 修复成功提示
    Rectangle {
        id: repairSuccessToast
        anchors.centerIn: parent
        width: 200
        height: 50
        color: "#2c3e50"
        radius: 10
        visible: false
        Text {
            anchors.centerIn: parent
            text: "修复成功！"
            color: "white"
        }
        Timer {
            id: repairSuccessToastTimer
            interval: 2000
            onTriggered: repairSuccessToast.visible = false
        }
    }

    Component.onCompleted: {
        initBlockTimer.start()
    }
}