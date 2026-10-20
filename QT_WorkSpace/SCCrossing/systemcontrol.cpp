#include "systemcontrol.h"
#include "datacontext.h"
#include "netcontext.h"
#include "configcontext.h"
#include <QJsonArray>
#include <QJsonObject>

SystemControl *SystemControl::obj = nullptr;
SystemControl::SystemControl(QObject *parent)
    : QObject{parent}
{
    ConfigContext::getGreenTime(greenTime);
    autoModeHorBuf = new int[(greenTime+3)*2*2];
    autoModeVerBuf = new int[(greenTime+3)*2*2];
    int index = 0;
    //绿灯常量
    for(int i = 0; i < (greenTime - 3)*2; i++){
        autoModeHorBuf[index++] = 0;
    }
    //绿灯闪烁
    int flag = 0;
    for(int i = 0; i < 6; i++){
        if(flag == 0){
            autoModeHorBuf[index++] = -1;
            flag = 1;
        }
        else{
            autoModeHorBuf[index++] = 0;
            flag = 0;
        }
    }
    //黄灯闪烁
    flag = 0;
    for(int i = 0; i < 6; i++){
        if(flag == 0){
            autoModeHorBuf[index++] = -1;
            flag = 1;
        }
        else{
            autoModeHorBuf[index++] = 1;
            flag = 0;
        }
    }
    //红灯常亮
    for(int i = 0; i < (greenTime + 3)*2; i++){
        autoModeHorBuf[index++] = 2;
    }
    //autoModeVerBuf
    index = 0;
    //红灯常亮
    for(int i = 0; i < (greenTime + 3)*2; i++){
        autoModeVerBuf[index++] = 2;
    }
    //绿灯常量
    for(int i = 0; i < (greenTime - 3)*2; i++){
        autoModeVerBuf[index++] = 0;
    }
    //绿灯闪烁
    flag = 0;
    for(int i = 0; i < 6; i++){
        if(flag == 0){
            autoModeVerBuf[index++] = -1;
            flag = 1;
        }
        else{
            autoModeVerBuf[index++] = 0;
            flag = 0;
        }
    }
    //黄灯闪烁
    flag = 0;
    for(int i = 0; i < 6; i++){
        if(flag == 0){
            autoModeVerBuf[index++] = -1;
            flag = 1;
        }
        else{
            autoModeVerBuf[index++] = 1;
            flag = 0;
        }
    }

    startTimer(500);
    connect(NetContext::getObject(),
            &NetContext::crossingModeSet,
            this,
            &SystemControl::crossingModeSetSlot);
    connect(DataContext::getObject(),
            &DataContext::lightColorChanged,
            this,
            &SystemControl::lightColorChangedSlot);
}

SystemControl *SystemControl::getObject()
{
    if(obj == nullptr)
        obj = new SystemControl;
    return obj;
}

void SystemControl::system_init()
{
    DataContext::getObject();
    NetContext::getObject();
    ConfigContext::getCrossingId(croId);
    int id,roadId;
    bool show;
    ConfigContext::getLeftLightInfo(id, show, roadId);
    DataContext::getObject()->setLeftLightInfo(id, show, roadId);

    ConfigContext::getRightLightInfo(id, show, roadId);
    DataContext::getObject()->setRightLightInfo(id, show, roadId);

    ConfigContext::getUpLightInfo(id, show, roadId);
    DataContext::getObject()->setUpLightInfo(id, show, roadId);

    ConfigContext::getDownLightInfo(id, show, roadId);
    DataContext::getObject()->setDownLightInfo(id, show, roadId);
}

void SystemControl::system_start()
{
    QString ip;
    quint16 port;
    ConfigContext::getServerInfo(ip, port);
    NetContext::getObject()->connectToServer(ip, port);
}

void SystemControl::setMode(LightMode mode)
{
    this->mode = mode;
    if(mode == HorMode){
        DataContext::getObject()->setLightColor(0, 2);
    }
    else if(mode == VerMode){
        DataContext::getObject()->setLightColor(2, 0);
    }
    else if(mode == NoMode){
        DataContext::getObject()->setLightColor(2, 2);
    }
    NetContext::getObject()->sendModeChanged(croId, (int)mode);
    emit modeChanged(mode);
}

int SystemControl::getCrossingId()
{
    return croId;
}

void SystemControl::crossingModeSetSlot(int id, int mode)
{
    if(id != croId)
        return;
    setMode((LightMode)mode);
}

void SystemControl::lightColorChangedSlot()
{
    QJsonArray arr = DataContext::getObject()->getLightList();
    for(int i = 0; i < arr.size(); i++){
        QJsonObject od = arr.at(i).toObject();
        if(od.value("show").toBool() == false)
            continue;
        int id = od.value("id").toInt();
        int roadId = od.value("roadId").toInt();
        int color = od.value("color").toInt();
        if(color == 1)
            color = 2;
        else if(color == 2)
            color = 1;
        NetContext::getObject()->sendLightColorChanged(
            id, roadId, color);
    }
}

void SystemControl::timerEvent(QTimerEvent *e)
{
    if(mode == NightMode){
        static int nmFlag = 1;
        if(nmFlag == 1){
            nmFlag = 0;
            DataContext::getObject()->setLightColor(1, 1);
        }
        else{
            nmFlag = 1;
            DataContext::getObject()->setLightColor(-1, -1);
        }
    }
    else if(mode == AutoMode){
        static int amIndex = 0;
        DataContext::getObject()->setLightColor(
            autoModeHorBuf[amIndex], autoModeVerBuf[amIndex]);
        amIndex++;
        if(amIndex >= (greenTime + 3)*2*2)
            amIndex = 0;
    }
}











