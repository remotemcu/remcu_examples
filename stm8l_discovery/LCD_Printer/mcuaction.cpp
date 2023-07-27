#include "mcuaction.h"

#include <QtDebug>
#include "remcu.h"
#include "stm8l15x.h"
#include "stm8l_discovery_lcd.h"

McuAction::McuAction(){}

static bool ask_send = false;
static QString send_text;

static bool connect_ok = false;


void run()
{

    bool success = remcu_resetRemoteUnit(__HALT);

    success &= remcu_isConnected();


    if(success == false
        || remcu_getErrorCount() != 0){
        qWarning() << "Error connect to server or board problem";
        return;
    }

    while(true){
        while(ask_send == false){}

    }
}

#define LED_GREEN_PORT		GPIOE
#define LED_GREEN_PIN		GPIO_Pin_7

#define LED_BLUE_PORT		GPIOC
#define LED_BLUE_PIN        GPIO_Pin_7

static void config_lcd(){

    /* Green led init: GPIO set in output */
    GPIO_Init( LED_GREEN_PORT, LED_GREEN_PIN, GPIO_Mode_Out_PP_High_Fast);

    /* Blue led init: GPIO set in output */
    GPIO_Init( LED_BLUE_PORT, LED_BLUE_PIN, GPIO_Mode_Out_PP_High_Fast);

    /* Initializes the LCD glass */
    LCD_GLASS_Init();
}

void McuAction::tryConnect(remcu_thread_options_t options)
{
    qDebug() << "server params:" << options.host << options.port;

    if(connect_ok == true){
        qWarning() << "Client conencted yet";
        return;
    }

    if(options.port == 6666){ //connect to OpenOcd server
        connect_ok = remcu_connect2OpenOCD(options.host.toLatin1().data(), 6666, 1);
    } else { //connect to GDB server
        connect_ok = remcu_connect2GDB(options.host.toLatin1().data(), options.port, 1);
    }

    connect_ok &= remcu_resetRemoteUnit(__HALT);

    connect_ok &= remcu_isConnected();

    if(connect_ok == false){
        qWarning() << "Server error - Can't run thread";
        return;
    }

    config_lcd();

    connect_ok &= remcu_getErrorCount() == 0;

    if(connect_ok == false){
        qWarning() << "Failed inition chip - check hardware board";
        return;
    }


}

void McuAction::sendText(const QString text)
{
    if(connect_ok == false){
        qWarning() << "No server connection";
        return;
    }

    qDebug() << "text:" << text;

    if(text.size() > 6){
        qWarning() << "text size is more 6 symbols. Print first six symbols only!";
    }

    QByteArray byte_text = text.toLatin1();

    byte_text.resize(6);

    LCD_GLASS_DisplayString((uint8_t*)byte_text.data());
}
