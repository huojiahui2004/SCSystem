#include "datacontext.h"
#include <QJsonObject>
#include <QRandomGenerator>
#include <QTimer>

DataContext::DataContext(QObject *parent)
    : QObject{parent}
{
    rl.append(new RoadInfo(96, 27, 648, 3, 0));
    rl.append(new RoadInfo(96, 59, 648, 3, 1));
    rl.append(new RoadInfo(96, 347, 648, 3, 0));
    rl.append(new RoadInfo(96, 379, 648, 3, 1));
    rl.append(new RoadInfo(96, 671, 648, 3, 0));
    rl.append(new RoadInfo(96, 703, 648, 3, 1));
    rl.append(new RoadInfo(27, 96, 3, 220, 3));
    rl.append(new RoadInfo(59, 96, 3, 220, 2));
    rl.append(new RoadInfo(783, 96, 3, 220, 3));
    rl.append(new RoadInfo(815, 96, 3, 220, 2));
    rl.append(new RoadInfo(27, 420, 3, 220, 3));
    rl.append(new RoadInfo(59, 420, 3, 220, 2));
    rl.append(new RoadInfo(783, 420, 3, 220, 3));
    rl.append(new RoadInfo(815, 420, 3, 220, 2));

    ll.append(new LightInfo(6));
    ll.append(new LightInfo(2));
    ll.append(new LightInfo(11));
    ll.append(new LightInfo(8));
    ll.append(new LightInfo(13));
    ll.append(new LightInfo(3));

    cl.append(new CarInfo());
    cl.append(new CarInfo());
    cl.append(new CarInfo());
    initCarStatuses();
    for (LightInfo* light : ll) {
        if (light->color == 0) {  // 默认绿灯
            light->time = 20;
        } else if (light->color == 1) {
            light->time = 3;
        } else if (light->color == 2) {
            light->time = 25;
        } else {
            light->time = 0;      // 无效灯
        }
    }
    m_lightTimer = new QTimer(this);
    connect(m_lightTimer, &QTimer::timeout, this, &DataContext::updateLightTimers);
    m_lightTimer->start(1000);
}


RoadInfo::RoadInfo(int x, int y, int w, int h, int dir)
    : x{x}, y{y}, w{w}, h{h}, dir{dir}, blocked(false)
{
}


int DataContext::getCarCountOnRoad(int roadId) const
{
    if (roadId < 0 || roadId >= rl.size()) return 0;
    int count = 0;
    for (int i = 0; i < cl.size(); ++i) {
        if (cl.at(i)->roadId == roadId) count++;
    }
    return count;
}

QVariantList DataContext::getRoadStatusList() const
{
    QVariantList list;
    for (int i = 0; i < rl.size(); ++i) {
        QVariantMap map;
        map["id"] = i;
        map["x"] = rl[i]->x;
        map["y"] = rl[i]->y;
        map["w"] = rl[i]->w;
        map["h"] = rl[i]->h;
        map["dir"] = rl[i]->dir;
        map["color"] = rl[i]->color;
        map["carCount"] = getCarCountOnRoad(i);
        map["blocked"] = rl[i]->isBlocked();
        list.append(map);
    }
    return list;
}

QVariantList DataContext::getLightStatusList() const
{
    QVariantList list;
    for (int i = 0; i < ll.size(); ++i) {
        QVariantMap map;
        map["id"] = i;
        map["roadId"] = ll[i]->roadId;
        map["color"] = ll[i]->color;
        QString colorText;
        if (ll[i]->color == 0) colorText = "绿灯";
        else if (ll[i]->color == 1) colorText = "黄灯";
        else if (ll[i]->color == 2) colorText = "红灯";
        else colorText = "未知";
        map["colorText"] = colorText;
        list.append(map);
    }
    return list;
}


QJsonArray DataContext::getRoadList()
{
    QJsonArray arr;
    for(int i = 0; i < rl.size(); i++){
        QJsonObject obj;
        obj.insert("x", rl.at(i)->x);
        obj.insert("y", rl.at(i)->y);
        obj.insert("w", rl.at(i)->w);
        obj.insert("h", rl.at(i)->h);
        obj.insert("color", rl.at(i)->color);
        obj.insert("dir", rl.at(i)->dir);
        obj.insert("blocked", rl.at(i)->isBlocked());   // 新增
        arr.append(obj);
    }
    return arr;
}
DataContext *DataContext::obj = nullptr;
DataContext *DataContext::getObject()
{
    if(obj == nullptr)
        obj = new DataContext;
    return obj;
}

LightInfo::LightInfo(int roadId)
    : roadId{roadId}, color(0), time(20)
{

}

QJsonArray DataContext::getLightList()
{
    QJsonArray arr;
    for(int i = 0; i < ll.size(); i++){
        QJsonObject obj;
        int id = ll.at(i)->roadId;
        obj.insert("roadId", id);
        obj.insert("color", ll.at(i)->color);
        obj.insert("time", ll.at(i)->time);
        obj.insert("roadX", rl.at(id)->x);
        obj.insert("roadY", rl.at(id)->y);
        obj.insert("roadW", rl.at(id)->w);
        obj.insert("roadH", rl.at(id)->h);
        obj.insert("roadDir", rl.at(id)->dir);
        arr.append(obj);
    }
    return arr;
}

QJsonArray DataContext::getCarList()
{
    QJsonArray arr;
    for(int i = 0; i < cl.size(); i++){
        QJsonObject obj;
        obj.insert("road_id", cl.at(i)->roadId);
        obj.insert("road_pos", cl.at(i)->roadPos);
        arr.append(obj);
    }
    return arr;
}

