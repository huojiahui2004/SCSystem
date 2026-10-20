#ifndef DATACONTEXT_H
#define DATACONTEXT_H

#include <QObject>
#include <QJsonArray>
class CarLight;
class CarAlarmLight;
class DataContext : public QObject
{
    Q_OBJECT
protected:
    explicit DataContext(QObject *parent = nullptr);

public:
    static DataContext *getObject();
    Q_INVOKABLE void setCarLightSw(bool sw);
    Q_INVOKABLE void setCarAlarmSw(bool sw);
    Q_INVOKABLE bool getCarLightSw();
    Q_INVOKABLE bool getCarAlarmSw();
    Q_INVOKABLE QJsonArray getCarLightList();
    Q_INVOKABLE QJsonArray getCarAlarmLightList();
    void setCrossingLight(int roadId, int color);
    int getCrossingLight(int roadId);

signals:
    void carLightSwChanged();
    void carAlarmSwChanged();

protected:
    static DataContext *obj;
    bool carLightSw = false;
    bool carAlarmSw = false;
    QVector<CarLight *> cll;
    QVector<CarAlarmLight *> call;
    QHash<int, int> crossingLightList;
};

class CarLight {
    friend class DataContext;
public:
    CarLight(int x, int y, int w, int h);
protected:
    int x,y,w,h;
};

class CarAlarmLight {
    friend class DataContext;
public:
    CarAlarmLight(int x, int y, int w, int h);
protected:
    int x,y,w,h;
};

#endif // DATACONTEXT_H
