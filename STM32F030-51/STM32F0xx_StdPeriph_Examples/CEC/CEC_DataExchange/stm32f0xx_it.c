/**
  ******************************************************************************
  * @file    CEC/CEC_DataExchange/stm32f0xx_it.c 
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

/** @addtogroup CEC_DataExchange
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Receive buffer */
uint8_t ReceiveBuffer[10];
/* Transmit buffer */
uint8_t TransmitBuffer[10] = {0};
__IO uint8_t ReceivedFrame1 = 0;
uint8_t send_inc = 0, rcv_inc = 0;
extern uint8_t HeaderBlockValueToSend;
uint16_t TransErrorCode1 = 0;
uint16_t RecepErrorCode = 0;
uint8_t ByteNumber = 0;

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
#ifdef USE_STM32072B_EVAL
/**
  * @brief  This function handles EXTI Lines 2 and 3 interrupts requests.
  * @param  None
  * @retval None
  */
void EXTI2_3_IRQHandler(void)
{
  
  for (send_inc=0;send_inc<10;send_inc++)
  {
    TransmitBuffer[send_inc]=0;
  }
  
  send_inc =0;
  STM_EVAL_LEDOff(LED1);
  STM_EVAL_LEDOff(LED2);
  STM_EVAL_LEDOff(LED3);
  STM_EVAL_LEDOff(LED4);

  /* Generate rising edge on Joystick button to detect when we push Joystick button to initiate 
  transmission */
  if(EXTI_GetITStatus(RIGHT_BUTTON_EXTI_LINE) != RESET)
  {
    TransmitBuffer[send_inc++] = 0x46;
    ByteNumber = 0x02;

    /* Clear RIGHT Button EXTI line pending bit */
    EXTI_ClearITPendingBit(RIGHT_BUTTON_EXTI_LINE);
  }
  else if(EXTI_GetITStatus(LEFT_BUTTON_EXTI_LINE) != RESET)
  {
    TransmitBuffer[send_inc++] = 0x9F;
    ByteNumber = 0x02;

    /* Clear LEFT Button EXTI line pending bit */
    EXTI_ClearITPendingBit(LEFT_BUTTON_EXTI_LINE);
  }
  
  send_inc =0;
  /* Write single Data in the TX Buffer to Transmit through the CEC peripheral */
  CEC_SendData(HeaderBlockValueToSend);

  /* Initiate Message Transmission */
  CEC_StartOfMessage();

}
#endif /* USE_STM32072B_EVAL */

/**
  * @brief  This function handles EXTI Lines 4 to 15 interrupts requests.
  * @param  None
  * @retval None
  */
void EXTI4_15_IRQHandler(void)
{
  
  for (send_inc=0;send_inc<10;send_inc++)
  {
    TransmitBuffer[send_inc]=0;
  }
  
  send_inc =0;
  STM_EVAL_LEDOff(LED1);
  STM_EVAL_LEDOff(LED2);
  STM_EVAL_LEDOff(LED3);
  STM_EVAL_LEDOff(LED4);

  /* Generate rising edge on Joystick button to detect when we push Joystick button to initiate 
  transmission */
  
  if(EXTI_GetITStatus(UP_BUTTON_EXTI_LINE) != RESET)
  {
    TransmitBuffer[send_inc++] = 0x44;
    TransmitBuffer[send_inc++] = 0x41;
    ByteNumber = 0x03;
    
    /* Clear UP Button EXTI line pending bit */
    EXTI_ClearITPendingBit(UP_BUTTON_EXTI_LINE);
  }
#ifdef USE_STM320518_EVAL
  else if(EXTI_GetITStatus(RIGHT_BUTTON_EXTI_LINE) != RESET)
  {
    TransmitBuffer[send_inc++] = 0x46;
    ByteNumber = 0x02;

    /* Clear RIGHT Button EXTI line pending bit */
    EXTI_ClearITPendingBit(RIGHT_BUTTON_EXTI_LINE);
  }
  else if(EXTI_GetITStatus(LEFT_BUTTON_EXTI_LINE) != RESET)
  {
    TransmitBuffer[send_inc++] = 0x9F;
    ByteNumber = 0x02;

    /* Clear LEFT Button EXTI line pending bit */
    EXTI_ClearITPendingBit(LEFT_BUTTON_EXTI_LINE);
  }
#endif /* USE_STM320518_EVAL */  
  else if(EXTI_GetITStatus(DOWN_BUTTON_EXTI_LINE) != RESET)
  {
    TransmitBuffer[send_inc++] = 0x44;
    TransmitBuffer[send_inc++] = 0x42;
    ByteNumber = 0x03;

    /* Clear DOWN Button EXTI line pending bit */
    EXTI_ClearITPendingBit(DOWN_BUTTON_EXTI_LINE);
  }

  send_inc =0;
  /* Write single Data in the TX Buffer to Transmit through the CEC peripheral */
  CEC_SendData(HeaderBlockValueToSend);

  /* Initiate Message Transmission */
  CEC_StartOfMessage();

}

