/**
  ******************************************************************************
  * @file    EXTI/EXTI_Example/stm32f0xx_it.c 
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

/** @addtogroup EXTI_Example
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
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
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F0xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f0xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles External line 0 to 1 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI0_1_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line0) != RESET)
  {
    /* Toggle LED2 */
    STM_EVAL_LEDToggle(LED2);

    /* Clear the EXTI line 0 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line0);
  }
}

/**
  * @brief  This function handles External line 2 to 3 interrupt request.
  * @param  None
  * @retval None
  */
#ifdef USE_STM32072B_EVAL 
void EXTI2_3_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line3) != RESET)
  {
    /* Toggle LED1 */
    STM_EVAL_LEDToggle(LED1);
    
    /* Clear the EXTI line 3 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line3);
  }
  
  if(EXTI_GetITStatus(EXTI_Line2) != RESET)
  {
    /* Toggle LED3 */
    STM_EVAL_LEDToggle(LED3);

    /* Clear the EXTI line 2 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line2);
  }
}
#endif

/**
  * @brief  This function handles External lines 4 to 15 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI4_15_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line8) != RESET)
  {
    /* Toggle LED1 */
    STM_EVAL_LEDToggle(LED1);
    
    /* Clear the EXTI line 8 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line8);
  }
  
  if(EXTI_GetITStatus(EXTI_Line13) != RESET)
  {
    /* Toggle LED4 */
    STM_EVAL_LEDToggle(LED4);

    /* Clear the EXTI line 13 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line13);
  }

#ifdef USE_STM320518_EVAL
  if(EXTI_GetITStatus(EXTI_Line9) != RESET)
  {
    /* Toggle LED4 */
    STM_EVAL_LEDToggle(LED3);

    /* Clear the EXTI line 13 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line9);
  }
#endif
  
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
