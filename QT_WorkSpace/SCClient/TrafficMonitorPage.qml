import QtQuick
import QtQuick.Controls

Rectangle {
    color: "white"
    radius: 8

    Canvas {
        id: barCanvas
        anchors.fill: parent
        onPaint: {
            var ctx = getContext('2d')
            ctx.clearRect(0, 0, width, height)

            var roads = dc.getRoadList()
            var carCounts = []
            var maxCars = 0
            for (var i=0; i<roads.length; i++) {
                var cnt = dc.getCarCountOnRoad ? dc.getCarCountOnRoad(i) : (i%5) // 模拟
                carCounts.push(cnt)
                if (cnt > maxCars) maxCars = cnt
            }

            var margin = 50
            var barWidth = (width - 2*margin) / carCounts.length
            for (i=0; i<carCounts.length; i++) {
                var barHeight = (carCounts[i] / maxCars) * (height - 2*margin)
                var x = margin + i * barWidth
                var y = height - margin - barHeight
                ctx.fillStyle = "#3498db"
                ctx.fillRect(x, y, barWidth-2, barHeight)
                ctx.fillStyle = "black"
                ctx.fillText("道"+i, x+5, y-5)
            }
        }
    }

    Timer {
        interval: 1000
        running: true
        repeat: true
        onTriggered: barCanvas.requestPaint()
    }
}