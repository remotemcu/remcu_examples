/**
  ******************************************************************************
  * @file    icc_measure.h
  * @author  Microcontroller Division
  * @version V1.2.0
  * @date    09/2010
  * @brief   Current measurements defines
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
#ifndef __ICC_MEASURE_H
#define __ICC_MEASURE_H

/* Includes ------------------------------------------------------------------*/
#include "stm8l15x.h"
#include "stm8l15x_adc.h"

/* Private define ------------------------------------------------------------*/
#define MCU_RUN		0
#define MCU_LPR		1
#define MCU_HALT	3
#define MCU_LPR_LCD	4

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
u16 ADC_Supply(void);
void ADC_Icc_Init(void);
u16 ADC_Icc_Test(u8 Mcu_State);
void GPIO_LowPower_Config(void);
void delay_ms(u16 n_ms);
void delay_10us(u16 n_10us);
void Delay(uint16_t nCount);

#endif /* __ICC_MEASURE_H*/

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
