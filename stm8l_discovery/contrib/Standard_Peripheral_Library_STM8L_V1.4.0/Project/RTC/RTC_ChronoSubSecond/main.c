/**
  ******************************************************************************
  * @file    RTC/RTC_ChronoSubSecond/main.c
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

/** @addtogroup RTC_ChronoSubSecond
  * @{
  */
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
RTC_InitTypeDef   RTC_InitStr;
RTC_TimeTypeDef   RTC_TimeStr;
RTC_TimeTypeDef   RTC_SaveTimeStr[MAXSAVETIME];


__IO uint8_t LCDStringTime[16]       = "  00:00:00:000 ";
__IO uint8_t TempBuffer[16];

uint16_t mstime = 0, subsecondvalue = 0, SaveSubsecond[MAXSAVETIME];
uint8_t   ms100 = 0,  ms10  = 0,  ms1 = 0;

BitStatus PauseStatus = RESET;
uint8_t SaveId = 0;
uint8_t SaveParsingId = 0;
uint8_t LastDisplay = 'D';
JOYState_TypeDef Key;

/* Private function prototypes ------ -----------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void Delay_Seconds(uint8_t Seconds);
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
  Delay_Seconds(1);

  /* Select LSE (32.768 KHz) as RTC clock source */
  CLK_RTCClockConfig(CLK_RTCCLKSource_LSE, CLK_RTCCLKDiv_1);
  CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE);

  /*RTC Tamper Configuration*/
  Tamper_Init();

  /* Calendar Configuration */
  Calendar_Init();

  /* EvalBoard Configuration */
  EvalBoard_Init();

  /* RTC Time fields reset*/
  Time_Reset();

  /* RTC Time pause waiting for a press on SEL Key */
  Time_Pause();

  while (1)
  {
    EnterSafeCode();
    Time_Show(LCD_LINE2);
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
  /* Tamper detection after time < 2 sample and time > 1 sample*/
  RTC_TamperFilterConfig(RTC_TamperFilter_2Sample);

  /* configure 1 Sample = RTCCLK/32768 = 1s */
  RTC_TamperSamplingFreqConfig(RTC_TamperSamplingFreq_RTCCLK_Div32768);

  /* Tamper sensitivity is to Low level */
  RTC_TamperLevelConfig(RTC_Tamper_1, RTC_TamperLevel_Low );

  /* Enable Tamper 1*/
  RTC_TamperCmd(RTC_Tamper_1, ENABLE);

  /* Enable Tamper Interrupt */
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

  LCD_SetCursorPos(LCD_LINE1, 0);
  LCD_Print("  Chronometer  ");
  LCD_SetCursorPos(LCD_LINE2, 0);
  LCD_Print("   using RTC   ");

  /* Initialize push-buttons mounted on STM8L1528-EVAL board */
  STM_EVAL_PBInit(BUTTON_LEFT, BUTTON_MODE_GPIO);   /* For Saved Time parsing */
  STM_EVAL_PBInit(BUTTON_RIGHT, BUTTON_MODE_GPIO);  /* For Saved Time parsing */
  STM_EVAL_PBInit(BUTTON_KEY, BUTTON_MODE_EXTI);    /* For recording Time     */
  STM_EVAL_PBInit(BUTTON_SEL, BUTTON_MODE_EXTI);    /* For Time Pause/Resume  */
  STM_EVAL_PBInit(BUTTON_DOWN, BUTTON_MODE_EXTI);   /* To enter to Saved Times*/
  STM_EVAL_PBInit(BUTTON_UP, BUTTON_MODE_GPIO);     /* To exit from Saved Times*/
  STM_EVAL_PBInit(BUTTON_TAMPER, BUTTON_MODE_GPIO); /* To Reset Chrono after 2s */

  /* Initialize LEDs mounted on STM8L1528-EVAL board */
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);

  STM_EVAL_LEDOn(LED1);

  /* wait 1 second */
  Delay_Seconds(1);

  STM_EVAL_LEDOn(LED2);

  /* wait 1 second */
  Delay_Seconds(1);

  STM_EVAL_LEDOn(LED3);

  /* wait 1 second */
  Delay_Seconds(1);

  STM_EVAL_LEDOn(LED4);

  /* wait 1 second */
  Delay_Seconds(1);

  STM_EVAL_LEDOff(LED1);
  STM_EVAL_LEDOff(LED2);
  STM_EVAL_LEDOff(LED3);
  STM_EVAL_LEDOff(LED4);

  LCD_SetCursorPos(LCD_LINE1, 0);
  LCD_Print(DEFAULTDISPLAY);
}


