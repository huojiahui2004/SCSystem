#ifndef SYSTEMCONTROL_H
#define SYSTEMCONTROL_H

#include <QObject>

class SystemControl : public QObject
{
    Q_OBJECT
protected:
    explicit SystemControl(QObject *parent = nullptr);

public:
    static SystemControl *getObject();

protected slots:
    void lightColorChangedSlot(int id, int roadId, int color);
    void carRunPosChangedSlot(int id, int roadId, int roadPos, int speed, double endurance, double mileage);

protected:
    static SystemControl *obj;
};

#endif // SYSTEMCONTROL_H









