/**
******************************************************************************
* @file    FreeRTOS/FreeRTOS_Timers/Src/stm32l0xx_it.c
* @author  MCD Application Team
* @brief   Main Interrupt Service Routines.
******************************************************************************
  *
  * Copyright (c) 2016 STMicroelectronics International N.V. All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "k_config.h"    
#include "k_module.h"    


/** @addtogroup CORE
* @{
*/

/** @defgroup 
* @brief  
* @{
*/


extern RTC_HandleTypeDef RtcHandle;
extern SMBUS_HandleTypeDef SmbusHandle;
extern UART_HandleTypeDef UartHandle;
extern LPTIM_HandleTypeDef  LptimHandle;

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M0+ Processor Exceptions Handlers                         */
/******************************************************************************/

/**
* @brief  NMI_Handler
*         This function handles NMI exception.
* @param  None
* @retval None
*/

void NMI_Handler(void)
{
}

/**
* @brief  HardFault_Handler
*         This function handles Hard Fault exception.
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

/******************************************************************************/
/*                           Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file .                                                                    */
/******************************************************************************/
void SysTick_Handler (void)
{
  HAL_IncTick(); 
}

/**
* @brief  This function handles LPTIM interrupt request.
* @param  None
* @retval None
*/
void LPTIM1_IRQHandler(void)
{
  /* LPTIM in time Base mode */
  HAL_LPTIM_IRQHandler(&LptimHandle);
}
/**
* @brief  This function handles External lines 4_15 interrupt request.
* @param  None
* @retval None
*/

void EXTI4_15_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(SD_DETECT_PIN);
  HAL_GPIO_EXTI_IRQHandler(TAMPER_BUTTON_PIN);
  HAL_GPIO_EXTI_IRQHandler(MFX_IRQOUT_PIN);  
  
}

/**
* @brief  This function handles External lines 2_3 interrupt request.
* @param  None
* @retval None
*/
void EXTI2_3_IRQHandler(void)
{
}

/**
* @brief  This function handles External lines 0_1 interrupt request.
* @param  None
* @retval None
*/
void EXTI0_1_IRQHandler(void)
{
}

/**
* @brief  This function handles the RTC interrupt request.
* @param  None
* @retval None
* @Note   Used for RTC interrupt
*/
void RTC_IRQHandler(void)
{
  if(LCSensorMode == CONFIG_LC_SENSOR_LOW_POWER)
  {
    /* Clear the WAKEUPTIMER interrupt pending bit */
    RTC->ISR &= ~RTC_FLAG_WUTF; 
    /* Clear the EXTI's line Flag for RTC WakeUpTimer */
    __HAL_RTC_WAKEUPTIMER_EXTI_CLEAR_FLAG();
    
    /* Enable the selected comparator */
    COMP2->CSR |= (COMP_CSR_COMPxEN);
    
    /* Enable DAC1 to feed COMP2 threshold */
    DAC->CR |= DAC_CR_EN1;

    /* Insert waiting time for COMP2 fast mode and DAC initialization time based on DSB assembler instruction */    
    __DSB(); __DSB(); __DSB();
    
    /* Re-configure all used pins in applicative conditions */
    /* Re-Enable PD7 VDD/2 reference power supply from Analog state to Output */
    GPIOD->MODER = 0xFFFF7FFF; 
    
    /* PA4 PA7 DAC + Comparator output */
    GPIOA->MODER &= 0xFFFFBFFF;
    
    /* Set PB4 AF + PP no pull to drive LC sensor*/
    GPIOB->MODER = 0xFFFFFDFF;
    
    /* then PB4 is back to Analog state */
    GPIOB->MODER = 0xFFFFFFFF;
    
    /* MSI time capture corresponding to 75µs while LC pulses are emitted. Based on DSB assembler instruction */
    /* DSB instruction is one clock cycle */
    __DSB(); __DSB(); __DSB(); __DSB(); __DSB(); __DSB(); __DSB(); __DSB(); __DSB(); __DSB(); __DSB(); __DSB();
    __DSB(); __DSB(); __DSB(); __DSB(); __DSB(); __DSB(); __DSB(); __DSB(); __DSB(); __DSB(); __DSB(); __DSB();
    __DSB(); __DSB(); __DSB(); __DSB(); __DSB(); __DSB(); __DSB(); __DSB(); __DSB(); __DSB(); __DSB(); __DSB();
    __DSB(); __DSB(); __DSB();
    
    /* Switch back PD7 VDD/2 reference power supply from Output state to Analog */
    GPIOD->MODER = 0xFFFFFFFF; 
    
    /* Disable DAC1 */
    DAC->CR &= ~(DAC_CR_EN1);
    
    /* Disable the selected comparator */
    COMP2->CSR &= ~(COMP_CSR_COMPxEN);
    
  }
  else
  {
    HAL_RTC_AlarmIRQHandler(&RtcHandle);
    HAL_RTCEx_WakeUpTimerIRQHandler(&RtcHandle);
  }
}

/**
* @brief  This function handles SMBUS event interrupt request.  
* @param  None
* @retval None
* @Note   This function is redefined in "main.h" and related to SMBUS data transmission     
*/
void SMBUSx_IRQHandler(void)
{
  HAL_SMBUS_EV_IRQHandler(&SmbusHandle);
  HAL_SMBUS_ER_IRQHandler(&SmbusHandle);
}

/**
* @brief  This function handles DMA interrupt request.
* @param  None
* @retval None
* @Note   This function is redefined in "main.h" and related to DMA stream
*         used for USART data transmission and reception
*/
void USARTx_DMA_TX_RX_IRQHandler(void)
{
  HAL_DMA_IRQHandler(UartHandle.hdmarx);
  HAL_DMA_IRQHandler(UartHandle.hdmatx);
}

/**
* @brief  This function handles UART interrupt request.  
* @param  None
* @retval None
* @Note   This function is redefined in "main.h" and related to DMA  
*         used for USART data transmission     
*/
void USARTx_IRQHandler(void)
{
  HAL_UART_IRQHandler(&UartHandle);
}

/**
* @}
*/

/**
* @}
*/


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
