#ifndef CONFIGCONTEXT_H
#define CONFIGCONTEXT_H

#include <QObject>

class ConfigContext : public QObject
{
    Q_OBJECT
protected:
    explicit ConfigContext(QObject *parent = nullptr);

public:

    static void getServerInfo(QString &ip, quint16 &port);
    static void getCrossingId(int &id);
    /**
     * @brief 获取上下左右四个信号灯的信息
     * @param id        信号灯id
     * @param show      是否显示
     * @param roadId    道路id
     */
    static void getLeftLightInfo(int &id, bool &show, int &roadId);
    static void getRightLightInfo(int &id, bool &show, int &roadId);
    static void getUpLightInfo(int &id, bool &show, int &roadId);
    static void getDownLightInfo(int &id, bool &show, int &roadId);
    /**
     * @brief 获取绿灯时长
     * @param time
     */
    static void getGreenTime(int &time);

};

#endif // CONFIGCONTEXT_H






