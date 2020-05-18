/**
  ******************************************************************************
  * @file    DMA/DMA_USARTTransfer/main.c 
  * @author  MCD Application Team
  * @version V1.4.0
  * @date    24-July-2014
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
#include "main.h"

/** @addtogroup STM32F0xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup DMA_USARTTransfer
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t TxBuffer1[] = "Communication between USART1-USART2 using DMA2";
uint8_t TxBuffer2[] = "Communication between USART2-USART1 using DMA2";
uint8_t RxBuffer2 [RXBUFFERSIZE] = {0};
uint8_t RxBuffer1 [RXBUFFERSIZE] = {0};
__IO TestStatus TransferStatus1 = FAILED, TransferStatus2 = FAILED;
USART_InitTypeDef USART_InitStructure;
DMA_InitTypeDef  DMA_InitStructure;

/* Private function prototypes -----------------------------------------------*/
static void RCC_Config(void);
static void GPIO_Config(void);
static TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f0xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f0xx.c file
  */
  /* Enable periph Clock */
  RCC_Config();
  
  /* Configure Usarts Pins TX/RX */
  GPIO_Config();
  
  /*------------------------------- USART-------------------------------------*/  
  /* USARTx configured as follow:
  - BaudRate = 9600 
  - Word Length = USART_WordLength_7b
  - Stop Bit = USART_StopBits_1
  - Parity = USART_Parity_No
  - Hardware flow control disabled (RTS and CTS signals)
  - Receive and transmit enabled
  */
  USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_WordLength = USART_WordLength_7b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Tx| USART_Mode_Rx;
  USART_Init(USART1, &USART_InitStructure);
  USART_Init(USART2, &USART_InitStructure);

  /*------------------------------- DMA---------------------------------------*/   
  /* Common DMA configuration */
  DMA_InitStructure.DMA_BufferSize = TXBUFFERSIZE;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  
  /* DMA2 Channel1 configuration */
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)TxBuffer1;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
  DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
  DMA_InitStructure.DMA_PeripheralBaseAddr = USART1_TDR_Address;
  DMA_Init(DMA2_Channel1, &DMA_InitStructure);
  
  /* DMA remap Channel1 to USART1 Tx */
  DMA_RemapConfig(DMA2, DMA2_CH1_USART1_TX);
  
  /* DMA2 Channel2 configuration */
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)RxBuffer2;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
  DMA_InitStructure.DMA_PeripheralBaseAddr = USART2_RDR_Address;
  DMA_Init(DMA2_Channel2, &DMA_InitStructure);
  
  /* DMA remap Channel2 to USART2 Rx */
  DMA_RemapConfig(DMA2, DMA2_CH2_USART2_RX);
  
  /* DMA2 Channel4 configuration */
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)TxBuffer2;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
  DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
  DMA_InitStructure.DMA_PeripheralBaseAddr = USART2_TDR_Address;
  DMA_Init(DMA2_Channel4, &DMA_InitStructure);
  
  /* DMA remap Channel4 to USART2 Tx */
  DMA_RemapConfig(DMA2, DMA2_CH4_USART2_TX);
  
  /* DMA2 Channel3 configuration */
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)RxBuffer1;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
  DMA_InitStructure.DMA_PeripheralBaseAddr = USART1_RDR_Address;
  DMA_Init(DMA2_Channel3, &DMA_InitStructure);
  
  /* DMA remap Channel43to USART1 Rx */
  DMA_RemapConfig(DMA2, DMA2_CH3_USART1_RX); 
    
  /* Enable the USART2 Rx and Tx DMA2 requests */
  USART_DMACmd(USART2, USART_DMAReq_Rx|USART_DMAReq_Tx, ENABLE);
  
  /* Enable the USART1 Rx and Tx DMA2 requests */
  USART_DMACmd(USART1, USART_DMAReq_Tx|USART_DMAReq_Rx, ENABLE);
  
  /* Enable the DMA2 channels */
  DMA_Cmd(DMA2_Channel2, ENABLE);
  DMA_Cmd(DMA2_Channel1, ENABLE); 
  DMA_Cmd(DMA2_Channel3, ENABLE); 
  DMA_Cmd(DMA2_Channel4, ENABLE);
  
  /* Enable Usart */
  USART_Cmd(USART2, ENABLE);
  USART_Cmd(USART1, ENABLE);
  
  /* Test on Channels DMA_FLAG_TC flag */
  while(!DMA_GetFlagStatus(DMA2_FLAG_TC1));
  while(!DMA_GetFlagStatus(DMA2_FLAG_TC2)); 
  while(!DMA_GetFlagStatus(DMA2_FLAG_TC4));
  while(!DMA_GetFlagStatus(DMA2_FLAG_TC3)); 
  
  /* Clear DMA2 TC flags */
  DMA_ClearFlag(DMA2_FLAG_TC1);
  DMA_ClearFlag(DMA2_FLAG_TC2);
  DMA_ClearFlag(DMA2_FLAG_TC3);
  DMA_ClearFlag(DMA2_FLAG_TC4);
  
  /* Disable DMA2 channels */
  DMA_Cmd(DMA2_Channel1, DISABLE); 
  DMA_Cmd(DMA2_Channel2, DISABLE);
  DMA_Cmd(DMA2_Channel3, DISABLE); 
  DMA_Cmd(DMA2_Channel4, DISABLE);  
  
  /* Check the received data with the send ones */
  /* TransferStatus1 = PASSED, if the data transmitted from USART2 and received 
     by USART1 are the same */
  /* TransferStatus1 = FAILED, if the data transmitted from USART2 and received 
     by USART1 are different */
  TransferStatus1 = Buffercmp((uint8_t*) RxBuffer1, (uint8_t*) TxBuffer2, RXBUFFERSIZE);

  /* TransferStatus2 = PASSED, if the data transmitted from USART1 and received 
     by USART2 are the same */
  /* TransferStatus2 = FAILED, if the data transmitted from USART1 and received 
     by USART2 are different */
  TransferStatus2 = Buffercmp((uint8_t*) RxBuffer2, (uint8_t*) TxBuffer1, RXBUFFERSIZE);
  
  /* Infinite Loop */
  while(1);
}

/**
  * @brief  Configures the USART Peripheral.
  * @param  None
  * @retval None
  */
static void RCC_Config(void)
{ 
  /* Enable GPIO clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA|RCC_AHBPeriph_GPIOD, ENABLE);
  
  /* Enable USARTs Clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);  
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  
  /* Enable the DMA periph */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);
  
  /* Enable Syscfg */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
}

/**
  * @brief  Configures the USART Peripheral.
  * @param  None
  * @retval None
  */
static void GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* USART1 Pins configuration ************************************************/
  /* Connect pin to Periph */
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1); 
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);    
  
  /* Configure pins as AF pushpull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);  

  /* USART2 Pins configuration ************************************************/  
  /* Connect pin to Periph */
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_0);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_0);    
  
  /* Configure pins as AF pushpull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &GPIO_InitStructure);   
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval PASSED: pBuffer1 identical to pBuffer2
  *         FAILED: pBuffer1 differs from pBuffer2
  */
static TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer1 != *pBuffer2)
    {
      return FAILED;
    }
    pBuffer1++;
    pBuffer2++;
  }
  
  return PASSED;
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
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
  {
  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
