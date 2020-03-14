/**
  ******************************************************************************
  * @file    LIS302DL/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    19-September-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "main.h"

/** @addtogroup STM32F4_Discovery_Peripheral_Examples
  * @{
  */

/** @addtogroup SysTick_Example
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t Counter  = 0;
extern __IO int8_t XOffset;
extern __IO int8_t YOffset;
extern uint8_t Buffer[4];
__IO uint8_t TempAcceleration = 0;
uint8_t ClickReg = 0;
extern __IO uint32_t TimingDelay;
extern __IO uint8_t SingleClickDetect;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
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
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
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
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
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
  uint8_t temp1, temp2 = 0;
  
  if (TimingDelay != 0x00)
  {
    TimingDelay_Decrement();
  }
  else
  {
    Counter ++;
    if (Counter == 10)
    {
      Buffer[0] = 0;
      Buffer[2] = 0;
      
      /* Disable All TIM4 Capture Compare Channels */
      TIM_CCxCmd(TIM4, TIM_Channel_1, DISABLE);
      TIM_CCxCmd(TIM4, TIM_Channel_2, DISABLE);
      TIM_CCxCmd(TIM4, TIM_Channel_3, DISABLE);
      TIM_CCxCmd(TIM4, TIM_Channel_4, DISABLE);
      
      LIS302DL_Read(Buffer, LIS302DL_OUT_X_ADDR, 6);
      /* Remove the offsets values from data */
      Buffer[0] -= XOffset;
      Buffer[2] -= YOffset;
      
      /* Update autoreload and capture compare registers value*/
      temp1 = ABS((int8_t)(Buffer[0]));
      temp2 = ABS((int8_t)(Buffer[2]));       
      TempAcceleration = MAX(temp1, temp2);
      
      if(TempAcceleration != 0)
      { 
        if ((int8_t)Buffer[0] < -2)
        {
          /* Enable TIM4 Capture Compare Channel 4 */
          TIM_CCxCmd(TIM4, TIM_Channel_4, ENABLE);
          /* Sets the TIM4 Capture Compare4 Register value */
          TIM_SetCompare4(TIM4, TIM_CCR/TempAcceleration);
        }
        if ((int8_t)Buffer[0] > 2)
        {
          /* Enable TIM4 Capture Compare Channel 2 */
          TIM_CCxCmd(TIM4, TIM_Channel_2, ENABLE);
          /* Sets the TIM4 Capture Compare2 Register value */
          TIM_SetCompare2(TIM4, TIM_CCR/TempAcceleration);
        }
        if ((int8_t)Buffer[2] > 2)
        { 
          /* Enable TIM4 Capture Compare Channel 1 */
          TIM_CCxCmd(TIM4, TIM_Channel_1, ENABLE);
          /* Sets the TIM4 Capture Compare1 Register value */
          TIM_SetCompare1(TIM4, TIM_CCR/TempAcceleration);
        }      
        if ((int8_t)Buffer[2] < -2)
        { 
          /* Enable TIM4 Capture Compare Channel 3 */
          TIM_CCxCmd(TIM4, TIM_Channel_3, ENABLE);
          /* Sets the TIM4 Capture Compare3 Register value */
          TIM_SetCompare3(TIM4, TIM_CCR/TempAcceleration);
        }
        
        /* Time base configuration */
        TIM_SetAutoreload(TIM4,  TIM_ARR/TempAcceleration);
        
        /* Read click status register */
        LIS302DL_Read(&ClickReg, LIS302DL_CLICK_SRC_REG_ADDR, 1); 
        
        if(ClickReg == SINGLECLICK_Z)
        {
          SingleClickDetect = 0x01;
        }
      }
      Counter = 0x00;
    }
  }  
}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

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

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
