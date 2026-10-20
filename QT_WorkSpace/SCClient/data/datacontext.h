#ifndef DATACONTEXT_H
#define DATACONTEXT_H

#include <QObject>
#include <QJsonArray>
#include <QTimer>

/**
 * @brief 道路信息类
 */
class RoadInfo {
    friend class DataContext;
public:
    RoadInfo(int x, int y, int w, int h, int dir);
    bool isBlocked() const { return blocked; }
    void setBlocked(bool b) { blocked = b; }
protected:
    int x, y, w, h;
    int color = 0;  //0-g 1-y 2-r
    int dir = -1;   //0-left 1-right 2-up 3-down
    bool blocked = false;
        // 新增：道路是否被阻断
};

/**
 * @brief 红绿灯信息类
 */
class LightInfo {
    friend class DataContext;
public:
    LightInfo(int roadId);
protected:
    int roadId;     //道路id
    int color = 0;  //0-g 1-y 2-r -1-w
    int time = 0;
};

class CarInfo {
    friend class DataContext;
public:
    CarInfo();
protected:
    int roadId = -1;
    int roadPos = -1;
};


class DataContext : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool showRoad READ showRoad WRITE setShowRoad NOTIFY dataChanged)
    Q_PROPERTY(bool showLight READ showLight WRITE setShowLight NOTIFY dataChanged)
    Q_PROPERTY(bool showCar READ showCar WRITE setShowCar NOTIFY dataChanged)
protected:
    explicit DataContext(QObject *parent = nullptr);

public:
    Q_INVOKABLE int getTotalRoadCount() const { return rl.size(); }
    Q_INVOKABLE int getTotalLightCount() const { return ll.size(); }
    Q_INVOKABLE int getTotalCarCount() const { return cl.size(); }
    Q_INVOKABLE int getCarCountOnRoad(int roadId) const;
    Q_INVOKABLE QVariantList getRoadStatusList() const;
    Q_INVOKABLE QVariantList getLightStatusList() const;
    Q_INVOKABLE QVariantMap getCarStatus(int id) const;

    Q_INVOKABLE void setCarLight(int id, bool on);
    Q_INVOKABLE void setCarAlarm(int id, bool on);
    Q_INVOKABLE void setCarRunning(int id, bool on);
    Q_INVOKABLE void setCarLock(int id, bool locked);

    // 新增道路阻断接口
    Q_INVOKABLE void setRoadBlocked(int roadId, bool blocked);
    Q_INVOKABLE bool isRoadBlocked(int roadId) const;

    bool showRoad() const { return m_showRoad; }
    bool showLight() const { return m_showLight; }
    bool showCar() const { return m_showCar; }
    void setShowRoad(bool show);
    void setShowLight(bool show);
    void setShowCar(bool show);
    void setCarSpeed(int id, int speed);
    void setCarEndurance(int id, double endurance);
    void setCarMileage(int id, double mileage);

    static DataContext *getObject();
    Q_INVOKABLE QJsonArray getRoadList();
    Q_INVOKABLE QJsonArray getLightList();
    Q_INVOKABLE QJsonArray getCarList();

    /**
     * @brief 修改信号灯颜色
     * @param id 信号的id
     * @param color 信号灯颜色
     */
    void setLightColor(int id, int color);
    /**
     * @brief 修改车辆位置
     * @param id
     * @param roadId
     * @param roadPos
     */
    void setCarRunPos(int id, int roadId, int roadPos);
private slots:
    void updateLightTimers();
private:
        QTimer* m_lightTimer;
        int getDefaultTimeForColor(int color) const;
        void nextLightState(LightInfo* light);
        struct CarStatus {
        bool light = false;
        bool alarm = false;
        bool running = false;
        bool locked = true;
        int range = 0;
        int mileage = 0;
        int speed = 0;
        double endurance = 0;
        QString warning = "正常";
    };
    QVector<CarStatus> carStatuses;
    bool m_showRoad = true;
    bool m_showLight = true;
    bool m_showCar = true;
    void initCarStatuses();

signals:
    void dataChanged();

protected:
    void autoSetRoadColor(int roadId);
    static DataContext *obj;
    QVector<RoadInfo *> rl;
    QVector<LightInfo *> ll;
    QVector<CarInfo *> cl;
};

#endif // DATACONTEXT_H