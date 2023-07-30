/**
  ******************************************************************************
  * @file    RTC/RTC_Calendar/main.c
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

/** @addtogroup RTC_Calendar
  * @{
  */
/* Private typedef -----------------------------------------------------------*/
typedef uint8_t* string;
/* Private define ------------------------------------------------------------*/
#define ASCII_NUM_0         ((uint8_t)48)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
RTC_InitTypeDef   RTC_InitStr;
RTC_TimeTypeDef   RTC_TimeStr;
RTC_DateTypeDef   RTC_DateStr;
RTC_AlarmTypeDef  RTC_AlarmStr;

__IO uint8_t LCDStringTime[16]       = "    00:00:00    ";
__IO uint8_t LCDStringDate[16]       = "   06/21/2013   ";
__IO uint8_t LCDStringAlarm[16]      = "    00:00:00    ";

__IO uint8_t TempBuffer[16];
/* Days names definition */
CONST string WeekDayNames[7] =
  {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT" };

/* Months names definition */
CONST   uint8_t MonthNames[] = {'J', 'A', 'N', 'F', 'E', 'B', 'M', 'A', 'R', 'A', 'P', 'R',
                                  'M', 'A', 'Y', 'J', 'U', 'N', 'J', 'U', 'L', 'A', 'U', 'G',
                                  'S', 'E', 'P', 'O', 'C', 'T', 'N', 'O', 'V', 'D', 'E', 'C'};

__IO bool AlarmOccurred = FALSE;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void main(void)
{
  uint8_t index = 0;

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

  /* Init the Eval board LCD */
  STM8_EVAL_LCD_Init();
  LCD_Clear();

  /* Initialize push-buttons mounted on STM8L1526-EVAL board */
  STM_EVAL_PBInit(BUTTON_LEFT, BUTTON_MODE_EXTI);  /* used for Date Adjust */
  STM_EVAL_PBInit(BUTTON_RIGHT, BUTTON_MODE_EXTI); /* used for Time Adjust */
  STM_EVAL_PBInit(BUTTON_KEY, BUTTON_MODE_EXTI);   /* used for Alarm Adjust */
  STM_EVAL_PBInit(BUTTON_SEL, BUTTON_MODE_GPIO);
  STM_EVAL_PBInit(BUTTON_DOWN, BUTTON_MODE_GPIO);
  STM_EVAL_PBInit(BUTTON_UP, BUTTON_MODE_GPIO);

  /* Initialize LEDs mounted on STM8L1526-EVAL board */
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);

  while (1)
  {
    disableInterrupts();
    Time_Show(LCD_LINE2);
    Date_Show();
    enableInterrupts();

    if (AlarmOccurred != FALSE )
    {
      LCD_SetCursorPos(LCD_LINE1, 0);
      LCD_Print(" !!! Alarm !!! ");
      Time_Show(LCD_LINE2);

      for (index = 32; index != 0; index--)
      {
        STM_EVAL_LEDToggle(LED1);
        STM_EVAL_LEDToggle(LED2);
        STM_EVAL_LEDToggle(LED3);
        STM_EVAL_LEDToggle(LED4);
        Delay(4000);
      }
      AlarmOccurred = FALSE;
    }
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
  RTC_InitStr.RTC_AsynchPrediv = 0x7F;
  RTC_InitStr.RTC_SynchPrediv = 0x00FF;
  RTC_Init(&RTC_InitStr);

  RTC_DateStructInit(&RTC_DateStr);
  RTC_DateStr.RTC_WeekDay = RTC_Weekday_Friday;
  RTC_DateStr.RTC_Date = 13;
  RTC_DateStr.RTC_Month = RTC_Month_May;
  RTC_DateStr.RTC_Year = 11;
  RTC_SetDate(RTC_Format_BIN, &RTC_DateStr);

  RTC_TimeStructInit(&RTC_TimeStr);
  RTC_TimeStr.RTC_Hours   = 01;
  RTC_TimeStr.RTC_Minutes = 00;
  RTC_TimeStr.RTC_Seconds = 00;
  RTC_SetTime(RTC_Format_BIN, &RTC_TimeStr);


  RTC_AlarmStructInit(&RTC_AlarmStr);
  RTC_AlarmStr.RTC_AlarmTime.RTC_Hours   = 01;
  RTC_AlarmStr.RTC_AlarmTime.RTC_Minutes = 01;
  RTC_AlarmStr.RTC_AlarmTime.RTC_Seconds = 00;
  RTC_AlarmStr.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay;
  RTC_SetAlarm(RTC_Format_BIN, &RTC_AlarmStr);

  RTC_ITConfig(RTC_IT_ALRA, ENABLE);
  RTC_AlarmCmd(ENABLE);
}

/**
  * @brief  Returns the time entered by user.
  * @param  None
  * @retval None
  */
void Time_Regulate(void)
{
  uint8_t tmphh = 0, tmpmm = 0, tmpss = 0;

  /* Read time hours */
  tmphh = ReadDigit((uint8_t*)LCDStringTime, 4, 0x2, 0x0);
  if (tmphh == 2)
  {
    if (LCDStringTime[5] > 0x03 + 0x30)
    {
      LCDStringTime[5] = 0 + 0x30;
    }
    tmphh = tmphh * 10 + ReadDigit((uint8_t*)LCDStringTime, 5,  0x3, 0x0);
  }
  else
  {
    tmphh = tmphh * 10 + ReadDigit((uint8_t*)LCDStringTime, 5, 0x9, 0x0);
  }

  /* Read time  minutes */
  tmpmm = ReadDigit((uint8_t*)LCDStringTime, 7, 5, 0x0);
  tmpmm = tmpmm * 10 + ReadDigit((uint8_t*)LCDStringTime, 8, 0x9, 0x0);
  /* Read time seconds */
  tmpss = ReadDigit((uint8_t*)LCDStringTime, 10, 5, 0x0);
  tmpss = tmpss * 10 + ReadDigit((uint8_t*)LCDStringTime, 11, 0x9, 0x0);

  /* Set the Time */
  RTC_TimeStr.RTC_Hours   = tmphh;
  RTC_TimeStr.RTC_Minutes = tmpmm;
  RTC_TimeStr.RTC_Seconds = tmpss;
  RTC_SetTime(RTC_Format_BIN, &RTC_TimeStr);
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

  /* Get the current Time*/
  RTC_GetTime(RTC_Format_BCD, &RTC_TimeStr);

  /* Fill the LCDString fields with the current Time*/
  LCDStringTime[4] = (uint8_t)(((uint8_t)(RTC_TimeStr.RTC_Hours & 0xF0) >> 4) + ASCII_NUM_0);
  LCDStringTime[5] = (uint8_t)(((uint8_t)(RTC_TimeStr.RTC_Hours & 0x0F)) + ASCII_NUM_0);

  LCDStringTime[7] = (uint8_t)(((uint8_t)(RTC_TimeStr.RTC_Minutes & 0xF0) >> 4) + ASCII_NUM_0);
  LCDStringTime[8] = (uint8_t)(((uint8_t)(RTC_TimeStr.RTC_Minutes & 0x0F)) + (uint8_t)ASCII_NUM_0);

  LCDStringTime[10] = (uint8_t)(((uint8_t)(RTC_TimeStr.RTC_Seconds & 0xF0) >> 4) + ASCII_NUM_0);
  LCDStringTime[11] = (uint8_t)(((uint8_t)(RTC_TimeStr.RTC_Seconds & 0x0F)) + ASCII_NUM_0);

  /* Print the Time Calendar on the LCD*/
  LCD_SetCursorPos(Line, 0);
  LCD_Print((uint8_t*)LCDStringTime);
}

/**
  * @brief  Returns the Date entered by user.
  * @param  None
  * @retval None
  */
void Date_Regulate(void)
{
  uint8_t tmpmonth = 0, tmpday = 0, tmp = 0;

  uint16_t tmpyear = 0;

  /* Wait until the calendar is synchronized */
  while (RTC_WaitForSynchro() != SUCCESS);

  /* Get the current Date */
  RTC_GetDate(RTC_Format_BCD, &RTC_DateStr);

  LCDStringDate[3] = (uint8_t) (((uint8_t)(RTC_DateStr.RTC_Month & 0xF0) >> 4) + ASCII_NUM_0);
  LCDStringDate[4] = (uint8_t) (((uint8_t)(RTC_DateStr.RTC_Month & 0x0F)) + ASCII_NUM_0);

  LCDStringDate[6] = (uint8_t) (((uint8_t)(RTC_DateStr.RTC_Date & 0xF0) >> 4) + ASCII_NUM_0);
  LCDStringDate[7] = (uint8_t) (((uint8_t)(RTC_DateStr.RTC_Date & 0x0F)) + ASCII_NUM_0);

  LCDStringDate[11] = (uint8_t) (((uint8_t)(RTC_DateStr.RTC_Year & 0xF0) >> 4) + ASCII_NUM_0);
  LCDStringDate[12] = (uint8_t) (((uint8_t)(RTC_DateStr.RTC_Year & 0x0F)) + ASCII_NUM_0);

  /* Print the Date Calendar on the LCD*/
  LCD_SetCursorPos(LCD_LINE2, 0);
  LCD_Print((uint8_t*)LCDStringDate);

  /* Read date months */
  tmpmonth = ReadDigit((uint8_t*)LCDStringDate, 3, 1, 0x0);
  tmp = RTC_DateStr.RTC_Month % 10;
  if (tmpmonth == 1)
  {
    if (tmp > 2)
    {
      LCDStringDate[4] = 0 + 0x30;
    }
    tmpmonth = tmpmonth * 10 + ReadDigit((uint8_t*)LCDStringDate, 4, 2, 0x0);
  }
  else
  {
    if (LCDStringDate[4] == 0x30)
    {
      LCDStringDate[4] = 1 + 0x30;
    }
    tmpmonth = tmpmonth * 10 + ReadDigit((uint8_t*)LCDStringDate, 4, 9, 0x1);
  }

  /* Read date days */
  if (tmpmonth == 2)
  {
    tmpday = ReadDigit((uint8_t*)LCDStringDate, 6, 2, 0x0);
  }
  else
  {
    tmpday = ReadDigit((uint8_t*)LCDStringDate, 6, 3, 0x0);
  }

  if (tmpday == 3)
  {
    tmp = RTC_DateStr.RTC_Date % 10;

    if (tmp >= 1)
    {
      LCDStringDate[7] = 0 + 0x30;
    }

    if (tmpmonth == 1 || tmpmonth == 3 || tmpmonth == 5 || tmpmonth == 7 ||
        tmpmonth == 8 || tmpmonth == 10 || tmpmonth == 12)
    {

      tmpday = tmpday * 10 + ReadDigit((uint8_t*)LCDStringDate, 7, 1, 0x0);
    }
    else
    {
      tmpday = tmpday * 10 + ReadDigit((uint8_t*)LCDStringDate, 7, 0, 0x0);
    }
  }
  else
  {
    if (tmpday == 0)
    {
      LCDStringDate[7] = 1 + 0x30;
      tmpday = tmpday * 10 + ReadDigit((uint8_t*)LCDStringDate, 7, 9, 0x1);
    }
    else
    {
      tmpday = tmpday * 10 + ReadDigit((uint8_t*)LCDStringDate, 7, 9, 0x0);
    }
  }

  /* Read date years */
  tmpyear = ReadDigit((uint8_t*)LCDStringDate, 11, 9, 0x0);
  tmpyear = tmpyear * 10 + ReadDigit((uint8_t*)LCDStringDate, 12, 9, 0x0);

  if (tmpmonth == 2 && tmpday == 29 && (tmpyear % 4) != 0)
  {
    tmpday = 28;
  }
  /* Return the date value to store in date structure */
  RTC_DateStr.RTC_Year = tmpyear;
  RTC_DateStr.RTC_Month = (RTC_Month_TypeDef)tmpmonth;
  RTC_DateStr.RTC_Date = tmpday;
  RTC_SetDate(RTC_Format_BIN, &RTC_DateStr);
}

/**
  * @brief  Displays the current Date on the LCD.
  * @param  None
  * @retval None
  */
void Date_Show(void)
{

  uint8_t tmpyear, tmpdate = 0;/* Get the current Date */

  /* Wait until the calendar is synchronized */
  while (RTC_WaitForSynchro() != SUCCESS);

  RTC_GetDate(RTC_Format_BIN, &RTC_DateStr);

  LCD_SetCursorPos(LCD_LINE1, 0);
  LCD_Print(WeekDayNames[DayOfWeek((RTC_DateStr.RTC_Year +2000), RTC_DateStr.RTC_Month,  RTC_DateStr.RTC_Date)]);

  LCD_Print(" ");
  sprintf((char*)TempBuffer, "%c", MonthNames[(((RTC_DateStr.RTC_Month) -1) * 3)]);
  LCD_Print((uint8_t*)TempBuffer);
  sprintf((char*)TempBuffer, "%c", MonthNames[(((RTC_DateStr.RTC_Month) -1) * 3)+1]);
  LCD_Print((uint8_t*)TempBuffer);
  sprintf((char*)TempBuffer, "%c", MonthNames[(((RTC_DateStr.RTC_Month) -1) * 3)+2]);
  LCD_Print((uint8_t*)TempBuffer);

  LCD_Print(" ");

  tmpdate = (RTC_DateStr.RTC_Date / 10) + 0x30;
  sprintf((char*)TempBuffer, "%c", tmpdate);
  LCD_Print((uint8_t*)TempBuffer);
  tmpdate = (RTC_DateStr.RTC_Date % 10) + 0x30;
  sprintf((char*)TempBuffer, "%c", tmpdate);
  LCD_Print((uint8_t*)TempBuffer);

  LCD_Print(" 20");

  tmpyear = ((RTC_DateStr.RTC_Year % 1000) % 100) / 10 + 0x30;
  sprintf((char*)TempBuffer, "%c", tmpyear);
  LCD_Print((uint8_t*)TempBuffer);
  tmpyear = ((RTC_DateStr.RTC_Year % 1000) % 10) % 10 + 0x30;
  sprintf((char*)TempBuffer, "%c", tmpyear);
  LCD_Print((uint8_t*)TempBuffer);

}

/**
  * @brief  Returns the Alarm entered by user..
  * @param  None
  * @retval None
  */
void Alarm_Regulate(void)
{
  uint8_t alarmhh = 0, alarmmm = 0, alarmss = 0;

  /* Read alarm hours */
  alarmhh = ReadDigit((uint8_t*)LCDStringAlarm, 4, 0x2, 0x0);

  if (alarmhh == 0x2)
  {
    if (LCDStringAlarm[5] > 0x03 + 0x30)
    {
      LCDStringAlarm[5] = 0 + 0x30;
    }
    alarmhh = alarmhh * 10 + ReadDigit((uint8_t*)LCDStringAlarm, 5, 0x3, 0x0);
  }
  else
  {
    alarmhh = alarmhh * 10 + ReadDigit((uint8_t*)LCDStringAlarm, 5, 0x9, 0x0);
  }

  /* Read alarm minutes */
  alarmmm = ReadDigit((uint8_t*)LCDStringAlarm, 7, 0x5, 0x0);
  alarmmm  = alarmmm * 10 + ReadDigit((uint8_t*)LCDStringAlarm, 8, 0x9, 0x0);

  /* Read alarm seconds */
  alarmss = ReadDigit((uint8_t*)LCDStringAlarm, 10, 0x5, 0x0);
  alarmss = alarmss * 10 + ReadDigit((uint8_t*)LCDStringAlarm, 11, 0x9, 0x0);

  /* Set the alarm */
  RTC_AlarmStr.RTC_AlarmTime.RTC_Hours   = alarmhh;
  RTC_AlarmStr.RTC_AlarmTime.RTC_Minutes = alarmmm;
  RTC_AlarmStr.RTC_AlarmTime.RTC_Seconds = alarmss;
  RTC_SetAlarm(RTC_Format_BIN, &RTC_AlarmStr);

  RTC_AlarmCmd(ENABLE);
}

/**
  * @brief  Reads digit entered by user, using Joystick buttons.
  * @param  None
  * @retval Digit value
  */
uint8_t ReadDigit(uint8_t* ptr, uint8_t ColBegin, uint8_t ValueMax, uint8_t ValueMin)
{
  __IO uint8_t tmpvalue, i = 0;
  JOYState_TypeDef Key;

  /* Initialize tmpvalue */
  tmpvalue = ptr[ColBegin];
  /* Display  */
  tmpvalue -= 0x30;
  SetCursorPosition(ptr, LCD_LINE2, ColBegin);
  sprintf((char*)TempBuffer, "%c", ptr[ColBegin]);
  LCD_Print((uint8_t*)TempBuffer);

  /* Endless loop */
  while (1)
  {
    /* Check which key is pressed */
    Key = ReadJoystick();

    /* If "UP" pushbutton is pressed */
    if (Key == JOY_UP)
    {
      /* Increase the value of the digit */
      if (tmpvalue >= ValueMax)
      {
        tmpvalue = ValueMin;
      }
      else
      {
        tmpvalue++;
      }

      SetCursorPosition(ptr, LCD_LINE2, ColBegin);
      ptr[ColBegin] = (tmpvalue + 0x30);
      sprintf((char*)TempBuffer, "%c", ptr[ColBegin]);
      LCD_Print((uint8_t*)TempBuffer);
    }

    /* If "DOWN" pushbutton is pressed */
    if (Key == JOY_DOWN)
    {
      /* Decrease the value of the digit */
      if ((tmpvalue <= ValueMin) || (tmpvalue > ValueMax))
      {
        tmpvalue = ValueMax;
      }
      else
      {
        tmpvalue--;
      }

      SetCursorPosition(ptr, LCD_LINE2, ColBegin);
      ptr[ColBegin] = (tmpvalue + 0x30);
      sprintf((char*)TempBuffer, "%c", ptr[ColBegin]);
      LCD_Print((uint8_t*)TempBuffer);
    }

    /* If "SEL" pushbutton is pressed */
    if (Key == JOY_SEL)
    {
      /* Display new value */
      SetCursorPosition(ptr, LCD_LINE2, ColBegin);

      ptr[ColBegin] = (tmpvalue + 0x30);
      sprintf((char*)TempBuffer, "%c", ptr[ColBegin]);
      LCD_Print((uint8_t*)TempBuffer);
      /* Return the digit value and exit */
      return tmpvalue;
    }
    if (i < 5)
    {
      ToggleCursorPosition(ptr, LCD_LINE2, ColBegin);
      i++;
    }
  }
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
  * @brief  Return the number of the day in the week.
  * @param  None
  * @retval The number of the day
  */
uint16_t DayOfWeek(uint16_t nYear, uint16_t nMonth, uint16_t nDay)
{
  uint16_t nDayOfWeek = 0;
  uint16_t pnDaysBeforeMonth[14] = {0, 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};

  /* The day of Jan 1, nYear */
  nDayOfWeek = 6 + nYear % 7 + CountOfFeb29(nYear) % 7 + 14; /* + 14 : makes nDayOfWeek >= 0 */

  /* The day of nMonth 1, nYear */
  nDayOfWeek += pnDaysBeforeMonth[ nMonth ];
  if ( nMonth > 2 && IsLeapYear(nYear))
  {
    nDayOfWeek++;
  }

  /* The day of nMonth nDay, nYear */
  nDayOfWeek += nDay - 1;
  nDayOfWeek %= 7;
  /* return the number of the day in the week */
  return nDayOfWeek;
}

/**
  * @brief  Return february days number.
  * @param  None
  * @retval February days number.
  */
uint16_t CountOfFeb29(uint16_t nYear)
{
  uint16_t nCount = 0;

  if (nYear > 0)
  {
    nCount = 1; /* Year 0 is a leap year */
    nYear--;    /* Year nYear is not in the period */
  }
  nCount += nYear / 4 - nYear / 100 + nYear / 400;
  return nCount;
}

/**
  * @brief  Check whether the passed year is Leap or not.
  * @param  None
  * @retval TRUE: leap year
  *         FALSE: not leap year
  */
bool IsLeapYear(u16 nYear)
{
  if (nYear % 4 != 0) return FALSE;
  if (nYear % 100 != 0) return TRUE;
  return (bool)(nYear % 400 == 0);
}

/**
  * @brief  Set the LCD cursor to the specified location
  * @param  ptr : Pointer to string to display on the LCD Glass.
  * @param  Line : line where the cursor will be set (LCD_LINE1 or LCD_LINE2)
  * @param  Offset : is the position offset (only even position are supported)
  * @retval None
  */
void SetCursorPosition(uint8_t* ptr, uint8_t Line, uint8_t Offset)
{
  if ((Offset % 2) == 0)
  {
    LCD_SendByte(COMMAND_TYPE, (uint8_t)(Line + Offset / 2));
  }
  else /* (ColBegin % 2)!= 0*/
  {
    LCD_SendByte(COMMAND_TYPE, (uint8_t)(Line + Offset / 2));

    sprintf((char*)TempBuffer, "%c", ptr[Offset-1]);
    LCD_Print((uint8_t*)TempBuffer);
  }

}
/**
  * @brief  Set the LCD cursor to the specified location
  * @param  ptr : Pointer to string to display on the LCD Glass.
  * @param  Line : line where the cursor will be set (LCD_LINE1 or LCD_LINE2)
  * @param  Offset : is the position offset (only even position are supported)
  * @retval None
  */
void ToggleCursorPosition(uint8_t* ptr, uint8_t Line, uint8_t Offset)
{
  SetCursorPosition(ptr, Line, Offset);
  LCD_Print(" ");
  Delay(3000);
  SetCursorPosition(ptr, Line, Offset);
  sprintf((char*)TempBuffer, "%c", ptr[Offset]);
  LCD_Print((uint8_t*)TempBuffer);
  Delay(3000);
}


/**
  * @brief  Wait 1 sec for LSE stabilization .
  * @param  None.
  * @retval None.
  * Note : TIM4 is configured for a system clock = 2MHz
  */
void LSE_StabTime(void)
{

  CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, ENABLE);

  /* Configure TIM4 to generate an update event each 1 s */
  TIM4_TimeBaseInit(TIM4_Prescaler_16384, 123);
  /* Clear update flag */
  TIM4_ClearFlag(TIM4_FLAG_Update);

  /* Enable TIM4 */
  TIM4_Cmd(ENABLE);

  /* Wait 1 sec */
  while ( TIM4_GetFlagStatus(TIM4_FLAG_Update) == RESET );

  TIM4_ClearFlag(TIM4_FLAG_Update);

  /* Disable TIM4 */
  TIM4_Cmd(DISABLE);

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
