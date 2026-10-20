import QtQuick
import Qt.labs.platform

Window {
    width: 300
    height: 300
    visible: true
    property int id: 0
    property string m_str: '自动模式'
    title: qsTr("模拟路口1-自动模式")

    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.RightButton
        onClicked: rightBtnMenu.open()
    }

    Menu {
        id: rightBtnMenu
        MenuItem {
            text: qsTr("自动模式")
            onTriggered: sc.setMode(0)
        }

        MenuItem {
            text: qsTr("水平通行模式")
            onTriggered: sc.setMode(1)
        }

        MenuItem {
            text: qsTr("垂直通行模式")
            onTriggered: sc.setMode(2)
        }

        MenuItem {
            text: qsTr("禁止通行模式")
            onTriggered: sc.setMode(3)
        }

        MenuItem {
            text: qsTr("夜间模式")
            onTriggered: sc.setMode(4)
        }
    }

    Image {
        id: crossingImg
        visible: false
        source: "images/RoadSig.jpg"
    }

    Canvas {
        id: cav
        anchors.fill: parent
        onPaint: {
            var x, y, w, h
            if(width * crossingImg.height > height * crossingImg.width){
                //比较宽
                h = cav.height
                w = crossingImg.width * h / crossingImg.height
                x = (cav.width - w)/2
                y = 0
            }
            else {
                w = cav.width
                h = crossingImg.height * w / crossingImg.width
                x = 0
                y = (cav.height - h)/2
            }
            var ctx = getContext("2d")
            ctx.clearRect(0,0,cav.width,cav.height)
            ctx.drawImage(crossingImg, x, y, w, h)

            var rl = dc.getLightList()
            var l = rl.length

            var iw = crossingImg.width
            var ih = crossingImg.height
            for(var i = 0; i < l; i++)
            {
                if(!rl[i]['show'])
                    continue
                var rx = rl[i]['x']
                var ry = rl[i]['y']
                var rw = rl[i]['w']
                var rh = rl[i]['h']
                var rc = rl[i]['color']
                var rd = rl[i]['dir']
                if(rd === -1)
                    continue
                ctx.fillStyle = (rc === 2 ? "red" : rc === 1 ? "yellow" : rc === 0 ? "green" : "white")
                ctx.fillRect(x + rx * w / iw, y + ry * h / ih,
                         rw * w / iw, rh * h / ih)
            }
        }
    }
    Component.onCompleted: {
        dc.lightColorChanged.connect(lightColorChangedSlot)
        id = sc.getCrossingId()
        title = '模拟路口' + String(id+1) + '-' + m_str
        sc.modeChanged.connect(modeChangedSlot)
    }
    function lightColorChangedSlot(){
        cav.requestPaint()
    }
    function modeChangedSlot(mode){
        if(mode === 0)
            m_str = '自动模式'
        else if(mode === 1)
            m_str = '水平通行模式'
        else if(mode === 2)
            m_str = '垂直通行模式'
        else if(mode === 3)
            m_str = '禁止通行模式'
        else if(mode === 4)
            m_str = '夜间模式'

        title = '模拟路口' + String(id+1) + '-' + m_str
    }
}







