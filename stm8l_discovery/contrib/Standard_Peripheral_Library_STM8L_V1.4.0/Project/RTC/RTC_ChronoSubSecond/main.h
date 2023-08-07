/**
  ******************************************************************************
  * @file    RTC/RTC_ChronoSubSecond/main.h
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
#define MAXSAVETIME ((uint8_t) 9)
#define EnterSafeCode() disableInterrupts()
#define ExitSafeCode()  enableInterrupts()
#define ASCII_NUM_0         ((uint8_t)48)
#define SHOW_POINT          ((uint8_t)2)
#define DEFAULTDISPLAY      "Chrono Running "
#define CHRONOATZERO        "  00:00:00:000 "
#define RESETDISPLAY        "  SEL to Run   "
#define PAUSELED            LED1  
#define RUNLED              LED2
#define RECORDLED           LED3

/* Exported functions ------------------------------------------------------- */


void Calendar_Init(void);
void Tamper_Init(void);
void EvalBoard_Init(void);

void Time_Show(uint8_t Line);
void Time_SaveCurrentTime(uint8_t SavedTimeId);
void Time_Pause(void);
void Time_Record(void);
void Time_ResetMenu(void);
void Time_Reset(void);

void SavedTime_Show(uint8_t Line, uint8_t SavedTimeId);
void SavedTime_Parsing(void);
void SavedTime_Erase(void);

void Restore_LastDisplay(void);
JOYState_TypeDef ReadJoystick(void);
void Delay(__IO uint32_t nCount);

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
