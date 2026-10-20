import QtQuick
import QtQuick.Controls

Item {
    Button{
        text:"大灯开"
        onClicked: {
            nc.sendCarLightSw(0, true)

        }
    }
    Button{
        y:50
        text:"双闪开"
        onClicked: {
             nc.sendCarAlarmSw(0, true)
        }
    }
    Button{
x:50
        y:50
        text:"双闪关"
        onClicked: {
             nc.sendCarAlarmSw(0, false)
        }
    }
    Button{
        text:"大灯关"
        x:50
        onClicked: {
            nc.sendCarLightSw(0, false)

        }
    }
}
