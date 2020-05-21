/**
  ******************************************************************************
  * @file    SPI/SPI_TwoBoards/DataExchangeInterrupt/stm32f0xx_it.c 
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

/** @addtogroup SPI_DataExchangeInterrupt
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern uint8_t TxBuffer[];
extern  uint8_t RxBuffer[];
extern __IO uint8_t Rx_Idx;
extern __IO uint8_t Tx_Idx;

extern __IO uint8_t CmdTransmitted;
extern __IO uint8_t CmdReceived;
extern __IO uint8_t CmdStatus;

__IO uint8_t Counter = 0x00;
extern __IO uint32_t TimeOut;

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
  /* Decrement the timeout value */
  if (TimeOut != 0x0)
  {
    TimeOut--;
  }
    
  if (Counter < 10)
  {
    Counter++;
  }
  else
  {
    Counter = 0x00;
    STM_EVAL_LEDToggle(LED1);
  }
}

/******************************************************************************/
/*            STM32F0xx Peripherals Interrupt Handlers                        */
/******************************************************************************/
/**
  * @brief  This function handles SPI interrupt request.
  * @param  None
  * @retval None
  */
void SPI1_IRQHandler(void)
{
#if defined (SPI_SLAVE)  
  
  /* SPI in Slave Tramitter mode--------------------------------------- */
  if (SPI_I2S_GetITStatus(SPIx, SPI_I2S_IT_TXE) == SET)
  {
    SPI_SendData8(SPIx, TxBuffer[Tx_Idx++]);
    if (Tx_Idx == GetVar_NbrOfData())
    {
      /* Disable the Tx buffer empty interrupt */
      SPI_I2S_ITConfig(SPIx, SPI_I2S_IT_TXE, DISABLE);
    }
  }
  
  /* SPI in Slave Receiver mode--------------------------------------- */
  if (SPI_I2S_GetITStatus(SPIx, SPI_I2S_IT_RXNE) == SET)
  {
    if (CmdReceived == 0x00)
    {
      CmdReceived = SPI_ReceiveData8(SPIx);
      CmdStatus = 0x01;
    }
    else
    {
      RxBuffer[Rx_Idx++] = SPI_ReceiveData8(SPIx);
    }
  }
  
  /* SPI Error interrupt--------------------------------------- */
  if (SPI_I2S_GetITStatus(SPIx, SPI_I2S_IT_OVR) == SET)
  {
    SPI_ReceiveData8(SPIx);
    SPI_I2S_GetITStatus(SPIx, SPI_I2S_IT_OVR);
  }
  
#endif /* SPI_SLAVE*/
  
#if defined (SPI_MASTER)
  
  /* SPI in Master Tramitter mode--------------------------------------- */
  if (SPI_I2S_GetITStatus(SPIx, SPI_I2S_IT_TXE) == SET)
  {
    if (CmdStatus == 0x00)
    {
      SPI_SendData8(SPIx, CmdTransmitted);
      CmdStatus = 0x01;
    }
    else
    {
      SPI_SendData8(SPIx, TxBuffer[Tx_Idx++]);
      if (Tx_Idx == GetVar_NbrOfData())
      {
        /* Disable the Tx buffer empty interrupt */
        SPI_I2S_ITConfig(SPIx, SPI_I2S_IT_TXE, DISABLE);
      }
    }
  }
  
  /* SPI in Master Receiver mode--------------------------------------- */
  if (SPI_I2S_GetITStatus(SPIx, SPI_I2S_IT_RXNE) == SET)
  {
    if (CmdReceived == 0x00)
    {
      CmdReceived = SPI_ReceiveData8(SPIx);
      Rx_Idx = 0x00;
    }
    else
    {
      RxBuffer[Rx_Idx++] = SPI_ReceiveData8(SPIx);
    }
  }
  
  /* SPI Error interrupt--------------------------------------- */
  if (SPI_I2S_GetITStatus(SPIx, SPI_I2S_IT_OVR) == SET)
  {
    SPI_ReceiveData8(SPIx);
    SPI_I2S_GetITStatus(SPIx, SPI_I2S_IT_OVR);
  }
  
#endif /* SPI_MASTER*/
}
/******************************************************************************/
/*                 STM32F0xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f0xx.s).                                            */
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


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
