#ifndef SYSTEMCONTROL_H
#define SYSTEMCONTROL_H

#include <QObject>

class SystemControl : public QObject
{
    Q_OBJECT
protected:
    explicit SystemControl(QObject *parent = nullptr);

public:
    enum LightMode{
        AutoMode = 0,
        HorMode,
        VerMode,
        NoMode,
        NightMode
    };
    static SystemControl *getObject();
    void system_init();
    void system_start();

    /**
     * @brief 设置信号灯模式
     * @param mode
     */
    Q_INVOKABLE void setMode(LightMode mode);

    Q_INVOKABLE int getCrossingId();

signals:
    void modeChanged(LightMode mode);

protected slots:
    void crossingModeSetSlot(int id, int mode);
    void lightColorChangedSlot();

protected:
    void timerEvent(QTimerEvent *e);

    static SystemControl *obj;
    LightMode mode = LightMode::AutoMode;
    int *autoModeHorBuf, *autoModeVerBuf;
    int croId = -1;
    int greenTime = 5;
};

#endif // SYSTEMCONTROL_H





