/**
  ******************************************************************************
  * @file    CAN/DualCAN/main.c 
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
#include "stm32_eval.h"
#include "stm3210c_eval_lcd.h"

/** @addtogroup STM32F10x_StdPeriph_Examples
  * @{
  */

/** @addtogroup CAN_DualCAN
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define CAN_BAUDRATE  1000      /* 1MBps   */
/* #define CAN_BAUDRATE  500*/  /* 500kBps */
/* #define CAN_BAUDRATE  250*/  /* 250kBps */
/* #define CAN_BAUDRATE  125*/  /* 125kBps */
/* #define CAN_BAUDRATE  100*/  /* 100kBps */ 
/* #define CAN_BAUDRATE  50*/   /* 50kBps  */ 
/* #define CAN_BAUDRATE  20*/   /* 20kBps  */ 
/* #define CAN_BAUDRATE  10*/   /* 10kBps  */ 
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
CAN_InitTypeDef        CAN_InitStructure;
CAN_FilterInitTypeDef  CAN_FilterInitStructure;
CanTxMsg TxMessage;

/* Private function prototypes -----------------------------------------------*/
void NVIC_Config(void);
void CAN_Config(void);
void LED_Display(uint8_t Ledstatus);
void Init_RxMes(CanRxMsg *RxMessage);
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
       file (startup_stm32f10x_xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f10x.c file
     */
        
  /* NVIC configuration */
  NVIC_Config();

  /* Configures LED 1..4 */
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);

  /* LCD Initialization */
  STM3210C_LCD_Init();
  LCD_Clear(LCD_COLOR_WHITE);

  /* Set the LCD Back Color */
  LCD_SetBackColor(LCD_COLOR_RED);
  /* Set the LCD Text Color */
  LCD_SetTextColor(LCD_COLOR_GREEN);
  
  LCD_DisplayStringLine(LCD_LINE_0, "   STM3210C-EVAL    ");
  LCD_DisplayStringLine(LCD_LINE_1, " STM32F10x Dual CAN ");
  LCD_DisplayStringLine(LCD_LINE_2, "To start Press on:  ");
  LCD_DisplayStringLine(LCD_LINE_3, "Key or Tamper Button");

  /* Set the LCD Back Color */
  LCD_SetBackColor(LCD_COLOR_BLUE);

#if CAN_BAUDRATE == 1000 /* 1MBps */
  LCD_DisplayStringLine(LCD_LINE_4, " BAUDRATE = 1MBps   ");
#elif CAN_BAUDRATE == 500 /* 500KBps */
  LCD_DisplayStringLine(LCD_LINE_4, " BAUDRATE = 500kBps   ");
#elif CAN_BAUDRATE == 250 /* 250KBps */
  LCD_DisplayStringLine(LCD_LINE_4, " BAUDRATE = 250kBps   ");
#elif CAN_BAUDRATE == 125 /* 125KBps */
  LCD_DisplayStringLine(LCD_LINE_4, " BAUDRATE = 125kBps   ");
#elif  CAN_BAUDRATE == 100 /* 100KBps */
  LCD_DisplayStringLine(LCD_LINE_4, " BAUDRATE = 100kBps   ");
#elif  CAN_BAUDRATE == 50 /* 50KBps */
  LCD_DisplayStringLine(LCD_LINE_4, " BAUDRATE = 50kBps   ");
#elif  CAN_BAUDRATE == 20 /* 20KBps */
  LCD_DisplayStringLine(LCD_LINE_4, " BAUDRATE = 20kBps   ");
#elif  CAN_BAUDRATE == 10 /* 10KBps */
  LCD_DisplayStringLine(LCD_LINE_4, " BAUDRATE = 10kBps   ");
