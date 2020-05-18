/**
  ******************************************************************************
  * @file    PWR/PWR_STANDBY/main.c 
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

/** @addtogroup PWR_STANDBY
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern __IO uint32_t CaptureNumber, PeriodValue;

/* Private function prototypes -----------------------------------------------*/
static void RTC_Config(void);
static void EnterSTANDBYMode(void);
static uint32_t GetLSIFrequency(void);
void SysTick_Configuration(void);

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
      
  /* Configure LED3 */
  STM_EVAL_LEDInit(LED3);
  
  /* Configure Systick */
  SysTick_Configuration();
  
  /* Turn ON LED3 */
  STM_EVAL_LEDOn(LED3);  

  /* Configures Tamper button */
  STM_EVAL_PBInit(BUTTON_TAMPER,BUTTON_MODE_GPIO);
                 
  /* Configure RTC clock source and prescaler */
  RTC_Config();

  /* Wait till Tamper button is pressed */  
  while(STM_EVAL_PBGetState(BUTTON_TAMPER) != RESET);

  /* Enter STANDBY mode, RTC Alarm within 3 second or an external RESET will 
   wake-up the system from STANDBY */
  EnterSTANDBYMode();
  
  while(1)
  {}
}

/**
  * @brief  Configures the RTC clock source.
  * @param  None
  * @retval None
  */
static void RTC_Config(void)
{  
  RTC_InitTypeDef   RTC_InitStructure;
  RTC_TimeTypeDef   RTC_TimeStructure;
  uint32_t LSIFreq = 0;

  /* Enable the PWR clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
  
  /* Allow access to Backup Domain */
  PWR_BackupAccessCmd(ENABLE);
  
  /* Disable wake-up source(ALARM) to guarantee free access to WUT level-OR input */
  RTC_ITConfig(RTC_IT_ALRA, DISABLE);

  /* Clear Wakeup flag */
  PWR_ClearFlag(PWR_FLAG_WU);

  /* Enable wake-up source(ALARM) to guarantee free access to WUT level-OR input */
  RTC_ITConfig(RTC_IT_ALRA, ENABLE);
  
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

    /* Check if the StandBy flag is cleared */
    if (PWR_GetFlagStatus(PWR_FLAG_SB) != RESET)
    {
      while(1);
    }
        
    RTC_WaitForSynchro();
    
    /* No need to configure the RTC as the RTC config(clock source, enable,
    prescaler,...) are kept after wake-up from STANDBY */
  }
  else
  {        
    /* RTC Configuration ******************************************************/
    /* Reset Backup Domain */
    RCC_BackupResetCmd(ENABLE);
    RCC_BackupResetCmd(DISABLE);
 
    /* Select the RTC Clock Source */
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);

    /* Enable the RTC Clock */
    RCC_RTCCLKCmd(ENABLE);

    /* Wait for RTC APB registers synchronisation */
    RTC_WaitForSynchro();

    /* Get the LSI frequency:  TIM14 is used to measure the LSI frequency */    
    LSIFreq = GetLSIFrequency();
    
    RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
    RTC_InitStructure.RTC_AsynchPrediv = 99;
    RTC_InitStructure.RTC_SynchPrediv =  (LSIFreq/100) - 1;

    RTC_Init(&RTC_InitStructure);

    /* Set the time to 01h 00mn 00s AM */
    RTC_TimeStructure.RTC_H12     = RTC_H12_AM;
    RTC_TimeStructure.RTC_Hours   = 0x01;
    RTC_TimeStructure.RTC_Minutes = 0x00;
    RTC_TimeStructure.RTC_Seconds = 0x00;

    RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure); 
  }     
}

/**
  * @brief  Enters STANDBY mode, RTC Alarm within 3 second or an external RESET 
  *         will wake-up the system from STANDBY
  * @param  None
  * @retval None
  */
