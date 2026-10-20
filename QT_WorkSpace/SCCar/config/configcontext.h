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
    static void getCarInfo(int &id, QString &key);
    static void getCarRunList(QStringList &list);

    static void getCarSpeed(int &speed);              // ← 加
    static void getCarEndurance(double &endurance);    // ← 加
    static void getCarMileage(double &mileage);        // ← 加
    static void saveCarEndurance(double endurance);    // ← 加
    static void saveCarMileage(double mileage);
};

#endif // CONFIGCONTEXT_H






