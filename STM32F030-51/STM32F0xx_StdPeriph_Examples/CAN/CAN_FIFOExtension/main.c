/**
  ******************************************************************************
  * @file    CAN/CAN_FIFOExtension/main.c 
  * @author  MCD Application Team
  * @version V1.4.0
  * @date    24-July-2014
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32F0xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup CAN_FIFOExtension
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define TAMPER_PRESSED        0x00
#define TAMPER_NOT_PRESSED    0x01

#define MESSAGE1   "   STM32072B-EVAL   "
#define MESSAGE2   "Press TAMP to Send message  "
#define MESSAGE3   "  Send message to STM32     "
#define MESSAGE4   "Receive message From STM32  "


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
CanTxMsg TxMessage = {0};
CanRxMsg RxMessage = {0};
uint8_t KeyNumber = 0x0;
extern __IO uint8_t MsgReceived;
CAN_FilterInitTypeDef  CAN_FilterInitStructure;
/* Private function prototypes -----------------------------------------------*/
static void CAN_Config(void);
static void Display_TransmittedMsg(uint8_t PushNumber);

void Delay(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f0xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f0xx.c file
     */
  
  /* Initialize the LCD */
  STM32072B_LCD_Init();      
  
  /* Clear the LCD */
  LCD_Clear(LCD_COLOR_WHITE);
  
  /* Set the LCD Back Color */
  LCD_SetBackColor(Blue);
  
  /* Set the LCD Text Color */
  LCD_SetTextColor(White);
  
  /* Displays MESSAGE1 on line 0 */
  LCD_DisplayStringLine(LINE(0), (uint8_t *)MESSAGE1);
    
  LCD_SetFont(&Font12x12);
  
  /* Display Messages on the the LCD */  
  LCD_DisplayStringLine(LINE(0x3), (uint8_t *)MESSAGE2);
  
  /* Set the LCD Text size */
  LCD_SetFont(&Font16x24);
  
  /* Configure Push button key */
  STM_EVAL_PBInit(BUTTON_TAMPER, BUTTON_MODE_GPIO); 
     
  /* CAN configuration */
  CAN_Config();
  
  /* Infinite loop */
  while(1)
  {
    while(STM_EVAL_PBGetState(BUTTON_TAMPER) == TAMPER_PRESSED)
    { 
      if(KeyNumber == 41) KeyNumber = 0;
      
      Display_TransmittedMsg(KeyNumber);
      
      KeyNumber++;
      
      Delay();
      
      while(STM_EVAL_PBGetState(BUTTON_TAMPER) != TAMPER_NOT_PRESSED)
      {
      }
    }
      if (MsgReceived != 0)
        {
            /* Display received the 6 messages on tghe LCD */
            Display_ReceivedMsg();
            MsgReceived = 0;
     
        }
  }
}

/**
  * @brief  Configures the CAN.
  * @param  None
  * @retval None
  */
