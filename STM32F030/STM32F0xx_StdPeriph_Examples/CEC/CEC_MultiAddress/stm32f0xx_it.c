/**
  ******************************************************************************
  * @file    CEC/CEC_MultiAddress/stm32f0xx_it.c 
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

/** @addtogroup CEC_MultiAddress
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
__IO uint8_t CECReceivedFrame = 0;
__IO uint8_t send_inc = 0, rcv_inc = 0;
uint8_t HeaderBlockValueToSend = 0;
uint16_t CECTransErrorCode = 0, ReceiverAddress = 0;
uint16_t RecepErrorCode = 0;
uint8_t ByteNumber = 11;
extern uint8_t MyLogicalAddress1, MyLogicalAddress2;
extern uint8_t MyFollowerAddress1, MyFollowerAddress2;
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
#ifdef USE_STM320518_EVAL
#if defined (DEVICE_1)
/**
  * @brief  This function handles EXTI Lines 4 to 15 interrupts requests.
  * @param  None
  * @retval None
  */
void EXTI4_15_IRQHandler(void)
{  
  STM_EVAL_LEDOff(LED1);
  STM_EVAL_LEDOff(LED2);
  STM_EVAL_LEDOff(LED3);
  STM_EVAL_LEDOff(LED4);

  /* Generate rising edge on Joystick button to detect when we push Joystick button to initiate 
  transmission */
  if(EXTI_GetITStatus(RIGHT_BUTTON_EXTI_LINE) != RESET)
  {
    /* Build the Header block to send */
    HeaderBlockValueToSend = (((MyLogicalAddress1 & 0xF) << 4)|(MyFollowerAddress1 & 0xF));

    /* Clear RIGHT Button EXTI line pending bit */
    EXTI_ClearITPendingBit(RIGHT_BUTTON_EXTI_LINE);
  }
  else if(EXTI_GetITStatus(LEFT_BUTTON_EXTI_LINE) != RESET)
  {
    /* Build the Header block to send */
    HeaderBlockValueToSend = (((MyLogicalAddress1 & 0xF) << 4)|(MyFollowerAddress2 & 0xF));

    /* Clear LEFT Button EXTI line pending bit */
    EXTI_ClearITPendingBit(LEFT_BUTTON_EXTI_LINE);
  }

  /* Write single Data in the TX Buffer to Transmit through the CEC peripheral */
  CEC_SendData(HeaderBlockValueToSend);

  /* Initiate Message Transmission */
  CEC_StartOfMessage();
}
#endif /* DEVICE_1 */

/**
  * @brief  This function handles CEC global interrupt request.
  * @param  None
  * @retval None
  */
