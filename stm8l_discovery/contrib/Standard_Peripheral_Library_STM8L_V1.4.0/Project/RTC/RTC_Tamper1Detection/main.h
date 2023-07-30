/**
  ******************************************************************************
  * @file    RTC/RTC_Tamper1Detection/main.h
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
#define EnterSafeCode() disableInterrupts()
#define ExitSafeCode()  enableInterrupts()
#define ASCII_NUM_0         ((uint8_t)48)
#define SHOW_POINT1          ((uint8_t)0)
#define SHOW_POINT2          ((uint8_t)9)
#define CHARPOS             ((uint8_t)13)

/* Exported functions ------------------------------------------------------- */
void Calendar_Init(void);
void Tamper_Init(void);
void EvalBoard_Init(void);
void Time_Show(uint8_t Line,uint8_t pos);
void Time_SaveCurrentTime(uint8_t SavedTimeId);
void Time_Record(void);
void Tamper_DelayShow(uint8_t DelayId );
void SavedTime_Show(uint8_t Line, uint8_t SavedTimeId);
void Delay(__IO uint32_t nCount);

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
