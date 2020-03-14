#ifndef MCUTHREAD_H
#define MCUTHREAD_H
#include <QThread>
#include <QCanBusFrame>

typedef struct {
    QString host;
    uint16_t port;
    bool loop_mode;
} remcu_thread_options_t;


Q_DECLARE_METATYPE(remcu_thread_options_t)

class McuThread : public QThread
{
    Q_OBJECT
public:
    McuThread();

    void run();

public slots:
    void tryConnect(remcu_thread_options_t options);
    void sendFrame(const QCanBusFrame frame);

signals:
    void frameRecieved(QCanBusFrame frame);
    void processReceivedFrames(const QCanBusFrame &frame);

private:
    remcu_thread_options_t options;

};

#endif // MCUTHREAD_H
