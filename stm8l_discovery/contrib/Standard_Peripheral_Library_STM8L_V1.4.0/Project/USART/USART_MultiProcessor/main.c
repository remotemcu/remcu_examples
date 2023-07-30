/**
  ******************************************************************************
  * @file    USART/USART_MultiProcessor/main.c
  * @author  MCD Application Team
  * @version V1.5.2
  * @date    30-September-2014
  * @brief   Main program body
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
#include "stm8l15x.h"
#include "stm8_eval.h"
#include "main.h"

/** @addtogroup STM8L15x_StdPeriph_Examples
  * @{
  */

/**
  * @addtogroup USART_MultiProcessor
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void Delay(uint16_t nCount);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void main(void)
{
  /*High speed external clock prescaler: 1*/
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);

  /* Initialize I/O in Input Mode with Interrupt for Key button */
  STM_EVAL_PBInit(BUTTON_KEY, BUTTON_MODE_EXTI);

  enableInterrupts();

  USART_DeInit(EVAL_COM1);
  /* EVAL COM (USARTx) configuration -----------------------------------------*/
  /* USART configured as follow:
        - BaudRate = 9600 baud  
        - Word Length = 9 Bits
        - One Stop Bit
        - No parity
        - Receive  enabled (USART_Slave)
        - Transmit enabled (USART_Master)
  */
#ifdef USART_SLAVE

  /* Initialize I/Os in Output Mode for LEDs */
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);

  STM_EVAL_COMInit(COM1, (uint32_t)9600, USART_WordLength_9b,
                   USART_StopBits_1, USART_Parity_No, USART_Mode_Rx);

  /* Enable the USART Receive interrupt: this interrupt is generated when the
    USART receive data register is not empty */
  USART_ITConfig(EVAL_COM1, USART_IT_RXNE, ENABLE);

  /* Set the USART Address */
  USART_SetAddress(EVAL_COM1, 0x2);

  /* Select the USART WakeUp Method */
  USART_WakeUpConfig(EVAL_COM1, USART_WakeUp_AddressMark);

  STM_EVAL_LEDOn(LED1);
  STM_EVAL_LEDOn(LED2);
  STM_EVAL_LEDOn(LED3);
  STM_EVAL_LEDOn(LED4);
#else /* USART_MASTER */

  STM_EVAL_COMInit(COM1, (uint32_t)9600, USART_WordLength_9b, USART_StopBits_1,
                   USART_Parity_No, USART_Mode_Tx);
  /* Enable the USART Transmit interrupt: this interrupt is generated when the USART transmit
  data register is empty */
  USART_ITConfig(EVAL_COM1, USART_IT_TXE, ENABLE);

  /* Set the USART Address */
  USART_SetAddress(EVAL_COM1, 0x1);
#endif /* USART_SLAVE */

  while (1)
  {}
}

/**
  * @brief  Delay.
  * @param  nCount
  * @retval None
  */
void Delay(uint16_t nCount)
{
  /* Decrement nCount value */
  while (nCount != 0)
  {
    nCount--;
  }
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif
/**
  * @}
  */

/**
  * @}
  */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/