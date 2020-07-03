/**
  ******************************************************************************
  * @file    main.h 
  * @author  MCD Application Team
  * @brief   Header for main.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2016 STMicroelectronics International N.V. All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
 extern "C" {
#endif
   
/* Includes ------------------------------------------------------------------*/
#include "stddef.h"

/* EVAL includes component */
#include "stm32l073z_eval.h"
#include "stm32l073z_eval_tsensor.h"
#include "stm32l073z_eval_lcd.h"
#include "stm32l073z_eval_sd.h"
#include "stm32l073z_eval_idd.h"

#include "stm32l0xx_tsensor.h" //change SMB
   
/* FatFs includes component */
#include "ff_gen_drv.h"
#include "sd_diskio.h"

/* Exported defines  -----------------------------------------------------------*/

/* RTC defines */
#define RTC_ASYNCH_PREDIV            0x7F
#define RTC_SYNCH_PREDIV             0x00FF

/* IDD defines */
#define   IDD_STATE_UNCHANGED       0
#define   IDD_MEASURE_COMPLETED     1
#define   IDD_ERR                   2

/* Priority defines */

#define IT_PRIO_SMBUS                 2
#define IT_PRIO_LPUART1               2
#define IT_PRIO_DMA_LPUART1           2
#define IT_PRIO_RTC                   2
#define IT_PRIO_LPTIM                 0
#define IT_PRIO_SYS_TICK              1


typedef enum {
  CONFIG_LC_SENSOR_LOW_POWER,
  CONFIG_LC_SENSOR_STD,
  CONFIG_LC_SENSOR_OFF,
  
} LcSensorModeTypeEnum;

typedef enum {
      RTC_CLOCK_NONE,
      RTC_CLOCK_LSI,
      RTC_CLOCK_LSE
} rtcConfigClockEnum;

extern LcSensorModeTypeEnum  LCSensorMode;
extern volatile FlagStatus FlagItTamperButton;

//---------------------------------------------------------- 
/* User can use this section to tailor SMBUSx/SMBUSx instance used and associated
   resources */
/* Definition for SMBUSx clock resources */
#define SMBUSx_RCC_PERIPHCLK              RCC_PERIPHCLK_I2C1
#define SMBUSx_CLKSOURCE_SYSCLK           RCC_I2C1CLKSOURCE_SYSCLK
#define SMBUSx_CLK_ENABLE()               __HAL_RCC_I2C1_CLK_ENABLE()
#define SMBUSx_SDA_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()
#define SMBUSx_SCL_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE() 

#define SMBUSx_FORCE_RESET()              __HAL_RCC_I2C1_FORCE_RESET()
#define SMBUSx_RELEASE_RESET()            __HAL_RCC_I2C1_RELEASE_RESET()

/* Definition for SMBUSx Pins */
#define SMBUSx_SCL_PIN                    GPIO_PIN_6
#define SMBUSx_SCL_GPIO_PORT              GPIOB
#define SMBUSx_SDA_PIN                    GPIO_PIN_7
#define SMBUSx_SDA_GPIO_PORT              GPIOB
#define SMBUSx_SCL_SDA_AF                 GPIO_AF1_I2C1
#define SMBUSx_ALERT_PIN                  GPIO_PIN_5
#define SMBUSx_ALERT_GPIO_PORT            GPIOB
#define SMBUSx_ALERT_AF                   GPIO_AF3_I2C1

/* Definition for SMBUSx's NVIC */
#define SMBUSx_IRQn                       I2C1_IRQn
#define SMBUSx_IRQHandler                 I2C1_IRQHandler

//---------------------------------------------------------- 
/* User can use this section to tailor USARTx/UARTx instance used and associated
   resources */
/* Definition for USARTx clock resources */
#define USARTx                           LPUART1
#define USARTx_CLK_ENABLE()              __HAL_RCC_LPUART1_CLK_ENABLE()
#define DMAx_CLK_ENABLE()                __HAL_RCC_DMA1_CLK_ENABLE()
#define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOD_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOD_CLK_ENABLE()

#define USARTx_FORCE_RESET()             __HAL_RCC_LPUART1_FORCE_RESET()
#define USARTx_RELEASE_RESET()           __HAL_RCC_LPUART1_RELEASE_RESET()

/* Definition for USARTx Pins */
#define USARTx_TX_PIN                    GPIO_PIN_8
#define USARTx_TX_GPIO_PORT              GPIOD
#define USARTx_TX_AF                     GPIO_AF0_LPUART1
#define USARTx_RX_PIN                    GPIO_PIN_9
#define USARTx_RX_GPIO_PORT              GPIOD
#define USARTx_RX_AF                     GPIO_AF0_LPUART1

/* Definition for USARTx's DMA */
#define USARTx_TX_DMA_CHANNEL             DMA1_Channel2
#define USARTx_RX_DMA_CHANNEL             DMA1_Channel3

/* Definition for USARTx's DMA Request */
#define USARTx_TX_DMA_REQUEST             DMA_REQUEST_5
#define USARTx_RX_DMA_REQUEST             DMA_REQUEST_5

/* Definition for USARTx's NVIC */
#define USARTx_DMA_TX_RX_IRQn             DMA1_Channel2_3_IRQn
#define USARTx_DMA_TX_RX_IRQHandler       DMA1_Channel2_3_IRQHandler

/* Definition for USARTx's NVIC */
#define USARTx_IRQn                      LPUART1_IRQn
#define USARTx_IRQHandler                LPUART1_IRQHandler

/* Size of Trasmission buffer */
#define TXBUFFERSIZE                      (COUNTOF(aTxBuffer) - 1)
/* Size of Reception buffer */
#define RXBUFFERSIZE                      10

     /* Exported macro ------------------------------------------------------------*/
#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))
/* Exported functions ------------------------------------------------------- */

//----------------------------------------------------------
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void SystemClock_Config_ForLowPower(void);
void SystemClock_ConfigMSI(void);
void SystemClock_DisableMSI(void);

void SystemClock_ConfigHSI_noPLL_16M(void);
void DecodeMFXInterrupt(uint32_t *ptJoyState, uint32_t *ptIddState);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