static void EnterSTANDBYMode(void)
{
  RTC_AlarmTypeDef  RTC_AlarmStructure;
  RTC_TimeTypeDef   RTC_TimeStructure;

  /* Disable the Alarm A */
  RTC_AlarmCmd(RTC_Alarm_A, DISABLE);
  
  /* Get the current time */
  RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
  
  /* Set the alarm to current time + 3s */
  RTC_AlarmStructure.RTC_AlarmTime.RTC_H12     = RTC_TimeStructure.RTC_H12;
  RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours   = RTC_TimeStructure.RTC_Hours;
  RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes = RTC_TimeStructure.RTC_Minutes;
  RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds = (RTC_TimeStructure.RTC_Seconds + 0x3) % 60;
  RTC_AlarmStructure.RTC_AlarmDateWeekDay = 31;
  RTC_AlarmStructure.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_Date;
  RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay | RTC_AlarmMask_Hours | RTC_AlarmMask_Minutes;
  RTC_SetAlarm(RTC_Format_BIN, RTC_Alarm_A, &RTC_AlarmStructure);
  
  /* Enable RTC Alarm A Interrupt: this Interrupt will wake-up the system from
  STANDBY mode (RTC Alarm IT not enabled in NVIC) */
  RTC_ITConfig(RTC_IT_ALRA, ENABLE);
  
  /* Enable the Alarm A */
  RTC_AlarmCmd(RTC_Alarm_A, ENABLE);
  
  /* Clear RTC Alarm Flag */ 
  RTC_ClearFlag(RTC_FLAG_ALRAF);
  
  /* Request to enter STANDBY mode (Wake Up flag is cleared in PWR_EnterSTANDBYMode function) */
  PWR_EnterSTANDBYMode();
}

/**
  * @brief  Configures TIM14 to measure the LSI oscillator frequency. 
  * @param  None
  * @retval LSI Frequency
  */
static uint32_t GetLSIFrequency(void)
{
  NVIC_InitTypeDef   NVIC_InitStructure;
  TIM_ICInitTypeDef  TIM_ICInitStructure;
  RCC_ClocksTypeDef  RCC_ClockFreq;

  /* TIM14 configuration *******************************************************/ 
  /* Enable TIM14 clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);
  
  /* Reset TIM14 registers */
  TIM_DeInit(TIM14);

  /* Configure TIM14 prescaler */
  TIM_PrescalerConfig(TIM14, 0, TIM_PSCReloadMode_Immediate);

  /* Connect internally the TIM14_CH1 to the RTC clock output */
  TIM_RemapConfig(TIM14, TIM14_RTC_CLK);

  /* TIM14 configuration: Input Capture mode ---------------------
     The reference clock(LSE or external) is connected to TIM14 CH1
     The Rising edge is used as active edge,
     The TIM14 CCR1 is used to compute the frequency value 
  ------------------------------------------------------------ */
  TIM_ICInitStructure.TIM_Channel     = TIM_Channel_1;
  TIM_ICInitStructure.TIM_ICPolarity  = TIM_ICPolarity_Rising;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV8;
  TIM_ICInitStructure.TIM_ICFilter = 0x0;
  TIM_ICInit(TIM14, &TIM_ICInitStructure);

  /* Enable the TIM14 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM14_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Enable TIM14 counter */
  TIM_Cmd(TIM14, ENABLE);

  /* Reset the flags */
  TIM14->SR = 0;
    
  /* Enable the CC1 Interrupt Request */  
  TIM_ITConfig(TIM14, TIM_IT_CC1, ENABLE);


  /* Wait until the TIM14 get 2 LSI edges (refer to TIM14_IRQHandler() in 
    stm32F0xx_it.c file) ******************************************************/
  while(CaptureNumber != 2)
  {
  }
  /* Deinitialize the TIM14 peripheral registers to their default reset values */
  TIM_DeInit(TIM14);


  /* Compute the LSI frequency, depending on TIM14 input clock frequency (PCLK1)*/
  /* Get SYSCLK, HCLK and PCLKx frequency */
  RCC_GetClocksFreq(&RCC_ClockFreq);
  
  /* PCLK1 prescaler equal to 1 => TIMCLK = PCLK1 */
  return ((RCC_ClockFreq.PCLK_Frequency / PeriodValue) * 8);
}

/**
  * @brief  Configures the SysTick to generate an interrupt each 250 ms.
  * @param  None
  * @retval None
  */
void SysTick_Configuration(void)
{
  /* SysTick interrupt each 250 ms */
  if (SysTick_Config((SystemCoreClock/8) / 4))
  { 
    /* Capture error */ 
    while (1);
  }

  /* Select AHB clock(HCLK) divided by 8 as SysTick clock source */
  SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);

  /* Set SysTick Preemption Priority to 1 */
  NVIC_SetPriority(SysTick_IRQn, 0x04);
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
