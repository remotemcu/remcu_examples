/**
  ******************************************************************************
  * @file    I2C/IOExpander/main.h 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/

#if !(defined USE_STM32100E_EVAL) && !(defined USE_STM3210C_EVAL)
 #error "Please select STM3210C_EVAL or STM32100E_EVAL board For this example"
#endif

#include "stm32f10x.h"
#include "stm32_eval.h"
#include <stdio.h>
 
#ifdef USE_STM3210C_EVAL
 #include "stm3210c_eval_lcd.h"
 #include "stm3210c_eval_ioe.h"
 
#elif defined USE_STM32100E_EVAL
 #include "stm32100e_eval_lcd.h"
 #include "stm32100e_eval_ioe.h"
#endif

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

// #define IOE_POLLING_MODE
#define IOE_INTERRUPT_MODE

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */


#endif /* __MAIN_H */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
