#ifndef VIEWCONTROLLER_H
#define VIEWCONTROLLER_H

#include <QObject>

#include "mcuthread.h"

class ViewController: public QObject
{
    Q_OBJECT
public:
    ViewController();

signals:
    void start(remcu_thread_options_t options);
    void dataChanged(accel_data_t data);

private slots:
    void connect_click();
};

#endif // VIEWCONTROLLER_H
