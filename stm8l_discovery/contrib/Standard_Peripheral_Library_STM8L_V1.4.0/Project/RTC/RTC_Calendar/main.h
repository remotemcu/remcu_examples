/**
  ******************************************************************************
  * @file    RTC/RTC_Calendar/main.h
  * @author  MCD Application Team
  * @version V1.5.2
  * @date    30-September-2014
  * @brief   Header for main.c
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm8l15x.h"
#include "stm8_eval.h"
#include "stm8_eval_lcd.h"
#include "stdio.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Calendar_Init(void);
void Time_Regulate(void);
void Time_Show(uint8_t Line);
void Date_Regulate(void);
void Date_Show(void);
void Alarm_Regulate(void);
uint16_t DayOfWeek(uint16_t nYear, uint16_t nMonth, uint16_t nDay);
uint16_t CountOfFeb29(uint16_t nYear);
bool IsLeapYear(uint16_t nYear);
uint8_t ReadDigit(uint8_t* ptr, uint8_t ColBegin, uint8_t ValueMax, uint8_t ValueMin) ;
void SetCursorPosition(uint8_t* ptr, uint8_t Line, uint8_t Offset);
void ToggleCursorPosition(uint8_t* ptr, uint8_t Line, uint8_t Offset);
JOYState_TypeDef ReadJoystick(void);
void LSE_StabTime(void);
void Delay(__IO uint32_t nCount);

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
