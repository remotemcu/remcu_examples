/**
  ******************************************************************************
  * @file    COMP/COMP_PulseWidthMeasurement/Src/stm32l0xx_hal_msp.c
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

/** @defgroup COMP_PulseWidthMeasurement
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
  * @brief COMP MSP Initialization 
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration  
  *           - NVIC configuration for COMP interrupt request enable
  * @param hcomp: COMP handle pointer
  * @retval None
  */
void HAL_COMP_MspInit(COMP_HandleTypeDef *hcomp)
{
  GPIO_InitTypeDef      GPIO_InitStruct;

  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO clock ***************************************************/
  COMPx_GPIO_CLK_ENABLE();
  
  /*##-2- Configure peripheral GPIO ##########################################*/
  /* COMP GPIO pin configuration */
  GPIO_InitStruct.Pin = COMPx_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(COMPx_GPIO_PORT, &GPIO_InitStruct);

  /*##-3- Configure the NVIC for COMPx #######################################*/
  /* Enable the COMPx IRQ Channel */

  HAL_NVIC_SetPriority(COMPx_IRQn, 3, 0);
  HAL_NVIC_EnableIRQ(COMPx_IRQn);

}

/**
  * @brief  DeInitializes the COMP MSP.
  * @param  hcomp: pointer to a COMP_HandleTypeDef structure that contains
  *         the configuration information for the specified COMP.  
  * @retval None
  */
void HAL_COMP_MspDeInit(COMP_HandleTypeDef* hcomp)
{
  /*##-1- De-initialize peripheral GPIO ######################################*/
  /* De-initialize the COMPx GPIO pin */
  HAL_GPIO_DeInit(COMPx_GPIO_PORT, COMPx_PIN);

  /*##-2- Disable peripherals and GPIO clocks ################################*/
  /* Disable COMP peripheral clock ***************************************/
  __HAL_RCC_SYSCFG_CLK_DISABLE();

  /* Disable GPIO clock **************************************************/
  /* Note: In a real application, by disabling GPIO port, be cautious to not  */
  /* interfere with potential other peripherals using other GPIOs on the same */
  /* port.                                                                    */
  COMPx_GPIO_CLK_DISABLE();

  /*##-3- Disable the NVIC for COMP ##########################################*/
  HAL_NVIC_DisableIRQ(COMPx_IRQn);
}

/**
  * @brief TIM MSP initialization
  *        This function configures the hardware resources used in this example:
  *          - Enable clock of peripheral
  * @param htim: TIM handle pointer
  * @retval None
  */
void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* TIM peripheral clock enable */
  TIMx_CLK_ENABLE();
  
  /*##-2- Configure the NVIC #################################################*/
   /* NVIC configuration for TIM interrupt */
   /* Priority: high-priority */
   HAL_NVIC_SetPriority(TIMx_IRQn, 0, 0);
   HAL_NVIC_EnableIRQ(TIMx_IRQn);
  
}

/**
  * @brief TIM MSP de-initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable clock of peripheral
  * @param htim: TIM handle pointer
  * @retval None
  */
void HAL_TIM_IC_MspDeInit(TIM_HandleTypeDef *htim)
{
  /*##-1- Reset peripherals ##################################################*/
  TIMx_FORCE_RESET();
  TIMx_RELEASE_RESET();
  
  /*##-2- Disable the NVIC ###################################################*/
  HAL_NVIC_DisableIRQ(TIMx_IRQn);
  
}

/**
  * @brief DAC MSP initialization
  *        This function configures the hardware resources used in this example:
  *          - Enable clock of peripheral
  *          - Configure the GPIO associated to the peripheral channels
  *          - Configure the DMA associated to the peripheral
  *          - Configure the NVIC associated to the peripheral interruptions
  * @param hdac: DAC handle pointer
  * @retval None
  */