/**
  * @brief  Displays the Saved Time on the LCD.
  * @param  None
  * @retval None
  */

void SavedTime_Show(uint8_t Line, uint8_t SavedTimeId)
{
  mstime = 1000 - ((uint32_t)((uint32_t)SaveSubsecond[SavedTimeId] * 1000) / (uint32_t)RTC_InitStr.RTC_SynchPrediv);
  ms100 = (uint8_t)(mstime / 100);
  ms10  = (uint8_t)((mstime % 100 ) / 10);
  ms1  =  (uint8_t)(mstime % 10);

  LCDStringTime[0] = (uint8_t)((uint8_t)( SavedTimeId + 1 + ASCII_NUM_0));
  LCDStringTime[1] = '-';

  /* Fill the LCDString fields with the current Time*/
  LCDStringTime[SHOW_POINT] = (uint8_t)(((uint8_t)(RTC_SaveTimeStr[SavedTimeId].RTC_Hours & 0xF0) >> 4) + ASCII_NUM_0);
  LCDStringTime[SHOW_POINT+1] = (uint8_t)(((uint8_t)(RTC_SaveTimeStr[SavedTimeId].RTC_Hours & 0x0F)) + ASCII_NUM_0);

  LCDStringTime[SHOW_POINT+3] = (uint8_t)(((uint8_t)(RTC_SaveTimeStr[SavedTimeId].RTC_Minutes & 0xF0) >> 4) + ASCII_NUM_0);
  LCDStringTime[SHOW_POINT+4] = (uint8_t)(((uint8_t)(RTC_SaveTimeStr[SavedTimeId].RTC_Minutes & 0x0F)) + (uint8_t)ASCII_NUM_0);

  LCDStringTime[SHOW_POINT+6] = (uint8_t)(((uint8_t)(RTC_SaveTimeStr[SavedTimeId].RTC_Seconds & 0xF0) >> 4) + ASCII_NUM_0);
  LCDStringTime[SHOW_POINT+7] = (uint8_t)(((uint8_t)(RTC_SaveTimeStr[SavedTimeId].RTC_Seconds & 0x0F)) + ASCII_NUM_0);

  LCDStringTime[SHOW_POINT+9] = (uint8_t)((uint8_t)(ms100 + ASCII_NUM_0));
  LCDStringTime[SHOW_POINT+10] = (uint8_t)((uint8_t)(ms10 + ASCII_NUM_0));
  LCDStringTime[SHOW_POINT+11] = (uint8_t)((uint8_t)(ms1 + ASCII_NUM_0));

  /* Print the Time Calendar on the LCD*/
  LCD_SetCursorPos(Line, 0);
  LCD_Print((uint8_t *)LCDStringTime);

  /* Restore String */
  LCDStringTime[0] = ' ';
  LCDStringTime[1] = ' ';
}

/**
  * @brief  Parse Saved Times.
  * @param  None
  * @retval None
  */
