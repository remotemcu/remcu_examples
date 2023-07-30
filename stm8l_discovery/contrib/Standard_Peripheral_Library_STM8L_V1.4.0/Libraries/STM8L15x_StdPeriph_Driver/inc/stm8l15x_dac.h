/**
  ******************************************************************************
  * @file    stm8l15x_dac.h
  * @author  MCD Application Team
  * @version V1.4.0
  * @date    09/24/2010
  * @brief   This file contains all the functions prototypes for the DAC firmware
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
#ifndef __STM8L15x_DAC_H
#define __STM8L15x_DAC_H

/* Includes ------------------------------------------------------------------*/
#include "stm8l15x.h"

/** @addtogroup STM8L15x_StdPeriph_Driver
  * @{
  */

/* Exported variables ------------------------------------------------------- */
/* Exported types ------------------------------------------------------------*/

/** @addtogroup DAC_Exported_Types
  * @{
  */

/**
  * @brief  DAC trigger selection
  */
typedef enum
{
  DAC_Trigger_None     = ((uint8_t)0x30), /*!< DAC trigger None  */
  DAC_Trigger_T4_TRGO  = ((uint8_t)0x00), /*!< DAC trigger TIM4 TRGO  */
  DAC_Trigger_T5_TRGO  = ((uint8_t)0x08), /*!< DAC trigger TIM5 TRGO  */
  DAC_Trigger_Ext      = ((uint8_t)0x10), /*!< DAC trigger External Trigger (PE4) */
  DAC_Trigger_Software = ((uint8_t)0x38)  /*!< DAC trigger software  */
}DAC_Trigger_TypeDef;

/**
  * @brief  DAC data alignement
  */
typedef enum
{
  DAC_Align_12b_R = ((uint8_t)0x00), /*!< DAC alignement  Right 12bit */
  DAC_Align_12b_L = ((uint8_t)0x04), /*!< DAC alignement  Left 12bit */
  DAC_Align_8b_R  = ((uint8_t)0x08)  /*!< DAC alignement  Right 8bit */
}DAC_Align_TypeDef;

/**
  * @brief  DAC Channel selection
  */
typedef enum
{
  DAC_Channel_1 = ((uint8_t)0x00), /*!< DAC Channel 1 selection  */
  DAC_Channel_2 = ((uint8_t)0x01)  /*!< DAC Channel 2 selection  */
}DAC_Channel_TypeDef;


/**
  * @brief  DAC wave generation
  */
typedef enum
{
  DAC_Wave_Noise    = ((uint8_t)0x40), /*!< Noise Wave Generation  */
  DAC_Wave_Triangle = ((uint8_t)0x80)  /*!< Triangle Wave Generation */
}DAC_Wave_TypeDef;
/**
  * @brief   DAC output buffer state
  */
typedef enum
{
  DAC_OutputBuffer_Enable  = ((uint8_t)0x00), /*!< DAC output buffer Enabled  */
  DAC_OutputBuffer_Disable = ((uint8_t)0x02)  /*!< DAC output buffer Disabled  */
}DAC_OutputBuffer_TypeDef;

/**
  * @brief  DAC interrupt definition
  */
typedef enum
{
  DAC_IT_DMAUDR = ((uint8_t)0x20) /*!< DMA Underrun Interrupt */
}DAC_IT_TypeDef;

/**
  * @brief  DAC Flag definition
  */
typedef enum
{
  DAC_FLAG_DMAUDR = ((uint8_t)0x01) /*!< DMA Underrun flag  */
}DAC_FLAG_TypeDef;

/**
  * @brief  DAC Trinagle Amplitude definitions
  */
