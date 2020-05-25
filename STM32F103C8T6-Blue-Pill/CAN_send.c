/**
  ******************************************************************************
  * @file    CAN/Networking/main.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "remcu.h"
#include "stm32f10x.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <limits.h>
#include <errno.h>
/** @addtogroup STM32F10x_StdPeriph_Examples
  * @{
  */
/** @addtogroup CAN_Networking
  * @{
  */ 


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
  #define RCC_APB2Periph_GPIO_CAN1    RCC_APB2Periph_GPIOB
  #define GPIO_Remapping_CAN1         GPIO_Remap1_CAN1
  #define GPIO_CAN1                   GPIOB
  #define GPIO_Pin_CAN1_RX            GPIO_Pin_8
  #define GPIO_Pin_CAN1_TX            GPIO_Pin_9

  #define CANx                       CAN1
  #define GPIO_CAN                   GPIO_CAN1
  #define GPIO_Remapping_CAN         GPIO_Remapping_CAN1
  #define GPIO_CAN                   GPIO_CAN1
  #define GPIO_Pin_CAN_RX            GPIO_Pin_CAN1_RX
  #define GPIO_Pin_CAN_TX            GPIO_Pin_CAN1_TX


/* Private macro -------------------------------------------------------------*/
#define MAX_MESSAGE_SIZE 8
/* Private variables ---------------------------------------------------------*/
CAN_InitTypeDef        CAN_InitStructure;
CAN_FilterInitTypeDef  CAN_FilterInitStructure;
CanTxMsg TxMessage;


uint8_t payload[MAX_MESSAGE_SIZE] = {0};
size_t len_payload = 0;
/* Private function prototypes -----------------------------------------------*/
void CAN_Config(void);
void Init_RxMes(CanRxMsg *RxMessage);

/* Private functions ---------------------------------------------------------*/
int parsePayloadArg(char * payload_arg){
  char * pEnd = payload_arg;
  char * prev = NULL;
  errno = 0;
  for(len_payload = 0; len_payload < MAX_MESSAGE_SIZE; len_payload++){
      long result= strtol(pEnd, &pEnd, 16);
      if (*pEnd == '\0' ||  pEnd == prev){
          return 0;
      }
      if (pEnd == payload_arg)
      {
          printf("Invalid parse payload error, It's digi order ex. \"0x11 0xFE 0x00 0x70 ... \"\n");
          return -1;
      }
      if ((result == LONG_MAX || result == LONG_MIN) && errno == ERANGE)
      {
          printf("Invalid parse payload error, from %s\n", pEnd);
          return -2;
      }
      payload[len_payload] = result & 0xFF;
      prev = pEnd;
    }
    return 0;
}

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(int argc, char** argv)
{
  if(argc < 3){
        printf("**The app requared 2 arguments: ID Payload(hex)\n"
          "This example works on 125 kBps. For other baud rates and\n"
          "this clock configuration(see system_stm32f10x.c),\n"
          "the CAN Initialization code (Prescaler value) should be amended accordingly.\n");
        printf("--------------------------------------------------\n");
        printf("Example:\n");
        printf("CAN_send x1F \"0x00 0xFF 0x17 0xBE\" \n");
        printf("The app send a message with ID=x1F \"0x00 0xFF 0x17 0xBE\" \n");
        printf("Message size : 4\n");
        printf("data[0]= 0x00\n");
        printf("data[1]= 0xFF\n");
        printf("data[2]= 0x17\n");
        printf("data[3]= 0xBE\n");
        return -1;
    }

    const char * id_arg = argv[1];
    const uint8_t ID = (strtol(argv[1], NULL, 16) & 0xFF);

    char * payload_arg = argv[2];
    if (parsePayloadArg(payload_arg) != 0){
      printf("Parse error\n");
      return -1;
    }

  if(remcu_connect2GDB("localhost", 3333, 1) == false){
    printf("Connection error. Run OpenOCD server\n");
    return -1;
  }
  remcu_resetRemoteUnit(__HALT);

  assert(remcu_isConnected());

  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f10x_xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f10x.c file
     */
  SystemInit();
       
  /* CAN configuration */
  CAN_Config();
  
  /* transmit */
  TxMessage.StdId=ID;
  TxMessage.RTR=CAN_RTR_DATA;
  TxMessage.IDE=CAN_ID_STD;
  TxMessage.DLC=len_payload;
  for(int i = 0; i < len_payload; i++){
    TxMessage.Data[i]=payload[i];
  }

  uint8_t TransmitMailbox = 0;
  TransmitMailbox=CAN_Transmit(CANx, &TxMessage);

  printf("Sending the CAN message");
  for( int times = 0;
    (CAN_TransmitStatus(CANx, TransmitMailbox) != CAN_TxStatus_Ok) && (times != 0xFF);
     times++)
  {
      printf(".");
  }
  printf("\n");

  if(CAN_TransmitStatus(CANx, TransmitMailbox) != CAN_TxStatus_Ok){
    printf("Error! error code: %d\n", CAN_GetLastErrorCode(CANx));
    return -1;
  }

  printf("ID: %s Payload: %s message done successfully!\n", id_arg, payload_arg);

  return 0;
}

