/**
  ******************************************************************************
  * @file    stm8l15x_lcd.h
  * @author  MCD Application Team
  * @version V1.4.0
  * @date    09/24/2010
  * @brief   This file contains all the functions prototypes for the LCD firmware
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
#ifndef __STM8L15x_LCD_H
#define __STM8L15x_LCD_H

/* Includes ------------------------------------------------------------------*/
#include "stm8l15x.h"

/** @addtogroup STM8L15x_StdPeriph_Driver
  * @{
  */

/* Exported variables ------------------------------------------------------- */
/* Exported types ------------------------------------------------------------*/

/** @addtogroup LCD_Exported_Types
  * @{
  */


/**
  * @brief LCD Duty
  * Warning: element values correspond to duty bits position
  */
typedef enum {
  LCD_Duty_Static = (uint8_t)0x00, /*!< Static duty */
  LCD_Duty_1_2    = (uint8_t)0x02, /*!< 1/2 duty    */
  LCD_Duty_1_3    = (uint8_t)0x04, /*!< 1/3 duty    */
  LCD_Duty_1_4    = (uint8_t)0x06, /*!< 1/4 duty    */
  LCD_Duty_1_8    = (uint8_t)0x20  /*!< 1/8 duty    */
} LCD_Duty_TypeDef;

/**
  * @brief LCD Bias
  * Warning: element values correspond to bias bits position
  */
typedef enum {
  LCD_Bias_1_4  = (uint8_t)0x10,  /*!< 1/4 bias  */
  LCD_Bias_1_3  = (uint8_t)0x00,  /*!< 1/3 bias  */
  LCD_Bias_1_2  = (uint8_t)0x01   /*!< 1/2 bias  */
} LCD_Bias_TypeDef;

/**
  * @brief LCD Clock Prescaler
  * Warning: element values correspond to prescaler bits position
  */
typedef enum {
  LCD_Prescaler_1     = (uint8_t)0x00, /*!< CLKprescaler = ClKinput       */
  LCD_Prescaler_2     = (uint8_t)0x10, /*!< CLKprescaler = ClKinput/2     */
  LCD_Prescaler_4     = (uint8_t)0x20, /*!< CLKprescaler = ClKinput/4     */
  LCD_Prescaler_8     = (uint8_t)0x30, /*!< CLKprescaler = ClKinput/8     */
  LCD_Prescaler_16    = (uint8_t)0x40, /*!< CLKprescaler = ClKinput/16    */
  LCD_Prescaler_32    = (uint8_t)0x50, /*!< CLKprescaler = ClKinput/32    */
  LCD_Prescaler_64    = (uint8_t)0x60, /*!< CLKprescaler = ClKinput/64    */
  LCD_Prescaler_128   = (uint8_t)0x70, /*!< CLKprescaler = ClKinput/128   */
  LCD_Prescaler_256   = (uint8_t)0x80, /*!< CLKprescaler = ClKinput/256   */
  LCD_Prescaler_512   = (uint8_t)0x90, /*!< CLKprescaler = ClKinput/512   */
  LCD_Prescaler_1024  = (uint8_t)0xA0, /*!< CLKprescaler = ClKinput/1024  */
  LCD_Prescaler_2048  = (uint8_t)0xB0, /*!< CLKprescaler = ClKinput/2048  */
  LCD_Prescaler_4096  = (uint8_t)0xC0, /*!< CLKprescaler = ClKinput/4096  */
  LCD_Prescaler_8192  = (uint8_t)0xD0, /*!< CLKprescaler = ClKinput/8192  */
  LCD_Prescaler_16384 = (uint8_t)0xE0, /*!< CLKprescaler = ClKinput/16384 */
  LCD_Prescaler_32768 = (uint8_t)0xF0  /*!< CLKprescaler = ClKinput/32768 */
} LCD_Prescaler_TypeDef;

/**
  * @brief LCD Clock divider
  * Warning: element values correspond to divider bits position
  */
