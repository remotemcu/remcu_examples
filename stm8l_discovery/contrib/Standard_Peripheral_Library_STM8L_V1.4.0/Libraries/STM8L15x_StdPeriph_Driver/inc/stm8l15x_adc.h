/**
  ******************************************************************************
  * @file    stm8l15x_adc.h
  * @author  MCD Application Team
  * @version V1.4.0
  * @date    09/24/2010
  * @brief   This file contains all the functions prototypes for the ADC firmware
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
#ifndef __STM8L15x_ADC_H
#define __STM8L15x_ADC_H

/* Includes ------------------------------------------------------------------*/
#include "stm8l15x.h"

/** @addtogroup STM8L15x_StdPeriph_Driver
  * @{
  */

/* Exported variables ------------------------------------------------------- */
/* Exported types ------------------------------------------------------------*/

/** @addtogroup ADC_Exported_Types
  * @{
  */


/**
  * @brief  ADC Channels definition
  */
typedef enum
{
  ADC_Channel_0           = ((uint16_t)0x0301), /*!< Channel 00 */
  ADC_Channel_1           = ((uint16_t)0x0302), /*!< Channel 01 */
  ADC_Channel_2           = ((uint16_t)0x0304), /*!< Channel 02 */
  ADC_Channel_3           = ((uint16_t)0x0308), /*!< Channel 03 */
  ADC_Channel_4           = ((uint16_t)0x0310), /*!< Channel 04 */
  ADC_Channel_5           = ((uint16_t)0x0320), /*!< Channel 05 */
  ADC_Channel_6           = ((uint16_t)0x0340), /*!< Channel 06 */
  ADC_Channel_7           = ((uint16_t)0x0380), /*!< Channel 07 */

  ADC_Channel_8           = ((uint16_t)0x0201), /*!< Channel 08 */
  ADC_Channel_9           = ((uint16_t)0x0202), /*!< Channel 09*/
  ADC_Channel_10          = ((uint16_t)0x0204), /*!< Channel 10 */
  ADC_Channel_11          = ((uint16_t)0x0208), /*!< Channel 11 */
  ADC_Channel_12          = ((uint16_t)0x0210), /*!< Channel 12 */
  ADC_Channel_13          = ((uint16_t)0x0220), /*!< Channel 13 */
  ADC_Channel_14          = ((uint16_t)0x0240), /*!< Channel 14 */
  ADC_Channel_15          = ((uint16_t)0x0280), /*!< Channel 15 */

  ADC_Channel_16          = ((uint16_t)0x0101), /*!< Channel 16 */
  ADC_Channel_17          = ((uint16_t)0x0102), /*!< Channel 17 */
  ADC_Channel_18          = ((uint16_t)0x0104), /*!< Channel 18 */
  ADC_Channel_19          = ((uint16_t)0x0108), /*!< Channel 19 */
  ADC_Channel_20          = ((uint16_t)0x0110), /*!< Channel 20 */
  ADC_Channel_21          = ((uint16_t)0x0120), /*!< Channel 21 */
  ADC_Channel_22          = ((uint16_t)0x0140), /*!< Channel 22 */
  ADC_Channel_23          = ((uint16_t)0x0180), /*!< Channel 23 */

  ADC_Channel_24          = ((uint16_t)0x0001), /*!< Channel 24 */
  ADC_Channel_25          = ((uint16_t)0x0002), /*!< Channel 25 */
  ADC_Channel_26          = ((uint16_t)0x0004), /*!< Channel 26 */
  ADC_Channel_27          = ((uint16_t)0x0008), /*!< Channel 27 */

  ADC_Channel_Vrefint     = ((uint16_t)0x0010), /*!< Vrefint  Channel  */
  ADC_Channel_TempSensor  = ((uint16_t)0x0020), /*!< Temperature sensor  Channel  */

  /* combination*/
  ADC_Channel_00To07      = ((uint16_t)0x03FF), /*!<select from channel00 to channel07 */
  ADC_Channel_08To15      = ((uint16_t)0x02FF), /*!<select from channel08 to channel15 */
  ADC_Channel_16To23      = ((uint16_t)0x01FF), /*!<select from channel16 to channel23 */
  ADC_Channel_24To27      = ((uint16_t)0x00FF) /*!<select from channel24 to channel27 : Fast channels */

}ADC_Channel_TypeDef;

