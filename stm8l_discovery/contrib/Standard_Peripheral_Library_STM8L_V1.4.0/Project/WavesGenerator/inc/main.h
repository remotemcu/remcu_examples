/**
  ******************************************************************************
  * @file    main.h
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    02/12/2010
  * @brief   This file contains additional peripheral register's definitions, bits 
  *          definitions and memory mapping for STM8L15x devices.  
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

/** @addtogroup STM8L15x_StdPeriph_Driver
  * @{
  */
  
/******************************************************************************/
/*                   Library configuration section                            */
/******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/

#define DAC_CH1RDHRH_ADDRESS      0x5388
#define TIM4_PRESTAB_SIZE         10
#define WAVES_TAB_SIZE            10

/**
  * @brief  DAC trigger selection
  */
typedef enum
{
  DAC_Trigger_None     = ((uint8_t)0x10), /*!< DAC trigger None  */
  DAC_Trigger_T4_TRGO  = ((uint8_t)0x00), /*!< DAC trigger TIM4 TRGO  */
  DAC_Trigger_Software = ((uint8_t)0x38)  /*!< DAC trigger software  */
}DAC_Trigger_TypeDef;


/**
  * @brief   DAC output buffer state
  */
typedef enum
{
  DAC_OutputBuffer_Enable  =((uint8_t)0x00), /*!< DAC output buffer Enabled  */
  DAC_OutputBuffer_Disable =((uint8_t)0x02)  /*!< DAC output buffer Disabled  */
}DAC_OutputBuffer_TypeDef;


/**
  * @brief TIM4 Trigger Output Source
  */
typedef enum
{
    TIM4_TRGOSource_Reset   = ((uint8_t)0x00),  /*!< Trigger Output source = Reset */
    TIM4_TRGOSource_Enable  = ((uint8_t)0x10),  /*!< Trigger Output source = TIM2 is enabled */
    TIM4_TRGOSource_Update  = ((uint8_t)0x20)   /*!< Trigger Output source = Update event */
}TIM4_TRGOSource_TypeDef;


/**
  * @brief TIM4 Prescaler
  */
typedef enum
{
    TIM4_Prescaler_1      = ((uint8_t)0x00),   /*!< Time base Prescaler = 1 (No effect)*/
    TIM4_Prescaler_2      = ((uint8_t)0x01),  /*!< Time base Prescaler = 2 */
    TIM4_Prescaler_4      = ((uint8_t)0x02),  /*!< Time base Prescaler = 4 */
    TIM4_Prescaler_8      = ((uint8_t)0x03),  /*!< Time base Prescaler = 8 */
    TIM4_Prescaler_16     = ((uint8_t)0x04),  /*!< Time base Prescaler = 16 */
    TIM4_Prescaler_32     = ((uint8_t)0x05),  /*!< Time base Prescaler = 32 */
    TIM4_Prescaler_64     = ((uint8_t)0x06),  /*!< Time base Prescaler = 64 */
    TIM4_Prescaler_128    = ((uint8_t)0x07),  /*!< Time base Prescaler = 128 */
    TIM4_Prescaler_256    = ((uint8_t)0x08),  /*!< Time base Prescaler = 256 */
    TIM4_Prescaler_512    = ((uint8_t)0x09),  /*!< Time base Prescaler = 512 */
    TIM4_Prescaler_1024   = ((uint8_t)0x0A),  /*!< Time base Prescaler = 1024 */
    TIM4_Prescaler_2048   = ((uint8_t)0x0B),  /*!< Time base Prescaler = 2048 */
    TIM4_Prescaler_4096   = ((uint8_t)0x0C),  /*!< Time base Prescaler = 4096 */
    TIM4_Prescaler_8192   = ((uint8_t)0x0D),  /*!< Time base Prescaler = 8196 */
    TIM4_Prescaler_16384  = ((uint8_t)0x0E),  /*!< Time base Prescaler = 16384 */
    TIM4_Prescaler_32768  = ((uint8_t)0x0F)   /*!< Time base Prescaler = 32768 */
}TIM4_Prescaler_TypeDef;


/**
  * @brief   DMA memory/periph data size
  */
typedef enum
{
    DMA_MemoryDataSize_Byte     = ((uint8_t)0x00),/*!< Memory Data Size is 1 Byte */
    DMA_MemoryDataSize_HalfWord = ((uint8_t)0x08) /*!< Memory Data Size is 2 Bytes */
}DMA_MemoryDataSize_TypeDef;


/**
  * @brief DMA priority level  
  */
typedef enum
{
    DMA_Priority_Low      = ((uint8_t)0x00), /*!< Software Priority is Low */
    DMA_Priority_Medium   = ((uint8_t)0x10), /*!< Software Priority is Medium */
    DMA_Priority_High     = ((uint8_t)0x20), /*!< Software Priority is High */
    DMA_Priority_VeryHigh = ((uint8_t)0x30)  /*!< Software Priority is VeryHigh */
}DMA_Priority_TypeDef;


/**
  * @brief DMA memory incremented mode
  */
typedef enum
{
  DMA_MemoryIncMode_Dec = ((uint8_t)0x00), /*!< DMA memory incremented mode is decremantal */
  DMA_MemoryIncMode_Inc = ((uint8_t)0x20)  /*!< DMA memory incremented mode is incremantal */
}DMA_MemoryIncMode_TypeDef;


/**
  * @brief DMA circular/normal buffer mode 
  */
typedef enum
{
    DMA_Mode_Normal = ((uint8_t)0x00),/*!< DMA normal buffer mode*/
  DMA_Mode_Circular = ((uint8_t)0x10) /*!< DMA circular buffer mode */
}DMA_Mode_TypeDef;


/**
  * @brief DMA data transfer direction 
  */
typedef enum
{
  DMA_DIR_PeripheralToMemory  = ((uint8_t)0x00), /*!< Data transfer direction is Peripheral To Memory   */
  DMA_DIR_MemoryToPeripheral  = ((uint8_t)0x08), /*!< Data transfer direction is Memory To Peripheral */
  DMA_DIR_Memory0ToMemory1    = ((uint8_t)0x40) /*!< Data transfer direction is Memory0 To Memory 1 */
}DMA_DIR_TypeDef;



/**
  * @}
  */


/**
  * @}
  */
    
/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
