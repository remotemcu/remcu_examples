/**
  ******************************************************************************
  * @file    SPI/FullDuplex_SoftNSS/platform_config.h 
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
#if !defined (USE_STM32100B_EVAL) && !defined (USE_STM3210B_EVAL) &&  !defined (USE_STM3210E_EVAL) &&  !defined (USE_STM3210C_EVAL) &&  !defined (USE_STM32100E_EVAL)
 //#define USE_STM32100B_EVAL
 //#define USE_STM3210B_EVAL
 //#define USE_STM3210E_EVAL
 //#define USE_STM32100E_EVAL
 #define USE_STM3210C_EVAL
#endif

/* Define the STM32F10x hardware depending on the used evaluation board */
#if defined (USE_STM3210B_EVAL) || defined (USE_STM3210E_EVAL) || defined(USE_STM32100B_EVAL) || defined (USE_STM32100E_EVAL)
  #define SPIy                   SPI1
  #define SPIy_CLK               RCC_APB2Periph_SPI1
  #define SPIy_GPIO              GPIOA
  #define SPIy_GPIO_CLK          RCC_APB2Periph_GPIOA  
  #define SPIy_PIN_SCK           GPIO_Pin_5
  #define SPIy_PIN_MISO          GPIO_Pin_6
  #define SPIy_PIN_MOSI          GPIO_Pin_7
  
  #define SPIz                    SPI2
  #define SPIz_CLK                RCC_APB1Periph_SPI2
  #define SPIz_GPIO               GPIOB
  #define SPIz_GPIO_CLK           RCC_APB2Periph_GPIOB 
  #define SPIz_PIN_SCK            GPIO_Pin_13
  #define SPIz_PIN_MISO           GPIO_Pin_14
  #define SPIz_PIN_MOSI           GPIO_Pin_15 
          
#elif defined (USE_STM3210C_EVAL)
  #define SPIy                   SPI3   /* SPI pins are remapped by software */
  #define SPIy_CLK               RCC_APB1Periph_SPI3
  #define SPIy_GPIO              GPIOC
  #define SPIy_GPIO_CLK          RCC_APB2Periph_GPIOC  
  #define SPIy_PIN_SCK           GPIO_Pin_10
  #define SPIy_PIN_MISO          GPIO_Pin_11
  #define SPIy_PIN_MOSI          GPIO_Pin_12
  
  #define SPIz                    SPI2
  #define SPIz_CLK                RCC_APB1Periph_SPI2
  #define SPIz_GPIO               GPIOB
  #define SPIz_GPIO_CLK           RCC_APB2Periph_GPIOB
  #define SPIz_PIN_SCK            GPIO_Pin_13
  #define SPIz_PIN_MISO           GPIO_Pin_14
  #define SPIz_PIN_MOSI           GPIO_Pin_15

#endif

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __PLATFORM_CONFIG_H */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
