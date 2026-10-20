import QtQuick

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
    Canvas {
        id: cav
        anchors.fill: parent
        onPaint: {
            var ctx = getContext('2d')
            //清除现有界面的内容
            ctx.clearRect(0, 0, width, height)
            var dx, dy, dw, dh
            var scale = Math.min(
                        width / mapImg.width,
                        height / mapImg.height)
            dw = mapImg.width * scale
            dh = mapImg.height * scale
            dx = (width - dw) / 2
            dy = (height - dh) / 2
            ctx.drawImage(mapImg, dx, dy, dw, dh)

            // 始终加载道路数据，车辆绘制需要用到
            var rl = dc.getRoadList();

            // ==================== 绘制道路 ====================
            if(dc.showRoad) {
                var rx, ry, rw, rh, rc
                for(var i = 0; i < rl.length; i++)
                {
                    rx = rl[i]['x']
                    ry = rl[i]['y']
                    rw = rl[i]['w']
                    rh = rl[i]['h']
                    rc = rl[i]['color']
                    // 根据阻断状态选择颜色
                    if (rl[i]['blocked']) {
                        ctx.fillStyle = "gray"
                    } else {
                        ctx.fillStyle = rc === 0 ?
                                    "green" :
                                    rc === 1 ?
                                        "yellow" :
                                        "red"
                    }
                    //绘制矩形
                    ctx.fillRect(dx + rx * dw / mapImg.width,
                                 dy + ry * dh / mapImg.height,
                                 rw * dw / mapImg.width,
                                 rh * dh / mapImg.height)
                }
            }

            // ==================== 绘制红绿灯 ====================
            if(dc.showLight) {
                var ll = dc.getLightList()
                var roadX, roadY, roadW, roadH
                var roadDir, lightColor
                for(var i = 0; i < ll.length; i++){
                    roadX = ll[i]['roadX']
                    roadY = ll[i]['roadY']
                    roadW = ll[i]['roadW']
                    roadH = ll[i]['roadH']
                    roadDir = ll[i]['roadDir']
                    lightColor = ll[i]['color']
                    if(lightColor === -1)
                        continue
                    // 根据道路参数计算灯的位置
                    // 0-left 1-right 2-up 3-down
                    switch (roadDir) {
                    case 0: //left
                        rw = 4;
                        rh = 28;
                        rx = roadX - rw - 3;
                        ry = roadY - (rh - roadH)/2;
                        break;
                    case 1:
                        rw = 4;
                        rh = 28;
                        rx = roadX + roadW + 2;
                        ry = roadY - (rh - roadH)/2;
                        break;
                    case 2: //up
                        rh = 4;
                        rw = 28;
                        ry = roadY - rh - 3;
                        rx = roadX - (rw - roadW)/2;
                        break;
                    case 3:
                        rh = 4;
                        rw = 28;
                        ry = roadY + roadH + 2;
                        rx = roadX - (rw - roadW)/2;
                        break;
                    }
                    ctx.fillStyle = lightColor === 0 ?
                                "green" :
                                lightColor === 1 ?
                                    "yellow" :
                                    "red"
                    //绘制矩形
                    ctx.fillRect(dx + rx * dw / mapImg.width,
                                 dy + ry * dh / mapImg.height,
                                 rw * dw / mapImg.width,
                                 rh * dh / mapImg.height)
                }
            }

            // ==================== 绘制车辆 ====================
            if(dc.showCar) {
                var cl = dc.getCarList();
                var roadId, roadPos
                var carW = 30;
                var carH = 30;

                for(var i = 0; i < cl.length; i++){
                    roadId = cl[i]['road_id']
                    roadPos = cl[i]['road_pos']
                    if(roadId > rl.length)
                        continue
                    if(roadId < 0){
                        roadX = 635
                        roadY = 545
                        roadW = 4
                        roadH = 90
                        roadDir = 3
                        roadPos = 0 + i * 50
                    }
                    else{
                        roadX = rl[roadId]['x']
                        roadY = rl[roadId]['y']
                        roadW = rl[roadId]['w']
                        roadH = rl[roadId]['h']
                        roadDir = rl[roadId]['dir']
                    }
                    var len;
                    switch (roadDir) {
                    case 0:
                        len = roadW - carW;
                        rx = roadX + carW/2 + len * (100-roadPos) / 100;
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
                        ry = roadY + carH/2 + len * (100-roadPos) / 100;
                        break;
                    case 3:
                        len = roadH - carH;
                        rx = roadX + roadW/2;
                        ry = roadY + carH/2 + len * roadPos / 100;
                        break;
                    }
                    rx = rx - carW/2
                    ry = ry - carH/2
                    rw = carW
                    rh = carH
                    rx = dx + rx * dw / mapImg.width
                    ry = dy + ry * dh / mapImg.height
                    rw = rw * dw / mapImg.width
                    rh = rh * dh / mapImg.height
                    ctx.drawImage(carImg, rx, ry,
                                  rw, rh)
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
    Component.onCompleted: {
        dc.dataChanged.connect(dataChangedSlot)
    }

    function dataChangedSlot(){
        cav.requestPaint();
    }
}