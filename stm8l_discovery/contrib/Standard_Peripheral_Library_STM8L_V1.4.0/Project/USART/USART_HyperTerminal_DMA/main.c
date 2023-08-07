/**
  ******************************************************************************
  * @file    USART/USART_HyperTerminal_DMA/main.c
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

/**
  * @addtogroup USART_HyperTerminal_DMA
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#ifdef USE_STM8L1526_EVAL
/* define the DMA parameters related to USART1 since USE_STM8L1526_EVAL is used */
#define USART_DMA_CHANNEL_RX   DMA1_Channel2
#define USART_DMA_CHANNEL_TX   DMA1_Channel1
#define USART_DMA_FLAG_TCRX    (uint16_t)DMA1_FLAG_TC2
#define USART_DMA_FLAG_TCTX    (uint16_t)DMA1_FLAG_TC1
#define USART_DR_ADDRESS       (uint16_t)0x5231  /* USART1 Data register Address */
#else /* USE_STM8L1528_EVAL */
/* define the DMA parameters related to USART2 since USE_STM8L1528_EVAL is used */
#define USART_DMA_CHANNEL_RX   DMA1_Channel3
#define USART_DMA_CHANNEL_TX   DMA1_Channel0
#define USART_DMA_FLAG_TCRX    (uint16_t)DMA1_FLAG_TC3
#define USART_DMA_FLAG_TCTX    (uint16_t)DMA1_FLAG_TC0
#define USART_DR_ADDRESS       (uint16_t)0x53E1  /* USART2 Data register Address */
#endif /* USE_STM8L1526_EVAL */

#define USART_BAUDRATE         (uint32_t)9600
#define DATA_TO_TRANSFER       (countof(TxBuffer) - 1)
#define DATA_TO_RECEIVE        (uint8_t) 0x20

/* Private macro -------------------------------------------------------------*/
#define countof(a)            (sizeof(a) / sizeof(*(a)))

/* Private variables ---------------------------------------------------------*/
uint8_t TxBuffer[] = "\n\rUSART Example:USART-Hyperterminal communication using DMA.\nEnter your Text\n\r";
uint8_t RxBuffer[DATA_TO_RECEIVE] = {0};

/* Private function prototypes -----------------------------------------------*/
static void CLK_Config(void);
static void DMA_Config(void);
static void USART_Config(void);
void Delay (uint32_t nCount);
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
  
  /* Initialize LEDs mounted on STM8L152X-EVAL board */
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);

   /* USART configuration -------------------------------------------*/
  USART_Config();
  
   /* DMA configuration -------------------------------------------*/
  DMA_Config();

  /* USART Enable */
  USART_Cmd(EVAL_COM1, ENABLE);

  /* Wait the USART DMA Tx\Rx transfer complete */
  while (DMA_GetFlagStatus((DMA_FLAG_TypeDef)USART_DMA_FLAG_TCTX) == RESET);
  while (DMA_GetFlagStatus((DMA_FLAG_TypeDef)USART_DMA_FLAG_TCRX) == RESET);

  while (1)
  {
    STM_EVAL_LEDToggle(LED1);
    STM_EVAL_LEDToggle(LED2);
    STM_EVAL_LEDToggle(LED3);
    STM_EVAL_LEDToggle(LED4);
    Delay((uint32_t)0xFFFFFF);
  }
}

/**
  * @brief  Configure peripherals Clock   
  * @param  None
  * @retval None
  */
static void CLK_Config(void)
{
  /*High speed external clock prescaler: 1*/
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);

  /*Enable DMA clock */
  CLK_PeripheralClockConfig(CLK_Peripheral_DMA1, ENABLE);
}

/**
  * @brief  Configure DMA peripheral  
  * @param  None
  * @retval None
  */
static void DMA_Config(void)
{
  /* Deinitialize DMA channels */
  DMA_GlobalDeInit();

  DMA_DeInit(DMA1_Channel1);
  DMA_DeInit(DMA1_Channel2);

  /* DMA channel Rx of USART Configuration */
  DMA_Init(USART_DMA_CHANNEL_RX, (uint16_t)RxBuffer, (uint16_t)USART_DR_ADDRESS,
           DATA_TO_RECEIVE, DMA_DIR_PeripheralToMemory, DMA_Mode_Normal,
           DMA_MemoryIncMode_Inc, DMA_Priority_Low, DMA_MemoryDataSize_Byte);

  /* DMA channel Tx of USART Configuration */
  DMA_Init(USART_DMA_CHANNEL_TX, (uint16_t)TxBuffer, (uint16_t)USART_DR_ADDRESS,
           DATA_TO_TRANSFER, DMA_DIR_MemoryToPeripheral, DMA_Mode_Normal,
           DMA_MemoryIncMode_Inc, DMA_Priority_High, DMA_MemoryDataSize_Byte);

  /* Enable the USART Tx/Rx DMA requests */
  USART_DMACmd(EVAL_COM1, USART_DMAReq_TX, ENABLE);
  USART_DMACmd(EVAL_COM1, USART_DMAReq_RX, ENABLE);

  /* Global DMA Enable */
  DMA_GlobalCmd(ENABLE);

  /* Enable the USART Tx DMA channel */
  DMA_Cmd(USART_DMA_CHANNEL_TX, ENABLE);
  /* Enable the USART Rx DMA channel */
  DMA_Cmd(USART_DMA_CHANNEL_RX, ENABLE);         
}

/**
  * @brief  Configure USART peripheral  
  * @param  None
  * @retval None
  */
static void USART_Config(void)
{
  /* EVAL COM (USARTx) configuration -----------------------------------------*/
  /* USART configured as follow:
        - BaudRate = USART_BAUDRATE baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Receive and transmit enabled
        - USART Clock disabled
  */
  STM_EVAL_COMInit(COM1, (uint32_t)USART_BAUDRATE, USART_WordLength_8b, USART_StopBits_1,
                   USART_Parity_No, (USART_Mode_TypeDef)(USART_Mode_Tx | USART_Mode_Rx));

  /* USART Disable */
  USART_Cmd(EVAL_COM1, DISABLE);
}

/**
  * @brief  Delay.
  * @param  nCount
  * @retval None
  */
void Delay(uint32_t nCount)
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
