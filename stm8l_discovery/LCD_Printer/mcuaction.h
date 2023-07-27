#ifndef MCUACTION_H
#define MCUACTION_H

#include <QObject>
#include <QString>


typedef struct {
    QString host;
    uint16_t port;
} remcu_thread_options_t;


Q_DECLARE_METATYPE(remcu_thread_options_t)

class McuAction: public QObject
{
    Q_OBJECT
public:
    McuAction();

public slots:
    void tryConnect(remcu_thread_options_t options);
    void sendText(const QString text);

private:
    remcu_thread_options_t options;

};

#endif // MCUACTION_H
