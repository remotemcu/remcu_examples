/**
  ******************************************************************************
  * @file    CEC/DataExchangeInterrupt/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
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
#include "stm32f10x_it.h"
#include "stm32_eval.h"

/** @addtogroup STM32F10x_StdPeriph_Examples
  * @{
  */

/** @addtogroup CEC_DataExchangeInterrupt
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Receive buffer */
uint8_t ReceiveBuffer[10];
/* Transmit buffer */
uint8_t TransmitBuffer[10] = {0xDF, 0x12, 0xD3, 0x56, 0x97, 
                              0xA1, 0xEC, 0x7B, 0x4F, 0x22};
__IO uint8_t ReceivedFrame = 0;
uint8_t send_inc = 0, rcv_inc = 0;
uint8_t HeaderBlockValueToSend = 0;
uint8_t InitiatorAddress = 0;
uint8_t TransErrorCode = 0;
uint8_t RecepErrorCode = 0;
extern uint8_t ByteNumber;

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
}

/**
  * @brief  This function handles EXTI Lines 9 to 5 interrupts requests.
  * @param  None
  * @retval None
  */
void EXTI9_5_IRQHandler(void)
{
  /* Generate rising edge on Key button to detect when we push key button to initiate 
     transmission */
  if(EXTI_GetITStatus(KEY_BUTTON_EXTI_LINE) != RESET)
  {
    /* Turn on LED3 */
    STM_EVAL_LEDOn(LED3);

    /* Build the Header block to send */
    HeaderBlockValueToSend = (((MY_DEVICE_ADDRESS & 0xF) << 4) | (FOLLOWER & 0xF));

    /* Write single Data in the TX Buffer to Transmit through the CEC peripheral */
    CEC_SendDataByte(HeaderBlockValueToSend);

    /* Initiate Message Transmission */
    CEC_StartOfMessage();

    /* Clear Key Button EXTI line pending bit */
    EXTI_ClearITPendingBit(KEY_BUTTON_EXTI_LINE);
  }
}

/**
  * @brief  This function handles CEC global interrupt request.
  * @param  None
  * @retval None
  */
void CEC_IRQHandler(void)
{
  /* Turn on LED4 */
  STM_EVAL_LEDOn(LED4);
/********************** Reception *********************************************/
  /* Check if a reception error occurred */
  if (CEC_GetFlagStatus(CEC_FLAG_RERR))
  {
    /* Set receive status bit (Error) */
    RecepErrorCode = CEC->ESR;
    CEC_ClearFlag(CEC_FLAG_RERR | CEC_FLAG_RSOM | CEC_FLAG_REOM  | CEC_FLAG_RBTF);
  }
  else if (CEC_GetFlagStatus(CEC_FLAG_RBTF))
  {
    /* Check if the byte received is the last one of the message */
    if (CEC_GetFlagStatus(CEC_FLAG_REOM))
    {
      ReceiveBuffer[rcv_inc] = CEC_ReceiveDataByte();
      rcv_inc++;
      ReceivedFrame = 1;
    }
    /* Check if the byte received is a Header */
    else if (CEC_GetFlagStatus(CEC_FLAG_RSOM))
    {
 
      InitiatorAddress = ((CEC_ReceiveDataByte() >> 4) & 0x0F);
      rcv_inc = 0;
    }
    /* Receive each byte except header in the reception buffer */
    else 
    {
      ReceiveBuffer[rcv_inc] = CEC_ReceiveDataByte();
      rcv_inc++;
    }
    /* Clear all reception flags */
    CEC_ClearFlag(CEC_FLAG_RSOM | CEC_FLAG_REOM  | CEC_FLAG_RBTF);
  }

/********************** Transmission ******************************************/
  /* Check if a transmission error occurred */
  if (CEC_GetFlagStatus(CEC_FLAG_TERR))
  {
    TransErrorCode = CEC->ESR;
    CEC_ClearFlag(CEC_FLAG_TBTRF | CEC_FLAG_TERR);
   
  }
  /* Check if end of message bit is set in the data to be transmitted */
  else if (CEC_GetFlagStatus(CEC_FLAG_TEOM))
  {
    CEC_ClearFlag(CEC_FLAG_TBTRF | CEC_FLAG_RBTF);
    CEC_EndOfMessageCmd(DISABLE);
  }
  /* Check if data byte has been sent */
  else if (CEC_GetFlagStatus(CEC_FLAG_TBTRF))
  {
    /* Set EOM bit if the byte to be transmitted is the last one of the Transmit Buffer */
    if (send_inc == (ByteNumber - 1))
    {
      CEC_SendDataByte(TransmitBuffer[send_inc]);
      send_inc++;
      CEC_ClearFlag(CEC_FLAG_TBTRF);
      CEC_EndOfMessageCmd(ENABLE);
    }
    else
    {
      /* Put the byte in the TX Buffer */
      CEC_SendDataByte(TransmitBuffer[send_inc]);
      send_inc++;
      CEC_ClearFlag(CEC_FLAG_TBTRF);
    }
  }
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
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
