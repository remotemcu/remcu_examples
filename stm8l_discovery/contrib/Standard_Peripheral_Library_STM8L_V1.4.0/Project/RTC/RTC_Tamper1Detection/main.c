/**
  ******************************************************************************
  * @file    RTC/RTC_Tamper1Detection/main.c
  * @author  MCD Application Team
  * @version V1.5.2
  * @date    30-September-2014
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

/** @addtogroup STM8L15x_StdPeriph_Examples
  * @{
  */

/** @addtogroup RTC_Tamper1Detection
  * @{
  */
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
RTC_InitTypeDef   RTC_InitStr;
RTC_TimeTypeDef   RTC_TimeStr;


__IO uint8_t LCDTamperTime[16]        = "TMP IT after Xs ";
__IO uint8_t LCDStringTime[16]        = "00:000   00:000 "; /* [Tamper Event Time] [Tamper IT Time] */


CONST uint8_t RTC_TampFilter[4] = {RTC_TamperFilter_1Sample, RTC_TamperFilter_2Sample,
                                     RTC_TamperFilter_4Sample, RTC_TamperFilter_8Sample};

__IO uint8_t TampFilterIndex = 1;


uint16_t Mstime = 0, Subsecondvalue = 0;
uint8_t   Ms100 = 0,  Ms10  = 0,  Ms1 = 0;

/* Private function prototypes ------ -----------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void LSE_StabTime(void);
/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void main(void)
{
  /* Enable LSE */
  CLK_LSEConfig(CLK_LSE_ON);
  /* Wait for LSE clock to be ready */
  while (CLK_GetFlagStatus(CLK_FLAG_LSERDY) == RESET);

  /* wait for 1 second for the LSE Stabilisation */
  LSE_StabTime();

  /* Select LSE (32.768 KHz) as RTC clock source */
  CLK_RTCClockConfig(CLK_RTCCLKSource_LSE, CLK_RTCCLKDiv_1);

  CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE);

  /* Calendar Configuration */
  Calendar_Init();

  /*RTC Tamper Configuration*/
  Tamper_Init();

  /* EvalBoard Configuration */
  EvalBoard_Init();

  Tamper_DelayShow(TampFilterIndex);

  while (1)
  {
    EnterSafeCode();
    Time_Show(LCD_LINE2, SHOW_POINT2);
    ExitSafeCode();
  }
}

/**
  * @brief  Calendar Configuration.
  * @param  None
  * @retval None
  */
void Calendar_Init(void)
{
  RTC_InitStr.RTC_HourFormat = RTC_HourFormat_24;
  RTC_InitStr.RTC_AsynchPrediv = 0x1F;
  RTC_InitStr.RTC_SynchPrediv = 0x03FF;
  RTC_Init(&RTC_InitStr);

}
/**
  * @brief  RTC Tamper Configuration.
  * @param  None
  * @retval None
  */
void Tamper_Init(void)
{
  RTC_TamperFilterConfig((RTC_TamperFilter_TypeDef)RTC_TampFilter[TampFilterIndex]);
  RTC_TamperSamplingFreqConfig(RTC_TamperSamplingFreq_RTCCLK_Div32768);

  RTC_TamperPinsPrechargeDuration(RTC_TamperPrechargeDuration_None);
  RTC_TamperLevelConfig(RTC_Tamper_1, RTC_TamperLevel_Low );
  RTC_TamperCmd(RTC_Tamper_1, ENABLE);
  RTC_ITConfig(RTC_IT_TAMP, ENABLE);
}
/**
  * @brief  Eval Board IO Configuration.
  * @param  None
  * @retval None
  */
void EvalBoard_Init(void)
{
  /* Init the Eval board LCD */
  STM8_EVAL_LCD_Init();
  LCD_Clear();

  /* Initialize push-buttons mounted on STM8L152x-EVAL board */
  STM_EVAL_PBInit(BUTTON_KEY, BUTTON_MODE_EXTI); /* used to decrease Tamper */

  STM_EVAL_PBInit(BUTTON_DOWN, BUTTON_MODE_EXTI); /* used to decrease Tamper time */
  STM_EVAL_PBInit(BUTTON_UP, BUTTON_MODE_EXTI);   /* used to increase Tamper time */

  /* Initialize LEDs mounted on STM8L152x-EVAL board */
  STM_EVAL_LEDInit(LED1);

  STM_EVAL_LEDInit(LED3);
}


/**
  * @brief  Displays the current Time on the LCD.
  * @param  None
  * @retval None
  */

void Time_Show(uint8_t Line, uint8_t pos)
{
  /* Wait until the calendar is synchronized */
  while (RTC_WaitForSynchro() != SUCCESS);

  /* Get the current subsecond Time*/
  Subsecondvalue = RTC_GetSubSecond();

  /* Wait until the calendar is synchronized */
  while (RTC_WaitForSynchro() != SUCCESS);
  /* Get the current Time*/
  RTC_GetTime(RTC_Format_BCD, &RTC_TimeStr);

  Mstime = 999 - ((uint32_t)((uint32_t)Subsecondvalue * 1000) / (uint32_t)RTC_InitStr.RTC_SynchPrediv);

  Ms100 = (uint8_t)(Mstime / 100);
  Ms10  = (uint8_t)((Mstime % 100 ) / 10);
  Ms1  =  (uint8_t)(Mstime % 10);


  /* Fill the LCDString fields with the current Time : second and milliseconds*/

  LCDStringTime[pos] = (uint8_t)(((uint8_t)(RTC_TimeStr.RTC_Seconds & 0xF0) >> 4) + ASCII_NUM_0);
  LCDStringTime[pos+1] = (uint8_t)(((uint8_t)(RTC_TimeStr.RTC_Seconds & 0x0F)) + ASCII_NUM_0);

  LCDStringTime[pos+3] = (uint8_t)((uint8_t)(Ms100 + ASCII_NUM_0));
  LCDStringTime[pos+4] = (uint8_t)((uint8_t)(Ms10 + ASCII_NUM_0));
  LCDStringTime[pos+5] = (uint8_t)((uint8_t)(Ms1 + ASCII_NUM_0));

  /* Print the Time Calendar on the LCD*/
  LCD_SetCursorPos(Line, 0);
  LCD_Print((uint8_t*)LCDStringTime);
}


/**
  * @brief  Tamper delay show function.
  * @param  None.
  * @retval None.
  */
void Tamper_DelayShow(uint8_t DelayId)
{
  LCDTamperTime[CHARPOS] = ((uint8_t)(1 << DelayId) - 1) + ASCII_NUM_0;
  LCD_SetCursorPos(LCD_LINE1, 0);
  LCD_Print((uint8_t*)LCDTamperTime);
}


/**
  * @brief  Wait 1 sec for LSE stabilisation .
  * @param  None.
  * @retval None.
  * Note : TIM4 is configured for a system clock = 2MHz
  */
void LSE_StabTime(void)
{
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, ENABLE);

  /* Configure TIM4 to generate an update event each 1 s */
  TIM4_TimeBaseInit(TIM4_Prescaler_16384, 123);

  /* Enable TIM4 */
  TIM4_Cmd(ENABLE);

  /* Wait 1 sec */
  while ( TIM4_GetFlagStatus(TIM4_FLAG_Update) == RESET );

  TIM4_ClearFlag(TIM4_FLAG_Update);

  /* Disable TIM4 */
  TIM4_Cmd(DISABLE);

  CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, DISABLE);
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
  {}
}


#endif

/**
  * @}
  */
/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
