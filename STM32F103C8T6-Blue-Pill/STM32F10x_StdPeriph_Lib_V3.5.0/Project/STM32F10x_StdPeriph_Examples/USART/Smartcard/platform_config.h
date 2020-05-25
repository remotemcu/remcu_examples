/**
  ******************************************************************************
  * @file    USART/Smartcard/platform_config.h 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Evaluation board specific configuration file.
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
#ifndef __PLATFORM_CONFIG_H
#define __PLATFORM_CONFIG_H

/* Includes ------------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Uncomment the line corresponding to the STMicroelectronics evaluation board
   used to run the example */
#if !defined (USE_STM3210B_EVAL) &&  !defined (USE_STM3210E_EVAL) &&  !defined (USE_STM3210C_EVAL)
 //#define USE_STM3210B_EVAL
 //#define USE_STM3210E_EVAL
 #define USE_STM3210C_EVAL 
#endif

/* Define the STM32F10x hardware depending on the used evaluation board */
#ifdef USE_STM3210B_EVAL
  /* Smartcard Inteface USART pins */
  #define SC_USART                 USART3
  #define SC_USART_GPIO            GPIOB
  #define SC_USART_CLK             RCC_APB1Periph_USART3
  #define SC_USART_GPIO_CLK        RCC_APB2Periph_GPIOB
  #define SC_USART_TxPin           GPIO_Pin_10
  #define SC_USART_ClkPin          GPIO_Pin_12
  #define SC_USART_IRQn            USART3_IRQn
  #define SC_USART_IRQHandler      USART3_IRQHandler  
  /* Smartcard Inteface GPIO pins */
  #define SC_3_5V                  GPIO_Pin_11 /* GPIOD Pin 11 */
  #define SC_RESET                 GPIO_Pin_11 /* GPIOB Pin 11 */
  #define SC_CMDVCC                GPIO_Pin_7  /* GPIOE Pin 7  */
  #define SC_OFF                   GPIO_Pin_14 /* GPIOE Pin 14 */
  #define GPIO_3_5V                GPIOD
  #define GPIO_RESET               GPIOB
  #define GPIO_CMDVCC              GPIOE
  #define GPIO_OFF                 GPIOE
  #define RCC_APB2Periph_3_5V      RCC_APB2Periph_GPIOD
  #define RCC_APB2Periph_RESET     RCC_APB2Periph_GPIOB
  #define RCC_APB2Periph_CMDVCC    RCC_APB2Periph_GPIOE
  #define RCC_APB2Periph_OFF       RCC_APB2Periph_GPIOE
  #define SC_EXTI                  EXTI_Line14
  #define SC_PortSource            GPIO_PortSourceGPIOE
  #define SC_PinSource             GPIO_PinSource14
  #define SC_EXTI_IRQ              EXTI15_10_IRQn
#elif defined USE_STM3210E_EVAL
  /* Smartcard Inteface USART pins */
  #define SC_USART                 USART3
  #define SC_USART_GPIO            GPIOB
  #define SC_USART_CLK             RCC_APB1Periph_USART3
  #define SC_USART_GPIO_CLK        RCC_APB2Periph_GPIOB
  #define SC_USART_TxPin           GPIO_Pin_10
  #define SC_USART_ClkPin          GPIO_Pin_12
  #define SC_USART_IRQn            USART3_IRQn 
  #define SC_USART_IRQHandler      USART3_IRQHandler    
  /* Smartcard Inteface GPIO pins */
  #define SC_3_5V                  GPIO_Pin_0  /* GPIOB Pin 0 */
  #define SC_RESET                 GPIO_Pin_11 /* GPIOB Pin 11 */
  #define SC_CMDVCC                GPIO_Pin_6  /* GPIOC Pin 6  */
  #define SC_OFF                   GPIO_Pin_7  /* GPIOC Pin 7 */ 
  #define GPIO_3_5V                GPIOB
  #define GPIO_RESET               GPIOB
  #define GPIO_CMDVCC              GPIOC
  #define GPIO_OFF                 GPIOC
  #define RCC_APB2Periph_3_5V      RCC_APB2Periph_GPIOB
  #define RCC_APB2Periph_RESET     RCC_APB2Periph_GPIOB
  #define RCC_APB2Periph_CMDVCC    RCC_APB2Periph_GPIOC
  #define RCC_APB2Periph_OFF       RCC_APB2Periph_GPIOC
  #define SC_EXTI                  EXTI_Line7
  #define SC_PortSource            GPIO_PortSourceGPIOC
  #define SC_PinSource             GPIO_PinSource7
  #define SC_EXTI_IRQ              EXTI9_5_IRQn
#elif defined USE_STM3210C_EVAL
  /* Smartcard Inteface USART pins */
  #define SC_USART                 USART3
  #define SC_USART_GPIO            GPIOD
  #define SC_USART_CLK             RCC_APB1Periph_USART3
  #define SC_USART_GPIO_CLK        RCC_APB2Periph_GPIOD
  #define SC_USART_TxPin           GPIO_Pin_8
  #define SC_USART_ClkPin          GPIO_Pin_10
  #define SC_USART_IRQn            USART3_IRQn 
  #define SC_USART_IRQHandler      USART3_IRQHandler    
  /* Smartcard Inteface GPIO pins */
  #define SC_3_5V                  GPIO_Pin_0  /* GPIOC Pin 0 */
  #define SC_RESET                 GPIO_Pin_9  /* GPIOD Pin 9 */
  #define SC_CMDVCC                GPIO_Pin_7  /* GPIOD Pin 7  */
  #define SC_OFF                   GPIO_Pin_7  /* GPIOE Pin 7 */ 
  #define GPIO_3_5V                GPIOC
  #define GPIO_RESET               GPIOD
  #define GPIO_CMDVCC              GPIOD
  #define GPIO_OFF                 GPIOE
  #define RCC_APB2Periph_3_5V      RCC_APB2Periph_GPIOC
  #define RCC_APB2Periph_RESET     RCC_APB2Periph_GPIOD
  #define RCC_APB2Periph_CMDVCC    RCC_APB2Periph_GPIOD
  #define RCC_APB2Periph_OFF       RCC_APB2Periph_GPIOE
  #define SC_EXTI                  EXTI_Line7
  #define SC_PortSource            GPIO_PortSourceGPIOE
  #define SC_PinSource             GPIO_PinSource7
  #define SC_EXTI_IRQ              EXTI9_5_IRQn  
#endif /* USE_STM3210B_EVAL */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __PLATFORM_CONFIG_H */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