/**
  * @brief  Configures the CAN.
  * @param  None
  * @retval None
  */
void CAN_Config(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  /* GPIO clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIO_CAN1, ENABLE);

  /* Configure CAN pin: RX */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_CAN_RX;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIO_CAN, &GPIO_InitStructure);
  
  /* Configure CAN pin: TX */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_CAN_TX;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIO_CAN, &GPIO_InitStructure);
  
  GPIO_PinRemapConfig(GPIO_Remapping_CAN , ENABLE);
  
  /* CANx Periph clock enable */

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
  
  /* CAN register init */
  CAN_DeInit(CANx);
  CAN_StructInit(&CAN_InitStructure);

  /* CAN cell init */
  CAN_InitStructure.CAN_TTCM = DISABLE;
  CAN_InitStructure.CAN_ABOM = DISABLE;
  CAN_InitStructure.CAN_AWUM = DISABLE;
  CAN_InitStructure.CAN_NART = DISABLE;
  CAN_InitStructure.CAN_RFLM = DISABLE;
  CAN_InitStructure.CAN_TXFP = DISABLE;
  CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
  //CAN_InitStructure.CAN_Mode = CAN_Mode_LoopBack;
  
  /* CAN Baudrate = 125 kBps*/
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
  CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;
  CAN_InitStructure.CAN_BS2 = CAN_BS2_5tq;
  //CAN_InitStructure.CAN_Prescaler = 4*16; //Prescaler for 62.5 kBps
  CAN_InitStructure.CAN_Prescaler = 4*8; //Prescaler for 125 kBps
  //CAN_InitStructure.CAN_Prescaler = 4*4; //Prescaler for 250 kBps
  //CAN_InitStructure.CAN_Prescaler = 4*2; //Prescaler for 500 kBps
  //CAN_InitStructure.CAN_Prescaler = 4*1; //Prescaler for 1 MBps

  assert(CAN_Init(CANx, &CAN_InitStructure) == CAN_InitStatus_Success);

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

/**
  * @brief  Initializes a Rx Message.
  * @param  CanRxMsg *RxMessage
  * @retval None
  */
void Init_RxMes(CanRxMsg *RxMessage)
{
  uint8_t i = 0;

  RxMessage->StdId = 0x00;
  RxMessage->ExtId = 0x00;
  RxMessage->IDE = CAN_ID_STD;
  RxMessage->DLC = 0;
  RxMessage->FMI = 0;
  for (i = 0;i < 8;i++)
  {
    RxMessage->Data[i] = 0x00;
  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}

#endif

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
