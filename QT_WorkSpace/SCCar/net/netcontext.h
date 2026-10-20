#ifndef NETCONTEXT_H
#define NETCONTEXT_H

#include <QObject>
class QTcpSocket;
class NetContext : public QObject
{
    Q_OBJECT
protected:
    explicit NetContext(QObject *parent = nullptr);

public:
    static NetContext *getObject();
    /**
     * @brief 连接服务器
     * @param ip
     * @param port
     */
    void connectToServer(QString ip, quint16 port);
    /**
     * @brief 发送客户端身份
     * @param type
     */
    void sendType(int type);
    /**
     * @brief 发送车辆位置
     * @param id
     * @param roadId
     * @param roadPos
     */
void sendCarRunPos(int id, int roadId, int roadPos, int speed, double endurance, double mileage);

signals:
    void carLockSet(int id, bool sw, QString key);
    void carLightSet(int id, bool sw, QString key);
    void carAlarmLightSet(int id, bool sw, QString key);
    void carRunSet(int id, bool sw, QString key);

    void trafficLightChanged(int roadId, int color);//红灯停车
    void carPosChanged(int carId, int roadId, int roadPos, int speed);//排队

protected slots:
    void connectedSlot();
    void disconnectedSlot();
    void readyReadSlot();

protected:
    void timerEvent(QTimerEvent *e);
    void handleData(QByteArray &data);
    void handleFrame(const QByteArray &frame);
    static NetContext *obj;
    QString ip;
    quint16 port;
    QTcpSocket *socket;
    int reConId = -1;
};

#endif // NETCONTEXT_H