/**
  * @brief  ADC Conversation Mode
  */
typedef enum
{
  ADC_ConversionMode_Single     = ((uint8_t)0x00), /*!< Single Conversion Mode */
  ADC_ConversionMode_Continuous = ((uint8_t)0x04)  /*!< Continuous Conversion Mode */
}ADC_ConversionMode_TypeDef;

/**
  * @brief  ADC Resolution
  */
typedef enum
{
  ADC_Resolution_12Bit = ((uint8_t)0x00), /*!< 12 bit resolution */
  ADC_Resolution_10Bit = ((uint8_t)0x20), /*!< 10 bit resolution */
  ADC_Resolution_8Bit  = ((uint8_t)0x40), /*!< 8 bit resolution */
  ADC_Resolution_6Bit  = ((uint8_t)0x60)  /*!< 6 bit resolution */
}ADC_Resolution_TypeDef;

/**
  * @brief  ADC  Clock Prescaler
  */
typedef enum
{
  ADC_Prescaler_1 = ((uint8_t)0x00), /*!< ADC Clock frequency is divided by 1 */
  ADC_Prescaler_2 = ((uint8_t)0x80)  /*!< ADC Clock frequency is divided by 2 */
}ADC_Prescaler_TypeDef;

/**
  * @brief  ADC  External Trigger Edge Sensitivity
  */
typedef enum
{
  ADC_ExtTRGSensitivity_Rising  = ((uint8_t)0x20), /*!< External Trigger Sensitivity is Rising Edge */
  ADC_ExtTRGSensitivity_Falling = ((uint8_t)0x40), /*!< External Trigger Sensitivity is Falling Edge */
  ADC_ExtTRGSensitivity_All     = ((uint8_t)0x60)  /*!< External Trigger Sensitivity is Falling and Rising Edge */
}ADC_ExtTRGSensitivity_TypeDef;

/**
  * @brief  ADC  External  Event source selection
  */
typedef enum
{
  ADC_ExtEventSelection_None     = ((uint8_t)0x00), /*!< Conversation starts only by software start */
  ADC_ExtEventSelection_Trigger1 = ((uint8_t)0x08), /*!< Trigger 1  Enables conversion */
  ADC_ExtEventSelection_Trigger2 = ((uint8_t)0x10), /*!< Trigger 2 Enables conversion */
  ADC_ExtEventSelection_Trigger3 = ((uint8_t)0x18)  /*!< Trigger 3 Enables conversion */
}ADC_ExtEventSelection_TypeDef;


/**
  * @brief  ADC  Group Channels definition
  */
typedef enum
{
  ADC_Group_SlowChannels = ((uint8_t)0x00), /*!<Slow Channels group(Channel 0 to 23) */
  ADC_Group_FastChannels = ((uint8_t)0x01)  /*!<Fast Channels group (Channel 24, Channel Vrefint, Channel TempSensor) */
}ADC_Group_TypeDef;

/**
  * @brief  ADC  sampling times
  */
typedef enum
{
  ADC_SamplingTime_4Cycles   = ((uint8_t)0x00), /*!< Sampling Time Cycles is 4 */
  ADC_SamplingTime_9Cycles   = ((uint8_t)0x01), /*!< Sampling Time Cycles is  9 */
  ADC_SamplingTime_16Cycles  = ((uint8_t)0x02), /*!< Sampling Time Cycles is  16 */
  ADC_SamplingTime_24Cycles  = ((uint8_t)0x03), /*!< Sampling Time Cycles is  24 */
  ADC_SamplingTime_48Cycles  = ((uint8_t)0x04), /*!< Sampling Time Cycles is  48 */
  ADC_SamplingTime_96Cycles  = ((uint8_t)0x05), /*!< Sampling Time Cycles is  96 */
  ADC_SamplingTime_192Cycles = ((uint8_t)0x06), /*!< Sampling Time Cycles is  192 */
  ADC_SamplingTime_384Cycles = ((uint8_t)0x07)  /*!< Sampling Time Cycles is  384 */
}ADC_SamplingTime_TypeDef;

/**
  * @brief  ADC  Analog WatchDog Channel selection
  */
