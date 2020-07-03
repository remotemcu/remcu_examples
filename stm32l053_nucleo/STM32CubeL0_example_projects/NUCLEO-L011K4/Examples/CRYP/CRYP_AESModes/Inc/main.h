/**
  ******************************************************************************
  * @file    CRYP/CRYP_AESModes/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"
#include "stm32l0xx_nucleo_32.h"
#include "stdio.h"
#include "string.h"


/* Exported types ------------------------------------------------------------*/
/* COM Types Definition */
typedef enum { COM1 = 0}COM_TypeDef;

/* Exported constants --------------------------------------------------------*/
#define  TIMEOUT_VALUE 0xFFFF



/* Definition for COM port1, connected to USART2 */ 
#define COMn                            1

#define BSP_COM1                       USART2
#define BSP_COM1_CLK_ENABLE()          __HAL_RCC_USART2_CLK_ENABLE()
#define BSP_COM1_CLK_DISABLE()         __HAL_RCC_USART2_CLK_DISABLE()

#define BSP_COM1_FORCE_RESET()         __HAL_RCC_USART2_FORCE_RESET()
#define BSP_COM1_RELEASE_RESET()       __HAL_RCC_USART2_RELEASE_RESET()
  
#define BSP_COM1_TX_PIN                GPIO_PIN_9
#define BSP_COM1_TX_GPIO_PORT          GPIOA
#define BSP_COM1_TX_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOA_CLK_ENABLE()
#define BSP_COM1_TX_GPIO_CLK_DISABLE() __HAL_RCC_GPIOA_CLK_DISABLE()
#define BSP_COM1_TX_AF                 GPIO_AF4_USART2

#define BSP_COM1_RX_PIN                GPIO_PIN_10
#define BSP_COM1_RX_GPIO_PORT          GPIOA
#define BSP_COM1_RX_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOA_CLK_ENABLE()
#define BSP_COM1_RX_GPIO_CLK_DISABLE() __HAL_RCC_GPIOA_CLK_DISABLE()
#define BSP_COM1_RX_AF                 GPIO_AF4_USART2

#define BSP_COM1_IRQn                  USART2_IRQn

#define COMx_CLK_ENABLE(__COM__)        (((__COM__) == COM1) ? BSP_COM1_CLK_ENABLE() : 0)
#define COMx_CLK_DISABLE(__COM__)       (((__COM__) == COM1) ? BSP_COM1_CLK_DISABLE() : 0)

#define COMx_TX_GPIO_CLK_ENABLE(__COM__) do { if(__COM__ == COM1) BSP_COM1_TX_GPIO_CLK_ENABLE(); } while (0)
#define COMx_TX_GPIO_CLK_DISABLE(__COM__) (((__COM__) == COM1) ? BSP_COM1_TX_GPIO_CLK_DISABLE() : 0)

#define COMx_RX_GPIO_CLK_ENABLE(__COM__) do { if(__COM__ == COM1) BSP_COM1_RX_GPIO_CLK_ENABLE(); } while (0)
#define COMx_RX_GPIO_CLK_DISABLE(__COM__) (((__COM__) == COM1) ? BSP_COM1_RX_GPIO_CLK_DISABLE() : 0)



/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void              BSP_COM_Init(COM_TypeDef COM, UART_HandleTypeDef* huart);

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