void SavedTime_Parsing(void)
{
  if ( SaveId != 0)
  {
    SaveParsingId = 1;
    LCD_SetCursorPos(LCD_LINE1, 0);
    LCD_Print("< Rec. Times  >");

    SavedTime_Show(LCD_LINE2, SaveParsingId - 1);

    /* Endless loop */
    while (1)
    {
      /* Check which key is pressed */
      Key = ReadJoystick();

      /* If "RIGHT" pushbutton is pressed */
      if (Key == JOY_RIGHT)
      {
        SaveParsingId++;

        if (SaveParsingId  <= SaveId)
        {
          SavedTime_Show(LCD_LINE2, SaveParsingId - 1);
        }
        else
        {
          SaveParsingId = SaveId;
        }
      }

      /* If "LEFT" pushbutton is pressed */
      if (Key == JOY_LEFT)
      {
        SaveParsingId--;
        if (SaveParsingId >= 1 )
        {
          SavedTime_Show(LCD_LINE2, SaveParsingId - 1);
        }
        else
        {
          SaveParsingId = 1;
        }
      }

      /* If "UP" pushbutton is pressed */
      if (Key == JOY_UP)
      {
        Restore_LastDisplay() ;
        /* Exit */
        return ;
      }

    }
  }
  else
  {
    LCD_SetCursorPos(LCD_LINE1, 0);
    LCD_Print("   No Times     ");

    LCD_SetCursorPos(LCD_LINE2, 0);
    LCD_Print(" are Recorded    ");
    Delay(0x1FFFF);
    Restore_LastDisplay();
  }


}


/**
  * @brief  Parse Saved Times.
  * @param  None
  * @retval None
  */
void SavedTime_Erase(void)
{
  if ( SaveId != 0)
  {


    LCD_SetCursorPos(LCD_LINE1, 0);
    LCD_Print("Erase Records ?");
    LCD_SetCursorPos(LCD_LINE2, 0);
    LCD_Print("<No        Yes>");

    /* Endless loop */
    while (1)
    {
      /* Check which key is pressed */
      Key = ReadJoystick();

      /* If "RIGHT" pushbutton is pressed */
      if (Key == JOY_RIGHT)
      {
        SaveId = 0;
        Restore_LastDisplay() ;
        /* Exit */
        return ;
      }

      /* If "LEFT" pushbutton is pressed */
      if (Key == JOY_LEFT)
      {
        Restore_LastDisplay() ;
        /* Exit */
        return ;
      }

    }
  }
}

/**
  * @brief  Displays the current Time on the LCD.
  * @param  None
  * @retval None
  */

void Time_Show(uint8_t Line)
{
  /* Wait until the calendar is synchronized */
  while (RTC_WaitForSynchro() != SUCCESS);

  /* Get the current subsecond Time*/
  subsecondvalue = RTC_GetSubSecond();

  /* Wait until the calendar is synchronized */
  while (RTC_WaitForSynchro() != SUCCESS);

  /* Get the current Time*/
  RTC_GetTime(RTC_Format_BCD, &RTC_TimeStr);

  mstime = 1000 - ((uint32_t)((uint32_t)subsecondvalue * 1000) / (uint32_t)RTC_InitStr.RTC_SynchPrediv);


  ms100 = (uint8_t)(mstime / 100);
  ms10  = (uint8_t)((mstime % 100 ) / 10);
  ms1  =  (uint8_t)(mstime % 10);


  /* Fill the LCDString fields with the current Time*/
  LCDStringTime[SHOW_POINT] = (uint8_t)(((uint8_t)(RTC_TimeStr.RTC_Hours & 0xF0) >> 4) + ASCII_NUM_0);
  LCDStringTime[SHOW_POINT+1] = (uint8_t)(((uint8_t)(RTC_TimeStr.RTC_Hours & 0x0F)) + ASCII_NUM_0);

  LCDStringTime[SHOW_POINT+3] = (uint8_t)(((uint8_t)(RTC_TimeStr.RTC_Minutes & 0xF0) >> 4) + ASCII_NUM_0);
  LCDStringTime[SHOW_POINT+4] = (uint8_t)(((uint8_t)(RTC_TimeStr.RTC_Minutes & 0x0F)) + (uint8_t)ASCII_NUM_0);

  LCDStringTime[SHOW_POINT+6] = (uint8_t)(((uint8_t)(RTC_TimeStr.RTC_Seconds & 0xF0) >> 4) + ASCII_NUM_0);
  LCDStringTime[SHOW_POINT+7] = (uint8_t)(((uint8_t)(RTC_TimeStr.RTC_Seconds & 0x0F)) + ASCII_NUM_0);

  LCDStringTime[SHOW_POINT+9] = (uint8_t)((uint8_t)(ms100 + ASCII_NUM_0));
  LCDStringTime[SHOW_POINT+10] = (uint8_t)((uint8_t)(ms10 + ASCII_NUM_0));
  LCDStringTime[SHOW_POINT+11] = (uint8_t)((uint8_t)(ms1 + ASCII_NUM_0));

  /* Print the Time Calendar on the LCD*/
  LCD_SetCursorPos(Line, 0);
  LCD_Print((uint8_t *)LCDStringTime);
}