typedef enum
{
  DAC_LFSRUnmask_Bit0                    =  ((uint8_t)0x00), /*!<  Noise LFSR Unmask 1 LSB */
  DAC_LFSRUnmask_Bits1_0                 =  ((uint8_t)0x01), /*!<  Noise LFSR Unmask 2 LSB */
  DAC_LFSRUnmask_Bits2_0                 =  ((uint8_t)0x02), /*!<  Noise LFSR Unmask 3 LSB */
  DAC_LFSRUnmask_Bits3_0                 =  ((uint8_t)0x03), /*!<  Noise LFSR Unmask 4 LSB */
  DAC_LFSRUnmask_Bits4_0                 =  ((uint8_t)0x04), /*!<  Noise LFSR Unmask 5 LSB */
  DAC_LFSRUnmask_Bits5_0                 =  ((uint8_t)0x05), /*!<  Noise LFSR Unmask 6 LSB */
  DAC_LFSRUnmask_Bits6_0                 =  ((uint8_t)0x06), /*!<  Noise LFSR Unmask 7 LSB */
  DAC_LFSRUnmask_Bits7_0                 =  ((uint8_t)0x07), /*!<  Noise LFSR Unmask 8 LSB */
  DAC_LFSRUnmask_Bits8_0                 =  ((uint8_t)0x08), /*!<  Noise LFSR Unmask 9 LSB */
  DAC_LFSRUnmask_Bits9_0                 =  ((uint8_t)0x09), /*!<  Noise LFSR Unmask 10 LSB */
  DAC_LFSRUnmask_Bits10_0                =  ((uint8_t)0x0A), /*!<  Noise LFSR Unmask 11 LSB */
  DAC_LFSRUnmask_Bits11_0                =  ((uint8_t)0x0B) /*!<  Noise LFSR Unmask 12 LSB */
}DAC_LFSRUnmask_TypeDef;

/**
  * @brief  DAC Trinagle Amplitude definitions
  */
typedef enum
{
  DAC_TriangleAmplitude_1                =  ((uint8_t)0x00), /*!<  Triangle Amplitude = Vref.(1/4096)*/
  DAC_TriangleAmplitude_3                =  ((uint8_t)0x01), /*!<  Triangle Amplitude = Vref.(3/4096)*/
  DAC_TriangleAmplitude_7                =  ((uint8_t)0x02), /*!<  Triangle Amplitude = Vref.(7/4096)*/
  DAC_TriangleAmplitude_15               =  ((uint8_t)0x03), /*!<  Triangle Amplitude = Vref.(15/4096)*/
  DAC_TriangleAmplitude_31               =  ((uint8_t)0x04), /*!<  Triangle Amplitude = Vref.(31/4096)*/
  DAC_TriangleAmplitude_63               =  ((uint8_t)0x05), /*!<  Triangle Amplitude = Vref.(63/4096)*/
  DAC_TriangleAmplitude_127              =  ((uint8_t)0x06), /*!<  Triangle Amplitude = Vref.(127/4096)*/
  DAC_TriangleAmplitude_255              =  ((uint8_t)0x07), /*!<  Triangle Amplitude = Vref.(255/4096)*/
  DAC_TriangleAmplitude_511              =  ((uint8_t)0x08), /*!<  Triangle Amplitude = Vref.(511/4096)*/
  DAC_TriangleAmplitude_1023             =  ((uint8_t)0x09), /*!<  Triangle Amplitude = Vref.(1023/4096)*/
  DAC_TriangleAmplitude_2047             =  ((uint8_t)0x0A), /*!<  Triangle Amplitude = Vref.(2047/4096)*/
  DAC_TriangleAmplitude_4095             =  ((uint8_t)0x0B) /*!<  Triangle Amplitude = Vref.(4095/4096)*/
}DAC_TriangleAmplitude_TypeDef;

/**
  * @}
  */


/** @addtogroup DAC_Private_Macros
  * @{
  */
/**
  * @brief Macro used by the assert_param function in order to check the  DAC  Trigger selection
  */
#define IS_DAC_TRIGGER(TRIGGER) (((TRIGGER) == DAC_Trigger_None) || \
                                 ((TRIGGER) == DAC_Trigger_T4_TRGO) || \
                                 ((TRIGGER) == DAC_Trigger_T5_TRGO) || \
                                 ((TRIGGER) == DAC_Trigger_Ext) || \
                                 ((TRIGGER) == DAC_Trigger_Software))

/**
  * @brief Macro used by the assert_param function in order to check the DAC data alignement
  */
#define IS_DAC_ALIGN(ALIGN) (((ALIGN) == DAC_Align_12b_R) || \
                             ((ALIGN) == DAC_Align_12b_L) || \
                             ((ALIGN) == DAC_Align_8b_R))
/**
  * @brief Macro used by the assert_param function in order to check the  DAC Channel selection
  */
#define IS_DAC_CHANNEL(CHANNEL) (((CHANNEL) == DAC_Channel_1) || \
                                 ((CHANNEL) == DAC_Channel_2))


/**
  * @brief Macro used by the assert_param function in order to check the  DAC Buffer state
  */
#define IS_DAC_OUTPUT_BUFFER_STATE(STATE) (((STATE) == DAC_OutputBuffer_Enable) || \
    ((STATE) == DAC_OutputBuffer_Disable))
