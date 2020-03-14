#include "mcuthread.h"

#include <QtCore/QRandomGenerator>
#include <QtDebug>
#include <QDateTime>

#include "remcu.h"
#include "stm32f4xx.h"

McuThread::McuThread(){}

static bool ask_send = false;
static QCanBusFrame canFrame;

#define CANx                       CAN1
#define CAN_CLK                    RCC_APB1Periph_CAN1
#define CAN_RX_PIN                 GPIO_Pin_0
#define CAN_TX_PIN                 GPIO_Pin_1
#define CAN_GPIO_PORT              GPIOD
#define CAN_GPIO_CLK               RCC_AHB1Periph_GPIOD
#define CAN_AF_PORT                GPIO_AF_CAN1
#define CAN_RX_SOURCE              GPIO_PinSource0
#define CAN_TX_SOURCE              GPIO_PinSource1


static void CAN_Config(){
    GPIO_InitTypeDef       GPIO_InitStructure;
    CAN_InitTypeDef        CAN_InitStructure;
    CAN_FilterInitTypeDef  CAN_FilterInitStructure;

    /* CAN GPIOs configuration **************************************************/

    /* Enable GPIO clock */
    RCC_AHB1PeriphClockCmd(CAN_GPIO_CLK, ENABLE);

    /* Connect CAN pins to AF9 */
    GPIO_PinAFConfig(CAN_GPIO_PORT, CAN_RX_SOURCE, CAN_AF_PORT);
    GPIO_PinAFConfig(CAN_GPIO_PORT, CAN_TX_SOURCE, CAN_AF_PORT);

    /* Configure CAN RX and TX pins */
    GPIO_InitStructure.GPIO_Pin = CAN_RX_PIN | CAN_TX_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(CAN_GPIO_PORT, &GPIO_InitStructure);
    /* CAN configuration ********************************************************/
  /* Enable CAN clock */
    RCC_APB1PeriphClockCmd(CAN_CLK, ENABLE);
    /* CAN register init */
    CAN_DeInit(CANx);

    /* CAN cell init */
    CAN_InitStructure.CAN_TTCM = DISABLE;
    CAN_InitStructure.CAN_ABOM = DISABLE;
    CAN_InitStructure.CAN_AWUM = DISABLE;
    CAN_InitStructure.CAN_NART = DISABLE;
    CAN_InitStructure.CAN_RFLM = DISABLE;
    CAN_InitStructure.CAN_TXFP = DISABLE;
    CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
    CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;

    /* CAN Baudrate = 125 KBps (CAN clocked at 30 MHz) */
    CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq;
    CAN_InitStructure.CAN_BS2 = CAN_BS2_8tq;
    CAN_InitStructure.CAN_Prescaler = 2*8; //125 KBps
    CAN_Init(CANx, &CAN_InitStructure);

    /* CAN filter init */
    CAN_FilterInitStructure.CAN_FilterNumber = 0;
    CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
    CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
    CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
    CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
    CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
    CAN_FilterInit(&CAN_FilterInitStructure);
}

void McuThread::run()
{
    QCanBusFrame frame;

    bool success = remcu_resetRemoteUnit(__HALT);

    success &= remcu_isConnected();

    if(success == false
        || remcu_getErrorCount() != 0){
        qWarning() << "Error connect to server or board problem";
        return;
    }

    SystemInit();

    CAN_Config();

    while(true){
        while(ask_send == false){
            if(CAN_MessagePending(CANx, CAN_FIFO0) < 1){
                continue;
            }
            CanRxMsg RxMessage;
            RxMessage.StdId=0x00;
            RxMessage.IDE=CAN_ID_STD;
            RxMessage.DLC=0;
            RxMessage.Data[0]=0x00;
            RxMessage.Data[1]=0x00;
            CAN_Receive(CANx, CAN_FIFO0, &RxMessage);

            const QByteArray data((char*)RxMessage.Data, RxMessage.DLC);

            QCanBusFrame frame(RxMessage.StdId, data);
            QCanBusFrame::TimeStamp ts(
                QDateTime::currentSecsSinceEpoch(),
                (QDateTime::currentMSecsSinceEpoch() % 1000)*1000
                );
            frame.setTimeStamp(ts);
            processReceivedFrames(frame);
        }
        CanTxMsg TxMessage;
        TxMessage.StdId=canFrame.frameId();
        TxMessage.ExtId = canFrame.hasExtendedFrameFormat();
        TxMessage.RTR=CAN_RTR_DATA;
        TxMessage.IDE=CAN_ID_STD;
        TxMessage.DLC=canFrame.payload().size() & 0xF;
        for (int i=0; i < TxMessage.DLC; i++) {
            TxMessage.Data[i] = canFrame.payload().at(i);
        }
        uint8_t TransmitMailbox = 0;
        TransmitMailbox = CAN_Transmit(CANx, &TxMessage);
        while((CAN_TransmitStatus(CANx, TransmitMailbox) != CANTXOK)){}

        qDebug() << "sent frame: " << canFrame.toString();
        ask_send = false;
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
        qWarning() << "Server error - Can't run thread";
        return;
    }

    this->start();
}

void McuThread::sendFrame(const QCanBusFrame frame)
{
    qDebug() << frame.toString();
    if(ask_send == true){
        qWarning() << "previous frame has not been sent yet";
        qWarning() << "try again later";
        return;
    }
    canFrame = frame;
    ask_send = true;
}
