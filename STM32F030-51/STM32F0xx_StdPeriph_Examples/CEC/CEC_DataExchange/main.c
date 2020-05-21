/**
  ******************************************************************************
  * @file    CEC/CEC_DataExchange/main.c 
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

/** @addtogroup CEC_DataExchange
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t MyLogicalAddress1 = 0x0, MyFollowerAddress = 0x0;
uint8_t HeaderBlockValueToSend = 0;
__IO uint8_t TransferStatus = 0;
extern uint8_t ReceiveBuffer[10];
extern __IO uint8_t ReceivedFrame1;
extern uint8_t rcv_inc;

/* Private function prototypes -----------------------------------------------*/
static void CEC_Config(void);

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
#ifdef LCD_DISPLAY
  /* Initialize the LCD */
#ifdef USE_STM320518_EVAL
    STM320518_LCD_Init();
#else
    STM32072B_LCD_Init();
#endif /* USE_STM320518_EVAL */
#endif /* LCD_DISPLAY */

  /* Initialize LEDs and JoyStick Button available on STM320518-EVAL board */
  /* Configure LED1, LED2, LED3 and LED4 */
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);

  /* Configure the Joystick button and its associated EXTI Line */
  STM_EVAL_PBInit(BUTTON_RIGHT, BUTTON_MODE_EXTI);
  STM_EVAL_PBInit(BUTTON_LEFT, BUTTON_MODE_EXTI);
  STM_EVAL_PBInit(Button_UP, BUTTON_MODE_EXTI);
  STM_EVAL_PBInit(Button_DOWN, BUTTON_MODE_EXTI);

#if defined (DEVICE_1)
  MyLogicalAddress1 = DEVICE_ADDRESS_1;
  MyFollowerAddress = DEVICE_ADDRESS_2;
#elif defined (DEVICE_2)
  MyLogicalAddress1 = DEVICE_ADDRESS_2;
  MyFollowerAddress = DEVICE_ADDRESS_1;
#endif /* DEVICE_1 */

  /* CEC configuration */
  CEC_Config();

  /* Build the Header block to send */
  HeaderBlockValueToSend = (((MyLogicalAddress1 & 0xF) << 4)|(MyFollowerAddress & 0xF));

#ifdef LCD_DISPLAY
  /* Clear the LCD */
  LCD_Clear(LCD_COLOR_BLUE);
  LCD_SetBackColor(LCD_COLOR_BLACK);
  LCD_SetTextColor(LCD_COLOR_WHITE);
  LCD_DisplayStringLine(LCD_LINE_0, (uint8_t *)" Select CEC Command ");
  LCD_SetBackColor(LCD_COLOR_BLUE);
  LCD_SetTextColor(LCD_COLOR_WHITE);
  LCD_DisplayStringLine(LCD_LINE_2, (uint8_t *)"RIGHT : OSD Name     ");
  LCD_DisplayStringLine(LCD_LINE_3, (uint8_t *)"LEFT  : CEC Version  ");
  LCD_DisplayStringLine(LCD_LINE_4, (uint8_t *)"UP    : Volume Up    ");
  LCD_DisplayStringLine(LCD_LINE_5, (uint8_t *)"DOWN  : Volume Down  ");
