#include "systemcontrol.h"
#include <QProcess>
#include "netcontext.h"
#include "datacontext.h"
#include "configcontext.h"
SystemControl *SystemControl::obj = nullptr;
SystemControl::SystemControl(QObject *parent)
    : QObject{parent}
{
    startTimer(200);
    connect(NetContext::getObject(), &NetContext::carLockSet,
            this, &SystemControl::carLockSetSlot);
    connect(NetContext::getObject(), &NetContext::carLightSet,
            this, &SystemControl::carLightSetSlot);
    connect(NetContext::getObject(), &NetContext::carAlarmLightSet,
            this, &SystemControl::carAlarmLightSetSlot);
    connect(NetContext::getObject(), &NetContext::carRunSet,
            this, &SystemControl::carRunSetSlot);
    connect(NetContext::getObject(), &NetContext::trafficLightChanged,
            this, &SystemControl::onTrafficLightChanged);
    connect(NetContext::getObject(), &NetContext::carPosChanged,
            this, &SystemControl::onCarPosChanged);

    QList<int> knownRoads = {2, 3, 6, 8, 11, 13};
    for(int roadId : knownRoads) {
        trafficLightState[roadId] = -1;
    }
    hornSound.setSource(QUrl::fromLocalFile("images/888.wav"));
    hornSound.setVolume(1.0);
}


SystemControl *SystemControl::getObject()
{
    if(obj == nullptr)
        obj = new SystemControl;
    return obj;
}

void SystemControl::systemInit()
{
    NetContext::getObject();
    DataContext::getObject();
    ConfigContext::getCarInfo(carId, carKey);
    ConfigContext::getCarRunList(runList);
    ConfigContext::getCarSpeed(carSpeed);
    ConfigContext::getCarEndurance(endurance);
    ConfigContext::getCarMileage(mileage);
    normalSpeed = carSpeed;
}

void SystemControl::systemStart()
{
    QString ip;
    quint16 port;
    ConfigContext::getServerInfo(ip, port);
    NetContext::getObject()->connectToServer(ip, port);
}

int SystemControl::getCarId()
{
    return carId;
}

void SystemControl::setCarLock(bool sw)
{
    lockState = sw;
    if(sw == true)
        runState = false;
    emit lockStateChanged(sw);
}

void SystemControl::setCarRun(bool sw)
{
    if(lockState == true)
        return;
    runState = sw;
    emit runStateChanged(sw);
}

void SystemControl::carLockSetSlot(int id, bool sw, QString key)
{
    if(id != carId || key != carKey)
        return;
    setCarLock(sw);
}

void SystemControl::carLightSetSlot(int id, bool sw, QString key)
{
    if(id != carId || key != carKey)
        return;
    DataContext::getObject()->setCarLightSw(sw);
}

void SystemControl::carAlarmLightSetSlot(int id, bool sw, QString key)
{
    if(id != carId || key != carKey)
        return;
    DataContext::getObject()->setCarAlarmSw(sw);
}

void SystemControl::carRunSetSlot(int id, bool sw, QString key)
{
    if(id != carId || key != carKey)
        return;
    setCarRun(sw);
}

void SystemControl::timerEvent(QTimerEvent *e)
{
    if(runState != true)
        return;
    if(runList.isEmpty())
        return;

    if(endurance <= 0)
        return;

    int currentRoadId = runList.at(runRoadIndex).toInt();

    // ========== 超车状态 ==========
    if(isOvertaking) {
            runRoadPos += carSpeed * 0.5;
        if(otherCarPos.contains(overtakeTarget)) {
            int targetPos = otherCarPos[overtakeTarget];
            int targetRoad = otherCarRoad.value(overtakeTarget, -1);
            if(targetRoad != currentRoadId || runRoadPos > targetPos + 10) {
                isOvertaking = false;
                carSpeed = normalSpeed;
                overtakeTarget = -1;
            }
        } else {
            isOvertaking = false;
            carSpeed = normalSpeed;
            overtakeTarget = -1;
        }

        endurance -= 0.1;
        mileage += (carSpeed + 5) * 0.01;

        if(runRoadPos > 100) {
            NetContext::getObject()->sendCarRunPos(carId, currentRoadId, 100, carSpeed, endurance, mileage);
            runRoadIndex++;
            if(runRoadIndex >= runList.length()) runRoadIndex = 0;
            runRoadPos = 0;
            isOvertaking = false;
            carSpeed = normalSpeed;
            overtakeTarget = -1;
        } else {
            NetContext::getObject()->sendCarRunPos(carId, currentRoadId, (int)runRoadPos, carSpeed, endurance, mileage);
        }
        return;
    }

    // ========== 红绿灯检测 ==========
    if(trafficLightState.contains(currentRoadId)) {
        int lightColor = trafficLightState[currentRoadId];
        if(lightColor == 1) {
            bool hasCarAhead = false;
            for(auto it = otherCarPos.begin(); it != otherCarPos.end(); ++it) {
                int pos = it.value();
                int road = otherCarRoad.value(it.key(), -1);
                if(road == currentRoadId && pos > runRoadPos && pos - runRoadPos < 20) {
                    hasCarAhead = true;
                    break;
                }
            }
            if(hasCarAhead || runRoadPos >= 95) {
                NetContext::getObject()->sendCarRunPos(carId, currentRoadId, (int)runRoadPos, carSpeed, endurance, mileage);
                return;
            }
        }
    }

    // ========== 正常行驶：检测前车 ==========
    int frontCarId = -1;
    int frontCarPos = 999;
    for(auto it = otherCarPos.begin(); it != otherCarPos.end(); ++it) {
        int pos = it.value();
        int road = otherCarRoad.value(it.key(), -1);
        if(road == currentRoadId && pos > runRoadPos && pos < frontCarPos) {
            frontCarPos = pos;
            frontCarId = it.key();
        }
    }

    if(frontCarId != -1 && frontCarPos - runRoadPos < 20) {
        carSpeed = otherCarSpeed.value(frontCarId, 1);
        followTimer++;
        // ========== 跟车时喇叭持续响 ==========
        if(followTimer >= 1 && !isHonking) {
            hornSound.play();
            isHonking = true;
        }
        if(followTimer >= 15) {
            hornSound.stop();   // 超车开始，停喇叭
            isHonking = false;
            isOvertaking = true;
            overtakeTarget = frontCarId;
            followTimer = 0;
            carSpeed = normalSpeed + 5;
        }
    } else {
        // 不跟车了，停喇叭
        if(isHonking) {
            hornSound.stop();
            isHonking = false;
        }
        followTimer = 0;
        carSpeed = normalSpeed;
    }

    runRoadPos += carSpeed * 0.1;
    endurance -= 0.1;
    mileage += carSpeed * 0.01;

    if(runRoadPos > 100) {
        NetContext::getObject()->sendCarRunPos(carId, currentRoadId, 100, carSpeed, endurance, mileage);
        runRoadIndex++;
        if(runRoadIndex >= runList.length()) runRoadIndex = 0;
        runRoadPos = 0;
    } else {
        NetContext::getObject()->sendCarRunPos(carId, currentRoadId, (int)runRoadPos, carSpeed, endurance, mileage);
    }
}

void SystemControl::onTrafficLightChanged(int roadId, int color)
{
    trafficLightState[roadId] = color;
}

void SystemControl::onCarPosChanged(int id, int roadId, int roadPos, int speed)
{
    if(id == carId)
        return;
    otherCarPos[id] = roadPos;
    otherCarRoad[id] = roadId;
    otherCarSpeed[id] = speed;
}