typedef enum
{
  ADC_AnalogWatchdogSelection_Channel0  = ((uint8_t)0x00), /*!< AWD affected to Channel 0 */
  ADC_AnalogWatchdogSelection_Channel1  = ((uint8_t)0x01), /*!< AWD affected to Channel 1 */
  ADC_AnalogWatchdogSelection_Channel2  = ((uint8_t)0x02), /*!< AWD affected to Channel 2 */
  ADC_AnalogWatchdogSelection_Channel3  = ((uint8_t)0x03), /*!< AWD affected to Channel 3 */
  ADC_AnalogWatchdogSelection_Channel4  = ((uint8_t)0x04), /*!< AWD affected to Channel 4 */
  ADC_AnalogWatchdogSelection_Channel5  = ((uint8_t)0x05), /*!< AWD affected to Channel 5 */
  ADC_AnalogWatchdogSelection_Channel6  = ((uint8_t)0x06), /*!< AWD affected to Channel 6 */
  ADC_AnalogWatchdogSelection_Channel7  = ((uint8_t)0x07), /*!< AWD affected to Channel 7 */
  ADC_AnalogWatchdogSelection_Channel8  = ((uint8_t)0x08), /*!< AWD affected to Channel 8 */
  ADC_AnalogWatchdogSelection_Channel9  = ((uint8_t)0x09), /*!< AWD affected to Channel 9 */
  ADC_AnalogWatchdogSelection_Channel10 = ((uint8_t)0x0A), /*!< AWD affected to Channel 10 */
  ADC_AnalogWatchdogSelection_Channel11 = ((uint8_t)0x0B), /*!< AWD affected to Channel 11 */
  ADC_AnalogWatchdogSelection_Channel12 = ((uint8_t)0x0C), /*!< AWD affected to Channel 12 */
  ADC_AnalogWatchdogSelection_Channel13 = ((uint8_t)0x0D), /*!< AWD affected to Channel 13 */
  ADC_AnalogWatchdogSelection_Channel14 = ((uint8_t)0x0E), /*!< AWD affected to Channel 14 */
  ADC_AnalogWatchdogSelection_Channel15 = ((uint8_t)0x0F), /*!< AWD affected to Channel 15 */
  ADC_AnalogWatchdogSelection_Channel16 = ((uint8_t)0x10), /*!< AWD affected to Channel 16 */
  ADC_AnalogWatchdogSelection_Channel17 = ((uint8_t)0x11), /*!< AWD affected to Channel 17 */
  ADC_AnalogWatchdogSelection_Channel18 = ((uint8_t)0x12), /*!< AWD affected to Channel 18 */
  ADC_AnalogWatchdogSelection_Channel19 = ((uint8_t)0x13), /*!< AWD affected to Channel 19 */
  ADC_AnalogWatchdogSelection_Channel20 = ((uint8_t)0x14), /*!< AWD affected to Channel 20 */
  ADC_AnalogWatchdogSelection_Channel21 = ((uint8_t)0x15), /*!< AWD affected to Channel 21 */
  ADC_AnalogWatchdogSelection_Channel22 = ((uint8_t)0x16), /*!< AWD affected to Channel 22 */
  ADC_AnalogWatchdogSelection_Channel23 = ((uint8_t)0x17), /*!< AWD affected to Channel 23 */

  ADC_AnalogWatchdogSelection_Channel24  = ((uint8_t)0x18), /*!< AWD affected to Channel 24 */
  ADC_AnalogWatchdogSelection_Channel25  = ((uint8_t)0x19), /*!< AWD affected to Channel 25 */
  ADC_AnalogWatchdogSelection_Channel26  = ((uint8_t)0x1A), /*!< AWD affected to Channel 26 */
  ADC_AnalogWatchdogSelection_Channel27  = ((uint8_t)0x1B), /*!< AWD affected to Channel 27 */

  ADC_AnalogWatchdogSelection_Vrefint    = ((uint8_t)0x1C), /*!< AWD affected to Internal Vref Channel  */
  ADC_AnalogWatchdogSelection_TempSensor = ((uint8_t)0x1D)  /*!< AWD affected to Temperature Sensor Channel */
}ADC_AnalogWatchdogSelection_TypeDef;

/**
  * @brief  ADC  interrupts definition
  */
