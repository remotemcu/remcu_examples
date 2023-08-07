/**
  ******************************************************************************
  * @file    DMA/DMA_USARTtoRAM/main.c
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
#include "stm8_eval_lcd.h"
#include "stm8_eval.h"

/** @addtogroup STM8L15x_StdPeriph_Examples
  * @{
  */

/** @addtogroup DMA_USARTtoRAM
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#ifdef USE_STM8L1526_EVAL
/* define the DMA parameters related to USART1 since USE_STM8L1526_EVAL is used */
#define USART_DMA_CHANNEL_RX   DMA1_Channel2
#define USART_DR_ADDRESS       (uint16_t)0x5231  /* USART1 Data register Address */
#else /* USE_STM8L1528_EVAL */
/* define the DMA parameters related to USART2 since USE_STM8L1528_EVAL is used */
#define USART_DMA_CHANNEL_RX   DMA1_Channel3
#define USART_DR_ADDRESS       (uint16_t)0x53E1  /* USART2 Data register Address */
#endif /* USE_STM8L1526_EVAL */

#define BUFFER_ADDRESS       ((uint16_t)&DisplayBuffer)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
char DisplayBuffer[] = "[    LINE 1   ][    LINE 2   ]";

/* Private function prototypes -----------------------------------------------*/
static void CLK_Config(void);
static void DMA_Config(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void main(void)
{
  /* CLK configuration -------------------------------------------*/
  CLK_Config();
  
  /* Init the Eval board LCD */
  STM8_EVAL_LCD_Init();

  /* Clear LCD */
  LCD_Clear();


  /* EVAL COM (USARTx) configuration -----------------------------------------*/
  /* USART configured as follow:
          - BaudRate = 115200 baud  
          - Word Length = 8 Bits
          - One Stop Bit
          - Odd parity
          - Receive and transmit enabled
          - USART Clock disabled
  */
  STM_EVAL_COMInit(COM1, (uint32_t)115200, USART_WordLength_8b, USART_StopBits_1,
                   USART_Parity_No, USART_Mode_Rx);

  /* DMA configuration -------------------------------------------*/
  DMA_Config();
  
  /* Enable the USART RX DMA request */
  USART_DMACmd(EVAL_COM1, USART_DMAReq_RX, ENABLE);

  while (1)
  {
    LCD_SetCursorPos(LCD_LINE1, 0);
    LCD_Print((uint8_t *)DisplayBuffer);

    LCD_SetCursorPos(LCD_LINE2, 0);
    LCD_Print((uint8_t *)&DisplayBuffer[15]);
  }
}

/**
  * @brief  Configure peripherals clock 
  * @param  None
  * @retval None
  */
static void CLK_Config(void)
{
  /* High speed internal clock prescaler: 1*/
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);

  /* Enable DMA1 clock */
  CLK_PeripheralClockConfig(CLK_Peripheral_DMA1, ENABLE);
}

/**
  * @brief  Configure DMA peripheral 
  * @param  None
  * @retval None
  */
static void DMA_Config(void)
{
  /* DMA1 configuration*/
  DMA_GlobalDeInit();
  DMA_DeInit(USART_DMA_CHANNEL_RX);

  DMA_Init(USART_DMA_CHANNEL_RX, BUFFER_ADDRESS, USART_DR_ADDRESS, 30, /* DMA_BufferSize */
           DMA_DIR_PeripheralToMemory, DMA_Mode_Circular, DMA_MemoryIncMode_Inc,
           DMA_Priority_Low, DMA_MemoryDataSize_Byte );
  DMA_Cmd(USART_DMA_CHANNEL_RX, ENABLE);
  DMA_GlobalCmd(ENABLE);
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