/**
  * @brief  Save current Time on the correspondent structure.
  * @param  None
  * @retval None
  */

void Time_SaveCurrentTime(uint8_t Id)
{
  /* Wait until the calendar is synchronized */
  while (RTC_WaitForSynchro() != SUCCESS);

  /* Get the current subsecond Time*/
  SaveSubsecond[Id] = RTC_GetSubSecond();

  /* Wait until the calendar is synchronized */
  while (RTC_WaitForSynchro() != SUCCESS);

  /* Get the current Time*/
  RTC_GetTime(RTC_Format_BCD, &RTC_SaveTimeStr[Id]);
}


/**
  * @brief  Record Time in memory.
  * @param  None
  * @retval None
  */
void Time_Record(void)
{
  if (SaveId < MAXSAVETIME)
  {
    STM_EVAL_LEDOn(LED3);
    LCD_SetCursorPos(LCD_LINE1, 0);
    LCD_Print(" Record Done  ");
    Time_SaveCurrentTime(SaveId);
    SavedTime_Show(LCD_LINE2, SaveId);
    SaveId++;
    Delay(0x3FFF) ;
    STM_EVAL_LEDOff(LED3);

  }
  else
  {
    LCD_SetCursorPos(LCD_LINE1, 0);
    LCD_Print("   Recordable   ");
    LCD_SetCursorPos(LCD_LINE2, 0);
    LCD_Print("  Mem is Full  ");
    Delay(0x17FFF) ;

  }

  Restore_LastDisplay() ;
}
/**
  * @brief  Reset Chrono to zero.
  * @param  None.
  * @retval None.
  */
void Time_ResetMenu(void)
{
  LCD_SetCursorPos(LCD_LINE1, 0);
  LCD_Print(" Reset Chrono ? ");
  LCD_SetCursorPos(LCD_LINE2, 0);
  LCD_Print("<No        Yes>");

  /* Endless loop */
  while (1)
  {
    /* Check which key is pressed */
    Key = ReadJoystick();

    /* If "RIGHT" pushbutton is pressed */
    if (Key == JOY_RIGHT)
    {
      Time_Reset();

      Time_Pause();

      Restore_LastDisplay() ;
      /* Exit */
      return ;
    }

    /* If "LEFT" pushbutton is pressed */
    if (Key == JOY_LEFT)
    {
      Restore_LastDisplay() ;
      /* Exit */
      return ;
    }

  }
}

/**
  * @brief  Reset Chrono to zero.
  * @param  None.
  * @retval None.
  */
void Time_Reset(void)
{
  PauseStatus = RESET;

  RTC_TimeStr.RTC_H12     = RTC_H12_AM;
  RTC_TimeStr.RTC_Hours   = 00;
  RTC_TimeStr.RTC_Minutes = 00;
  RTC_TimeStr.RTC_Seconds = 00;

  RTC_SetTime(RTC_Format_BIN, &RTC_TimeStr);
  CLK_RTCClockConfig(CLK_RTCCLKSource_Off, CLK_RTCCLKDiv_1);


}
/**
  * @brief  Pause Chronometer
  * @param  None.
  * @retval None.
  */