typedef enum
{
  ADC_IT_EOC  = ((uint8_t)0x08), /*!< End of Conversation  Interrupt  */
  ADC_IT_AWD  = ((uint8_t)0x10), /*!< Analog WatchDog  Interrupt  */
  ADC_IT_OVER = ((uint8_t)0x80)  /*!< Over Run Interrupt  */
}ADC_IT_TypeDef;

/**
  * @brief  ADC  Flags definition
  */
typedef enum
{
  ADC_FLAG_EOC  = ((uint8_t)0x01), /*!< End of Conversation  flag  */
  ADC_FLAG_AWD  = ((uint8_t)0x02), /*!< Analog WatchDog  flag  */
  ADC_FLAG_OVER = ((uint8_t)0x04)  /*!< Over Run flag  */
}ADC_FLAG_TypeDef;

/**
  * @}
  */


/** @addtogroup ADC_Private_Macros
  * @{
  */
/**
  * @brief Macro used by the assert_param function in order to check the  ADC Coversion Mode
  */
#define IS_ADC_CONVERSION_MODE(MODE) (((MODE) == ADC_ConversionMode_Single) || \
                                      ((MODE) == ADC_ConversionMode_Continuous))

/**
  * @brief Macro used by the assert_param function in order to check the  ADC Coversion resolution
  */
#define IS_ADC_RESOLUTION(RESOLUTION) (((RESOLUTION) == ADC_Resolution_12Bit) || \
                                       ((RESOLUTION) == ADC_Resolution_10Bit) || \
                                       ((RESOLUTION) == ADC_Resolution_8Bit) || \
                                       ((RESOLUTION) == ADC_Resolution_6Bit))

/**
  * @brief Macro used by the assert_param function in order to check the  ADC Clock prescaler
  */
#define IS_ADC_PRESCALER(PRESCALER) (((PRESCALER) == ADC_Prescaler_1) || \
                                     ((PRESCALER) == ADC_Prescaler_2))

/**
  * @brief Macro used by the assert_param function in order to check the  ADC  External Trigger Edge Sensitivity
  */
#define IS_ADC_EXT_TRG_SENSITIVITY(SENSITIVITY) (((SENSITIVITY) == ADC_ExtTRGSensitivity_Rising) || \
    ((SENSITIVITY) == ADC_ExtTRGSensitivity_Falling) || \
    ((SENSITIVITY) == ADC_ExtTRGSensitivity_All))

/**
  * @brief Macro used by the assert_param function in order to check the  ADC  External  Event source selection
  */
#define IS_ADC_EXT_EVENT_SELECTION(SELECTION) (((SELECTION) == ADC_ExtEventSelection_None) || \
    ((SELECTION) == ADC_ExtEventSelection_Trigger1) || \
    ((SELECTION) == ADC_ExtEventSelection_Trigger2) || \
    ((SELECTION) == ADC_ExtEventSelection_Trigger3))

/**
  * @brief Macro used by the assert_param function in order to check the  ADC Channels group
  */
#define IS_ADC_GROUP(GROUP) (((GROUP) == ADC_Group_SlowChannels) || \
                             ((GROUP) == ADC_Group_FastChannels))
/**
  * @brief Macro used by the assert_param function in order to check the  ADC  sampling times
  */
#define IS_ADC_SAMPLING_TIME_CYCLES(TIME) (((TIME) == ADC_SamplingTime_4Cycles) || \
    ((TIME) == ADC_SamplingTime_9Cycles) || \
    ((TIME) == ADC_SamplingTime_16Cycles) || \
    ((TIME) == ADC_SamplingTime_24Cycles) || \
    ((TIME) == ADC_SamplingTime_48Cycles) || \
    ((TIME) == ADC_SamplingTime_96Cycles) || \
    ((TIME) == ADC_SamplingTime_192Cycles) || \
    ((TIME) == ADC_SamplingTime_384Cycles))

/**
  * @brief Macro used by the assert_param function in order to check the  ADC  Analog WatchDog Channel selection
  */
#define IS_ADC_ANALOGWATCHDOG_SELECTION(CHANNEL) (((CHANNEL) <= 0x1D))
/**
  * @brief Macro used by the assert_param function in order to check the  ADC  interrupt to config
  */
#define IS_ADC_IT(IT) ((((IT) & (uint8_t)0x67) == 0x00) && ((IT) != 0x00))
/**
  * @brief Macro used by the assert_param function in order to check the  ADC  interrupt to get status
  */
