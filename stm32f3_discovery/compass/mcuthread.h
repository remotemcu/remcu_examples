#ifndef MCUTHREAD_H
#define MCUTHREAD_H
#include <QThread>

#include "analogcompas.h"

typedef struct {
    QString host;
    uint16_t port;
} remcu_thread_options_t;

typedef struct {
    uint8_t accX;
    uint8_t accY;
    uint8_t accZ;
} accel_data_t;

Q_DECLARE_METATYPE(remcu_thread_options_t)
Q_DECLARE_METATYPE(accel_data_t)

class McuThread : public QThread
{
    Q_OBJECT
public:
    McuThread(AnalogCompas * analogCompas);

    void run();

public slots:
    void tryConnect(remcu_thread_options_t options);

signals:
    void dataChanged(accel_data_t data);

private:
    remcu_thread_options_t options;
    AnalogCompas * analogCompas;

};

#endif // MCUTHREAD_H
