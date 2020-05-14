/**
  ******************************************************************************
  * @file    PWR_STANDBY/main.c 
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    20-September-2012
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
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

/** @addtogroup STM32F3_Discovery_Peripheral_Examples
  * @{
  */

/** @addtogroup PWR_STANDBY
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
RTC_InitTypeDef  RTC_InitStructure;
RTC_AlarmTypeDef RTC_AlarmStructure;
RTC_TimeTypeDef  RTC_TimeStructure;
GPIO_InitTypeDef GPIO_InitStructure;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f30x.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f30x.c file
  */  
  
  /* SysTick interrupt each 10 ms */
  if (SysTick_Config(SystemCoreClock / 100))
  { 
    /* Capture error */ 
    while (1);
  }
  
  /* Configures the USER button */
  STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_GPIO);
  
  /* Configure LED3 */
  STM_EVAL_LEDInit(LED3);
  
  while(STM_EVAL_PBGetState(BUTTON_USER) == RESET);

  /* RTC Configuration ********************************************************/ 
  /* Enable the PWR clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
  
  /* Allow access to Backup Domain */
  PWR_BackupAccessCmd(ENABLE);

  /* Enable the LSI OSC */
  RCC_LSICmd(ENABLE);
  
  /* Wait till LSI is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
  {}
  
  /* Check if the StandBy flag is set */
  if (PWR_GetFlagStatus(PWR_FLAG_SB) != RESET)
  {       
    /* Clear StandBy flag */
    PWR_ClearFlag(PWR_FLAG_SB);
    
    /*Waits until the RTC Time and Date registers (RTC_TR and RTC_DR) are 
      synchronized with RTC APB clock.*/
    RTC_WaitForSynchro();
    
    /* No need to configure the RTC as the RTC config(clock source, enable,
       prescaler,...) are kept after wake-up from STANDBY */
  }
  else
  {
    /* Reset Backup Domain */
    RCC_BackupResetCmd(ENABLE);
    RCC_BackupResetCmd(DISABLE);

    /* Select the RTC Clock Source */
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);

    /* Enable the RTC Clock */
    RCC_RTCCLKCmd(ENABLE);
    
    /* Wait for RTC APB registers synchronisation */
    RTC_WaitForSynchro();
    
    RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
    RTC_InitStructure.RTC_AsynchPrediv = 0x7F;
    RTC_InitStructure.RTC_SynchPrediv = 0x0138;
    
    RTC_Init(&RTC_InitStructure);
 
    /* Set the alarm X+3s */
    RTC_AlarmStructure.RTC_AlarmTime.RTC_H12     = RTC_H12_AM;
    RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours   = 0x01;
    RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes = 0x00;
    RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds = 0x03;
    RTC_AlarmStructure.RTC_AlarmDateWeekDay = 0x31;
    RTC_AlarmStructure.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_Date;
    RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay;
    RTC_SetAlarm(RTC_Format_BCD, RTC_Alarm_A, &RTC_AlarmStructure);
  
    /* Enable RTC Alarm A Interrupt */
    RTC_ITConfig(RTC_IT_ALRA, ENABLE);
  
    /* Enable the alarm */
    RTC_AlarmCmd(RTC_Alarm_A, ENABLE);
  }
    
  /* Set the time to 01h 00mn 00s AM */
  RTC_TimeStructure.RTC_H12     = RTC_H12_AM;
  RTC_TimeStructure.RTC_Hours   = 0x01;
  RTC_TimeStructure.RTC_Minutes = 0x00;
  RTC_TimeStructure.RTC_Seconds = 0x00;  
  
  RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure);
   
  /* Clear Wakeup flag */
  PWR_ClearFlag(PWR_FLAG_WU);      
  
  RTC_ClearFlag(RTC_FLAG_ALRAF);
  
  /* Request to enter STANDBY mode (Wake Up flag is cleared in PWR_EnterSTANDBYMode function) */
  PWR_EnterSTANDBYMode(); 
  
  while(1);
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
