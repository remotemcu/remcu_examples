#ifndef MCUTHREAD_H
#define MCUTHREAD_H
#include <QThread>

typedef struct {
    QString host;
    uint16_t port;
} remcu_thread_options_t;

typedef struct {
    float accX;
    float accY;
    float accZ;
} accel_data_t;

Q_DECLARE_METATYPE(remcu_thread_options_t)
Q_DECLARE_METATYPE(accel_data_t)

class McuThread : public QThread
{
    Q_OBJECT
public:
    McuThread();

    void run();

public slots:
    void tryConnect(remcu_thread_options_t options);

signals:
    void dataChanged(accel_data_t data);

private:
    remcu_thread_options_t options;

};

#endif // MCUTHREAD_H
