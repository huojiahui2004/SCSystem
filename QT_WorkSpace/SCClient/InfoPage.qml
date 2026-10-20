import QtQuick
import QtQuick.Controls

Rectangle {
    color: "white"
    radius: 8

    Canvas {
        id: canvas
        anchors.fill: parent
        onPaint: {
            var ctx = getContext('2d')
            ctx.clearRect(0, 0, width, height)

            // 获取道路数据
            var roads = dc.getRoadList()
            var mapW = 800, mapH = 600
            var scale = Math.min(width/mapW, height/mapH)
            var offX = (width - mapW*scale)/2
            var offY = (height - mapH*scale)/2

            // 绘制每条道路（矩形）
            for (var i=0; i<roads.length; i++) {
                var r = roads[i]
                var x = offX + r.x * scale
                var y = offY + r.y * scale
                var w = r.w * scale
                var h = r.h * scale
                ctx.fillStyle = r.color === 0 ? "#00ff00" : (r.color === 1 ? "#ffff00" : "#ff0000")
                ctx.fillRect(x, y, w, h)
                ctx.strokeStyle = "black"
                ctx.strokeRect(x, y, w, h)
            }

            // 绘制车辆（圆形）
            var cars = dc.getCarList()
            ctx.fillStyle = "blue"
            for (i=0; i<cars.length; i++) {
                // 这里简化处理，真实坐标需要根据道路计算，演示用固定偏移
                var cx = offX + 100 + i*50
                var cy = offY + 200
                ctx.beginPath()
                ctx.arc(cx, cy, 8*scale, 0, 2*Math.PI)
                ctx.fill()
            }
        }
    }

    Timer {
        interval: 500
        running: true
        repeat: true
        onTriggered: canvas.requestPaint()
    }
}