#endif
  /* Set the LCD Text Color */
  LCD_SetTextColor(LCD_COLOR_WHITE);   
    
  /* Configure BUTTON_KEY */
  STM_EVAL_PBInit(BUTTON_KEY, BUTTON_MODE_GPIO); 
  
  /* Configure BUTTON_TAMPER */
  STM_EVAL_PBInit(BUTTON_TAMPER, BUTTON_MODE_GPIO); 
   
  /* CANs configuration */
  CAN_Config();

  /* IT Configuration for CAN1 */  
  CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);

  /* IT Configuration for CAN2 */  
  CAN_ITConfig(CAN2, CAN_IT_FMP0, ENABLE);

  /* turn off all leds*/
  STM_EVAL_LEDOff(LED1);
  STM_EVAL_LEDOff(LED2);
  STM_EVAL_LEDOff(LED3);
  STM_EVAL_LEDOff(LED4);
 
  /* Infinite loop */
  while(1)
  {
    if(STM_EVAL_PBGetState(BUTTON_KEY)== RESET)
    {
      /* Turn On LED1 */
      LED_Display(0x01);
      TxMessage.Data[0] = 0x55;
      CAN_Transmit(CAN1, &TxMessage);

      /* Loop while KEY button is pressed */
      while(STM_EVAL_PBGetState(BUTTON_KEY)== RESET)
      {
      }
    }
    if(STM_EVAL_PBGetState(BUTTON_TAMPER)== RESET)
    {
      /* Turn On LED2 */
      LED_Display(0x2);
      TxMessage.Data[0] = 0xAA;
      CAN_Transmit(CAN2, &TxMessage);

      /* Loop while TAMPER button is pressed */
      while(STM_EVAL_PBGetState(BUTTON_TAMPER)== RESET)
      {
      }
    }
  }
}

/**
  * @brief  Configures CAN1 and CAN2.
  * @param  None
  * @retval None
  */
void CAN_Config(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;

  /* Configure CAN1 and CAN2 IOs **********************************************/
  /* GPIOB, GPIOD and AFIO clocks enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOB, ENABLE);
     
  /* Configure CAN1 RX pin */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
   
  /* Configure CAN2 RX pin */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* Configure CAN1 TX pin */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  /* Configure CAN2 TX pin */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* Remap CAN1 and CAN2 GPIOs */
  GPIO_PinRemapConfig(GPIO_Remap2_CAN1 , ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap_CAN2, ENABLE);

  /* Configure CAN1 and CAN2 **************************************************/  
  /* CAN1 and CAN2 Periph clocks enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1 | RCC_APB1Periph_CAN2, ENABLE);  
  
   /* CAN1 and CAN2 register init */
  CAN_DeInit(CAN1);
  CAN_DeInit(CAN2);

  /* Struct init*/
  CAN_StructInit(&CAN_InitStructure);

  /* CAN1 and CAN2  cell init */
  CAN_InitStructure.CAN_TTCM = DISABLE;
  CAN_InitStructure.CAN_ABOM = DISABLE;
  CAN_InitStructure.CAN_AWUM = DISABLE;
  CAN_InitStructure.CAN_NART = DISABLE;
  CAN_InitStructure.CAN_RFLM = DISABLE;
  CAN_InitStructure.CAN_TXFP = ENABLE;
  CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;  
  CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;
  CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;
 
#if CAN_BAUDRATE == 1000 /* 1MBps */
  CAN_InitStructure.CAN_Prescaler =6;
#elif CAN_BAUDRATE == 500 /* 500KBps */
  CAN_InitStructure.CAN_Prescaler =12;
#elif CAN_BAUDRATE == 250 /* 250KBps */
  CAN_InitStructure.CAN_Prescaler =24;
#elif CAN_BAUDRATE == 125 /* 125KBps */
  CAN_InitStructure.CAN_Prescaler =48;
#elif  CAN_BAUDRATE == 100 /* 100KBps */
  CAN_InitStructure.CAN_Prescaler =60;
#elif  CAN_BAUDRATE == 50 /* 50KBps */
  CAN_InitStructure.CAN_Prescaler =120;
#elif  CAN_BAUDRATE == 20 /* 20KBps */
  CAN_InitStructure.CAN_Prescaler =300;
#elif  CAN_BAUDRATE == 10 /* 10KBps */
  CAN_InitStructure.CAN_Prescaler =600;
#else
   #error "Please select first the CAN Baudrate in Private defines in main.c "
#endif  /* CAN_BAUDRATE == 1000 */

  
  /*Initializes the CAN1  and CAN2 */
  CAN_Init(CAN1, &CAN_InitStructure);
  CAN_Init(CAN2, &CAN_InitStructure);

  /* CAN1 filter init */
  CAN_FilterInitStructure.CAN_FilterNumber = 1;
  CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
  CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
  CAN_FilterInitStructure.CAN_FilterIdHigh = 0x6420;
  CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
  CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
  CAN_FilterInit(&CAN_FilterInitStructure);
  
  /* CAN2 filter init */
  CAN_FilterInitStructure.CAN_FilterIdHigh =0x2460;
  CAN_FilterInitStructure.CAN_FilterNumber = 15;
  CAN_FilterInit(&CAN_FilterInitStructure);
  
  /* Transmit */
  TxMessage.StdId = 0x321;
  TxMessage.ExtId = 0x01;
  TxMessage.RTR = CAN_RTR_DATA;
  TxMessage.IDE = CAN_ID_STD;
  TxMessage.DLC = 1;  
}

