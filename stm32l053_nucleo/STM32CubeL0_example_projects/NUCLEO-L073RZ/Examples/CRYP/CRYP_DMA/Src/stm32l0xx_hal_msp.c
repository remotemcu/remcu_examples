/**
  ******************************************************************************
  * @file    CRYP/CRYP_DMA/Src/stm32l0xx_hal_msp.c
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
  * @brief CRYP MSP Initialization 
  *        This function configures the hardware resources used in this example: 
  *           - DMA configuration for transmission request by peripheral 
  *           - NVIC configuration for DMA interrupt request enable
  * @param hcryp: CRYP handle pointer
  * @retval None
  */
void HAL_CRYP_MspInit(CRYP_HandleTypeDef *hcryp)
{
  static  DMA_HandleTypeDef   hdmaIn;
  static  DMA_HandleTypeDef   hdmaOut;

  /* Enable CRYP clock */
  __HAL_RCC_AES_CLK_ENABLE();

  /* Force the CRYP Periheral Clock Reset */  
  __HAL_RCC_AES_FORCE_RESET(); 
  /* Release the CRYP Periheral Clock Reset */  
  __HAL_RCC_AES_RELEASE_RESET();

  /* Enable and set CRYP Interrupt to the highest priority */
  HAL_NVIC_SetPriority(AES_RNG_LPUART1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(AES_RNG_LPUART1_IRQn);
  
    /* Enable DMA1 clocks */
  __HAL_RCC_DMA1_CLK_ENABLE();
  __HAL_RCC_DMA1_FORCE_RESET();
  __HAL_RCC_DMA1_RELEASE_RESET();
  
  /***************** Configure common DMA In parameters ***********************/  
  hdmaIn.Instance                 = DMA1_Channel1;
  hdmaIn.Init.Request             = DMA_REQUEST_11;
  hdmaIn.Init.Direction           = DMA_MEMORY_TO_PERIPH;
  hdmaIn.Init.PeriphInc           = DMA_PINC_DISABLE;
  hdmaIn.Init.MemInc              = DMA_MINC_ENABLE;
  hdmaIn.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  hdmaIn.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
  hdmaIn.Init.Mode                = DMA_NORMAL;
  hdmaIn.Init.Priority            = DMA_PRIORITY_MEDIUM;
  
  
  /* Associate the DMA handle */
  __HAL_LINKDMA(hcryp, hdmain, hdmaIn);
  
  /* Deinitialize the Stream for new transfer */
  HAL_DMA_DeInit(hcryp->hdmain);
  
  /* Configure the DMA Stream */
  HAL_DMA_Init(hcryp->hdmain);      
  
  /* NVIC configuration for DMA Input data interrupt */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
  
  /***************** Configure common DMA Out parameters **********************/
  hdmaOut.Instance                 = DMA1_Channel3;
  hdmaOut.Init.Request             = DMA_REQUEST_11;
  hdmaOut.Init.Direction           = DMA_PERIPH_TO_MEMORY;
  hdmaOut.Init.PeriphInc           = DMA_PINC_DISABLE;
  hdmaOut.Init.MemInc              = DMA_MINC_ENABLE;
  hdmaOut.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  hdmaOut.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
  hdmaOut.Init.Mode                = DMA_NORMAL;
  hdmaOut.Init.Priority            = DMA_PRIORITY_MEDIUM;
  
  /* Associate the DMA handle */
  __HAL_LINKDMA(hcryp, hdmaout, hdmaOut);
  
  /* Deinitialize the Stream for new processing */
  HAL_DMA_DeInit(&hdmaOut);
  
  /* Configure the DMA Stream */
  HAL_DMA_Init(&hdmaOut);
  
  /* NVIC configuration for DMA output data interrupt */
  /* Already configured */
  HAL_NVIC_SetPriority(DMA1_Channel2_3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);
  
}


/**
  * @brief  DeInitializes the CRYP MSP.
  * @param  hcryp: CRYP handle pointer
  * @retval None
  */
void HAL_CRYP_MspDeInit(CRYP_HandleTypeDef *hcryp)
{
  /* Disable CRYP clock */
  __HAL_RCC_AES_CLK_DISABLE();
  

}


/**
  * @brief  Initializes the Global MSP.
  * @param  None
  * @retval None
  */
void HAL_MspInit(void)
{
  /* NOTE : This function is eventually modified by the user */

}

/**
  * @brief  DeInitializes the Global MSP.
  * @param  None  
  * @retval None
  */
void HAL_MspDeInit(void)
{
  /* NOTE : This function is eventually modified by the user */

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
