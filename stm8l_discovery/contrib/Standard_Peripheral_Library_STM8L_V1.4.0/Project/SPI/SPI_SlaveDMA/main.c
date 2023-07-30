/**
  ******************************************************************************
  * @file    SPI/SPI_SlaveDMA/main.c
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
#include "stm8_eval.h"

/** @addtogroup STM8L15x_StdPeriph_Examples
  * @{
  */

/** @addtogroup SPI_SlaveDMA
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/* Private define ------------------------------------------------------------*/
#define  RX_BUFFER_SIZE              50

#define MESSAGE1_SIZE               (countof(Message1)-1)
#define MESSAGE2_SIZE               (countof(Message2)-1)
#define MESSAGE3_SIZE               (countof(Message3)-1)

#define SPI_DR_Address       ((uint16_t)0x5204)  /*SPI Data register Address*/
#define SPI_DMAChannelRx   DMA1_Channel1
#define SPI_DMA_FlagTCRx    (uint16_t)DMA1_FLAG_TC1

/* Private macro -------------------------------------------------------------*/
#define countof(a) (sizeof(a) / sizeof(*(a)))

/* Private variables ---------------------------------------------------------*/
__IO TestStatus TransferStatus1 = FAILED;
extern __IO ITStatus UsartTransferStatus;
extern __IO uint8_t RxBuffer1[RX_BUFFER_SIZE];
uint8_t SPIBuffer_Rx[RX_BUFFER_SIZE] = {0};
CONST uint8_t Message1[] = "\nRxBuffer1 Filled \nSPI write/read operations started";
CONST uint8_t Message2[] = "\nSPI operations done with Success";
CONST uint8_t Message3[] = "\nSPI operations Failed";
/* Private functions prototypes ----------------------------------------------*/
static void CLK_Config(void);
static void USART_Config(void);
static void DMA_Config(void);
static void SPI_Config(void);

TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void main(void)
{
  uint32_t index = 0;

   /* CLK configuration -------------------------------------------*/
  CLK_Config(); 
 
  /* Set the MOSI and SCK at high level */
  GPIO_ExternalPullUpConfig(GPIOB, GPIO_Pin_6 | GPIO_Pin_5, ENABLE);

#ifdef USE_STM8L1526_EVAL
  /* Set USART CK at high level */
  GPIO_ExternalPullUpConfig(GPIOC, GPIO_Pin_4, ENABLE);
#else /* USE_STM8L1528_EVAL is defined */
  /* Set USART CK at high level */
  GPIO_ExternalPullUpConfig(GPIOH, GPIO_Pin_6, ENABLE);
#endif  /* USE_STM8L1526_EVAL */

   /* USART configuration -------------------------------------------*/
  USART_Config(); 
  
   /* SPI configuration -------------------------------------------*/
  SPI_Config(); 
  
  /*Disable USART the master*/
  USART_Cmd(EVAL_COM1, DISABLE);

   /* DMA configuration -------------------------------------------*/
  DMA_Config(); 
 
  /* Enable USART */
  USART_Cmd(EVAL_COM1, ENABLE);

  /* Insert Delay to ensure Synchro w/ SPI */
  for (index = 0; index < 0xFF; index++);

  /* Enable SPI */
  SPI_Cmd(SPI1, ENABLE);

  USART_ITConfig(EVAL_COM1, USART_IT_TC, ENABLE);

  /* Wait until Data transmitted to SPI*/
  while (UsartTransferStatus != 2);
  /* Wait the SPI DMA Rx transfer complete */
  while (DMA_GetFlagStatus((DMA_FLAG_TypeDef)SPI_DMA_FlagTCRx) == RESET);


  /* Check the correctness of written dada */
  TransferStatus1 = Buffercmp((uint8_t*)RxBuffer1, SPIBuffer_Rx, RX_BUFFER_SIZE);
  /* TransferStatus1 = PASSED, if the received data by USART and received data
    by SPI1 are the same */
  /* TransferStatus1 = FAILED, if the received data by USART and received data
     by SPI1 are different */

  if (TransferStatus1 != FAILED)
  {
    for (index = 0; index < MESSAGE2_SIZE; index++)
    {
      /* Wait while USART TC = 0 */
      while (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TC) == RESET);
      /* Send on byte from stm8l15x USART to HyperTerminal */
      USART_SendData8(EVAL_COM1, Message2[index]);
    }
  }
  else
  {
    for (index = 0; index < MESSAGE3_SIZE; index++)
    {
      /* Wait while USART TC = 0 */
      while (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TC) == RESET);
      /* Send on byte from stm8l15x USART to HyperTerminal */
      USART_SendData8(EVAL_COM1, Message3[index]);
    }
  }

  while (1)
  {}
}