/**
  * @brief  This function handles CEC global interrupt request.
  * @param  None
  * @retval None
  */
#ifdef USE_STM320518_EVAL
void CEC_IRQHandler(void)
{
  /********************** Reception *******************************************/
  /* Check if a reception error occured */
  if (CEC->ISR & (CEC_IT_RXACKE | CEC_IT_LBPE | CEC_IT_SBPE | CEC_IT_BRE | CEC_IT_RXOVR))
  {
    RecepErrorCode = CEC->ISR;
    CEC->ISR = (CEC_IT_RXACKE | CEC_IT_LBPE | CEC_IT_SBPE | CEC_IT_BRE | CEC_IT_RXOVR);
    ReceivedFrame1 = 2;
  }

  if (CEC_GetITStatus(CEC_IT_RXBR))
  { 
    /* Check if the byte received is a Header */
    if (rcv_inc == 0)
    {
      ReceiveBuffer[rcv_inc] = ((CEC_ReceiveData() >> 4) & 0x0F);
      rcv_inc++;
    }
    else /* Receive each byte except header and opcode in the reception buffer */
    {
      ReceiveBuffer[rcv_inc] = CEC_ReceiveData();
      rcv_inc++;
    }
    CEC_ClearITPendingBit(CEC_IT_RXBR);
  }

  /* Check if the byte received is the last one of the message */
  if (CEC_GetITStatus(CEC_IT_RXEND))
  {  
    ReceivedFrame1 = 1;

    /* Clear all reception flags */
    CEC_ClearITPendingBit(CEC_IT_RXEND);
  }

  /********************** Transmission ****************************************/
  /* Check if a transmission error occurred */
  if (CEC->ISR & (CEC_IT_TXACKE | CEC_IT_TXERR | CEC_IT_TXUDR | CEC_IT_ARBLST))
  {
    TransErrorCode1 = CEC->ISR;
    CEC->ISR = (CEC_IT_TXACKE | CEC_IT_TXERR | CEC_IT_TXUDR | CEC_IT_ARBLST);
    /* KO */
    /* Turn on LED3 */
    STM_EVAL_LEDOn(LED3);
#ifdef LCD_DISPLAY
    LCD_SetBackColor(LCD_COLOR_RED);
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
    LCD_SetBackColor(LCD_COLOR_CYAN);
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
void CEC_CAN_IRQHandler(void)
{
  /********************** Reception *******************************************/
  /* Check if a reception error occured */
  if (CEC->ISR & (CEC_IT_RXACKE | CEC_IT_LBPE | CEC_IT_SBPE | CEC_IT_BRE | CEC_IT_RXOVR))
  {
    RecepErrorCode = CEC->ISR;
    CEC->ISR = (CEC_IT_RXACKE | CEC_IT_LBPE | CEC_IT_SBPE | CEC_IT_BRE | CEC_IT_RXOVR);
    ReceivedFrame1 = 2;
  }

  if (CEC_GetITStatus(CEC_IT_RXBR))
  { 
    /* Check if the byte received is a Header */
    if (rcv_inc == 0)
    {
      ReceiveBuffer[rcv_inc] = ((CEC_ReceiveData() >> 4) & 0x0F);
      rcv_inc++;
    }
    else /* Receive each byte except header and opcode in the reception buffer */
    {
      ReceiveBuffer[rcv_inc] = CEC_ReceiveData();
      rcv_inc++;
    }
    CEC_ClearITPendingBit(CEC_IT_RXBR);
  }

  /* Check if the byte received is the last one of the message */
  if (CEC_GetITStatus(CEC_IT_RXEND))
  {  
    ReceivedFrame1 = 1;

    /* Clear all reception flags */
    CEC_ClearITPendingBit(CEC_IT_RXEND);
  }

  /********************** Transmission ****************************************/
  /* Check if a transmission error occurred */
  if (CEC->ISR & (CEC_IT_TXACKE | CEC_IT_TXERR | CEC_IT_TXUDR | CEC_IT_ARBLST))
  {
    TransErrorCode1 = CEC->ISR;
    CEC->ISR = (CEC_IT_TXACKE | CEC_IT_TXERR | CEC_IT_TXUDR | CEC_IT_ARBLST);
    /* KO */
    /* Turn on LED3 */
    STM_EVAL_LEDOn(LED3);
#ifdef LCD_DISPLAY
    LCD_SetBackColor(LCD_COLOR_RED);
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
    LCD_SetBackColor(LCD_COLOR_CYAN);
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
