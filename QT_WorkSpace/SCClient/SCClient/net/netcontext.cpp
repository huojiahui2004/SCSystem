#include "netcontext.h"
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QTimerEvent>
NetContext *NetContext::obj = nullptr;
NetContext::NetContext(QObject *parent)
    : QObject{parent}
    , socket{new QTcpSocket(this)}
{
    connect(socket, &QTcpSocket::connected,
            this, &NetContext::connectedSlot);
    connect(socket, &QTcpSocket::disconnected,
            this, &NetContext::disconnectedSlot);
    connect(socket, &QTcpSocket::readyRead,
            this, &NetContext::readyReadSlot);
}

NetContext *NetContext::getObject()
{
    if(obj == nullptr)
        obj = new NetContext;
    return obj;
}

void NetContext::connectToServer(QString ip, quint16 port)
{
    if(socket->state() == QTcpSocket::ConnectedState)
        socket->disconnectFromHost();
    socket->connectToHost(QHostAddress(ip), port);
    this->ip = ip;
    this->port = port;
    reConId = startTimer(10000);
}

void NetContext::sendType(int type)
{
    QJsonObject data;
    data.insert("type", 1000);
    data.insert("client_type", type);
    socket->write(QJsonDocument(data).toJson());
}

void NetContext::sendCrossingMode(int id, int mode)
{
    QJsonObject data;
    data.insert("type", 6001);
    data.insert("id", id);
    data.insert("mode", mode);
    socket->write(QJsonDocument(data).toJson());
}

void NetContext::sendCarLightSw(int id, bool sw, QString key)
{
    QJsonObject data;
    data.insert("type", 5002);
    data.insert("id", id);
    data.insert("sw", sw);
    data.insert("key", key);
    socket->write(QJsonDocument(data).toJson());
}

void NetContext::sendCarAlarmLightSw(int id, bool sw, QString key)
{
    QJsonObject data;
    data.insert("type", 5003);
    data.insert("id", id);
    data.insert("sw", sw);
    data.insert("key", key);
    socket->write(QJsonDocument(data).toJson());
}

void NetContext::sendCarLockSw(int id, bool sw, QString key)
{
    QJsonObject data;
    data.insert("type", 5004);
    data.insert("id", id);
    data.insert("sw", sw);
    data.insert("key", key);
    socket->write(QJsonDocument(data).toJson());
}

void NetContext::sendCarRunSw(int id, bool sw, QString key)
{
    QJsonObject data;
    data.insert("type", 5001);
    data.insert("id", id);
    data.insert("sw", sw);
    data.insert("key", key);
    socket->write(QJsonDocument(data).toJson());
}

void NetContext::connectedSlot()
{
    sendType(2);
    if(reConId != -1)
        killTimer(reConId);
}

void NetContext::disconnectedSlot()
{
    reConId = startTimer(5000);
}

void NetContext::readyReadSlot()
{
    QByteArray data = socket->readAll();
    handleData(data);
}

void NetContext::timerEvent(QTimerEvent *e)
{
    if(e->timerId() == reConId){
        socket->connectToHost(QHostAddress(ip), port);
    }
}

void NetContext::handleData(QByteArray &data)
{
    int count = 0;
    for(int i = 0; i < data.length(); i++){
        if(data.at(i) == '{')
            count++;
        else if(data.at(i) == '}'){
            count--;
            if(count == 0){
                QByteArray frame = data.mid(0, i+1);
                handleFrame(frame);
                if(frame.length() == data.length())
                    return;
                data = data.mid(i+1);
                i = -1;
            }
        }
    }
}

void NetContext::handleFrame(const QByteArray &frame)
{
    QJsonObject rf = QJsonDocument::fromJson(frame).object();
    int type = rf.value("type").toInt();
    if(type == 4001){
        int id = rf.value("crossing_id").toInt();
        int mode = rf.value("mode").toInt();
        emit crossingModeChanged(id, mode);
    }
    else if(type == 3001){
        int id = rf.value("light_id").toInt();
        int roadId = rf.value("road_id").toInt();
        int color = rf.value("color").toInt();
        emit lightColorChanged(id, roadId, color);
    }
    else if(type == 2008){
        int id = rf.value("id").toInt();
        int roadId = rf.value("road_id").toInt();
        int roadPos = rf.value("road_pos").toInt();
        int speed = rf.value("speed").toInt();
        double endurance = rf.value("endurance").toDouble();
        double mileage = rf.value("mileage").toDouble();
        emit carRunPosChanged(id, roadId, roadPos, speed, endurance, mileage);
    }
}