/**
  * @brief  Initializes a Rx Message.
  * @param  CanRxMsg *RxMessage.
  * @retval None
  */
void Init_RxMes(CanRxMsg *RxMessage)
{
  uint8_t i = 0;

  RxMessage->StdId = 0;
  RxMessage->ExtId = 0;
  RxMessage->IDE = CAN_ID_STD;
  RxMessage->DLC = 0;
  RxMessage->FMI = 0;
  for (i = 0; i < 8; i++)
  {
    RxMessage->Data[i] = 0;
  }
}

/**
  * @brief  Configures the NVIC for CAN.
  * @param  None
  * @retval None
  */
void NVIC_Config(void)
{
  NVIC_InitTypeDef  NVIC_InitStructure;

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
 
  NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x1;
  NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  Turn ON/OFF the dedicate led.
  * @param  Ledstatus: Communication status. 
  * @retval None
  */
void LED_Display(uint8_t Ledstatus)
{


  switch(Ledstatus)
  {
    case(1): 
      STM_EVAL_LEDOn(LED1);
      LCD_DisplayStringLine(LCD_LINE_5, "CAN1 send Frame     ");
      LCD_DisplayStringLine(LCD_LINE_6, "                    ");
      break;
   
    case(2): 
      STM_EVAL_LEDOn(LED2);
      LCD_DisplayStringLine(LCD_LINE_5, "                    ");
      LCD_DisplayStringLine(LCD_LINE_6, "CAN2 send Frame     ");
      break;
            
    case(3): 
      STM_EVAL_LEDOff(LED1);
      STM_EVAL_LEDOn(LED3);
      LCD_DisplayStringLine(LCD_LINE_5, "CAN1 receive Passed ");
      break;
     
    case(4): 
      STM_EVAL_LEDOff(LED2);
      STM_EVAL_LEDOn(LED4);
      LCD_DisplayStringLine(LCD_LINE_6, "CAN2 receive Passed ");
      break;

    case(5): 
      STM_EVAL_LEDOff(LED1);
      STM_EVAL_LEDOff(LED3);
      LCD_DisplayStringLine(LCD_LINE_7, "Communication Failed ");
      break;

    case(6): 
      STM_EVAL_LEDOff(LED2);
      STM_EVAL_LEDOff(LED4);
      LCD_DisplayStringLine(LCD_LINE_7, "Communication Failed ");
      break;
      
    default:
      break;
  }
}

/**
  * @brief  Delay
  * @param  None
  * @retval None
  */
void Delay(void)
{
  uint16_t nTime = 0x0000;

  for(nTime = 0; nTime <0xFFF; nTime++)
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

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
