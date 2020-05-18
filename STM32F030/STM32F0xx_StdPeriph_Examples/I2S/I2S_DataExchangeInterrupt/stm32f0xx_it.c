/**
  ******************************************************************************
  * @file    I2S/I2S_DataExchangeInterrupt/stm32f0xx_it.c 
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

/** @addtogroup I2S_DataExchangeInterrupt
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern const uint16_t I2S_Buffer_Tx[32];
extern __IO uint16_t TxIdx, RxIdx;
extern uint16_t I2S_Buffer_Rx[32];

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
  * @brief  This function handles SPI1 global interrupt request
  * @param  None
  * @retval : None
  */
void SPI1_IRQHandler(void)
{
  uint32_t tmpreg = 0x00;
  tmpreg = SPI1->SR;
#if defined (I2S_SLAVE_RECEIVER)
  if ((tmpreg&0x01)==0x01)
  {
    I2S_Buffer_Rx[RxIdx++] = SPI_I2S_ReceiveData16(SPI1);
    /* Disable the Interrupt when transfer is complete */
    if(RxIdx == 32)
    {
      /* Disable the I2S1 RXNE Interrupt */
      SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_RXNE, DISABLE);
    }
  }
#elif defined (I2S_MASTER_TRANSMITTER)
  if((tmpreg&0x02)==0x02)
  {
    SPI_I2S_SendData16(SPI1, I2S_Buffer_Tx[TxIdx++]);
    if(TxIdx == 32)
    {
      /* Disable the I2S1 TXE Interrupt */
      SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_TXE, DISABLE);
    }
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
