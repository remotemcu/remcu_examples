/**
  ******************************************************************************
  * @file    Lib_DEBUG/RunTime_Check/stm32f10x_ip_dbg.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   This file provides all peripherals pointers initialization.
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

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_ip_dbg.h"

/** @addtogroup STM32F10x_StdPeriph_Examples
  * @{
  */

/** @addtogroup Lib_DEBUG_RunTime_Check
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
TIM_TypeDef             *TIM2_DBG;
TIM_TypeDef             *TIM3_DBG;
TIM_TypeDef             *TIM4_DBG;
TIM_TypeDef             *TIM5_DBG;
TIM_TypeDef             *TIM6_DBG;
TIM_TypeDef             *TIM7_DBG;
RTC_TypeDef             *RTC_DBG;
WWDG_TypeDef            *WWDG_DBG;
IWDG_TypeDef            *IWDG_DBG;
SPI_TypeDef             *SPI2_DBG;
SPI_TypeDef             *SPI3_DBG;
USART_TypeDef           *USART2_DBG;
USART_TypeDef           *USART3_DBG;
USART_TypeDef           *UART4_DBG;
USART_TypeDef           *UART5_DBG;
I2C_TypeDef             *I2C1_DBG;
I2C_TypeDef             *I2C2_DBG;
CAN_TypeDef             *CAN1_DBG;
BKP_TypeDef             *BKP_DBG;
PWR_TypeDef             *PWR_DBG;
DAC_TypeDef             *DAC_DBG;
CEC_TypeDef             *CEC_DBG;
AFIO_TypeDef            *AFIO_DBG;
EXTI_TypeDef            *EXTI_DBG;
GPIO_TypeDef            *GPIOA_DBG;
GPIO_TypeDef            *GPIOB_DBG;
GPIO_TypeDef            *GPIOC_DBG;
GPIO_TypeDef            *GPIOD_DBG;
GPIO_TypeDef            *GPIOE_DBG;
GPIO_TypeDef            *GPIOF_DBG;
GPIO_TypeDef            *GPIOG_DBG;
ADC_TypeDef             *ADC1_DBG;
ADC_TypeDef             *ADC2_DBG;
TIM_TypeDef             *TIM1_DBG;
SPI_TypeDef             *SPI1_DBG;
TIM_TypeDef             *TIM8_DBG;
USART_TypeDef           *USART1_DBG;
ADC_TypeDef             *ADC3_DBG;
TIM_TypeDef             *TIM15_DBG;
TIM_TypeDef             *TIM16_DBG;
TIM_TypeDef             *TIM17_DBG;
SDIO_TypeDef            *SDIO_DBG;
DMA_TypeDef             *DMA1_DBG;
DMA_TypeDef             *DMA2_DBG;
DMA_Channel_TypeDef     *DMA1_Channel1_DBG;
DMA_Channel_TypeDef     *DMA1_Channel2_DBG;
DMA_Channel_TypeDef     *DMA1_Channel3_DBG;
DMA_Channel_TypeDef     *DMA1_Channel4_DBG;
DMA_Channel_TypeDef     *DMA1_Channel5_DBG;
DMA_Channel_TypeDef     *DMA1_Channel6_DBG;
DMA_Channel_TypeDef     *DMA1_Channel7_DBG;
DMA_Channel_TypeDef     *DMA2_Channel1_DBG;
DMA_Channel_TypeDef     *DMA2_Channel2_DBG;
DMA_Channel_TypeDef     *DMA2_Channel3_DBG;
DMA_Channel_TypeDef     *DMA2_Channel4_DBG;
DMA_Channel_TypeDef     *DMA2_Channel5_DBG;
RCC_TypeDef             *RCC_DBG;
CRC_TypeDef             *CRC_DBG;
FLASH_TypeDef           *FLASH_DBG;
OB_TypeDef              *OB_DBG;  
FSMC_Bank1_TypeDef      *FSMC_Bank1_DBG;
FSMC_Bank1E_TypeDef     *FSMC_Bank1E_DBG;
FSMC_Bank2_TypeDef      *FSMC_Bank2_DBG;
FSMC_Bank3_TypeDef      *FSMC_Bank3_DBG;
FSMC_Bank4_TypeDef      *FSMC_Bank4_DBG;
DBGMCU_TypeDef          *DBGMCU_DBG;
SysTick_Type            *SysTick_DBG;
NVIC_Type               *NVIC_DBG;
SCB_Type                *SCB_DBG;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  This function initialize peripherals pointers.
  * @param  None
  * @retval None
  */
