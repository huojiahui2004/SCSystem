import QtQuick
import QtQuick.Controls
import Qt.labs.platform

Item {
    Image {
        id: mapImg
        source: "images/RoadMap.jpg"
        visible: false
    }
    Image {
        id: carImg
        source: "images/car.png"
        visible: false
    }

    // ==================== 路口右键菜单 ====================
    property int rightClickedCrossingId: -1

    Menu {
        id: crossingMenu
        MenuItem { text: "自动模式"; onTriggered: nc.sendCrossingMode(rightClickedCrossingId, 0) }
        MenuItem { text: "水平通行"; onTriggered: nc.sendCrossingMode(rightClickedCrossingId, 1) }
        MenuItem { text: "垂直通行"; onTriggered: nc.sendCrossingMode(rightClickedCrossingId, 2) }
        MenuItem { text: "禁止通行"; onTriggered: nc.sendCrossingMode(rightClickedCrossingId, 3) }
        MenuItem { text: "夜间模式"; onTriggered: nc.sendCrossingMode(rightClickedCrossingId, 4) }
    }

    // ==================== 车辆右键菜单 ====================
    property int rightClickedCarId: -1

    Menu {
        id: carMenu
        MenuItem { text: "启动"; onTriggered: nc.sendCarRunSw(rightClickedCarId, true, "ADMIN123") }
        MenuItem { text: "停止"; onTriggered: nc.sendCarRunSw(rightClickedCarId, false, "ADMIN123") }
        MenuItem { text: "解锁"; onTriggered: nc.sendCarLockSw(rightClickedCarId, false, "ADMIN123") }
        MenuItem { text: "锁定"; onTriggered: nc.sendCarLockSw(rightClickedCarId, true, "ADMIN123") }
        MenuItem { text: "打开大灯"; onTriggered: nc.sendCarLightSw(rightClickedCarId, true, "ADMIN123") }
        MenuItem { text: "关闭大灯"; onTriggered: nc.sendCarLightSw(rightClickedCarId, false, "ADMIN123") }
        MenuItem { text: "打开双闪"; onTriggered: nc.sendCarAlarmLightSw(rightClickedCarId, true, "ADMIN123") }
        MenuItem { text: "关闭双闪"; onTriggered: nc.sendCarAlarmLightSw(rightClickedCarId, false, "ADMIN123") }
    }

    Canvas {
        id: cav
        anchors.fill: parent

        // ==================== 右键检测 ====================
        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.RightButton
            onClicked: (mouse) => {
                var carId = checkCarClick(mouse.x, mouse.y)
                var crossingId = checkCrossingClick(mouse.x, mouse.y)

                if(carId >= 0) {
                    rightClickedCarId = carId
                    carMenu.open()
                } else if(crossingId >= 0) {
                    rightClickedCrossingId = crossingId
                    crossingMenu.open()
                }
            }
        }

        onPaint: {
            var ctx = getContext('2d')
            ctx.clearRect(0, 0, width, height)

            var dx, dy, dw, dh
            var scale = Math.min(width / mapImg.width, height / mapImg.height)
            dw = mapImg.width * scale
            dh = mapImg.height * scale
            dx = (width - dw) / 2
            dy = (height - dh) / 2
            ctx.drawImage(mapImg, dx, dy, dw, dh)

            var rl = dc.getRoadList()

            // ========== 绘制道路 ==========
            if (dc.showRoad) {
                for (var i = 0; i < rl.length; i++) {
                    var rx = rl[i]['x']
                    var ry = rl[i]['y']
                    var rw = rl[i]['w']
                    var rh = rl[i]['h']
                    var rc = rl[i]['color']
                    if (rl[i]['blocked']) {
                        ctx.fillStyle = "gray"
                    } else {
                        ctx.fillStyle = rc === 0 ? "green" : (rc === 1 ? "yellow" : "red")
                    }
                    ctx.fillRect(dx + rx * dw / mapImg.width,
                                 dy + ry * dh / mapImg.height,
                                 rw * dw / mapImg.width,
                                 rh * dh / mapImg.height)
                }
            }

            // ========== 绘制红绿灯 + 倒计时数字 ==========
            if (dc.showLight) {
                var ll = dc.getLightList()
                for (i = 0; i < ll.length; i++) {
                    var roadX = ll[i]['roadX']
                    var roadY = ll[i]['roadY']
                    var roadW = ll[i]['roadW']
                    var roadH = ll[i]['roadH']
                    var roadDir = ll[i]['roadDir']
                    var lightColor = ll[i]['color']
                    var lightTime = ll[i]['time']
                    if (lightColor === -1) continue

                    var lightRx, lightRy, lightRw, lightRh
                    switch (roadDir) {
                    case 0:
                        lightRw = 4; lightRh = 28;
                        lightRx = roadX - lightRw - 3;
                        lightRy = roadY - (lightRh - roadH) / 2;
                        break;
                    case 1:
                        lightRw = 4; lightRh = 28;
                        lightRx = roadX + roadW + 2;
                        lightRy = roadY - (lightRh - roadH) / 2;
                        break;
                    case 2:
                        lightRh = 4; lightRw = 28;
                        lightRy = roadY - lightRh - 3;
                        lightRx = roadX - (lightRw - roadW) / 2;
                        break;
                    case 3:
                        lightRh = 4; lightRw = 28;
                        lightRy = roadY + roadH + 2;
                        lightRx = roadX - (lightRw - roadW) / 2;
                        break;
                    default: continue;
                    }

                    var fillColor = lightColor === 0 ? "green" : (lightColor === 1 ? "yellow" : "red")
                    ctx.fillStyle = fillColor
                    ctx.fillRect(dx + lightRx * dw / mapImg.width,
                                 dy + lightRy * dh / mapImg.height,
                                 lightRw * dw / mapImg.width,
                                 lightRh * dh / mapImg.height)

                    if (lightTime !== undefined && lightTime >= 0) {
                        var fontSizeBase = 14
                        var textX, textY
                        switch (roadDir) {
                        case 0:
                            textX = lightRx + lightRw + 3;
                            textY = lightRy + lightRh / 2 - fontSizeBase / 2;
                            break;
                        case 1:
                            textX = lightRx - fontSizeBase - 3;
                            textY = lightRy + lightRh / 2 - fontSizeBase / 2;
                            break;
                        case 2:
                            textX = lightRx + lightRw / 2 - fontSizeBase / 3;
                            textY = lightRy + lightRh + 3;
                            break;
                        case 3:
                            textX = lightRx + lightRw / 2 - fontSizeBase / 3;
                            textY = lightRy - 3;
                            break;
                        default:
                            textX = lightRx + lightRw + 3;
                            textY = lightRy;
                        }
                        var canvasX = dx + textX * dw / mapImg.width
                        var canvasY = dy + textY * dh / mapImg.height
                        var canvasFontSize = fontSizeBase * Math.min(dw / mapImg.width, dh / mapImg.height)

                        ctx.save()
                        ctx.font = canvasFontSize + "px sans-serif"
                        var textStr = lightTime.toString()
                        var textW = ctx.measureText(textStr).width
                        ctx.fillStyle = "white"
                        ctx.fillRect(canvasX - 2, canvasY - canvasFontSize + 2, textW + 4, canvasFontSize + 2)
                        ctx.fillStyle = fillColor
                        ctx.fillText(textStr, canvasX, canvasY)
                        ctx.restore()
                    }
                }
            }

            // ========== 绘制车辆 ==========
            if (dc.showCar) {
                var cl = dc.getCarList()
                var carW = 30, carH = 30
                for (i = 0; i < cl.length; i++) {
                    var roadId = cl[i]['road_id']
                    var roadPos = cl[i]['road_pos']
                    if (roadId > rl.length) continue
                    var roadX, roadY, roadW, roadH, roadDir
                    if (roadId < 0) {
                        roadX = 635; roadY = 545; roadW = 4; roadH = 90; roadDir = 3;
                        roadPos = 0 + i * 50
                    } else {
                        roadX = rl[roadId]['x']
                        roadY = rl[roadId]['y']
                        roadW = rl[roadId]['w']
                        roadH = rl[roadId]['h']
                        roadDir = rl[roadId]['dir']
                    }
                    var len, rx, ry
                    switch (roadDir) {
                    case 0:
                        len = roadW - carW;
                        rx = roadX + carW/2 + len * (100 - roadPos) / 100;
                        ry = roadY + roadH/2;
                        break;
                    case 1:
                        len = roadW - carW;
                        rx = roadX + carW/2 + len * roadPos / 100;
                        ry = roadY + roadH/2;
                        break;
                    case 2:
                        len = roadH - carH;
                        rx = roadX + roadW/2;
                        ry = roadY + carH/2 + len * (100 - roadPos) / 100;
                        break;
                    case 3:
                        len = roadH - carH;
                        rx = roadX + roadW/2;
                        ry = roadY + carH/2 + len * roadPos / 100;
                        break;
                    default: continue;
                    }
                    rx = rx - carW/2
                    ry = ry - carH/2
                    var rw = carW, rh = carH
                    rx = dx + rx * dw / mapImg.width
                    ry = dy + ry * dh / mapImg.height
                    rw = rw * dw / mapImg.width
                    rh = rh * dh / mapImg.height
                    ctx.drawImage(carImg, rx, ry, rw, rh)
                    ctx.beginPath()
                    ctx.fillStyle = "#000000"
                    ctx.font = (rh * 0.9).toString() + "px sans-serif"
                    ctx.text((i+1).toString(), rx + rw * 0.25, ry + rh * 0.8)
                    ctx.fill()
                    ctx.closePath()
                }
            }
        }
    }

    // ==================== 检测：鼠标点在哪个车上 ====================
    function checkCarClick(mx, my) {
        var rl = dc.getRoadList()
        var cl = dc.getCarList()
        var dx, dy, dw, dh
        var scale = Math.min(cav.width / mapImg.width, cav.height / mapImg.height)
        dw = mapImg.width * scale
        dh = mapImg.height * scale
        dx = (cav.width - dw) / 2
        dy = (cav.height - dh) / 2

        for(var i = 0; i < cl.length; i++) {
            var roadId = cl[i]['road_id']
            var roadPos = cl[i]['road_pos']

            var roadX, roadY, roadW, roadH, roadDir
            if(roadId < 0 || roadId >= rl.length) {
                // 停车场
                roadX = 635; roadY = 545; roadW = 4; roadH = 90; roadDir = 3
                roadPos = 0 + i * 50
            } else {
                roadX = rl[roadId]['x']
                roadY = rl[roadId]['y']
                roadW = rl[roadId]['w']
                roadH = rl[roadId]['h']
                roadDir = rl[roadId]['dir']
            }

            var carW = 30, carH = 30
            var len, carX, carY
            switch(roadDir) {
            case 0:
                len = roadW - carW
                carX = roadX + carW/2 + len * (100 - roadPos) / 100 - carW/2
                carY = roadY + roadH/2 - carH/2
                break
            case 1:
                len = roadW - carW
                carX = roadX + carW/2 + len * roadPos / 100 - carW/2
                carY = roadY + roadH/2 - carH/2
                break
            case 2:
                len = roadH - carH
                carX = roadX + roadW/2 - carW/2
                carY = roadY + carH/2 + len * (100 - roadPos) / 100 - carH/2
                break
            case 3:
                len = roadH - carH
                carX = roadX + roadW/2 - carW/2
                carY = roadY + carH/2 + len * roadPos / 100 - carH/2
                break
            default: continue
            }

            var screenX = dx + carX * dw / mapImg.width
            var screenY = dy + carY * dh / mapImg.height
            var screenW = carW * dw / mapImg.width
            var screenH = carH * dh / mapImg.height

            if(mx >= screenX - 10 && mx <= screenX + screenW + 10 &&
               my >= screenY - 10 && my <= screenY + screenH + 10) {
                return i
            }
        }
        return -1
    }

    // ==================== 检测：鼠标点是否在路口中心区域 ====================
    function checkCrossingClick(mx, my) {
        var dx, dy, dw, dh
        var scale = Math.min(cav.width / mapImg.width, cav.height / mapImg.height)
        dw = mapImg.width * scale
        dh = mapImg.height * scale
        dx = (cav.width - dw) / 2
        dy = (cav.height - dh) / 2

        // 路口0：x=30~96, y=316~420
        var c0x1 = dx + 30 * dw / mapImg.width
        var c0y1 = dy + 316 * dh / mapImg.height
        var c0x2 = dx + 96 * dw / mapImg.width
        var c0y2 = dy + 420 * dh / mapImg.height

        if(mx >= c0x1 && mx <= c0x2 && my >= c0y1 && my <= c0y2) {
            return 0
        }

        // 路口1：x=744~786, y=316~420
        var c1x1 = dx + 744 * dw / mapImg.width
        var c1y1 = dy + 316 * dh / mapImg.height
        var c1x2 = dx + 786 * dw / mapImg.width
        var c1y2 = dy + 420 * dh / mapImg.height

        if(mx >= c1x1 && mx <= c1x2 && my >= c1y1 && my <= c1y2) {
            return 1
        }

        return -1
    }

    Component.onCompleted: {
        dc.dataChanged.connect(dataChangedSlot)
        if (mapImg.status === Image.Ready) cav.requestPaint()
        else mapImg.statusChanged.connect(function() {
            if (mapImg.status === Image.Ready) cav.requestPaint()
        })
    }

    function dataChangedSlot() {
        cav.requestPaint()
    }
}