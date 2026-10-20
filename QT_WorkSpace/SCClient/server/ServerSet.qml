import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    color: "white"
    radius: 8

    ColumnLayout {
        anchors.centerIn: parent
        width: 350
        spacing: 20

        Text {
            text: "服务器连接设置"
            font.pixelSize: 24
            font.bold: true
            Layout.alignment: Qt.AlignHCenter
        }

        RowLayout {
            Label { text: "IP地址：" }
            TextField {
                id: ipInput
                text: nc.serverIp
                placeholderText: "127.0.0.1"
                Layout.fillWidth: true
            }
        }

        RowLayout {
            Label { text: "端口：" }
            TextField {
                id: portInput
                text: nc.serverPort
                placeholderText: "10086"
                validator: IntValidator { bottom: 1; top: 65535 }
                Layout.fillWidth: true
            }
        }

        Button {
            text: "保存并重连"
            Layout.alignment: Qt.AlignHCenter
            onClicked: {
                nc.connectToServer(ipInput.text, parseInt(portInput.text))
            }
        }

        Text {
            text: "提示：修改后会自动断开当前连接并重连"
            font.pixelSize: 12
            color: "#7f8c8d"
            Layout.alignment: Qt.AlignHCenter
        }
    }
}
