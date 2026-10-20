#include "configcontext.h"
#include <QSettings>
#define CONF_FILENAME   "car_conf.ini"
#define DEF_GROUP       "set"
#define SERVER_IP       "server_ip"
#define SERVER_PORT     "server_port"
#define CAR_ID          "car_id"
#define CAR_KEY         "car_key"
#define CAR_RUN_LIST    "car_run_list"
#define CAR_SPEED       "car_speed"         // ← 加
#define CAR_ENDURANCE   "car_endurance"     // ← 加
#define CAR_MILEAGE     "car_mileage"

ConfigContext::ConfigContext(QObject *parent)
    : QObject{parent}
{}

void ConfigContext::getServerInfo(QString &ip, quint16 &port)
{
    QSettings set(CONF_FILENAME, QSettings::IniFormat);
    set.beginGroup(DEF_GROUP);

    if(!set.allKeys().contains(SERVER_IP))
        set.setValue(SERVER_IP, "127.0.0.1");
    ip = set.value(SERVER_IP).toString();

    if(!set.allKeys().contains(SERVER_PORT))
        set.setValue(SERVER_PORT, 10086);
    port = set.value(SERVER_PORT).toInt();

    set.endGroup();
}

void ConfigContext::getCarInfo(int &id, QString &key)
{
    QSettings set(CONF_FILENAME, QSettings::IniFormat);
    set.beginGroup(DEF_GROUP);

    if(!set.allKeys().contains(CAR_ID))
        set.setValue(CAR_ID, 0);
    id = set.value(CAR_ID).toInt();

    if(!set.allKeys().contains(CAR_KEY))
        set.setValue(CAR_KEY, "ADMIN123");
    key = set.value(CAR_KEY).toString();

    set.endGroup();
}

void ConfigContext::getCarRunList(QStringList &list)
{
    QSettings set(CONF_FILENAME, QSettings::IniFormat);
    set.beginGroup(DEF_GROUP);

    if(!set.allKeys().contains(CAR_RUN_LIST)){
        QStringList temp;
        temp << "0" << "6" << "10" << "5" << "13" << "9";
        set.setValue(CAR_RUN_LIST, temp);
    }
    list = set.value(CAR_RUN_LIST).toStringList();

    set.endGroup();
}
void ConfigContext::getCarSpeed(int &speed)
{
    QSettings set(CONF_FILENAME, QSettings::IniFormat);
    set.beginGroup(DEF_GROUP);
    if(!set.allKeys().contains(CAR_SPEED))
        set.setValue(CAR_SPEED, 5);
    speed = set.value(CAR_SPEED).toInt();
    set.endGroup();
}

void ConfigContext::getCarEndurance(double &endurance)
{
    QSettings set(CONF_FILENAME, QSettings::IniFormat);
    set.beginGroup(DEF_GROUP);
    if(!set.allKeys().contains(CAR_ENDURANCE))
        set.setValue(CAR_ENDURANCE, 1000.0);
    endurance = set.value(CAR_ENDURANCE).toDouble();
    set.endGroup();
}

void ConfigContext::getCarMileage(double &mileage)
{
    QSettings set(CONF_FILENAME, QSettings::IniFormat);
    set.beginGroup(DEF_GROUP);
    if(!set.allKeys().contains(CAR_MILEAGE))
        set.setValue(CAR_MILEAGE, 0.0);
    mileage = set.value(CAR_MILEAGE).toDouble();
    set.endGroup();
}

void ConfigContext::saveCarEndurance(double endurance)
{
    QSettings set(CONF_FILENAME, QSettings::IniFormat);
    set.beginGroup(DEF_GROUP);
    set.setValue(CAR_ENDURANCE, endurance);
    set.endGroup();
}

void ConfigContext::saveCarMileage(double mileage)
{
    QSettings set(CONF_FILENAME, QSettings::IniFormat);
    set.beginGroup(DEF_GROUP);
    set.setValue(CAR_MILEAGE, mileage);
    set.endGroup();
}