/**
  * @brief Macro used by the assert_param function in order to check the  DAC  Flag to get status
  */
#define IS_DAC_GET_FLAG(FLAG) (((FLAG) == DAC_FLAG_DMAUDR))

/**
  * @brief Macro used by the assert_param function in order to check the  DAC interrupts
  */
#define IS_DAC_IT(IT) (((IT) == DAC_IT_DMAUDR))

/**
  * @brief Macro used by the assert_param function in order to check the  DAC Flags
  */
#define IS_DAC_FLAG(FLAG) (((FLAG) == DAC_FLAG_DMAUDR))

/**
  * @brief Macro used by the assert_param function in order to check the  DAC Data 8 bit
  */
#define IS_DAC_DATA_08R(DATA) ((DATA) <= 0x00FF)


/**
  * @brief Macro used by the assert_param function in order to check the  DAC Wave form
  */

#define IS_DAC_WAVE(WAVE) (((WAVE) == DAC_Wave_Noise) || \
                           ((WAVE) == DAC_Wave_Triangle))
/**
  * @brief macro used by the assert_param function in order to check the  dac wave form lfsr unmask or triangle amplitude
  */
#define IS_DAC_LFSR_UNMASK_TRIANGLE_AMPLITUDE(VALUE) ((VALUE) <= 0x0F)



/**
  * @}
  */


#define CR1_Offset         ((uint8_t)0x00)
#define CR2_Offset         ((uint8_t)0x01)
#define DCH1RDHRH_Offset   ((uint8_t)0x20)
#define CH1RDHRH_Offset    ((uint8_t)0x08)
#define CH2RDHRH_Offset    ((uint8_t)0x14)


/** @addtogroup DAC_Exported_Functions
  * @{
  */
/* Initialisation and general configuration functions*/
void DAC_DeInit(void);
void DAC_Init(DAC_Channel_TypeDef DAC_Channel,
              DAC_Trigger_TypeDef DAC_Trigger,
              DAC_OutputBuffer_TypeDef DAC_OutputBuffer);
void DAC_Cmd(DAC_Channel_TypeDef DAC_Channel, FunctionalState NewState);
void DAC_ITConfig(DAC_Channel_TypeDef DAC_Channel, DAC_IT_TypeDef DAC_IT,
                  FunctionalState NewState);
void DAC_DMACmd(DAC_Channel_TypeDef DAC_Channel, FunctionalState NewState);

/* Special Wave form generation functions*/
void DAC_WaveGenerationCmd(DAC_Channel_TypeDef DAC_Channel,
                           DAC_Wave_TypeDef DAC_Wave,
                           FunctionalState NewState);
void DAC_NoiseWaveLFSR(DAC_Channel_TypeDef DAC_Channel, DAC_LFSRUnmask_TypeDef DAC_LFSRUnmask);
void DAC_TriangleWaveAmplitude(DAC_Channel_TypeDef DAC_Channel, DAC_TriangleAmplitude_TypeDef DAC_TriangleAmplitude);

/* Single Mode functions*/
void DAC_SoftwareTriggerCmd(DAC_Channel_TypeDef DAC_Channel, FunctionalState NewState);
void DAC_SetChannel1Data(DAC_Align_TypeDef DAC_Align, uint16_t DAC_Data);
void DAC_SetChannel2Data(DAC_Align_TypeDef DAC_Align, uint16_t DAC_Data);

/* Dual Mode functions*/
void DAC_DualSoftwareTriggerCmd(FunctionalState NewState);
void DAC_SetDualChannelData(DAC_Align_TypeDef DAC_Align,
                            uint16_t DAC_Data2,
                            uint16_t DAC_Data1);

/* Single/Dual modes functions */
uint16_t DAC_GetDataOutputValue(DAC_Channel_TypeDef DAC_Channel);

/* Flags and Interrupts functions*/
FlagStatus DAC_GetFlagStatus(DAC_Channel_TypeDef DAC_Channel, DAC_FLAG_TypeDef DAC_FLAG);
void DAC_ClearFlag(DAC_Channel_TypeDef DAC_Channel, DAC_FLAG_TypeDef DAC_FLAG);
ITStatus DAC_GetITStatus(DAC_Channel_TypeDef DAC_Channel, DAC_IT_TypeDef DAC_IT);
void DAC_ClearITPendingBit(DAC_Channel_TypeDef DAC_Channel, DAC_IT_TypeDef DAC_IT);
/**
  * @}
  */

#endif /*__STM8L15x_DAC_H */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