#define IS_ADC_GET_IT(IT) (((IT) == ADC_IT_EOC) || ((IT) == ADC_IT_AWD) || \
                           ((IT) == ADC_IT_OVER))
/**
  * @brief Macro used by the assert_param function in order to check the  ADC  Flag to clear
  */
#define IS_ADC_CLEAR_FLAG(FLAG) ((((FLAG) & (uint8_t)0xF8) == 0x00) && ((FLAG) != 0x00))
/**
  * @brief Macro used by the assert_param function in order to check the  ADC  Flag to get status
  */
#define IS_ADC_GET_FLAG(FLAG) (((FLAG) == ADC_FLAG_EOC) || ((FLAG) == ADC_FLAG_AWD) || \
                               ((FLAG) == ADC_FLAG_OVER))
/**
  * @brief Macro used by the assert_param function in order to check the  ADC AWD thresholds
  */
#define IS_ADC_THRESHOLD(THRESHOLD) ((THRESHOLD) <= 0xFFF)

/**
  * @}
  */

/** @addtogroup ADC_Exported_Functions
  * @{
  */
void ADC_DeInit(ADC_TypeDef* ADCx);
void ADC_Init(ADC_TypeDef* ADCx,
              ADC_ConversionMode_TypeDef ADC_ConversionMode,
              ADC_Resolution_TypeDef ADC_Resolution,
              ADC_Prescaler_TypeDef ADC_Prescaler);
void ADC_ChannelCmd(ADC_TypeDef* ADCx, ADC_Channel_TypeDef ADC_Channels,
                    FunctionalState NewState);
void ADC_Cmd(ADC_TypeDef* ADCx, FunctionalState NewState);
void ADC_ITConfig(ADC_TypeDef* ADCx, ADC_IT_TypeDef ADC_IT,  FunctionalState NewState);
void ADC_DMACmd(ADC_TypeDef* ADCx, FunctionalState NewState);
void ADC_TempSensorCmd(FunctionalState NewState);
void ADC_VrefintCmd(FunctionalState NewState);
void ADC_SoftwareStartConv(ADC_TypeDef* ADCx);
void ADC_SamplingTimeConfig(ADC_TypeDef* ADCx, ADC_Group_TypeDef ADC_GroupChannels,
                            ADC_SamplingTime_TypeDef ADC_SamplingTime);
void ADC_SchmittTriggerConfig(ADC_TypeDef* ADCx, ADC_Channel_TypeDef ADC_Channels,
                              FunctionalState NewState);
void ADC_ExternalTrigConfig(ADC_TypeDef* ADCx,
                            ADC_ExtEventSelection_TypeDef ADC_ExtEventSelection,
                            ADC_ExtTRGSensitivity_TypeDef ADC_ExtTRGSensitivity);
uint16_t ADC_GetConversionValue(ADC_TypeDef* ADCx);
void ADC_AnalogWatchdogChannelSelect(ADC_TypeDef* ADCx,
                                     ADC_AnalogWatchdogSelection_TypeDef ADC_AnalogWatchdogSelection);
void ADC_AnalogWatchdogThresholdsConfig(ADC_TypeDef* ADCx, uint16_t HighThreshold,
                                        uint16_t LowThreshold);
void ADC_AnalogWatchdogConfig(ADC_TypeDef* ADCx,
                              ADC_AnalogWatchdogSelection_TypeDef ADC_AnalogWatchdogSelection,
                              uint16_t HighThreshold,
                              uint16_t LowThreshold);
void ADC_AnalogWatchdogSingleChannelConfig(ADC_TypeDef* ADCx, u8 ADC_Channel);
FlagStatus ADC_GetFlagStatus(ADC_TypeDef* ADCx, ADC_FLAG_TypeDef ADC_FLAG);
void ADC_ClearFlag(ADC_TypeDef* ADCx, ADC_FLAG_TypeDef ADC_FLAG);
ITStatus ADC_GetITStatus(ADC_TypeDef* ADCx, ADC_IT_TypeDef ADC_IT);
void ADC_ClearITPendingBit(ADC_TypeDef* ADCx, ADC_IT_TypeDef ADC_IT);

/**
  * @}
  */

#endif /*__STM8L15x_ADC_H */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