void CEC_IRQHandler(void)
{
  /********************** Reception *******************************************/
  /* Check if a reception error occured */
  if (CEC->ISR & (CEC_IT_RXACKE | CEC_IT_LBPE | CEC_IT_SBPE | CEC_IT_BRE | CEC_IT_RXOVR))
  {
    RecepErrorCode = CEC->ISR;
    CEC->ISR = (CEC_IT_RXACKE | CEC_IT_LBPE | CEC_IT_SBPE | CEC_IT_BRE | CEC_IT_RXOVR);
    CECReceivedFrame = 2;
  }

  if (CEC_GetITStatus(CEC_IT_RXBR))
  {
    /* Check if the byte received is a Header */
    if (rcv_inc == 0)
    {
      ReceiverAddress = (CEC_ReceiveData() & 0x0F);
      rcv_inc++;
    }
    else /* Receive each byte except header and opcode in the reception buffer */
    {
      ReceiveBuffer[rcv_inc-1] = CEC_ReceiveData();
      rcv_inc++;
    }
    CEC_ClearITPendingBit(CEC_IT_RXBR);
  }

  /* Check if the byte received is the last one of the message */
  if (CEC_GetITStatus(CEC_IT_RXEND))
  {
    CECReceivedFrame = 1;
    rcv_inc = 0;

    /* Clear all reception flags */
    CEC_ClearITPendingBit(CEC_IT_RXEND);
  }

  /********************** Transmission ****************************************/
  /* Check if a transmission error occurred */
  if (CEC->ISR & (CEC_IT_TXACKE | CEC_IT_TXERR | CEC_IT_TXUDR | CEC_IT_ARBLST))
  {
    CECTransErrorCode = CEC->ISR;
    CEC->ISR = (CEC_IT_TXACKE | CEC_IT_TXERR | CEC_IT_TXUDR | CEC_IT_ARBLST);
    /* KO */
    /* Turn on LED3 */
    STM_EVAL_LEDOn(LED3);
#ifdef LCD_DISPLAY
    LCD_SetBackColor(LCD_COLOR_RED);
    LCD_SetTextColor(LCD_COLOR_WHITE);
    LCD_DisplayStringLine(LCD_LINE_7, (uint8_t *)"Send status : Failed");
#endif /* LCD_DISPLAY */
  }

  /* Check if end of message bit is set in the data to be transmitted */
  if (CEC_GetITStatus(CEC_IT_TXEND))
  {
    CEC_ClearITPendingBit(CEC_IT_TXEND | CEC_IT_TXBR);
    /* OK */
    /* Turn on LED1 */
    STM_EVAL_LEDOn(LED1);
#ifdef LCD_DISPLAY
    LCD_SetBackColor(LCD_COLOR_GREEN);
    LCD_SetTextColor(LCD_COLOR_WHITE);
    LCD_DisplayStringLine(LCD_LINE_7, (uint8_t *)"Send status: Succeeded");
#endif /* LCD_DISPLAY */
  }
  /* Check if data byte has been sent */
  else if (CEC_GetITStatus(CEC_IT_TXBR))
  {
    /* Set EOM bit if the byte to be transmitted is the last one of the Transmit Buffer */
     if (send_inc == (ByteNumber - 1))
    {
      CEC_EndOfMessage();
      CEC_SendData(TransmitBuffer[send_inc++]);
      send_inc=0;
    }
    else
    {
      /* Put the byte in the TX Buffer */
      CEC_SendData(TransmitBuffer[send_inc++]);
    }
    CEC_ClearITPendingBit(CEC_IT_TXBR);
  }
}
#else
#if defined (DEVICE_1)
/**
  * @brief  This function handles EXTI Lines 2 and 3 interrupts requests.
  * @param  None
  * @retval None
  */
void EXTI2_3_IRQHandler(void)
{  
  STM_EVAL_LEDOff(LED1);
  STM_EVAL_LEDOff(LED2);
  STM_EVAL_LEDOff(LED3);
  STM_EVAL_LEDOff(LED4);

  /* Generate rising edge on Joystick button to detect when we push Joystick button to initiate 
  transmission */
  if(EXTI_GetITStatus(RIGHT_BUTTON_EXTI_LINE) != RESET)
  {
    /* Build the Header block to send */
    HeaderBlockValueToSend = (((MyLogicalAddress1 & 0xF) << 4)|(MyFollowerAddress1 & 0xF));

    /* Clear RIGHT Button EXTI line pending bit */
    EXTI_ClearITPendingBit(RIGHT_BUTTON_EXTI_LINE);
  }
  else if(EXTI_GetITStatus(LEFT_BUTTON_EXTI_LINE) != RESET)
  {
    /* Build the Header block to send */
    HeaderBlockValueToSend = (((MyLogicalAddress1 & 0xF) << 4)|(MyFollowerAddress2 & 0xF));

    /* Clear LEFT Button EXTI line pending bit */
    EXTI_ClearITPendingBit(LEFT_BUTTON_EXTI_LINE);
  }

  /* Write single Data in the TX Buffer to Transmit through the CEC peripheral */
  CEC_SendData(HeaderBlockValueToSend);

  /* Initiate Message Transmission */
  CEC_StartOfMessage();
}
#endif /* DEVICE_1 */