typedef enum {
  LCD_Divider_16 = (uint8_t)0x00, /*!< LCD frequency = CLKprescaler/16 */
  LCD_Divider_17 = (uint8_t)0x01, /*!< LCD frequency = CLKprescaler/17 */
  LCD_Divider_18 = (uint8_t)0x02, /*!< LCD frequency = CLKprescaler/18 */
  LCD_Divider_19 = (uint8_t)0x03, /*!< LCD frequency = CLKprescaler/19 */
  LCD_Divider_20 = (uint8_t)0x04, /*!< LCD frequency = CLKprescaler/20 */
  LCD_Divider_21 = (uint8_t)0x05, /*!< LCD frequency = CLKprescaler/21 */
  LCD_Divider_22 = (uint8_t)0x06, /*!< LCD frequency = CLKprescaler/22 */
  LCD_Divider_23 = (uint8_t)0x07, /*!< LCD frequency = CLKprescaler/23 */
  LCD_Divider_24 = (uint8_t)0x08, /*!< LCD frequency = CLKprescaler/24 */
  LCD_Divider_25 = (uint8_t)0x09, /*!< LCD frequency = CLKprescaler/25 */
  LCD_Divider_26 = (uint8_t)0x0A, /*!< LCD frequency = CLKprescaler/26 */
  LCD_Divider_27 = (uint8_t)0x0B, /*!< LCD frequency = CLKprescaler/27 */
  LCD_Divider_28 = (uint8_t)0x0C, /*!< LCD frequency = CLKprescaler/28 */
  LCD_Divider_29 = (uint8_t)0x0D, /*!< LCD frequency = CLKprescaler/29 */
  LCD_Divider_30 = (uint8_t)0x0E, /*!< LCD frequency = CLKprescaler/30 */
  LCD_Divider_31 = (uint8_t)0x0F  /*!< LCD frequency = CLKprescaler/31 */
} LCD_Divider_TypeDef;

/**
  * @brief LCD Maximum voltage Vlcd
  * Warning: element values correspond to contrast bits position
  */
typedef enum {
  LCD_Contrast_2V6 = (uint8_t)0x00, /*!< Maximum Voltage = 2.60V */
  LCD_Contrast_2V7 = (uint8_t)0x02, /*!< Maximum Voltage = 2.70V */
  LCD_Contrast_2V8 = (uint8_t)0x04, /*!< Maximum Voltage = 2.80V */
  LCD_Contrast_2V9 = (uint8_t)0x06, /*!< Maximum Voltage = 2.90V */
  LCD_Contrast_3V0 = (uint8_t)0x08, /*!< Maximum Voltage = 3.00V */
  LCD_Contrast_3V1 = (uint8_t)0x0A, /*!< Maximum Voltage = 3.10V */
  LCD_Contrast_3V2 = (uint8_t)0x0C, /*!< Maximum Voltage = 3.20V */
  LCD_Contrast_3V3 = (uint8_t)0x0E  /*!< Maximum Voltage = 3.30V */
} LCD_Contrast_TypeDef;

/**
  * @brief LCD Voltage Source
  * Warning: element values correspond to voltage source bits position
  */
typedef enum {
  LCD_VoltageSource_Internal = (uint8_t)0x00, /*!< Internal voltage source for the LCD */
  LCD_VoltageSource_External = (uint8_t)0x01  /*!< External voltage source for the LCD */
} LCD_VoltageSource_TypeDef;

/**
  * @brief LCD pulse on duration
  * Warning: element values correspond to pulse on duration bits position
  */
typedef enum {
  LCD_PulseOnDuration_0 = (uint8_t)0x00, /*!< Pulse on duration = 0/CLKprescaler  */
  LCD_PulseOnDuration_1 = (uint8_t)0x20, /*!< Pulse on duration = 1/CLKprescaler  */
  LCD_PulseOnDuration_2 = (uint8_t)0x40, /*!< Pulse on duration = 2/CLKprescaler  */
  LCD_PulseOnDuration_3 = (uint8_t)0x60, /*!< Pulse on duration = 3/CLKprescaler  */
  LCD_PulseOnDuration_4 = (uint8_t)0x80, /*!< Pulse on duration = 4/CLKprescaler  */
  LCD_PulseOnDuration_5 = (uint8_t)0xA0, /*!< Pulse on duration = 5/CLKprescaler  */
  LCD_PulseOnDuration_6 = (uint8_t)0xC0, /*!< Pulse on duration = 6/CLKprescaler  */
  LCD_PulseOnDuration_7 = (uint8_t)0xE0  /*!< Pulse on duration = 7/CLKprescaler  */
} LCD_PulseOnDuration_TypeDef;

/**
  * @brief LCD Dead Time
  * Warning: element values correspond to dead time bits position
  */
