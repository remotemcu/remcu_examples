/**
  ******************************************************************************
  * @file    stm32l152c_discovery.h
  * @author  MCD Application Team
  * @brief   This file contains definitions for STM32L152C_DISCOVERY's LEDs and 
  *          push-buttons hardware resources.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2013 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */ 
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32L152C_DISCOVERY_H
#define __STM32L152C_DISCOVERY_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"

/** @addtogroup STM32L152C_DISCOVERY
  * @{
  */
      
/** @addtogroup STM32L152C_DISCOVERY_LOW_LEVEL
  * @{
  */ 

/** @defgroup STM32L152C_DISCOVERY_LOW_LEVEL_Exported_Types
  * @{
  */
typedef enum 
{
  LED3 = 0,
  LED4 = 1  
} Led_TypeDef;

typedef enum 
{  
  BUTTON_USER = 0,
} Button_TypeDef;

typedef enum 
{  
  BUTTON_MODE_GPIO = 0,
  BUTTON_MODE_EXTI = 1
} ButtonMode_TypeDef;  


/**
  * @}
  */ 

/** @defgroup STM32L152C_DISCOVERY_LOW_LEVEL_Exported_Constants
  * @{
  */ 

/** 
  * @brief  Define for STM32L152C_DISCOVERY board  
  */ 
#if !defined (USE_STM32L152C_DISCOVERY)
 #define USE_STM32L152C_DISCOVERY
#endif

/** @addtogroup STM32L152C_DISCOVERY_LOW_LEVEL_LED
  * @{
  */
#define LEDn                               2

#define LED3_PIN                           GPIO_PIN_7
#define LED3_GPIO_PORT                     GPIOB
#define LED3_GPIO_CLK_ENABLE()             __HAL_RCC_GPIOB_CLK_ENABLE()  
#define LED3_GPIO_CLK_DISABLE()            __HAL_RCC_GPIOB_CLK_DISABLE()
  
#define LED4_PIN                           GPIO_PIN_6
#define LED4_GPIO_PORT                     GPIOB
#define LED4_GPIO_CLK_ENABLE()             __HAL_RCC_GPIOB_CLK_ENABLE()  
#define LED4_GPIO_CLK_DISABLE()            __HAL_RCC_GPIOB_CLK_DISABLE()

#define LEDx_GPIO_CLK_ENABLE(__INDEX__)   do { if (__INDEX__ == 0) LED3_GPIO_CLK_ENABLE(); \
                                               else LED4_GPIO_CLK_ENABLE();} while (0)
#define LEDx_GPIO_CLK_DISABLE(__INDEX__)  (((__INDEX__) == 0) ? LED3_GPIO_CLK_DISABLE() :\
                                           LED4_GPIO_CLK_DISABLE())
                                         
/**
  * @}
  */ 
  
/** @addtogroup STM32L152C_DISCOVERY_LOW_LEVEL_BUTTON
  * @{
  */  
#define BUTTONn                            1



/**
  * @brief Key push-button
  */
#define KEY_BUTTON_PIN                       GPIO_PIN_0
#define KEY_BUTTON_GPIO_PORT                 GPIOA
#define KEY_BUTTON_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOA_CLK_ENABLE()   
#define KEY_BUTTON_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOA_CLK_DISABLE()  
#define KEY_BUTTON_EXTI_LINE                 GPIO_PIN_0
#define KEY_BUTTON_EXTI_IRQn                 EXTI0_1_IRQn

#define BUTTONx_GPIO_CLK_ENABLE(__INDEX__)   KEY_BUTTON_GPIO_CLK_ENABLE()
                                              

#define BUTTONx_GPIO_CLK_DISABLE(__INDEX__)   KEY_BUTTON_GPIO_CLK_DISABLE()
                                               
/**
  * @}
  */ 
  

/* Exported constant ---------------------------------------------------------*/



/**
  * @}
  */ 

/**
  * @}
  */ 
  
/** @defgroup STM32L152C_DISCOVERY_LOW_LEVEL_Exported_Macros
  * @{
  */  
/**
  * @}
  */ 


/** @defgroup STM32L152C_DISCOVERY_LOW_LEVEL_Exported_Functions
  * @{
  */
void             BSP_LED_Init(Led_TypeDef Led);
void             BSP_LED_On(Led_TypeDef Led);
void             BSP_LED_Off(Led_TypeDef Led);
void             BSP_LED_Toggle(Led_TypeDef Led);
void             BSP_PB_Init(Button_TypeDef Button, ButtonMode_TypeDef Button_Mode);
uint32_t         BSP_PB_GetState(Button_TypeDef Button);
/**
  * @}
  */

/**
  * @}
  */ 

#ifdef __cplusplus
}
#endif

#endif /* __STM32L152C_DISCOVERY_H */
 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