void HAL_DAC_MspInit(DAC_HandleTypeDef *hdac)
{
    GPIO_InitTypeDef          GPIO_InitStruct;
#if defined DAC_SPARE_CHANNEL_FOR_SIGNAL_TEST
  static DMA_HandleTypeDef  DmaHandle;
#endif //DAC_SPARE_CHANNEL_FOR_SIGNAL_TEST
  /*##-1- Enable and configure GPIO for DAC_CHANNEL_2 ######################*/

   DACx_CHANNEL_GPIO_CLK_ENABLE();

   GPIO_InitStruct.Pin = DACx_CHANNEL_SIGNAL_FOR_TEST_PIN;
   GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
   GPIO_InitStruct.Pull = GPIO_NOPULL;
   HAL_GPIO_Init(DACx_CHANNEL_SIGNAL_FOR_TEST_GPIO_PORT, &GPIO_InitStruct);

#if defined DAC_SPARE_CHANNEL_FOR_SIGNAL_TEST

  /*##-2- Enable and configure DMA for DAC_CHANNEL_2 ###############*/
   __HAL_RCC_DAC_CLK_ENABLE();
   DACx_CHANNEL_SIGNAL_FOR_TEST_DMA_CLK_ENABLE();

   /* Configure DMA parameters */
   DmaHandle.Instance = DACx_CHANNEL_SIGNAL_FOR_TEST_DMA;

   DmaHandle.Init.Direction           = DMA_MEMORY_TO_PERIPH;
   DmaHandle.Init.PeriphInc           = DMA_PINC_DISABLE;
   DmaHandle.Init.MemInc              = DMA_MINC_ENABLE;
   DmaHandle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
   DmaHandle.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
   DmaHandle.Init.Mode                = DMA_CIRCULAR;
   DmaHandle.Init.Priority            = DMA_PRIORITY_HIGH;
   DmaHandle.Init.Request             = DMA_REQUEST_15;
   /* Deinitialize  & Initialize the DMA for new transfer */
   HAL_DMA_DeInit(&DmaHandle);
   HAL_DMA_Init(&DmaHandle);

    __HAL_LINKDMA(hdac, DMA_Handle2, DmaHandle);

  /*##-3- Configure the NVIC #################################################*/

   /* NVIC configuration for DMA interrupt (transfer completion or error) */
   /* Priority: high-priority */
   HAL_NVIC_SetPriority(DACx_CHANNEL_SIGNAL_FOR_TEST_DMA_IRQn, 1, 0);
   HAL_NVIC_EnableIRQ(DACx_CHANNEL_SIGNAL_FOR_TEST_DMA_IRQn);

   /* NVIC configuration for DAC interrupt */
   /* Priority: mid-priority */
   HAL_NVIC_SetPriority(DACx_IRQn, 3, 0);
   HAL_NVIC_EnableIRQ(DACx_IRQn);
#endif /* DAC_SPARE_CHANNEL_FOR_SIGNAL_TEST */

}

/**
  * @brief DAC MSP de-initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable clock of peripheral
  *          - Revert GPIO associated to the peripheral channels to their default state
  *          - Revert DMA associated to the peripheral to its default state
  *          - Revert NVIC associated to the peripheral interruptions to its default state
  * @param hadc: DAC handle pointer
  * @retval None
  */
void HAL_DAC_MspDeInit(DAC_HandleTypeDef *hdac)
{
  /*##-1- Reset peripherals ##################################################*/
  DACx_FORCE_RESET();
  DACx_RELEASE_RESET();

 /*##-2- Disable peripherals and GPIO Clocks ################################*/
 /* De-initialize the ADC Channel GPIO pin */
 HAL_GPIO_DeInit(DACx_CHANNEL_SIGNAL_FOR_TEST_GPIO_PORT, DACx_CHANNEL_SIGNAL_FOR_TEST_PIN);
#if defined DAC_SPARE_CHANNEL_FOR_SIGNAL_TEST
  /*##-3- Disable the DMA ####################################################*/
  /* De-Initialize the DMA associated to transmission process */
    HAL_DMA_DeInit(hdac->DMA_Handle1);
    HAL_DMA_DeInit(hdac->DMA_Handle2);

  /*##-4- Disable the NVIC ###################################################*/
  /* Disable the NVIC configuration for DMA interrupt */
  HAL_NVIC_DisableIRQ(DACx_CHANNEL_SIGNAL_FOR_TEST_DMA_IRQn);
  
  /* Disable the NVIC configuration for DAC interrupt */
  HAL_NVIC_DisableIRQ(DACx_IRQn);
  
#endif /* DAC_SPARE_CHANNEL_FOR_SIGNAL_TEST */
}

#if defined DAC_SPARE_CHANNEL_FOR_SIGNAL_TEST
/**
  * @brief TIM MSP initialization
  *        This function configures the hardware resources used in this example:
  *          - Enable clock of peripheral
  * @param htim: TIM handle pointer
  * @retval None
  */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
  /* TIM peripheral clock enable */
  if (htim->Instance == TIM7)
  {
    __HAL_RCC_TIM7_CLK_ENABLE();
  }
  else
  {
    __HAL_RCC_TIM6_CLK_ENABLE();
  }
}

/**
  * @brief TIM MSP de-initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable clock of peripheral
  * @param htim: TIM handle pointer
  * @retval None
  */
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef *htim)
{
  /* TIM peripheral clock disable */
  if (htim->Instance == TIM7)
  {
    __HAL_RCC_TIM7_CLK_DISABLE();
  }
  else
  {
    __HAL_RCC_TIM6_CLK_DISABLE();
  }
}
#endif /* DAC_SPARE_CHANNEL_FOR_SIGNAL_TEST */
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