typedef enum {
  LCD_DeadTime_0 = (uint8_t)0x00, /*!< No dead Time  */
  LCD_DeadTime_1 = (uint8_t)0x01, /*!< One Phase between different couple of Frame   */
  LCD_DeadTime_2 = (uint8_t)0x02, /*!< Two Phase between different couple of Frame   */
  LCD_DeadTime_3 = (uint8_t)0x03, /*!< Tree Phase between different couple of Frame  */
  LCD_DeadTime_4 = (uint8_t)0x04, /*!< Four Phase between different couple of Frame  */
  LCD_DeadTime_5 = (uint8_t)0x05, /*!< Five Phase between different couple of Frame  */
  LCD_DeadTime_6 = (uint8_t)0x06, /*!< Six Phase between different couple of Frame   */
  LCD_DeadTime_7 = (uint8_t)0x07  /*!< Seven Phase between different couple of Frame */
} LCD_DeadTime_TypeDef;

/**
  * @brief LCD blink
  * Warning: element values correspond to blink mode bits position
  */
typedef enum {
  LCD_BlinkMode_Off           = (uint8_t)0x00, /*!< Blink inactive            */
  LCD_BlinkMode_SEG0_COM0     = (uint8_t)0x40, /*!< SEG0 on COM0 blink        */
  LCD_BlinkMode_SEG0_AllCOM   = (uint8_t)0x80, /*!< SEG0 on All COM blink     */
  LCD_BlinkMode_AllSEG_AllCOM = (uint8_t)0xC0  /*!< All SEG on All COm Blink  */
} LCD_BlinkMode_TypeDef;

/**
  * @brief LCD blink
  * Warning: element values correspond to blink frequency bits position
  */
typedef enum {
  LCD_BlinkFrequency_Div8    = (uint8_t)0x00, /*!< The Blink frequency = fLcd/8    */
  LCD_BlinkFrequency_Div16   = (uint8_t)0x08, /*!< The Blink frequency = fLcd/16   */
  LCD_BlinkFrequency_Div32   = (uint8_t)0x10, /*!< The Blink frequency = fLcd/32   */
  LCD_BlinkFrequency_Div64   = (uint8_t)0x18, /*!< The Blink frequency = fLcd/64   */
  LCD_BlinkFrequency_Div128  = (uint8_t)0x20, /*!< The Blink frequency = fLcd/128  */
  LCD_BlinkFrequency_Div256  = (uint8_t)0x28, /*!< The Blink frequency = fLcd/256  */
  LCD_BlinkFrequency_Div512  = (uint8_t)0x30, /*!< The Blink frequency = fLcd/512  */
  LCD_BlinkFrequency_Div1024 = (uint8_t)0x38  /*!< The Blink frequency = fLcd/1024 */
} LCD_BlinkFrequency_TypeDef;

/**
  * @brief LCD RAM Register index
  */
typedef enum {
  LCD_RAMRegister_0   = (uint8_t)0x00,  /*!< RAM Register 0  */
  LCD_RAMRegister_1   = (uint8_t)0x01,  /*!< RAM Register 1  */
  LCD_RAMRegister_2   = (uint8_t)0x02,  /*!< RAM Register 2  */
  LCD_RAMRegister_3   = (uint8_t)0x03,  /*!< RAM Register 3  */
  LCD_RAMRegister_4   = (uint8_t)0x04,  /*!< RAM Register 4  */
  LCD_RAMRegister_5   = (uint8_t)0x05,  /*!< RAM Register 5  */
  LCD_RAMRegister_6   = (uint8_t)0x06,  /*!< RAM Register 6  */
  LCD_RAMRegister_7   = (uint8_t)0x07,  /*!< RAM Register 7  */
  LCD_RAMRegister_8   = (uint8_t)0x08,  /*!< RAM Register 8  */
  LCD_RAMRegister_9   = (uint8_t)0x09,  /*!< RAM Register 9  */
  LCD_RAMRegister_10  = (uint8_t)0x0A,  /*!< RAM Register 10 */
  LCD_RAMRegister_11  = (uint8_t)0x0B,  /*!< RAM Register 11 */
  LCD_RAMRegister_12  = (uint8_t)0x0C,  /*!< RAM Register 12 */
  LCD_RAMRegister_13  = (uint8_t)0x0D,  /*!< RAM Register 13 */
  LCD_RAMRegister_14  = (uint8_t)0x0E,  /*!< RAM Register 14 */
  LCD_RAMRegister_15  = (uint8_t)0x0F,  /*!< RAM Register 15 */
  LCD_RAMRegister_16  = (uint8_t)0x10,  /*!< RAM Register 16 */
  LCD_RAMRegister_17  = (uint8_t)0x11,  /*!< RAM Register 17 */
  LCD_RAMRegister_18  = (uint8_t)0x12,  /*!< RAM Register 18 */
  LCD_RAMRegister_19  = (uint8_t)0x13,  /*!< RAM Register 19 */
  LCD_RAMRegister_20  = (uint8_t)0x14,  /*!< RAM Register 20 */
  LCD_RAMRegister_21  = (uint8_t)0x15   /*!< RAM Register 21 */
} LCD_RAMRegister_TypeDef;

