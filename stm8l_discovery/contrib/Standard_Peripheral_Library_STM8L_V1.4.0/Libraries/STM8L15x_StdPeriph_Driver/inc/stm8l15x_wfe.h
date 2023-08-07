/**
  ******************************************************************************
  * @file    stm8l15x_wfe.h
  * @author  MCD Application Team
  * @version V1.4.0
  * @date    09/24/2010
  * @brief   This file contains all the functions prototypes for the WFE firmware
  *          library.
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
#ifndef __STM8L15x_WFE_H
#define __STM8L15x_WFE_H

/* Includes ------------------------------------------------------------------*/
#include "stm8l15x.h"

/** @addtogroup STM8L15x_StdPeriph_Driver
  * @{
  */

/* Exported variables ------------------------------------------------------- */
/* Exported types ------------------------------------------------------------*/

/** @addtogroup WFE_Exported_Types
  * @{
  */

/**
  * @brief Signal Sources to generate wake_up events
  * Elements values convention: 0xXXYY
  *  X = SRx registers index
  *      XX = 01 : CR1
  *      XX = 02 : CR2
  *      XX = 03 : CR3
  *      XX = 04 : CR4
  *  YY = flag mask in the dedicated register(XX register)
  */
typedef enum {
  WFE_Source_TIM2_EV0     = (uint16_t)0x0101, /*!< TIM2 Upadte/Trigger and Break interrupt */
  WFE_Source_TIM2_EV1     = (uint16_t)0x0102, /*!< TIM2 Capture/Compare interrupt          */
  WFE_Source_TIM1_EV0     = (uint16_t)0x0104, /*!< TIM1 Upadte/Trigger and Break interrupt */
  WFE_Source_TIM1_EV1     = (uint16_t)0x0108, /*!< TIM1 Capture/Compare interrupt          */
  WFE_Source_EXTI_EV0     = (uint16_t)0x0110, /*!< I/O port interrupt from Pins 0          */
  WFE_Source_EXTI_EV1     = (uint16_t)0x0120, /*!< I/O port interrupt from Pins 1          */
  WFE_Source_EXTI_EV2     = (uint16_t)0x0140, /*!< I/O port interrupt from Pins 2          */
  WFE_Source_EXTI_EV3     = (uint16_t)0x0180, /*!< I/O port interrupt from Pins 3          */
  WFE_Source_EXTI_EV4     = (uint16_t)0x0201, /*!< I/O port interrupt from Pins 4          */
  WFE_Source_EXTI_EV5     = (uint16_t)0x0202, /*!< I/O port interrupt from Pins 5          */
  WFE_Source_EXTI_EV6     = (uint16_t)0x0204, /*!< I/O port interrupt from Pins 6          */
  WFE_Source_EXTI_EV7     = (uint16_t)0x0208, /*!< I/O port interrupt from Pins 7          */
  WFE_Source_EXTI_EVB_G   = (uint16_t)0x0210, /*!< I/O port interrupt from port B and G    */
  WFE_Source_EXTI_EVD_H   = (uint16_t)0x0220, /*!< I/O port interrupt from Port D and H    */
  WFE_Source_EXTI_EVE_F   = (uint16_t)0x0240, /*!< I/O port interrupt from Port E and F    */
  WFE_Source_ADC1_COMP_EV = (uint16_t)0x0280, /*!< ADC1, COMP1 and COMP2 interrupts        */
  WFE_Source_TIM3_EV0     = (uint16_t)0x0301, /*!< TIM3 Upadte/Trigger and Break interrupt */
  WFE_Source_TIM3_EV1     = (uint16_t)0x0302, /*!< TIM3 Capture/Compare interrupt          */
  WFE_Source_TIM4_EV      = (uint16_t)0x0304, /*!< TIM4 Upadte and Trigger interrupt       */
  WFE_Source_SPI1_EV      = (uint16_t)0x0308, /*!< SPI1 Rx and Tx interrupt                */
  WFE_Source_I2C1_EV      = (uint16_t)0x0310, /*!< I2C1 Rx and Tx interrupt                */
  WFE_Source_USART1_EV    = (uint16_t)0x0320, /*!< USART1 Rx and Tx interrupt              */
  WFE_Source_DMA1CH01_EV  = (uint16_t)0x0340, /*!< DMA1 channel 0 and  1 interrupt         */
  WFE_Source_DMA1CH23_EV  = (uint16_t)0x0380,  /*!< DMA1 channel 2 and  3  interrupt        */
  WFE_Source_RTC_CSS_EV   = (uint16_t)0x0401,  /*!< RTC or CSS on LSE event */
  WFE_Source_SPI2_EV      = (uint16_t)0x0402,  /*!< SPI2 Rx and Tx interrupt */
  WFE_Source_USART2_EV    = (uint16_t)0x0404,  /*!< USART2 Rx and Tx interrupt */
  WFE_Source_USART3_EV    = (uint16_t)0x0408,  /*!< USART2 Rx and Tx interrupt */
  WFE_Source_TIM5_EV0     = (uint16_t)0x0410,  /*!< TIM5 Upadte/Trigger and Break interrupt */
  WFE_Source_TIM5_EV1     = (uint16_t)0x0420,  /*!< TIM5 Capture/Compare interrupt */
  WFE_Source_AES_EV       = (uint16_t)0x0440   /*!< AES interrupt */
} WFE_Source_TypeDef;



