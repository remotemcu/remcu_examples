/**
  ******************************************************************************
  * @file    CEC/CEC_MultiAddress/main.h 
  * @author  MCD Application Team
  * @version V1.4.0
  * @date    24-July-2014
  * @brief   Header for main.c module
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
#include "stm32f0xx.h"
#ifdef USE_STM320518_EVAL
  #include "stm320518_eval.h"
  #include "stm320518_eval_lcd.h"
#else 
  #include "stm32072b_eval.h"
  #include "stm32072b_eval_lcd.h" 
#endif /* USE_STM320518_EVAL */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Uncomment the line below if you will use the CEC peripheral as a Device1 */
#define DEVICE_1
/* Uncomment the line below if you will use the CEC peripheral as a Device2 */
//#define DEVICE_2

#define DEVICE_ADDRESS_1               0x01  /* CEC device 1 address   */
#define DEVICE_ADDRESS_2               0x03  /* Tuner: CEC device 2 address   */
#define DEVICE_ADDRESS_3               0x05  /* Audio System: CEC device 3 address   */

 /* Uncomment the define to use LCD */
#define LCD_DISPLAY
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