/**
  * @brief LCD RAM Register index
  */
typedef enum {
  LCD_PortMaskRegister_0 = (uint8_t)0x00,  /*!< PortMask Register 0  */
  LCD_PortMaskRegister_1 = (uint8_t)0x01,  /*!< PortMask Register 1  */
  LCD_PortMaskRegister_2 = (uint8_t)0x02,  /*!< PortMask Register 2  */
  LCD_PortMaskRegister_3 = (uint8_t)0x03,  /*!< PortMask Register 3  */
  LCD_PortMaskRegister_4 = (uint8_t)0x04,  /*!< PortMask Register 4  */
  LCD_PortMaskRegister_5 = (uint8_t)0x05   /*!< PortMask Register 5  */
} LCD_PortMaskRegister_TypeDef;

/**
  * @brief LCD Page select
  */
typedef enum {
  LCD_PageSelection_FirstPage  = (uint8_t)0x00,  /*!< The LCD RAM is selected as the first page  */
  LCD_PageSelection_SecondPage = (uint8_t)0x04   /*!< The LCD RAM is selected as the second page */
} LCD_PageSelection_TypeDef;


/**
  * @}
  */

/* Private define ------------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/

/** @addtogroup LCD_Private_Macros
  * @brief Macros used by the assert_param function to check the different functions parameters.
  * @{
  */


/**
  * @brief Macro used by the assert_param function in order to check the clock prescaler values
  */
#define IS_LCD_CLOCK_PRESCALER(PRESCALER) (((PRESCALER) == LCD_Prescaler_1) || \
    ((PRESCALER) == LCD_Prescaler_2) || \
    ((PRESCALER) == LCD_Prescaler_4) || \
    ((PRESCALER) == LCD_Prescaler_8) || \
    ((PRESCALER) == LCD_Prescaler_16) || \
    ((PRESCALER) == LCD_Prescaler_32) || \
    ((PRESCALER) == LCD_Prescaler_64) || \
    ((PRESCALER) == LCD_Prescaler_128) || \
    ((PRESCALER) == LCD_Prescaler_256) || \
    ((PRESCALER) == LCD_Prescaler_512) || \
    ((PRESCALER) == LCD_Prescaler_1024) || \
    ((PRESCALER) == LCD_Prescaler_2048) || \
    ((PRESCALER) == LCD_Prescaler_4096) || \
    ((PRESCALER) == LCD_Prescaler_8192) || \
    ((PRESCALER) == LCD_Prescaler_16384) || \
    ((PRESCALER) == LCD_Prescaler_32768))

/**
  * @brief Macro used by the assert_param function in order to check the clock divider values
  */
#define IS_LCD_CLOCK_DIVIDER(DIVIDER) (((DIVIDER) == LCD_Divider_16) || \
                                       ((DIVIDER) == LCD_Divider_17) || \
                                       ((DIVIDER) == LCD_Divider_18) || \
                                       ((DIVIDER) == LCD_Divider_19) || \
                                       ((DIVIDER) == LCD_Divider_20) || \
                                       ((DIVIDER) == LCD_Divider_21) || \
                                       ((DIVIDER) == LCD_Divider_22) || \
                                       ((DIVIDER) == LCD_Divider_23) || \
                                       ((DIVIDER) == LCD_Divider_24) || \
                                       ((DIVIDER) == LCD_Divider_25) || \
                                       ((DIVIDER) == LCD_Divider_26) || \
                                       ((DIVIDER) == LCD_Divider_27) || \
                                       ((DIVIDER) == LCD_Divider_28) || \
                                       ((DIVIDER) == LCD_Divider_29) || \
                                       ((DIVIDER) == LCD_Divider_30) || \
                                       ((DIVIDER) == LCD_Divider_31))