/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/

/** @addtogroup WFE_Private_Macros
  * @{
  */

/**
  * @brief Macro used by the assert function in order to check the different IRQ values
  */

#define IS_WFE_SOURCE(Source) (((Source) == WFE_Source_TIM2_EV0) || \
                               ((Source) == WFE_Source_TIM2_EV1) || \
                               ((Source) == WFE_Source_TIM1_EV0) || \
                               ((Source) == WFE_Source_TIM1_EV1) || \
                               ((Source) == WFE_Source_EXTI_EV0) || \
                               ((Source) == WFE_Source_EXTI_EV1) || \
                               ((Source) == WFE_Source_EXTI_EV2) || \
                               ((Source) == WFE_Source_EXTI_EV3) || \
                               ((Source) == WFE_Source_EXTI_EV4) || \
                               ((Source) == WFE_Source_EXTI_EV5) || \
                               ((Source) == WFE_Source_EXTI_EV6) || \
                               ((Source) == WFE_Source_EXTI_EV7) || \
                               ((Source) == WFE_Source_EXTI_EVB_G) || \
                               ((Source) == WFE_Source_EXTI_EVD_H) || \
                               ((Source) == WFE_Source_EXTI_EVE_F) || \
                               ((Source) == WFE_Source_ADC1_COMP_EV) || \
                               ((Source) == WFE_Source_TIM3_EV0) || \
                               ((Source) == WFE_Source_TIM3_EV1) || \
                               ((Source) == WFE_Source_TIM4_EV) || \
                               ((Source) == WFE_Source_SPI1_EV) || \
                               ((Source) == WFE_Source_I2C1_EV) || \
                               ((Source) == WFE_Source_USART1_EV) || \
                               ((Source) == WFE_Source_DMA1CH01_EV) || \
                               ((Source) == WFE_Source_AES_EV) || \
                               ((Source) == WFE_Source_TIM5_EV1) || \
                               ((Source) == WFE_Source_TIM5_EV0) || \
                               ((Source) == WFE_Source_USART3_EV) || \
                               ((Source) == WFE_Source_USART2_EV) || \
                               ((Source) == WFE_Source_SPI2_EV) || \
                               ((Source) == WFE_Source_RTC_CSS_EV) || \
                               ((Source) == WFE_Source_DMA1CH23_EV))

/**
  * @}
  */

/* Exported functions ------------------------------------------------------- */

/** @addtogroup WFE_Exported_Functions
  * @{
  */

void WFE_DeInit(void);
void WFE_WakeUpSourceEventCmd(WFE_Source_TypeDef WFE_Source, FunctionalState NewState);
FunctionalState WFE_GetWakeUpSourceEventStatus(WFE_Source_TypeDef WFE_Source);

/**
  * @}
  */

#endif /* __STM8L15x_WFE_H */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
