/**
  ******************************************************************************
  * @file    TSC/TSC_BasicAcquisition_Interrupt/Src/stm32l0xx_hal_msp.c
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
  * @brief TSC MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  * @param htsc: TSC handle pointer
  * @retval None
  */
void HAL_TSC_MspInit(TSC_HandleTypeDef *htsc)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /*##-1- Enable TSC and GPIO clocks #########################################*/
  TSCx_CLK_ENABLE();
  TSCx_TS1_IO_GPIO_CLK_ENABLE();
  TSCx_TS2_IO_GPIO_CLK_ENABLE();
  TSCx_TS3_IO_GPIO_CLK_ENABLE();
  TSCx_SHIELD_GPIO_CLK_ENABLE();

  /*##-2- Configure Sampling Capacitor IOs (Alternate-Function Open-Drain) ###*/
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_MEDIUM;

  /* Shield Sampling Capacitor IO = TSC_GROUP8_IO2 */
  GPIO_InitStruct.Pin       = TSCx_SHIELD_SAMPLING_PIN;
  GPIO_InitStruct.Alternate = TSCx_SHIELD_SAMPLING_AF;
  HAL_GPIO_Init(TSCx_SHIELD_SAMPLING_GPIO_PORT, &GPIO_InitStruct);

  /* Channel TS1 Sampling Capacitor IO = TSC_GROUP6_IO3 */
  GPIO_InitStruct.Pin       = TSCx_TS1_SAMPLING_PIN;
  GPIO_InitStruct.Alternate = TSCx_TS1_SAMPLING_AF;
  HAL_GPIO_Init(TSCx_TS1_SAMPLING_GPIO_PORT, &GPIO_InitStruct);
  
  /* Channel TS2 Sampling Capacitor IO = TSC_GROUP3_IO3 */
  GPIO_InitStruct.Pin       = TSCx_TS2_SAMPLING_PIN;
  GPIO_InitStruct.Alternate = TSCx_TS2_SAMPLING_AF;
  HAL_GPIO_Init(TSCx_TS2_SAMPLING_GPIO_PORT, &GPIO_InitStruct);

  /* Channel TS3 Sampling Capacitor IO = TSC_GROUP1_IO3 */
  GPIO_InitStruct.Pin       = TSCx_TS3_SAMPLING_PIN;
  GPIO_InitStruct.Alternate = TSCx_TS3_SAMPLING_AF;
  HAL_GPIO_Init(TSCx_TS3_SAMPLING_GPIO_PORT, &GPIO_InitStruct);

  /*##-3- Configure Channel & Shield IOs (Alternate-Function Output PP) ######*/
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_MEDIUM;

  /* Shield IO = TSC_GROUP8_IO1 */
  GPIO_InitStruct.Pin       = TSCx_SHIELD_IO_PIN;
  GPIO_InitStruct.Alternate = TSCx_SHIELD_IO_AF;
  HAL_GPIO_Init(TSCx_SHIELD_IO_GPIO_PORT, &GPIO_InitStruct);

  /* Channel IO (TS1) = TSC_GROUP6_IO2 */
  GPIO_InitStruct.Pin       = TSCx_TS1_IO_PIN;
  GPIO_InitStruct.Alternate = TSCx_TS1_IO_AF;
  HAL_GPIO_Init(TSCx_TS2_IO_GPIO_PORT, &GPIO_InitStruct);

  /* Channel IO (TS2) = TSC_GROUP3_IO2 */
  GPIO_InitStruct.Pin       = TSCx_TS2_IO_PIN;
  GPIO_InitStruct.Alternate = TSCx_TS2_IO_AF;
  HAL_GPIO_Init(TSCx_TS2_IO_GPIO_PORT, &GPIO_InitStruct);

  /* Channel IO (TS3) = TSC_GROUP1_IO2 */
  GPIO_InitStruct.Pin       = TSCx_TS3_IO_PIN;
  GPIO_InitStruct.Alternate = TSCx_TS3_IO_AF;
  HAL_GPIO_Init(TSCx_TS3_IO_GPIO_PORT, &GPIO_InitStruct);
 
  /*##-4- Configure the NVIC for TSC #########################################*/
  HAL_NVIC_SetPriority(TSCx_EXTI_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(TSCx_EXTI_IRQn);
}

/**
  * @brief TSC MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO to their default state
  * @param htsc: TSC handle pointer
  * @retval None
  */
void HAL_TSC_MspDeInit(TSC_HandleTypeDef *htsc)
{
  /*##-1- Reset peripherals ##################################################*/
  TSCx_FORCE_RESET();
  TSCx_RELEASE_RESET();

  /*##-2- Disable peripherals and GPIO Clocks ################################*/
  /* De-initialize the TSC GPIO pins */
  HAL_GPIO_DeInit(TSCx_TS1_IO_GPIO_PORT, TSCx_TS1_IO_PIN);
  HAL_GPIO_DeInit(TSCx_TS2_IO_GPIO_PORT, TSCx_TS2_IO_PIN);
  HAL_GPIO_DeInit(TSCx_TS3_IO_GPIO_PORT, TSCx_TS3_IO_PIN);
  HAL_GPIO_DeInit(TSCx_SHIELD_IO_GPIO_PORT, TSCx_SHIELD_IO_PIN);
  HAL_GPIO_DeInit(TSCx_TS1_SAMPLING_GPIO_PORT, TSCx_TS1_SAMPLING_PIN);
  HAL_GPIO_DeInit(TSCx_TS2_SAMPLING_GPIO_PORT, TSCx_TS2_SAMPLING_PIN);
  HAL_GPIO_DeInit(TSCx_TS3_SAMPLING_GPIO_PORT, TSCx_TS3_SAMPLING_PIN);
  HAL_GPIO_DeInit(TSCx_SHIELD_SAMPLING_GPIO_PORT, TSCx_SHIELD_SAMPLING_PIN);
  
  /*##-3- Disable the NVIC for TSC ###########################################*/
  HAL_NVIC_DisableIRQ(TSCx_EXTI_IRQn);
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