void Time_Pause(void)
{
  Time_Show(LCD_LINE2);
  LCD_SetCursorPos(LCD_LINE1, 0);

  if (PauseStatus == RESET)
  {
    STM_EVAL_LEDOn(LED2);
    LCD_Print(RESETDISPLAY);
    LastDisplay = 'P';
    CLK_RTCClockConfig(CLK_RTCCLKSource_Off, CLK_RTCCLKDiv_1);
    STM_EVAL_LEDOff(LED1);
  }
  else
  {
    STM_EVAL_LEDOn(LED1);
    LCD_Print(DEFAULTDISPLAY);
    LastDisplay = 'D';
    CLK_RTCClockConfig(CLK_RTCCLKSource_LSE, CLK_RTCCLKDiv_1);
    STM_EVAL_LEDOff(LED2);
  }

  /* Invert Pause Status */
  PauseStatus = (BitStatus)(~PauseStatus);
}


/**
  * @brief  Restore Last Display
  * @param  None.
  * @retval None.
  */
void Restore_LastDisplay(void)
{
  LCD_SetCursorPos(LCD_LINE1, 0);
  if (LastDisplay == 'D')      LCD_Print(DEFAULTDISPLAY);
  else if (LastDisplay == 'P') LCD_Print(RESETDISPLAY);

}

/**
  * @brief  Reads joystick position from evalboard.
  * @param  None.
  * @retval Joystick position.
  */
JOYState_TypeDef ReadJoystick(void)
{
  /* "right" key is pressed */
  if (!STM_EVAL_PBGetState(BUTTON_RIGHT))
  {
    while (STM_EVAL_PBGetState(BUTTON_RIGHT) == 0);
    return JOY_RIGHT;
  }
  /* "left" key is pressed */
  if (!STM_EVAL_PBGetState(BUTTON_LEFT))
  {
    while (STM_EVAL_PBGetState(BUTTON_LEFT) == 0);
    return JOY_LEFT;
  }
  /* "up" key is pressed */
  if (!STM_EVAL_PBGetState(BUTTON_UP))
  {
    while (STM_EVAL_PBGetState(BUTTON_UP) == 0);
    return JOY_UP;
  }
  /* "down" key is pressed */
  if (!STM_EVAL_PBGetState(BUTTON_DOWN))
  {
    while (STM_EVAL_PBGetState(BUTTON_DOWN) == 0);
    return JOY_DOWN;
  }
  /* "sel" key is pressed */
  if (!STM_EVAL_PBGetState(BUTTON_SEL))
  {
    while (STM_EVAL_PBGetState(BUTTON_SEL) == 0);
    return JOY_SEL;
  }
  /* No key is pressed */
  else
  {
    return JOY_NONE;
  }
}

/**
  * @brief  Delay x sec
  * @param  Seconds : number of seconds to delay
  * @retval None.
  * Note : TIM4 is configured for a system clock = 2MHz
  */
void Delay_Seconds(uint8_t Seconds)
{
  uint8_t i = 0;

  /* Enable TIM4 Clock */
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, ENABLE);

  /* Configure TIM4 to generate an update event each 1 s */
  TIM4_TimeBaseInit(TIM4_Prescaler_16384, 123);

  /* Enable TIM4 */
  TIM4_Cmd(ENABLE);

  /* Clear the Flag */
  TIM4_ClearFlag(TIM4_FLAG_Update);

  for (i = 0; i < Seconds; i++)
  {
    /* Wait 1 sec */
    while ( TIM4_GetFlagStatus(TIM4_FLAG_Update) == RESET )
    {}

    /* Clear the Flag */
    TIM4_ClearFlag(TIM4_FLAG_Update);
  }

  /* Disable TIM4 */
  TIM4_Cmd(DISABLE);

  /* Disable TIM4 Clock */
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, DISABLE);
}



/**
  * @brief  Inserts a delay time.
  * @param  nCount: specifies the delay time length.
  * @retval None
  */
void Delay(__IO uint32_t nCount)
{
  for (; nCount != 0; nCount--);
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
    LCD_SetCursorPos(LCD_LINE1, 0);
    LCD_Print("      ERR       ");
    LCD_SetCursorPos(LCD_LINE2, 0);
    LCD_Print("     ASSERT     ");
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
