#include "mytcpsocket.h"
#include <QJsonObject>
#include <QJsonDocument>

MyTcpSocket::MyTcpSocket(QObject *parent)
    : QTcpSocket{parent}
{
    connect(this, &MyTcpSocket::readyRead,
            this, &MyTcpSocket::readyReadSlot);
}

void MyTcpSocket::serverForwardToClientSlot(int clientType, const QByteArray &data)
{
    if(ct != clientType || ct < 0)
        return;
    write(data);
}

void MyTcpSocket::readyReadSlot()
{
    QByteArray data = this->readAll();
    handleData(data);
}

void MyTcpSocket::handleData(QByteArray &data)
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

void MyTcpSocket::handleFrame(const QByteArray &frame)
{
    QJsonObject rf = QJsonDocument::fromJson(frame).object();
    int type = rf.value("type").toInt();
    if(type == 1000){
        ct = rf.value("client_type").toInt();
        if(ct != -1)
            emit newClientConnected(ct);
    }
    else if(type >= 2001 && type <= 2008){
        emit forwardToClient(0, frame);
        emit forwardToClient(2, frame);
        emit forwardToClient(3, frame);
    }
    else if(type == 3001){
        emit forwardToClient(0, frame);
        emit forwardToClient(2, frame);
        emit forwardToClient(3, frame);
    }
    else if(type == 4001){
        emit forwardToClient(2, frame);
        emit forwardToClient(3, frame);
    }
    else if(type >= 5001 && type <= 5004){
        emit forwardToClient(0, frame);
    }
    else if(type == 6001){
        emit forwardToClient(1, frame);
    }
}