#endif /* LCD_DISPLAY */

  while(1)
  {
    /* If a frame has been received */
    while(ReceivedFrame1 == 0)
    {}

    if (ReceivedFrame1 == 1)
    {
      /* OK */
      if (ReceiveBuffer[1] ==0x44) /* Test on the opcode value */
      {
        /* Receive command is equal to Volume Up(Button Up) */
        if (ReceiveBuffer[2] == 0x41) /* Test on the operand value */
        {
          STM_EVAL_LEDOn(LED1);
          STM_EVAL_LEDOn(LED2);
          STM_EVAL_LEDOn(LED3);
          STM_EVAL_LEDOn(LED4);
        }/* Test on the operand value */
        else if (ReceiveBuffer[2] == 0x42) /* Receive command is equal to Volume Down(Button Down) */
        {
          STM_EVAL_LEDOff(LED1);
          STM_EVAL_LEDOff(LED2);
          STM_EVAL_LEDOff(LED3);
          STM_EVAL_LEDOff(LED4);
        }
      }
      else if (ReceiveBuffer[1] ==0x46) /* Test on the opcode value */
      {
        STM_EVAL_LEDOn(LED1);
        STM_EVAL_LEDOn(LED2);
        STM_EVAL_LEDOff(LED3);
        STM_EVAL_LEDOff(LED4);
      }
      else if (ReceiveBuffer[1] ==0x9F) /* Test on the opcode value */
      {
        STM_EVAL_LEDOff(LED1);
        STM_EVAL_LEDOff(LED2);
        STM_EVAL_LEDOn(LED3);
        STM_EVAL_LEDOn(LED4);
      }

#ifdef LCD_DISPLAY
      LCD_SetBackColor(LCD_COLOR_GREEN);
      LCD_DisplayStringLine(LCD_LINE_7, (uint8_t *)"Receive : Succeeded ");
#endif /* LCD_DISPLAY */
    }
    else
    {
      /* KO */
      /* Turn on LED2 */
      STM_EVAL_LEDOn(LED3);
#ifdef LCD_DISPLAY
      LCD_SetBackColor(LCD_COLOR_RED);
      LCD_DisplayStringLine(LCD_LINE_7, (uint8_t *)"Receive : Failed   ");
#endif /* LCD_DISPLAY */
    }
    ReceivedFrame1 = 0;

    for (rcv_inc=0;rcv_inc<10;rcv_inc++)
    {
      ReceiveBuffer[rcv_inc]=0;
    }
    rcv_inc=0;
  }
}
/**
  * @brief  Configures the CEC peripheral.
  * @param  None
  * @retval None
  */
static void CEC_Config(void)
{
  CEC_InitTypeDef CEC_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable CEC clocks */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CEC , ENABLE);
  
  /* Enable CEC_LINE_GPIO clocks */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

  /* Configure CEC_LINE_GPIO as Output open drain */
#ifdef USE_STM320518_EVAL
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
#else
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
#endif /* USE_STM320518_EVAL */  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* Enable the CEC global Interrupt (with higher priority) */
#ifdef USE_STM320518_EVAL
  NVIC_InitStructure.NVIC_IRQChannel = CEC_IRQn;
#else
  NVIC_InitStructure.NVIC_IRQChannel = CEC_CAN_IRQn;
#endif /* USE_STM320518_EVAL */
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Configure CEC */
  CEC_InitStructure.CEC_SignalFreeTime = CEC_SignalFreeTime_Standard;
  CEC_InitStructure.CEC_RxTolerance = CEC_RxTolerance_Standard;
  CEC_InitStructure.CEC_StopReception = CEC_StopReception_Off;
  CEC_InitStructure.CEC_BitRisingError = CEC_BitRisingError_Off;
  CEC_InitStructure.CEC_LongBitPeriodError = CEC_LongBitPeriodError_Off;
  CEC_InitStructure.CEC_BRDNoGen = CEC_BRDNoGen_Off;
  CEC_InitStructure.CEC_SFTOption = CEC_SFTOption_Off;
  CEC_Init(&CEC_InitStructure);

  /* Set the CEC initiator address */
  CEC_OwnAddressConfig(MyLogicalAddress1);

  /* Activate CEC interrupts associated to the set of TX flags */
  CEC_ITConfig(CEC_IT_TXEND|CEC_IT_TXBR,ENABLE);

  /* Activate CEC interrupts associated to the set of RX flags */
  CEC_ITConfig(CEC_IT_RXEND|CEC_IT_RXBR,ENABLE);

  /* Activate CEC interrupts associated to the set of TX error */
  CEC_ITConfig(CEC_IT_TXACKE|CEC_IT_TXERR|CEC_IT_TXUDR|CEC_IT_ARBLST,ENABLE);

  /* Activate CEC interrupts associated to the set of RX error */
  CEC_ITConfig(CEC_IT_RXACKE|CEC_IT_LBPE|CEC_IT_SBPE|CEC_IT_BRE|CEC_IT_RXOVR,ENABLE);

  /* Enable CEC */
  CEC_Cmd(ENABLE);
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
