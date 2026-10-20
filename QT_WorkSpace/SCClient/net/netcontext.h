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
    Q_PROPERTY(QString serverIp READ getServerIp CONSTANT)
    Q_PROPERTY(int serverPort READ getServerPort CONSTANT)

    QString getServerIp() const { return ip; }
    int getServerPort() const { return port; }
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
     * @brief 设置路口模式
     * @param id
     * @param mode
     */
    Q_INVOKABLE void sendCrossingMode(int id, int mode);
    /**
     * @brief 设置车辆大灯、双闪开关
     * @param id
     * @param sw
     */
    Q_INVOKABLE void sendCarLightSw(int id, bool sw, QString key);
    Q_INVOKABLE void sendCarAlarmLightSw(int id, bool sw, QString key);
    Q_INVOKABLE void sendCarLockSw(int id, bool sw, QString key);
    Q_INVOKABLE void sendCarRunSw(int id, bool sw, QString key);

signals:
    void crossingModeChanged(int id, int mode);
    void lightColorChanged(int id, int roadId, int color);
    void carRunPosChanged(int id, int roadId, int roadPos, int speed, double endurance, double mileage);

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