/**
  * @brief  Configure peripheral clock 
  * @param  None
  * @retval None
  */
static void CLK_Config(void)
{
  /* Select HSE as system clock source */
  CLK_SYSCLKSourceSwitchCmd(ENABLE);
  CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSE);
  
  /*High speed external clock prescaler: 1*/
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_2);

  while (CLK_GetSYSCLKSource() != CLK_SYSCLKSource_HSE)
  {}

  /* Enable SPI clock */
  CLK_PeripheralClockConfig(CLK_Peripheral_SPI1, ENABLE);
  /* Enable DMA clock */
  CLK_PeripheralClockConfig(CLK_Peripheral_DMA1, ENABLE);
}

/**
  * @brief  Configure USART peripheral  
  * @param  None
  * @retval None
  */
static void USART_Config(void)
{
  USART_DeInit(EVAL_COM1);
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
                   USART_Parity_Odd, (USART_Mode_TypeDef)(USART_Mode_Tx | USART_Mode_Rx));

  /* Enable the USART Receive interrupt: this interrupt is generated when the USART
      receives data: DR is not empty */
  USART_ITConfig(EVAL_COM1, USART_IT_RXNE, ENABLE);

  /* Enable the USART Transmit complete interrupt: this interrupt is generated when
     the USART transmit Shift Register is empty */
  USART_ITConfig(EVAL_COM1, USART_IT_TC, ENABLE);

  /* Enable general interrupts */
  enableInterrupts();

  /* Wait until data to be sent to SPI is Received */
  while (UsartTransferStatus != 1);

  /*Disable USART the master*/
  USART_DeInit(EVAL_COM1);

  /* USART configuration  for data transmission to SPI */
  /* USART configured as follow:
          - BaudRate = 115200 baud  
          - Word Length = 8 Bits
          - One Stop Bit
          - No parity
          - Receive and transmit enabled
  */
  STM_EVAL_COMInit(COM1, (uint32_t)115200, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No,
                   (USART_Mode_TypeDef)(USART_Mode_Tx | USART_Mode_Rx));
  USART_ClockInit(EVAL_COM1, USART_Clock_Enable, USART_CPOL_Low, USART_CPHA_2Edge, USART_LastBit_Enable);
}

/**
  * @brief  Configure SPI peripheral  
  * @param  None
  * @retval None
  */
static void SPI_Config(void)
{
SPI_DeInit(SPI1);
  /* SPI Configuration --------------------------------------------------------*/
  SPI_Init(SPI1, SPI_FirstBit_LSB, SPI_BaudRatePrescaler_2, SPI_Mode_Slave,
           SPI_CPOL_Low, SPI_CPHA_2Edge, SPI_Direction_2Lines_FullDuplex,
           SPI_NSS_Soft, (uint8_t)0x07);
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
  DMA_DeInit(SPI_DMAChannelRx);
  DMA_SetTimeOut(0x3F);

  /* DMA channel Rx of SPI Configuration */
  DMA_Init(SPI_DMAChannelRx, (uint16_t)SPIBuffer_Rx, (uint16_t)SPI_DR_Address, \
           RX_BUFFER_SIZE, DMA_DIR_PeripheralToMemory, DMA_Mode_Normal, \
           DMA_MemoryIncMode_Inc, DMA_Priority_High, DMA_MemoryDataSize_Byte);


  /* Enable the SPI Rx DMA requests */
  SPI_DMACmd(SPI1, SPI_DMAReq_RX, ENABLE);

  /* Enable Global DMA */
  DMA_GlobalCmd(ENABLE);

  /* Enable the SPI RX DMA channel */
  DMA_Cmd(SPI_DMAChannelRx, ENABLE);
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval PASSED: pBuffer1 identical to pBuffer2
  *         FAILED: pBuffer1 differs from pBuffer2
  */
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
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