/**
  * @brief Macro used by the assert_param function in order to check the duty values
  */
#define IS_LCD_DUTY(DUTY) (((DUTY) == LCD_Duty_Static) || \
                           ((DUTY) == LCD_Duty_1_2) || \
                           ((DUTY) == LCD_Duty_1_3) || \
                           ((DUTY) == LCD_Duty_1_4) || \
                           ((DUTY) == LCD_Duty_1_8))
/**
  * @brief Macro used by the assert_param function in order to check the Bias values
  */
#define IS_LCD_BIAS(BIAS) (((BIAS) == LCD_Bias_1_4) || \
                           ((BIAS) == LCD_Bias_1_3) || \
                           ((BIAS) == LCD_Bias_1_2))
/**
  * @brief Macro used by the assert_param function in order to check the contrast values
  */
#define IS_LCD_CONTRAST(CONTRAST) (((CONTRAST) == LCD_Contrast_2V6) || \
                                   ((CONTRAST) == LCD_Contrast_2V7) || \
                                   ((CONTRAST) == LCD_Contrast_2V8) || \
                                   ((CONTRAST) == LCD_Contrast_2V9) || \
                                   ((CONTRAST) == LCD_Contrast_3V0) || \
                                   ((CONTRAST) == LCD_Contrast_3V1) || \
                                   ((CONTRAST) == LCD_Contrast_3V2) || \
                                   ((CONTRAST) == LCD_Contrast_3V3))

/**
  * @brief Macro used by the assert_param function in order to check the voltage source values
  */
#define IS_LCD_VOLTAGE_SOURCE(SOURCE) (((SOURCE) == LCD_VoltageSource_Internal) || \
                                       ((SOURCE) == LCD_VoltageSource_External))

/**
  * @brief Macro used by the assert_param function in order to check the pulse duration values
  */
#define IS_LCD_PULSE_DURATION(DURATION) (((DURATION) == LCD_PulseOnDuration_0) || \
    ((DURATION) == LCD_PulseOnDuration_1) || \
    ((DURATION) == LCD_PulseOnDuration_2) || \
    ((DURATION) == LCD_PulseOnDuration_3) || \
    ((DURATION) == LCD_PulseOnDuration_4) || \
    ((DURATION) == LCD_PulseOnDuration_5) || \
    ((DURATION) == LCD_PulseOnDuration_6) || \
    ((DURATION) == LCD_PulseOnDuration_7))

/**
  * @brief Macro used by the assert_param function in order to check the dead time values
  */
#define IS_LCD_DEAD_TIME(TIME) (((TIME) == LCD_DeadTime_0) || \
                                ((TIME) == LCD_DeadTime_1) || \
                                ((TIME) == LCD_DeadTime_2) || \
                                ((TIME) == LCD_DeadTime_3) || \
                                ((TIME) == LCD_DeadTime_4) || \
                                ((TIME) == LCD_DeadTime_5) || \
                                ((TIME) == LCD_DeadTime_6) || \
                                ((TIME) == LCD_DeadTime_7))

/**
  * @brief Macro used by the assert_param function in order to check the blink mode values
  */
#define IS_LCD_BLINK_MODE(BLINK) (((BLINK) == LCD_BlinkMode_Off) || \
                                  ((BLINK) == LCD_BlinkMode_SEG0_COM0) || \
                                  ((BLINK) == LCD_BlinkMode_SEG0_AllCOM) || \
                                  ((BLINK) == LCD_BlinkMode_AllSEG_AllCOM))

/**
  * @brief Macro used by the assert_param function in order to check the blink frequency values
  */
#define IS_LCD_BLINK_FREQUENCY(BLINKF) (((BLINKF) == LCD_BlinkFrequency_Div8) || \
                                        ((BLINKF) == LCD_BlinkFrequency_Div16) || \
                                        ((BLINKF) == LCD_BlinkFrequency_Div32) || \
                                        ((BLINKF) == LCD_BlinkFrequency_Div64) || \
                                        ((BLINKF) == LCD_BlinkFrequency_Div128) || \
                                        ((BLINKF) == LCD_BlinkFrequency_Div256) || \
                                        ((BLINKF) == LCD_BlinkFrequency_Div512) || \
                                        ((BLINKF) == LCD_BlinkFrequency_Div1024))

/**
  * @brief Macro used by the assert_param function in order to check the RAM register values
  */
