/**
  ******************************************************************************
  * @file    CAN/CAN_DualFIFO/stm32f0xx_it.c 
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

/** @addtogroup CAN_DualFIFO
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define MESSAGE1   "FIFO0 : Msg received" 
#define MESSAGE2   "FIFO1 : Msg received" 

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
CanRxMsg RxMessage, RxMessage1;
extern uint8_t KeyNumber;
extern CanTxMsg TxMessage, TxMessage1;

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

/******************************************************************************/
/*                 STM32F0xx Peripherals Interrupt Handlers                   */
/******************************************************************************/
/**
* @brief  This function handles CAN request.
* @param  None
* @retval None
*/
void CEC_CAN_IRQHandler(void)
{
  if (CAN_MessagePending(CANx, CAN_FIFO0) != 0) 
  {
    /* Set the LCD Back Color */
    LCD_SetBackColor(Green);
    /* Set the LCD Text Color */
    LCD_SetTextColor(Black);
    /* Displays MESSAGE1 on line 1 */
    LCD_DisplayStringLine(LINE(6), (uint8_t *)MESSAGE1);
    
    CAN_Receive(CANx, CAN_FIFO0, &RxMessage);
    LED_Display(RxMessage.Data[0]);
    KeyNumber = RxMessage.Data[0];
  }
  
  if (CAN_MessagePending(CANx, CAN_FIFO1) != 0) 
  {
    /* Set the LCD Back Color */
    LCD_SetBackColor(Cyan);
    
    /* Set the LCD Text Color */
    LCD_SetTextColor(Black);
    
    /* Displays MESSAGE1 on line 1 */
    LCD_DisplayStringLine(LINE(6), (uint8_t *)MESSAGE2);
    CAN_Receive(CANx, CAN_FIFO1, &RxMessage1);
    LED_Display(RxMessage1.Data[0]);
    KeyNumber = RxMessage1.Data[0];
  }

}

/**
  * @brief  This function handles EXTI 6 request.
  * @param  None
  * @retval None
  */
void EXTI4_15_IRQHandler(void)
{ 
 if(KeyNumber < 0x2) 

  {
    KeyNumber = 0x05;
  }
  LED_Display(--KeyNumber);

  TxMessage.Data[0] = KeyNumber;

  CAN_Transmit(CANx, &TxMessage);
  
  /* Set the LCD Back Color */
  LCD_SetBackColor(Green);
  /* Set the LCD Text Color */
  LCD_SetTextColor(Black);
  /* Displays MESSAGE1 on line 6 */
  LCD_DisplayStringLine(LINE(6), (uint8_t *)MESSAGE1);
   
  /* Clear the EXTI line 6 pending bit */
  EXTI_ClearITPendingBit(EXTI_Line13);
}

/**
  * @brief  This function handles EXTI 10 to 15 request.
  * @param  None
  * @retval None
  */
void EXTI0_1_IRQHandler(void)
{
  if(KeyNumber == 0x4) 
  {
    KeyNumber = 0x00;
  }
  LED_Display(++KeyNumber);
  TxMessage1.Data[0] = KeyNumber;
  CAN_Transmit(CANx, &TxMessage1);
  
  /* Set the LCD Back Color */
  LCD_SetBackColor(Cyan);
  
  /* Set the LCD Text Color */
  LCD_SetTextColor(Black);
  
  /* Displays MESSAGE1 on line 6 */
  LCD_DisplayStringLine(LINE(6), (uint8_t *)MESSAGE2);
   
  /* Clear the EXTI line 13 pending bit */
  EXTI_ClearITPendingBit(EXTI_Line0);
  
}
/******************************************************************************/
/*                 STM32F0xx Peripherals Interrupt Handlers                   */
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