/**
  * @brief  This function handles CEC global interrupt request.
  * @param  None
  * @retval None
  */
void CEC_CAN_IRQHandler(void)
{
  /********************** Reception *******************************************/
  /* Check if a reception error occured */
  if (CEC->ISR & (CEC_IT_RXACKE | CEC_IT_LBPE | CEC_IT_SBPE | CEC_IT_BRE | CEC_IT_RXOVR))
  {
    RecepErrorCode = CEC->ISR;
    CEC->ISR = (CEC_IT_RXACKE | CEC_IT_LBPE | CEC_IT_SBPE | CEC_IT_BRE | CEC_IT_RXOVR);
    CECReceivedFrame = 2;
  }

  if (CEC_GetITStatus(CEC_IT_RXBR))
  {
    /* Check if the byte received is a Header */
    if (rcv_inc == 0)
    {
      ReceiverAddress = (CEC_ReceiveData() & 0x0F);
      rcv_inc++;
    }
    else /* Receive each byte except header and opcode in the reception buffer */
    {
      ReceiveBuffer[rcv_inc-1] = CEC_ReceiveData();
      rcv_inc++;
    }
    CEC_ClearITPendingBit(CEC_IT_RXBR);
  }

  /* Check if the byte received is the last one of the message */
  if (CEC_GetITStatus(CEC_IT_RXEND))
  {
    CECReceivedFrame = 1;
    rcv_inc = 0;

    /* Clear all reception flags */
    CEC_ClearITPendingBit(CEC_IT_RXEND);
  }

  /********************** Transmission ****************************************/
  /* Check if a transmission error occurred */
  if (CEC->ISR & (CEC_IT_TXACKE | CEC_IT_TXERR | CEC_IT_TXUDR | CEC_IT_ARBLST))
  {
    CECTransErrorCode = CEC->ISR;
    CEC->ISR = (CEC_IT_TXACKE | CEC_IT_TXERR | CEC_IT_TXUDR | CEC_IT_ARBLST);
    /* KO */
    /* Turn on LED3 */
    STM_EVAL_LEDOn(LED3);
#ifdef LCD_DISPLAY
    LCD_SetBackColor(LCD_COLOR_RED);
    LCD_SetTextColor(LCD_COLOR_WHITE);
    LCD_DisplayStringLine(LCD_LINE_7, (uint8_t *)"Send status : Failed");
#endif /* LCD_DISPLAY */
  }

  /* Check if end of message bit is set in the data to be transmitted */
  if (CEC_GetITStatus(CEC_IT_TXEND))
  {
    CEC_ClearITPendingBit(CEC_IT_TXEND | CEC_IT_TXBR);
    /* OK */
    /* Turn on LED1 */
    STM_EVAL_LEDOn(LED1);
#ifdef LCD_DISPLAY
    LCD_SetBackColor(LCD_COLOR_GREEN);
    LCD_SetTextColor(LCD_COLOR_WHITE);
    LCD_DisplayStringLine(LCD_LINE_7, (uint8_t *)"Send status: Succeeded");
#endif /* LCD_DISPLAY */
  }
  /* Check if data byte has been sent */
  else if (CEC_GetITStatus(CEC_IT_TXBR))
  {
    /* Set EOM bit if the byte to be transmitted is the last one of the Transmit Buffer */
     if (send_inc == (ByteNumber - 1))
    {
      CEC_EndOfMessage();
      CEC_SendData(TransmitBuffer[send_inc++]);
      send_inc=0;
    }
    else
    {
      /* Put the byte in the TX Buffer */
      CEC_SendData(TransmitBuffer[send_inc++]);
    }
    CEC_ClearITPendingBit(CEC_IT_TXBR);
  }
}
#endif /* USE_STM320518_EVAL */

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
