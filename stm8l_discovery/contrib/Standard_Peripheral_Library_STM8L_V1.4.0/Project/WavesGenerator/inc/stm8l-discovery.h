/**
  ******************************************************************************
  * @file    stm8l_eval.h
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    02/12/2010
  * @brief   Header file for stm8l_eval.c module.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */ 
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM8L_EVAL_H
#define __STM8L_EVAL_H

/* Includes ------------------------------------------------------------------*/
#include "stm8l15x.h"

/** @addtogroup Utilities
  * @{
  */ 
  
/** @addtogroup STM8L_EVAL
  * @{
  */ 


/** @defgroup STM8L_EVAL_Exported_Types
  * @{
  */ 
/**
  * @}
  */ 
  
/** @defgroup STM8L_EVAL_Exported_Constants
  * @{
  */ 

/** 
  * @brief  Uncomment the line corresponding to the STMicroelectronics evaluation
  *   board used in your application.
  *   
  *  Tip: To avoid modifying this file each time you need to switch between these
  *       boards, you can define the board in your toolchain compiler preprocessor.    
  */ 
//#if !defined (USE_STM8L1526_EVAL) 
//  #define USE_STM8L1526_EVAL
//#endif

//#ifdef USE_STM8L1526_EVAL
// #include "stm8l1526_eval.h"
//#else 
// #error "Please select first the STM8L15X_EVAL board to be used (in stm8l_eval.h)"
//#endif


typedef enum 
{
  LED1 = 0,
  LED2 = 1,
  LED3 = 2,
  LED4 = 3,
  LED5 = 4  /* Bicolor Led */
} Led_TypeDef;

typedef enum 
{  
  BUTTON_USER = 0,
  BUTTON_USER2 = 1	
} Button_TypeDef;

typedef enum 
{  
  BUTTON_MODE_GPIO = 0,
  BUTTON_MODE_EXTI = 1
} ButtonMode_TypeDef;

typedef enum 
{ 
  JOY_NONE = 0,
  JOY_SEL = 1,
  JOY_DOWN = 2,
  JOY_LEFT = 3,
  JOY_RIGHT = 4,
  JOY_UP = 5
} JOYState_TypeDef;

typedef enum 
{
  COM1 = 0
} COM_TypeDef;
 
/**
  * @}
  */ 

/** @defgroup STM8L_EVAL_Exported_Macros
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup STM8L_EVAL_Exported_Functions
  * @{
  */ 

#endif /* __STM8L_EVAL_H */
/**
  * @}
  */ 
/** @addtogroup Utilities
  * @{
  */
  
/** @addtogroup STM8L1526_EVAL
  * @{
  */ 


/** @defgroup STM8L1526_EVAL_Exported_Types
  * @{
  */ 
/**
  * @}
  */ 



/** @defgroup STM8L1526_EVAL_Exported_Constants
  * @{
  */ 
/** @addtogroup STM8L1526_EVAL_LED
  * @{
  */

#define LEDn                        5
#define LED1_GPIO_PORT              GPIOA
#define LED1_GPIO_PIN               GPIO_Pin_4
  
#define LED2_GPIO_PORT              GPIOA
#define LED2_GPIO_PIN               GPIO_Pin_5
  
#define LED3_GPIO_PORT              GPIOA
#define LED3_GPIO_PIN               GPIO_Pin_6
  
#define LED4_GPIO_PORT              GPIOA
#define LED4_GPIO_PIN               GPIO_Pin_7

#define LED5_GPIO_PORT              GPIOD
#define LED5_GPIO_PIN               GPIO_Pin_2

/**
  * @}
  */ 
  
/** @addtogroup STM8L1526_EVAL_BUTTON
  * @{
  */  
#define BUTTONn                     6

/**
 * @brief Key push-button
 */
#define USER_BUTTON_PORT             GPIOC
#define USER_BUTTON_PIN              GPIO_Pin_1

/**
 * @brief Key push-button
 */
#define USER2_BUTTON_PORT             GPIOC
#define USER2_BUTTON_PIN              GPIO_Pin_0


/**
 * @brief Joystick EXTI push-button definition
 */
#define USER_BUTTON_EXTI             EXTI_Pin_1
#define USER2_BUTTON_EXTI           EXTI_Pin_0
//#define LEFT_BUTTON_EXTI            EXTI_Pin_1
//#define UP_BUTTON_EXTI              EXTI_Pin_7
//#define DOWN_BUTTON_EXTI            EXTI_Pin_4
//#define SEL_BUTTON_EXTI             EXTI_Pin_2

/**
  * @}
  */ 

/** @addtogroup STM8L1526_EVAL_COM
  * @{
  */
#define COMn                        1

/**
 * @brief Definition for COM port1
 */ 
#define EVAL_COM1                   USART1
#define EVAL_COM1_GPIO              GPIOC
#define EVAL_COM1_CLK               CLK_Peripheral_USART1
#define EVAL_COM1_RxPin             GPIO_Pin_2
#define EVAL_COM1_TxPin             GPIO_Pin_3


/**
  * @}
  */ 

/**
  * @}
  */ 
  
/**
  * @}
  */     

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
