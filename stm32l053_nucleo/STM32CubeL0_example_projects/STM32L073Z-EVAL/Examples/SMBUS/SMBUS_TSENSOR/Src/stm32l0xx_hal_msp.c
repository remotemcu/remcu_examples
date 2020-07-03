/**
  ******************************************************************************
  * @file    SMBUS/SMBUS_TSENSOR/Src/stm32l0xx_hal_msp.c
  * @author  MCD Application Team
  * @brief   HAL MSP module.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32L0xx_HAL_Examples
  * @{
  */

/** @defgroup HAL_MSP
  * @brief HAL MSP module.
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup HAL_MSP_Private_Functions
  * @{
  */

/**
  * @brief SMBUS MSP Initialization 
  *        This function configures the hardware resources used in this example: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration  
  *           - NVIC configuration for DMA interrupt request enable
  * @param hsmbus: SMBUS handle pointer
  * @retval None
  */
void HAL_SMBUS_MspInit(SMBUS_HandleTypeDef *hsmbus)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  RCC_PeriphCLKInitTypeDef  RCC_PeriphCLKInitStruct;

  /*##-1- Configure the SMBUS clock source. The clock is derived from the SYSCLK #*/
  RCC_PeriphCLKInitStruct.PeriphClockSelection = SMBUSx_RCC_PERIPHCLK;
  RCC_PeriphCLKInitStruct.I2c1ClockSelection = SMBUSx_CLKSOURCE_SYSCLK;
  HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitStruct);
  
  /*##-2- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO TX/RX clock */
  SMBUSx_SCL_GPIO_CLK_ENABLE();
  SMBUSx_SDA_GPIO_CLK_ENABLE();
  /* Enable SMBUSx clock */
  SMBUSx_CLK_ENABLE(); 

  /*##-3- Configure peripheral GPIO ##########################################*/  
  /* SMBUS TX GPIO pin configuration  */
  GPIO_InitStruct.Pin              = SMBUSx_SCL_PIN;
  GPIO_InitStruct.Mode             = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull             = GPIO_PULLUP;
  GPIO_InitStruct.Speed            = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate        = SMBUSx_SCL_SDA_AF;
  
  HAL_GPIO_Init(SMBUSx_SCL_GPIO_PORT, &GPIO_InitStruct);
    
  /* SMBUS RX GPIO pin configuration  */
  GPIO_InitStruct.Pin              = SMBUSx_SDA_PIN;
  GPIO_InitStruct.Alternate        = SMBUSx_SCL_SDA_AF;
    
  HAL_GPIO_Init(SMBUSx_SDA_GPIO_PORT, &GPIO_InitStruct);
    
  /* SMBUS ALERT GPIO pin configuration  */
  GPIO_InitStruct.Pin              = SMBUSx_ALERT_PIN;
  GPIO_InitStruct.Alternate        = SMBUSx_ALERT_AF;
    
  HAL_GPIO_Init(SMBUSx_ALERT_GPIO_PORT, &GPIO_InitStruct);
    
  /*##-4- Configure the NVIC for SMBUS ########################################*/   
  /* NVIC for SMBUSx */
  HAL_NVIC_SetPriority(SMBUSx_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(SMBUSx_IRQn);
}

/**
  * @brief SMBUS MSP De-Initialization 
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO, DMA and NVIC configuration to their default state
  * @param hsmbus: SMBUS handle pointer
  * @retval None
  */
void HAL_SMBUS_MspDeInit(SMBUS_HandleTypeDef *hsmbus)
{
  
  /*##-1- Reset peripherals ##################################################*/
  SMBUSx_FORCE_RESET();
  SMBUSx_RELEASE_RESET();

  /*##-2- Disable peripherals and GPIO Clocks #################################*/
  /* Configure SMBUS Tx as alternate function  */
  HAL_GPIO_DeInit(SMBUSx_SCL_GPIO_PORT, SMBUSx_SCL_PIN);
  /* Configure SMBUS Rx as alternate function  */
  HAL_GPIO_DeInit(SMBUSx_SDA_GPIO_PORT, SMBUSx_SDA_PIN);
  /* Configure SMBUS Alert as alternate function  */
  HAL_GPIO_DeInit(SMBUSx_ALERT_GPIO_PORT, SMBUSx_ALERT_PIN);
   
  /*##-4- Disable the NVIC for SMBUS ###########################################*/
  HAL_NVIC_DisableIRQ(SMBUSx_IRQn);
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
