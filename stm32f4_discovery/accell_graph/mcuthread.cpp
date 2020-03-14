#include "mcuthread.h"

#include <QtCore/QRandomGenerator>
#include <QThread>
#include <QtDebug>

#include "remcu.h"
#include "stm32f4xx.h"
#include "stm32f4_discovery_lis302dl.h"


McuThread::McuThread(){}

static void init_LIS302DL(){
    uint8_t ctrl = 0;

    LIS302DL_InitTypeDef  LIS302DL_InitStruct;
    LIS302DL_InterruptConfigTypeDef LIS302DL_InterruptStruct;

    /* Set configuration of LIS302DL*/
    LIS302DL_InitStruct.Power_Mode = LIS302DL_LOWPOWERMODE_ACTIVE;
    LIS302DL_InitStruct.Output_DataRate = LIS302DL_DATARATE_100;
    LIS302DL_InitStruct.Axes_Enable = LIS302DL_X_ENABLE | LIS302DL_Y_ENABLE | LIS302DL_Z_ENABLE;
    LIS302DL_InitStruct.Full_Scale = LIS302DL_FULLSCALE_2_3;
    LIS302DL_InitStruct.Self_Test = LIS302DL_SELFTEST_NORMAL;
    LIS302DL_Init(&LIS302DL_InitStruct);

    /* Set configuration of Internal High Pass Filter of LIS302DL*/
    LIS302DL_InterruptStruct.Latch_Request = LIS302DL_INTERRUPTREQUEST_LATCHED;
    LIS302DL_InterruptStruct.SingleClick_Axes = LIS302DL_CLICKINTERRUPT_Z_ENABLE;
    LIS302DL_InterruptStruct.DoubleClick_Axes = LIS302DL_DOUBLECLICKINTERRUPT_Z_ENABLE;
    LIS302DL_InterruptConfig(&LIS302DL_InterruptStruct);

    /* Required sleep for the MEMS Accelerometre: Turn-on time = 3/Output data Rate
                                                             = 3/100 = 30ms */
    QThread::msleep(30);

    /* Configure Interrupt control register: enable Click interrupt1 */
    ctrl = 0x07;
    LIS302DL_Write(&ctrl, LIS302DL_CTRL_REG3_ADDR, 1);

    /* Enable Interrupt generation on click/double click on Z axis */
    ctrl = 0x70;
    LIS302DL_Write(&ctrl, LIS302DL_CLICK_CFG_REG_ADDR, 1);

    /* Configure Click Threshold on X/Y axis (10 x 0.5g) */
    ctrl = 0xAA;
    LIS302DL_Write(&ctrl, LIS302DL_CLICK_THSY_X_REG_ADDR, 1);

    /* Configure Click Threshold on Z axis (10 x 0.5g) */
    ctrl = 0x0A;
    LIS302DL_Write(&ctrl, LIS302DL_CLICK_THSZ_REG_ADDR, 1);

    /* Configure Time Limit */
    ctrl = 0x03;
    LIS302DL_Write(&ctrl, LIS302DL_CLICK_TIMELIMIT_REG_ADDR, 1);

    /* Configure Latency */
    ctrl = 0x7F;
    LIS302DL_Write(&ctrl, LIS302DL_CLICK_LATENCY_REG_ADDR, 1);

    /* Configure Click Window */
    ctrl = 0x7F;
    LIS302DL_Write(&ctrl, LIS302DL_CLICK_WINDOW_REG_ADDR, 1);
}

void McuThread::run()
{
    accel_data_t data;
    bool success = remcu_resetRemoteUnit(__HALT);

    success &= remcu_isConnected();

    if(success == false
        || remcu_getErrorCount() != 0){
        qWarning() << "Error connect to server or board problem";
        return;
    }

    SystemInit();

    init_LIS302DL();

    #define _SIZE_BUFFER_ACC 6
    uint8_t Buffer[_SIZE_BUFFER_ACC];

    while(1){
        LIS302DL_Read(Buffer, LIS302DL_OUT_X_ADDR, _SIZE_BUFFER_ACC);
        data.accX = Buffer[0];
        data.accY = Buffer[2];
        data.accZ = Buffer[4];
        emit dataChanged(data);
        QThread::msleep(100);

        qDebug() << "err: " << remcu_getErrorCount();

        if(remcu_getErrorCount() != 0){
            qWarning() << "Error connect to server or hardware problem";
            break;
        }
    }
}

void McuThread::tryConnect(remcu_thread_options_t options)
{
    qDebug() << "server params:" << options.host << options.port;

    if(this->isRunning()){
        qWarning() << "MCU thread is running yet";
        return;
    }

    bool connect_ok = true;

    if(options.port == 6666){ //connect to OpenOcd server
        connect_ok = remcu_connect2OpenOCD(options.host.toLatin1().data(), 6666, 1);
    } else { //connect to GDB server
        connect_ok = remcu_connect2GDB(options.host.toLatin1().data(), options.port, 1);
    }

    if(connect_ok == false){
        qWarning() << "Server " << options.host << options.port << " error - Can't run thread";
        return;
    }

    this->start();
}
