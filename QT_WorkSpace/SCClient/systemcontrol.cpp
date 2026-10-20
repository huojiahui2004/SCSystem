#include "systemcontrol.h"
#include "netcontext.h"
#include "datacontext.h"
SystemControl *SystemControl::obj = nullptr;
SystemControl::SystemControl(QObject *parent)
    : QObject{parent}
{
    connect(NetContext::getObject(),
            &NetContext::lightColorChanged,
            this,
            &SystemControl::lightColorChangedSlot);
    connect(NetContext::getObject(),
            &NetContext::carRunPosChanged,
            this,
            &SystemControl::carRunPosChangedSlot);
}

SystemControl *SystemControl::getObject()
{
    if(obj == nullptr)
        obj = new SystemControl;
    return obj;
}

void SystemControl::lightColorChangedSlot(int id, int roadId, int color)
{
    if(color == 1)
        color = 2;
    else if(color == 2)
        color = 1;
    DataContext::getObject()->setLightColor(id, color);
}

void SystemControl::carRunPosChangedSlot(int id, int roadId, int roadPos, int speed, double endurance, double mileage)
{
    DataContext::getObject()->setCarRunPos(id, roadId, roadPos);
    DataContext::getObject()->setCarSpeed(id, speed);
    DataContext::getObject()->setCarEndurance(id, endurance);
    DataContext::getObject()->setCarMileage(id, mileage);
}