#define IS_LCD_RAM_REGISTER(REGISTER) (((REGISTER) == LCD_RAMRegister_0) || \
                                       ((REGISTER) == LCD_RAMRegister_1) || \
                                       ((REGISTER) == LCD_RAMRegister_2) || \
                                       ((REGISTER) == LCD_RAMRegister_3) || \
                                       ((REGISTER) == LCD_RAMRegister_4) || \
                                       ((REGISTER) == LCD_RAMRegister_5) || \
                                       ((REGISTER) == LCD_RAMRegister_6) || \
                                       ((REGISTER) == LCD_RAMRegister_7) || \
                                       ((REGISTER) == LCD_RAMRegister_8) || \
                                       ((REGISTER) == LCD_RAMRegister_9) || \
                                       ((REGISTER) == LCD_RAMRegister_10) || \
                                       ((REGISTER) == LCD_RAMRegister_11) || \
                                       ((REGISTER) == LCD_RAMRegister_12) || \
                                       ((REGISTER) == LCD_RAMRegister_13) || \
                                       ((REGISTER) == LCD_RAMRegister_14) || \
                                       ((REGISTER) == LCD_RAMRegister_15) || \
                                       ((REGISTER) == LCD_RAMRegister_16) || \
                                       ((REGISTER) == LCD_RAMRegister_17) || \
                                       ((REGISTER) == LCD_RAMRegister_18) || \
                                       ((REGISTER) == LCD_RAMRegister_19) || \
                                       ((REGISTER) == LCD_RAMRegister_20) || \
                                       ((REGISTER) == LCD_RAMRegister_21))


/**
  * @brief Macro used by the assert_param function in order to check the port mask register values.
  */
#define IS_LCD_PORT_MASK(MASK) (((MASK) == LCD_PortMaskRegister_0) || \
                                ((MASK) == LCD_PortMaskRegister_1) || \
                                ((MASK) == LCD_PortMaskRegister_2) || \
                                ((MASK) == LCD_PortMaskRegister_3) || \
                                ((MASK) == LCD_PortMaskRegister_4) || \
                                ((MASK) == LCD_PortMaskRegister_5))

/**
  * @brief Macro used by the assert_param function in order to check the selected page values.
  */
#define IS_LCD_PAGE_SELECT(PAGE) (((PAGE) == LCD_PageSelection_FirstPage) || \
                                  ((PAGE) == LCD_PageSelection_SecondPage))

/**
  * @}
  */

/* Exported functions ------------------------------------------------------- */

/** @addtogroup LCD_Exported_Functions
  * @{
  */
void LCD_DeInit(void);
void LCD_Init(LCD_Prescaler_TypeDef LCD_Prescaler,
              LCD_Divider_TypeDef LCD_Divider,
              LCD_Duty_TypeDef LCD_Duty,
              LCD_Bias_TypeDef LCD_Bias,
              LCD_VoltageSource_TypeDef LCD_VoltageSource);
void LCD_Cmd(FunctionalState NewState);
void LCD_ITConfig(FunctionalState NewState);
void LCD_HighDriveCmd(FunctionalState NewState);
void LCD_PulseOnDurationConfig(LCD_PulseOnDuration_TypeDef LCD_PulseOnDuration);
void LCD_DeadTimeConfig(LCD_DeadTime_TypeDef LCD_DeadTime);
void LCD_ContrastConfig(LCD_Contrast_TypeDef LCD_Contrast);
void LCD_BlinkConfig(LCD_BlinkMode_TypeDef LCD_BlinkMode,
                     LCD_BlinkFrequency_TypeDef LCD_BlinkFrequency);
void LCD_PortMaskConfig(LCD_PortMaskRegister_TypeDef LCD_PortMaskRegister, uint8_t LCD_Mask);
void LCD_WriteRAM(LCD_RAMRegister_TypeDef LCD_RAMRegister, uint8_t LCD_Data);

enum LCD_WriteRAM_OP{ OP_AND = 0, OP_OR};
void LCD_WriteRAM_withOP(LCD_RAMRegister_TypeDef LCD_RAMRegister, const enum LCD_WriteRAM_OP, const uint8_t LCD_Data);

void LCD_PageSelect(LCD_PageSelection_TypeDef LCD_PageSelection);
FlagStatus LCD_GetFlagStatus(void);
void LCD_ClearFlag(void);
ITStatus LCD_GetITStatus(void);
void LCD_ClearITPendingBit(void);

/**
  * @}
  */

#endif /* __STM8L15x_LCD_H */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
