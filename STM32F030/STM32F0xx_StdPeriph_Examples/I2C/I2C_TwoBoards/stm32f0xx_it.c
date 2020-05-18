/**
  ******************************************************************************
  * @file    I2C/I2C_TwoBoards/stm32f0xx_it.c 
  * @author  MCD Application Team
  * @version V1.4.0
  * @date    24-July-2014
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_it.h"

/** @addtogroup STM32F0xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup I2C_TwoBoards
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern __IO uint32_t ActionState, RecieverMode;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M0 Processor Exceptions Handlers                         */
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
/*void SysTick_Handler(void)
{
}*/

/******************************************************************************/
/*                 STM32F0xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f0xx.s).                                            */
/******************************************************************************/
 
/**
  * @brief  This function handles EXTI0_1 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI0_1_IRQHandler(void)
{
  /* Check if the Key push button has been pushed */
  if (EXTI_GetFlagStatus(SEL_BUTTON_EXTI_LINE) != RESET)  
  {
    if ((ActionState == ACTION_NONE) && (RecieverMode == STATE_OFF))
    {
      ActionState = BUTTON_SEL;
    }    
    /* Clear the interrupt pending bit */
    EXTI_ClearITPendingBit(SEL_BUTTON_EXTI_LINE);
  }
}
  
/**
  * @brief  This function handles EXTI4_15 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI4_15_IRQHandler(void)
{   
  /* Check if the Tamper push button has been pushed */
  if (EXTI_GetFlagStatus(TAMPER_BUTTON_EXTI_LINE) != RESET)  
  {
    if ((ActionState == ACTION_NONE) && (RecieverMode == STATE_OFF))
    {
      ActionState = BUTTON_TAMPER;
    }    
    /* Clear the interrupt pending bit */
    EXTI_ClearITPendingBit(TAMPER_BUTTON_EXTI_LINE);
  }
}
  
/**
  * @brief  This function handles TIM6 global interrupt request.
  * @param  None
  * @retval None
  */  
void TIM6_DAC_IRQHandler(void)  
{
  if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
  {    
    STM_EVAL_LEDToggle(LED1);
    STM_EVAL_LEDToggle(LED4);
    
    TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
  } 
}  

/**
  * @brief  This function handles TIM17 global interrupt request.
  * @param  None
  * @retval None
  */
void TIM17_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM17, TIM_IT_Update) != RESET)
  {    
    if ((ActionState == ACTION_NONE) && (RecieverMode == STATE_OFF))
    {
      ActionState =  ACTION_PERIODIC ;
    }
    
    /* Clear Timer interrupt pending bit */
    TIM_ClearITPendingBit(TIM17, TIM_IT_Update);
    
    /* Clear Key Button Interrupt pending bit */
    EXTI_ClearITPendingBit(SEL_BUTTON_EXTI_LINE);
    EXTI_ClearFlag(SEL_BUTTON_EXTI_LINE); 
  } 
}

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
