/**
  ******************************************************************************
  * @file    RTC/RTC_Timer/main.c 
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
  
/** @addtogroup RTC_Timer
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint8_t ALARM_Occured = 0;
__IO uint32_t RTCAlarmCount = 0;

/* Private function prototypes -----------------------------------------------*/
static void RTC_Config(void);
static void RTC_AlarmConfig(void);

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
#ifdef USE_STM320518_EVAL
    STM320518_LCD_Init();
#else
    STM32072B_LCD_Init();
#endif /* USE_STM320518_EVAL */
         
  /* Enable The Display */
  LCD_DisplayOn(); 

  /* Clear the Background Layer */ 
  LCD_Clear(LCD_COLOR_WHITE);
  
  /* Clear the LCD */ 
  LCD_Clear(White);

  /* Set the LCD Back Color */
  LCD_SetBackColor(Blue);
  
  /* Set the LCD Text Color */
  LCD_SetTextColor(White);
   
  /* Displays MESSAGE1 on line 0 */
  LCD_DisplayStringLine(LINE(0), (uint8_t *)MESSAGE1);

  /* RTC configuration */
  RTC_Config();
  
  /* Set the LCD Text Color */
  LCD_SetTextColor(Red);
  
  /* Displays a rectangle on the LCD */
  LCD_DrawRect(80, 280, 25, 240 );
  
  /* Configure the external interrupt "WAKEUP" and "TAMPER" buttons */
  STM_EVAL_PBInit(BUTTON_SEL, BUTTON_MODE_EXTI);
  STM_EVAL_PBInit(BUTTON_TAMPER, BUTTON_MODE_EXTI);  
     
  /* Configure RTC AlarmA register to generate 8 interrupts per 1 Second */
  RTC_AlarmConfig();
  
  /* set LCD Font */
  LCD_SetFont(&Font12x12);

  /* Set the LCD Back Color */
  LCD_SetBackColor(White); 

  /* Set the LCD Text Color */
  LCD_SetTextColor(Black);
  
  /* Set the Back Color */
  LCD_SetBackColor(LCD_COLOR_CYAN);
  /* Displays MESSAGE2 and MESSAGE3 on the LCD */
  LCD_DisplayStringLine(LINE(18), (uint8_t *)MESSAGE2);  
  LCD_DisplayStringLine(LINE(19), (uint8_t *)MESSAGE3);  

  /* Infinite loop */
  while (1)
  {
    uint32_t tmp =0;
    /* ALARM Interrupt */
    if (ALARM_Occured)
    {
      if(RTCAlarmCount != 480)
      {
        /* Increament the counter of Alarma interrupts */
        RTCAlarmCount++;
        
        /* Set the LCD Back Color */
        LCD_SetTextColor(Green);
        
        /* Draw rectangle on the LCD */
        LCD_DrawFullRect(81, 359, 80+ (((RTCAlarmCount)-1)/2) , 24);
        
        /* Set the LCD text color */
        LCD_SetTextColor(Red);
        
        /* Display rectangle on the LCD */
        LCD_DrawRect(80, 280, 25, 240 );
        
        /* Define the rate of Progress bar */
        tmp = (RTCAlarmCount * 100)/ 480; 
        
        /* Set the LCD Font */
        LCD_SetFont(&Font16x24);
        
        /* Display Char on the LCD : XXX% */
        LCD_DisplayChar(LINE(2),200, (tmp / 100) +0x30);
        LCD_DisplayChar(LINE(2),180, ((tmp  % 100 ) / 10) +0x30);
        LCD_DisplayChar(LINE(2),160, (tmp % 10) +0x30);
        LCD_DisplayChar(LINE(2),140, 0x25);
      }
      else
      {
        /* Disable the RTC Clock */
        RCC_RTCCLKCmd(DISABLE);
        
      }
      /* Reinitialize the ALARM variable */
      ALARM_Occured = 0;
    }
  }
}

/**
  * @brief  Configures the RTC peripheral and select the clock source.
  * @param  None
  * @retval None
  */
static void RTC_Config(void)
{
  RTC_InitTypeDef  RTC_InitStructure;
  RTC_TimeTypeDef  RTC_TimeStruct;

  /* Enable the PWR clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

  /* Allow access to RTC */
  PWR_BackupAccessCmd(ENABLE);

  /* Reset RTC Domain */
  RCC_BackupResetCmd(ENABLE);
  RCC_BackupResetCmd(DISABLE);

  /* Enable the LSE OSC */
  RCC_LSEConfig(RCC_LSE_ON);

  /* Wait till LSE is ready */  
  while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
  {}

  /* Select the RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

  /* Configure the RTC data register and RTC prescaler */
  /* ck_spre(1Hz) = RTCCLK(LSI) /(AsynchPrediv + 1)*(SynchPrediv + 1)*/
  RTC_InitStructure.RTC_AsynchPrediv = 0x7F;
  RTC_InitStructure.RTC_SynchPrediv  = 0xFF;
  RTC_InitStructure.RTC_HourFormat   = RTC_HourFormat_24;
  RTC_Init(&RTC_InitStructure);
  
  /* Set the time to 00h 00mn 00s AM */
  RTC_TimeStruct.RTC_H12     = RTC_H12_AM;
  RTC_TimeStruct.RTC_Hours   = 0x00;
  RTC_TimeStruct.RTC_Minutes = 0x00;
  RTC_TimeStruct.RTC_Seconds = 0x00;  
  RTC_SetTime(RTC_Format_BCD, &RTC_TimeStruct);
}

/**
  * @brief  Configures the RTC Alarm.
  * @param  None
  * @retval None
  */
static void RTC_AlarmConfig(void)
{
  EXTI_InitTypeDef EXTI_InitStructure;
  RTC_AlarmTypeDef RTC_AlarmStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  RTC_AlarmStructInit(&RTC_AlarmStructure);
    
  /* EXTI configuration */
  EXTI_ClearITPendingBit(EXTI_Line17);
  EXTI_InitStructure.EXTI_Line = EXTI_Line17;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
  /* Enable the RTC Alarm Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
 
  /* Set the alarmA Masks */
  RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_All;
  RTC_SetAlarm(RTC_Format_BCD, RTC_Alarm_A, &RTC_AlarmStructure);
  
  /* Set AlarmA subseconds and enable SubSec Alarm : generate 8 interripts per Second */
  RTC_AlarmSubSecondConfig(RTC_Alarm_A, 0xFF, RTC_AlarmSubSecondMask_SS14_5);

  /* Enable AlarmA interrupt */
  RTC_ITConfig(RTC_IT_ALRA, ENABLE);
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
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
