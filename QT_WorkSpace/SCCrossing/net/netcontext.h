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
     * @brief 模式改变
     * @param id
     * @param mode
     */
    void sendModeChanged(int id, int mode);
    /**
     * @brief 信号灯颜色改变
     * @param id
     * @param roadId
     * @param color
     */
    void sendLightColorChanged(int id, int roadId, int color);

signals:
    void crossingModeSet(int id, int mode);

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