void DataContext::initCarStatuses()
{
    carStatuses.resize(3);
    for (int i = 0; i < 3; ++i) {
        carStatuses[i].light = false;
        carStatuses[i].alarm = false;
        carStatuses[i].running = false;
        carStatuses[i].locked = true;
        carStatuses[i].range = 200 + QRandomGenerator::global()->bounded(101);
        carStatuses[i].mileage = 5000 + QRandomGenerator::global()->bounded(10001);
        if (i == 0) carStatuses[i].warning = "正常";
        else if (i == 1) carStatuses[i].warning = "胎压低";
        else carStatuses[i].warning = "保养提醒";
    }
}

QVariantMap DataContext::getCarStatus(int id) const
{
    QVariantMap map;
    if (id < 0 || id >= carStatuses.size()) return map;
    const auto& s = carStatuses[id];
    map["light"] = s.light;
    map["alarm"] = s.alarm;
    map["running"] = s.running;
    map["locked"] = s.locked;
    map["range"] = s.range;
    map["mileage"] = s.mileage;
    map["speed"] = s.speed;
    map["warning"] = s.warning;
    return map;
}

void DataContext::setCarLight(int id, bool on)
{
    if (id < 0 || id >= carStatuses.size()) return;
    carStatuses[id].light = on;
    emit dataChanged();
}

void DataContext::setCarAlarm(int id, bool on)
{
    if (id < 0 || id >= carStatuses.size()) return;
    carStatuses[id].alarm = on;
    emit dataChanged();
}

void DataContext::setCarRunning(int id, bool on)
{
    if (id < 0 || id >= carStatuses.size()) return;
    carStatuses[id].running = on;
    emit dataChanged();
}

void DataContext::setCarLock(int id, bool locked)
{
    if (id < 0 || id >= carStatuses.size()) return;
    carStatuses[id].locked = locked;
    emit dataChanged();
}


void DataContext::setLightColor(int id, int color)
{
    if (id < 0 || id >= ll.size()) return;
    LightInfo* light = ll.at(id);
    if (light->color == color) return;  // 颜色未变则无需重置
    light->color = color;
    light->time = getDefaultTimeForColor(color);
    emit dataChanged();
}

void DataContext::setCarRunPos(int id, int roadId, int roadPos)
{
    if(id >= cl.size())
        return;
    int oldRoadId = cl.at(id)->roadId;
    cl.at(id)->roadId = roadId;
    cl.at(id)->roadPos = roadPos;

    if(oldRoadId != roadId){
        autoSetRoadColor(oldRoadId);
        autoSetRoadColor(roadId);
    }

    emit dataChanged();
}

void DataContext::updateLightTimers()
{
    bool changed = false;
    for (LightInfo* light : ll) {
        if (light->color == -1) continue; // 无效灯跳过

        if (light->time > 0) {
            light->time--;
            changed = true;
        } else {
            // 时间到，切换状态
            nextLightState(light);
            changed = true;
        }
    }
    if (changed) {
        emit dataChanged();   // 通知 QML 刷新界面
    }
}

int DataContext::getDefaultTimeForColor(int color) const
{
    switch (color) {
    case 0: return 5;   // 绿灯持续20秒
    case 1: return 3;    // 黄灯持续3秒
    case 2: return 8;   // 红灯持续25秒
    default: return 0;
    }
}

void DataContext::nextLightState(LightInfo *light)
{
    if (light->color == 0) {        // 绿 -> 黄
        light->color = 1;
        light->time = getDefaultTimeForColor(1);
    } else if (light->color == 1) { // 黄 -> 红
        light->color = 2;
        light->time = getDefaultTimeForColor(2);
    } else {                        // 红 -> 绿
        light->color = 0;
        light->time = getDefaultTimeForColor(0);
    }
}

void DataContext::autoSetRoadColor(int roadId)
{
    if(roadId < 0 || roadId>=rl.length())
        return;
    int count = 0;
    for(int i = 0; i<cl.length();i++)
        if(cl.at(i)->roadId == roadId)
            count++;
    rl.at(roadId)->color = count == 0 ? 0 : count == 1 ? 1 : 2;
}

CarInfo::CarInfo()
{

}

void DataContext::setCarSpeed(int id, int speed)
{
    if (id < 0 || id >= carStatuses.size()) return;
    carStatuses[id].speed = speed;
    emit dataChanged();
}

void DataContext::setCarEndurance(int id, double endurance)
{
    if (id < 0 || id >= carStatuses.size()) return;
    carStatuses[id].endurance = endurance;
    carStatuses[id].range = (int)endurance;
    if(endurance <= 0)
        carStatuses[id].warning = "续航耗尽";
    emit dataChanged();
}

void DataContext::setCarMileage(int id, double mileage)
{
    if (id < 0 || id >= carStatuses.size()) return;
    carStatuses[id].mileage = (int)mileage;
    emit dataChanged();
}

void DataContext::setShowRoad(bool show)
{
    m_showRoad = show;
    emit dataChanged();
}

void DataContext::setShowLight(bool show)
{
    m_showLight = show;
    emit dataChanged();
}

void DataContext::setShowCar(bool show)
{
    m_showCar = show;
    emit dataChanged();
}

// 新增阻断方法实现
void DataContext::setRoadBlocked(int roadId, bool blocked)
{
    if (roadId < 0 || roadId >= rl.size()) return;
    rl[roadId]->setBlocked(blocked);
    emit dataChanged();
}

bool DataContext::isRoadBlocked(int roadId) const
{
    if (roadId < 0 || roadId >= rl.size()) return false;
    return rl[roadId]->isBlocked();
}