/**
  ******************************************************************************
  * @file    DMA_WFI/main.c
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

/** @addtogroup STM8L15x_StdPeriph_Examples
  * @{
  */
/** @addtogroup DMA_WFI
  * @{
  */
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#ifdef USE_STM8L1526_EVAL
#define USART_DMA_CHANNEL_RX   DMA1_Channel2
#define USART_DR_ADDRESS       (uint16_t)0x5231
#elif defined USE_STM8L1528_EVAL
#define USART_DMA_CHANNEL_RX   DMA1_Channel3
#define USART_DR_ADDRESS       (uint16_t)0x53E1
#endif /* USE_STM8L1526_EVAL */

#define RAM_BUFFER_ADDRESS    ((uint16_t)(&RAMBuffer))
#define RAM_BUFFER_SIZE       ((uint8_t)(0x04))

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t RAMBuffer[RAM_BUFFER_SIZE] = {0, 0, 0, 0};
/* Private function prototypes -----------------------------------------------*/
static void DMA_Config(void);
void LedControl(void);
bool Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void main(void)
{

  /* Enable DMA1 clock */
  CLK_PeripheralClockConfig(CLK_Peripheral_DMA1, ENABLE);

  /* Leds Init*/
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);

  /* EVAL COM (USARTx) configuration -----------------------------------------*/
  /* USART configured as follow:
          - BaudRate = 9600 baud  
          - Word Length = 8 Bits
          - One Stop Bit
          - Odd parity
          - Receive and transmit enabled
          - USART Clock disabled
  */
  STM_EVAL_COMInit(COM1, (uint32_t)9600/*BAUDRATE */, USART_WordLength_8b,
                   USART_StopBits_1, USART_Parity_No, USART_Mode_Rx);

  /* DMA configuration -------------------------------------------*/
  DMA_Config();
  
  while (1)
  {
    /* Enter Wait for Interrupt mode */
    wfi();
  }
}

/**
  * @brief  Configure DMA peripheral 
  * @param  None
  * @retval None
  */
static void DMA_Config(void)
{
  /* DMA channel Rx of USART Configuration */
  DMA_Init(USART_DMA_CHANNEL_RX,
           (uint16_t)RAM_BUFFER_ADDRESS,
           (uint16_t)USART_DR_ADDRESS,
           RAM_BUFFER_SIZE, DMA_DIR_PeripheralToMemory, DMA_Mode_Circular,
           DMA_MemoryIncMode_Inc, DMA_Priority_Low, DMA_MemoryDataSize_Byte);

  /* Enable the USART Rx DMA request */
  USART_DMACmd(EVAL_COM1, USART_DMAReq_RX, ENABLE);

  /* Enable TC interrupt to wake up from wfi mode*/
  DMA_ITConfig(USART_DMA_CHANNEL_RX, DMA_ITx_TC, ENABLE);

  /* Global DMA Enable */
  DMA_GlobalCmd(ENABLE);

  /* Enable the USART Rx DMA channel */
  DMA_Cmd(USART_DMA_CHANNEL_RX, ENABLE);
}

/**
  * @brief  Control Leds activation.
  * @param  None
  * @retval None
  */
void LedControl(void)
{
  /* All leds off*/
  STM_EVAL_LEDOff(LED1);
  STM_EVAL_LEDOff(LED3);
  STM_EVAL_LEDOff(LED2);
  STM_EVAL_LEDOff(LED4);

  if (Buffercmp(RAMBuffer, "led1", RAM_BUFFER_SIZE) != FALSE)
  {
    /* Action 1*/
    STM_EVAL_LEDOn(LED1);
  }
  else if (Buffercmp(RAMBuffer, "led2", RAM_BUFFER_SIZE) != FALSE)
  {
    /* Action 2*/
    STM_EVAL_LEDOn(LED2);
  }
  else if (Buffercmp(RAMBuffer, "led3", RAM_BUFFER_SIZE) != FALSE)
  {
    /* Action 3*/
    STM_EVAL_LEDOn(LED3);
  }
  else if (Buffercmp(RAMBuffer, "led4", RAM_BUFFER_SIZE) != FALSE)
  {
    /* Action 4*/
    STM_EVAL_LEDOn(LED4);
  }
  else
  {
    /* Error : all leds on*/
    STM_EVAL_LEDOn(LED1);
    STM_EVAL_LEDOn(LED3);
    STM_EVAL_LEDOn(LED2);
    STM_EVAL_LEDOn(LED4);
  }

}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval TRUE: pBuffer1 identical to pBuffer2
  *         FALSE: pBuffer1 differs from pBuffer2
  */
bool Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer1 != *pBuffer2)
    {
      return FALSE;
    }

    pBuffer1++;
    pBuffer2++;
  }

  return TRUE;
}
#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
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
