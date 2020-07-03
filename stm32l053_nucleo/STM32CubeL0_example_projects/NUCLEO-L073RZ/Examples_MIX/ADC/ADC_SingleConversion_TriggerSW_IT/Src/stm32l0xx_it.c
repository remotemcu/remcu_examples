/** 
  ******************************************************************************
  * @file    Examples_MIX/ADC/ADC_SingleConversion_TriggerSW_IT/Src/stm32l0xx_it.c
  * @author  MCD Application Team
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and
  *          peripherals interrupt service routine.
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
#include "stm32l0xx_it.h"

/** @addtogroup STM32L0xx_MIX_HAL_Examples
  * @{
  */

/** @addtogroup ADC_SingleConversion_TriggerSW_IT
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
//extern ADC_HandleTypeDef    AdcHandle;
#if defined(WAVEFORM_GENERATION)
extern DAC_HandleTypeDef    DacHandle;
#endif /* WAVEFORM_GENERATION */

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M0+ Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  HAL_IncTick();
}

/******************************************************************************/
/*                 STM32L0xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32l0xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles external lines 4 to 15 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI4_15_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(KEY_BUTTON_PIN);
}

/* Note: Lines of code commented below correspond to the example using        */
/*       HAL driver only.                                                     */
/*       This example demonstrating a mix of HAL and LL drivers has replaced  */
/*       these lines using LL driver.                                         */
// /**
//   * @brief  This function handles ADC interrupt request.
//   * @param  None
//   * @retval None
//   */
// void ADCx_IRQHandler(void)
// {
//   HAL_ADC_IRQHandler(&AdcHandle);
// }

/**
  * @brief  This function handles ADCx interrupt request.
  * @param  None
  * @retval None
  */
void ADC1_COMP_IRQHandler(void)
{
  /* Customize process using LL interface to improve the performance          */
  /* (exhaustive feature management not handled).                             */
  
  /* ########## Starting from this point HAL API must not be used ########### */
  
  /* Check whether ADC group regular end of unitary conversion caused         */
  /* the ADC interruption.                                                    */
  if(LL_ADC_IsActiveFlag_EOC(ADCx) != 0)
  {
    /* Clear flag ADC group regular end of unitary conversion */
    LL_ADC_ClearFlag_EOC(ADCx);
    
    /* Clear flag ADC group regular end of sequence conversions */
    /* Note: Clear this flag optionaly, this flag is set with end of          */
    /*       unitary conversion since there is only 1 rank in                 */
    /*       group regular sequencer.                                         */
    LL_ADC_ClearFlag_EOS(ADCx);
    
    /* Call interruption treatment function */
    AdcGrpRegularUnitaryConvComplete_Callback();
  }
  
  /* Check whether ADC group regular overrun caused the ADC interruption */
  if(LL_ADC_IsActiveFlag_OVR(ADCx) != 0)
  {
    /* Clear flag ADC group regular overrun */
    LL_ADC_ClearFlag_OVR(ADCx);
    
    /* Call interruption treatment function */
    AdcGrpRegularOverrunError_Callback();
  }
  /* EOS interrupt is enabled in HAL_Init, it has to be treated */
  if(LL_ADC_IsActiveFlag_EOS(ADCx) != 0)
  {
    /* Clear flag ADC group regular overrun */
    LL_ADC_ClearFlag_EOS(ADCx);
  }
}

#if defined(WAVEFORM_GENERATION)
/**
  * @brief  This function handles DAC interrupt request.
  * @param  None
  * @retval None
  */
void TIM6_DAC_IRQHandler(void)
{
  HAL_DAC_IRQHandler(&DacHandle);
}
#endif /* WAVEFORM_GENERATION */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
