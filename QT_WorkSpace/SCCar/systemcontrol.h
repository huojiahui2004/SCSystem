#ifndef SYSTEMCONTROL_H
#define SYSTEMCONTROL_H
#include <QHash>
#include <QObject>
#include <QSoundEffect>

class SystemControl : public QObject
{
    Q_OBJECT
protected:
    explicit SystemControl(QObject *parent = nullptr);

public:
    static SystemControl *getObject();
    void systemInit();
    void systemStart();
    /**
     * @brief 获取车辆id
     * @return
     */
    Q_INVOKABLE int getCarId();
    /**
     * @brief 设置车辆锁定状态
     * @param sw
     */
    Q_INVOKABLE void setCarLock(bool sw);
    /**
     * @brief 设置车辆运行状态
     * @param sw
     */
    Q_INVOKABLE void setCarRun(bool sw);

signals:
    void lockStateChanged(bool sw);
    void runStateChanged(bool sw);

protected slots:
    void carLockSetSlot(int id, bool sw, QString key);
    void carLightSetSlot(int id, bool sw, QString key);
    void carAlarmLightSetSlot(int id, bool sw, QString key);
    void carRunSetSlot(int id, bool sw, QString key);
    void onTrafficLightChanged(int roadId, int color);
    void onCarPosChanged(int carId, int roadId, int roadPos, int speed);

protected:
    void timerEvent(QTimerEvent *e);
    static SystemControl *obj;
    int carId = -1;
    QString carKey;
    QStringList runList;
    int runRoadIndex = 0;
    double runRoadPos = 0;

    bool lockState = true;  //默认锁定
    bool runState = false;  //默认停止

    QHash<int, int> trafficLightState;
    bool isWaitingAtLight = false;

    QHash<int, int> otherCarPos;
    QHash<int, int> otherCarRoad;
    QHash<int, int> otherCarSpeed;    // ← 加

    int carSpeed = 5;                 // ← 加
    int normalSpeed = 5;              // ← 加
    double endurance = 1000;          // ← 加
    double mileage = 0;               // ← 加
    bool isOvertaking = false;        // ← 加
    int followTimer = 0;              // ← 加
    int overtakeTarget = -1;
    QSoundEffect hornSound;
    bool isHonking = false;   // 是否在按喇叭
};

#endif // SYSTEMCONTROL_H
