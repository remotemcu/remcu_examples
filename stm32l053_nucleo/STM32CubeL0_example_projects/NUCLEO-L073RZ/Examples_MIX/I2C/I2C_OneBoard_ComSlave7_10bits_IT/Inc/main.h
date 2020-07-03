/**
  ******************************************************************************
  * @file    Examples_MIX/I2C/I2C_OneBoard_ComSlave7_10bits_IT/Inc/main.h
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
#include "stm32l0xx_nucleo.h"
#include "stm32l0xx_ll_i2c.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* User can use this section to tailor I2Cx/I2Cx instance used and associated
   resources */
/* Definition for I2Cx_MASTER clock resources */
#define I2Cx_MASTER                             I2C3
#define RCC_PERIPHCLK_I2Cx_MASTER               RCC_PERIPHCLK_I2C3
#define RCC_I2Cx_MASTERCLKSOURCE_SYSCLK         RCC_I2C3CLKSOURCE_SYSCLK
#define I2Cx_MASTER_CLK_ENABLE()                __HAL_RCC_I2C3_CLK_ENABLE()
#define I2Cx_MASTER_SDA_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOC_CLK_ENABLE()
#define I2Cx_MASTER_SCL_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOC_CLK_ENABLE() 

#define I2Cx_MASTER_FORCE_RESET()               __HAL_RCC_I2C3_FORCE_RESET()
#define I2Cx_MASTER_RELEASE_RESET()             __HAL_RCC_I2C3_RELEASE_RESET()

/* Definition for I2Cx_MASTER Pins */
#define I2Cx_MASTER_SCL_PIN                     GPIO_PIN_0
#define I2Cx_MASTER_SCL_GPIO_PORT               GPIOC
#define I2Cx_MASTER_SDA_PIN                     GPIO_PIN_1
#define I2Cx_MASTER_SDA_GPIO_PORT               GPIOC
#define I2Cx_MASTER_SCL_SDA_AF                  GPIO_AF7_I2C3

/* Definition for I2Cx_MASTER's NVIC */
#define I2Cx_MASTER_IRQn                        I2C3_IRQn
#define I2Cx_MASTER_IRQHandler                  I2C3_IRQHandler

/* Definition for I2Cx_SLAVE clock resources */
#define I2Cx_SLAVE                              I2C1
#define RCC_PERIPHCLK_I2Cx_SLAVE                RCC_PERIPHCLK_I2C1
#define RCC_I2Cx_SLAVECLKSOURCE_SYSCLK          RCC_I2C1CLKSOURCE_SYSCLK
#define I2Cx_SLAVE_CLK_ENABLE()                 __HAL_RCC_I2C1_CLK_ENABLE()
#define I2Cx_SLAVE_SDA_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOB_CLK_ENABLE()
#define I2Cx_SLAVE_SCL_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOB_CLK_ENABLE() 

#define I2Cx_SLAVE_FORCE_RESET()                __HAL_RCC_I2C1_FORCE_RESET()
#define I2Cx_SLAVE_RELEASE_RESET()              __HAL_RCC_I2C1_RELEASE_RESET()

/* Definition for I2Cx_SLAVE Pins */
#define I2Cx_SLAVE_SCL_PIN                      GPIO_PIN_8
#define I2Cx_SLAVE_SCL_GPIO_PORT                GPIOB
#define I2Cx_SLAVE_SDA_PIN                      GPIO_PIN_9
#define I2Cx_SLAVE_SDA_GPIO_PORT                GPIOB
#define I2Cx_SLAVE_SCL_SDA_AF                   GPIO_AF4_I2C1

/* Definition for I2Cx_SLAVE's NVIC */
#define I2Cx_SLAVE_IRQn                         I2C1_IRQn
#define I2Cx_SLAVE_IRQHandler                   I2C1_IRQHandler

/* Size of Transmission buffer */
#define TXBUFFERSIZE                            (COUNTOF(aTxBuffer) - 1)
/* Size of Reception buffer */
#define RXBUFFERSIZE                            TXBUFFERSIZE

/* Exported macro ------------------------------------------------------------*/
#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