static void CAN_Config(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  NVIC_InitTypeDef  NVIC_InitStructure;
  CAN_InitTypeDef   CAN_InitStructure;
  
  /* CAN GPIOs configuration **************************************************/

  /* Enable GPIO clock */
  RCC_AHBPeriphClockCmd(CAN_GPIO_CLK, ENABLE);

  /* Connect CAN pins to AF7 */
  GPIO_PinAFConfig(CAN_GPIO_PORT, CAN_RX_SOURCE, CAN_AF_PORT);
  GPIO_PinAFConfig(CAN_GPIO_PORT, CAN_TX_SOURCE, CAN_AF_PORT); 
  
  /* Configure CAN RX and TX pins */
  GPIO_InitStructure.GPIO_Pin = CAN_RX_PIN | CAN_TX_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(CAN_GPIO_PORT, &GPIO_InitStructure);

  /* CAN NVIC  configuration **************************************************/  
  NVIC_InitStructure.NVIC_IRQChannel = CEC_CAN_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* CAN configuration ********************************************************/  
  /* Enable CAN clock */
  RCC_APB1PeriphClockCmd(CAN_CLK, ENABLE);
  
  /* CAN register init */
  CAN_DeInit(CANx);
  CAN_StructInit(&CAN_InitStructure);

  /* CAN cell init */
  CAN_InitStructure.CAN_TTCM = DISABLE;
  CAN_InitStructure.CAN_ABOM = DISABLE;
  CAN_InitStructure.CAN_AWUM = DISABLE;
  CAN_InitStructure.CAN_NART = DISABLE;
  CAN_InitStructure.CAN_RFLM = ENABLE;
  CAN_InitStructure.CAN_TXFP = ENABLE;
  CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
    
  /* CAN Baudrate = 1MBps (CAN clocked at 36 MHz) */
  CAN_InitStructure.CAN_BS1 = CAN_BS1_9tq;
  CAN_InitStructure.CAN_BS2 = CAN_BS2_8tq;
  CAN_InitStructure.CAN_Prescaler = 2;
  CAN_Init(CANx, &CAN_InitStructure);

  /* CAN filter init "FIFO0" */
  CAN_FilterInitStructure.CAN_FilterNumber = 0;
  CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;
  CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
  CAN_FilterInitStructure.CAN_FilterIdHigh = 0x6420;
  CAN_FilterInitStructure.CAN_FilterIdLow = 0x2461;
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
  CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
  CAN_FilterInit(&CAN_FilterInitStructure);
  
  /* Transmit Structure preparation */
  TxMessage.StdId = 0x321;
  TxMessage.ExtId = 0x00;
  TxMessage.RTR = CAN_RTR_DATA;
  TxMessage.IDE = CAN_ID_STD;
  TxMessage.DLC = 1;
    
  /* Enable FIFO 0 full Interrupt */
  CAN_ITConfig(CANx, CAN_IT_FF0, ENABLE);
  
  /* Enable FIFO 1 full Interrupt */
  CAN_ITConfig(CANx, CAN_IT_FF1, ENABLE);
}


/**
  * @brief  Display transmitted msg on the LCD 
  * @param  None.
  * @retval None
  */
static void Display_TransmittedMsg(uint8_t PushNumber)
{
  uint8_t text[50] = {0}, dataindex =0;
  
  uint8_t TxMessages[6] = { 0, 1, 2, 3, 4, 5 };
  
  /* Set the LCD Back Color and Text Color*/
  LCD_SetBackColor(Blue);
  LCD_SetTextColor(White);
  
   /* Set the LCD Text size */
  LCD_SetFont(&Font12x12);
  
  /* Display Messages on the the LCD */  
  LCD_DisplayStringLine(LINE(5), (uint8_t *)MESSAGE3);
  
  
  for(dataindex = 0 ; dataindex < 6 ;dataindex++)
  {
    
    TxMessage.Data[0] = (uint8_t)(TxMessages[dataindex] + (6 * PushNumber));
    
    CAN_Transmit(CANx, &TxMessage);
    
    sprintf((char*)text,"Send Message Num%d    = %d            ",dataindex+1,TxMessage.Data[0] );
    
    LCD_SetBackColor(Cyan);
    LCD_SetTextColor(Black);
    
    LCD_DisplayStringLine(LINE(6+dataindex),text);
  }
  
}

/**
  * @brief  Display received msg on the LCD 
  * @param  None.
  * @retval None
  */
void Display_ReceivedMsg(void)
{
  uint8_t text[50] = {0}, dataindex =0;
  
  /* Set the LCD Back Color and Text Color*/
  LCD_SetBackColor(Blue);
  LCD_SetTextColor(White);
  
   /* Set the LCD Text size */
  LCD_SetFont(&Font12x12);
  
  
  /* Display Messages on the the LCD */  
  LCD_DisplayStringLine(LINE(13), (uint8_t *)MESSAGE4);

  
  for(dataindex = 0 ; dataindex < 6 ;dataindex++)
  {
    if (dataindex < 3)
    {  
      /* read data from CAN1 FIFO 0 */
      CAN_Receive(CANx, CAN_FIFO0, &RxMessage);
    }
    else
    {
      /* read data from CAN1 FIFO 1 */
      CAN_Receive(CANx, CAN_FIFO1, &RxMessage);
    }
    
    LCD_SetBackColor(Green);
    LCD_SetTextColor(Black);
    sprintf((char*)text,"Message received Num%d= %d   ",dataindex +1,RxMessage.Data[0]);
    
    /* Display message on the LCD*/
    LCD_DisplayStringLine(LINE(14+dataindex),text);
  }
}

/**
  * @brief  Delay
  * @param  None
  * @retval None
  */
void Delay(void)
{
  uint32_t nTime = 0x0000;

  for(nTime = 0; nTime <0xFFFFF; nTime++)
  {
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