void debug(void)
{
/************************************* ADC ************************************/
  ADC1_DBG = (ADC_TypeDef *)  ADC1_BASE;

  ADC2_DBG = (ADC_TypeDef *)  ADC2_BASE;

  ADC3_DBG = (ADC_TypeDef *)  ADC3_BASE;

/************************************* BKP ************************************/
  BKP_DBG = (BKP_TypeDef *)  BKP_BASE;

/************************************* CAN ************************************/			
  CAN1_DBG = (CAN_TypeDef *)  CAN1_BASE;			

/************************************* CEC ************************************/			
  CEC_DBG = (CEC_TypeDef *)  CEC_BASE;	
  
/************************************* CRC ************************************/
  CRC_DBG = (CRC_TypeDef *)  CRC_BASE;

/************************************* DAC ************************************/
  DAC_DBG = (DAC_TypeDef *)  DAC_BASE;

/************************************* DBGMCU**********************************/
  DBGMCU_DBG = (DBGMCU_TypeDef *)  DBGMCU_BASE;


/************************************* DMA ************************************/
  DMA1_DBG = (DMA_TypeDef *)  DMA1_BASE;
  DMA2_DBG = (DMA_TypeDef *)  DMA2_BASE;

  DMA1_Channel1_DBG = (DMA_Channel_TypeDef *)  DMA1_Channel1_BASE;

  DMA1_Channel2_DBG = (DMA_Channel_TypeDef *)  DMA1_Channel2_BASE;

  DMA1_Channel3_DBG = (DMA_Channel_TypeDef *)  DMA1_Channel3_BASE;

  DMA1_Channel4_DBG = (DMA_Channel_TypeDef *)  DMA1_Channel4_BASE;

  DMA1_Channel5_DBG = (DMA_Channel_TypeDef *)  DMA1_Channel5_BASE;

  DMA1_Channel6_DBG = (DMA_Channel_TypeDef *)  DMA1_Channel6_BASE;

  DMA1_Channel7_DBG = (DMA_Channel_TypeDef *)  DMA1_Channel7_BASE;

  DMA2_Channel1_DBG = (DMA_Channel_TypeDef *)  DMA2_Channel1_BASE;

  DMA2_Channel2_DBG = (DMA_Channel_TypeDef *)  DMA2_Channel2_BASE;

  DMA2_Channel3_DBG = (DMA_Channel_TypeDef *)  DMA2_Channel3_BASE;

  DMA2_Channel4_DBG = (DMA_Channel_TypeDef *)  DMA2_Channel4_BASE;

  DMA2_Channel5_DBG = (DMA_Channel_TypeDef *)  DMA2_Channel5_BASE;

/************************************* EXTI ***********************************/
  EXTI_DBG = (EXTI_TypeDef *)  EXTI_BASE;

/************************************* FLASH and Option Bytes *****************/
  FLASH_DBG = (FLASH_TypeDef *)  FLASH_R_BASE;
  OB_DBG = (OB_TypeDef *)        OB_BASE;

/************************************* FSMC ***********************************/
  FSMC_Bank1_DBG = (FSMC_Bank1_TypeDef *)    FSMC_Bank1_R_BASE;
  FSMC_Bank1E_DBG = (FSMC_Bank1E_TypeDef *)  FSMC_Bank1E_R_BASE;  
  FSMC_Bank2_DBG = (FSMC_Bank2_TypeDef *)    FSMC_Bank2_R_BASE; 
  FSMC_Bank3_DBG = (FSMC_Bank3_TypeDef *)    FSMC_Bank3_R_BASE;
  FSMC_Bank4_DBG = (FSMC_Bank4_TypeDef *)    FSMC_Bank4_R_BASE;

/************************************* GPIO ***********************************/
  GPIOA_DBG = (GPIO_TypeDef *)  GPIOA_BASE;

  GPIOB_DBG = (GPIO_TypeDef *)  GPIOB_BASE;

  GPIOC_DBG = (GPIO_TypeDef *)  GPIOC_BASE;

  GPIOD_DBG = (GPIO_TypeDef *)  GPIOD_BASE;

  GPIOE_DBG = (GPIO_TypeDef *)  GPIOE_BASE;

  GPIOF_DBG = (GPIO_TypeDef *)  GPIOF_BASE;

  GPIOG_DBG = (GPIO_TypeDef *)  GPIOG_BASE;
  
  AFIO_DBG = (AFIO_TypeDef *)  AFIO_BASE;

/************************************* I2C ************************************/
  I2C1_DBG = (I2C_TypeDef *)  I2C1_BASE;

  I2C2_DBG = (I2C_TypeDef *)  I2C2_BASE;

/************************************* IWDG ***********************************/
  IWDG_DBG = (IWDG_TypeDef *) IWDG_BASE;

/************************************* NVIC ***********************************/
  NVIC_DBG = (NVIC_Type *)  NVIC_BASE;
  SCB_DBG = (SCB_Type *)  SCB_BASE;

/************************************* PWR ************************************/
  PWR_DBG = (PWR_TypeDef *)  PWR_BASE;

/************************************* RCC ************************************/
  RCC_DBG = (RCC_TypeDef *)  RCC_BASE;

/************************************* RTC ************************************/
  RTC_DBG = (RTC_TypeDef *)  RTC_BASE;

/************************************* SDIO ***********************************/
  SDIO_DBG = (SDIO_TypeDef *)  SDIO_BASE;

/************************************* SPI ************************************/
  SPI1_DBG = (SPI_TypeDef *)  SPI1_BASE;

  SPI2_DBG = (SPI_TypeDef *)  SPI2_BASE;

  SPI3_DBG = (SPI_TypeDef *)  SPI3_BASE;

/************************************* SysTick ********************************/
  SysTick_DBG = (SysTick_Type *)  SysTick_BASE;

/************************************* TIM ************************************/
  TIM1_DBG = (TIM_TypeDef *)  TIM1_BASE;

  TIM2_DBG = (TIM_TypeDef *)  TIM2_BASE;

  TIM3_DBG = (TIM_TypeDef *)  TIM3_BASE;

  TIM4_DBG = (TIM_TypeDef *)  TIM4_BASE;

  TIM5_DBG = (TIM_TypeDef *)  TIM5_BASE;

  TIM6_DBG = (TIM_TypeDef *)  TIM6_BASE;

  TIM7_DBG = (TIM_TypeDef *)  TIM7_BASE;

  TIM8_DBG = (TIM_TypeDef *)  TIM8_BASE;
  
  TIM15_DBG = (TIM_TypeDef *)  TIM15_BASE;

  TIM16_DBG = (TIM_TypeDef *)  TIM16_BASE;

  TIM17_DBG = (TIM_TypeDef *)  TIM17_BASE;      

/************************************* USART **********************************/
  USART1_DBG = (USART_TypeDef *) USART1_BASE;

  USART2_DBG = (USART_TypeDef *) USART2_BASE;

  USART3_DBG = (USART_TypeDef *) USART3_BASE;

  UART4_DBG = (USART_TypeDef *) UART4_BASE;

  UART5_DBG = (USART_TypeDef *) UART5_BASE;

/************************************* WWDG ***********************************/
  WWDG_DBG = (WWDG_TypeDef *)  WWDG_BASE;
}

/**
  * @}
  */

/**
  * @}
  */
  
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
