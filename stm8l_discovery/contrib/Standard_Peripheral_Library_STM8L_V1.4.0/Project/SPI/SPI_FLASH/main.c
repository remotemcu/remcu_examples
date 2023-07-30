/**
  ******************************************************************************
  * @file    SPI/SPI_FLASH/main.c
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
#include "stm8_eval_spi_flash.h"
#include "stm8_eval.h"

/** @addtogroup STM8L15x_StdPeriph_Examples
  * @{
  */

/** @addtogroup SPI_FLASH
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;
/* Private define ------------------------------------------------------------*/
#define  FLASH_WRITE_ADDRESS        0x700000
#define  FLASH_READ_ADDRESS         FLASH_WRITE_ADDRESS
#define  FLASH_ERASE_ADDRESS        FLASH_WRITE_ADDRESS
#define  M25P128_FLASH_ID           0x202018

#define  RX_BUFFER_SIZE              50

#define MESSAGE1_SIZE               (countof(Message1)-1)
#define MESSAGE2_SIZE               (countof(Message2)-1)
#define MESSAGE3_SIZE               (countof(Message3)-1)
/* Private macro -------------------------------------------------------------*/
#define countof(a) (sizeof(a) / sizeof(*(a)))

/* Private variables ---------------------------------------------------------*/
uint8_t Index = 0;
__IO TestStatus TransferStatus1 = FAILED, TransferStatus2 = PASSED;
__IO uint32_t FlashID = 0;
extern __IO ITStatus UsartTransferStatus;
extern uint8_t RxBuffer1[RX_BUFFER_SIZE];
uint8_t RxBuffer2[RX_BUFFER_SIZE] =
  {
    0
  };

CONST uint8_t Message1[] = "\nRxBuffer1 Filled \nSPI write/read operations started";
CONST uint8_t Message2[] = "\nSPI operations done with Success";
CONST uint8_t Message3[] = "\nSPI operations Failed";
/* Private functions ---------------------------------------------------------*/
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void main(void)
{
  uint8_t index = 0;
  /* Initialize Leds mounted on STM8L152X-EVAL board */
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED2);

  /* Select HSE as system clock source */
  CLK_SYSCLKSourceSwitchCmd(ENABLE);
  CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSE);
  /*High speed external clock prescaler: 1*/
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_2);

  while (CLK_GetSYSCLKSource() != CLK_SYSCLKSource_HSE)
  {}

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

  /* Enable general interrupts */
  enableInterrupts();

  /* Enable the USART Receive interrupt: this interrupt is generated when the USART
      receive data register is not empty */
  USART_ITConfig(EVAL_COM1, USART_IT_RXNE, ENABLE);

  /* Enable the USART Transmit complete interrupt: this interrupt is generated when
     the USART transmit Shift Register is empty */
  USART_ITConfig(EVAL_COM1, USART_IT_TC, ENABLE);

  while (UsartTransferStatus == RESET);
  for (index = 0; index < MESSAGE1_SIZE; index++)
  {
    /* Wait while USART TC = 0 */
    while (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TC) == RESET);
    /* Send on byte from stm8l15x USART to HyperTherminal */
    USART_SendData8(EVAL_COM1, Message1[index]);
  }

  /* Disable general interrupts */
  disableInterrupts();

  /* Initialize the SPI FLASH driver */
  sFLASH_Init();

  /* Get SPI Flash ID */
  FlashID = sFLASH_ReadID();

  /* Check the SPI Flash ID */
  if (FlashID == M25P128_FLASH_ID)
  {
    /* OK: Turn on LD1 */
    STM_EVAL_LEDOn(LED1);

    /* Perform a write in the Flash followed by a read of the written data */
    /* Erase SPI FLASH Sector to write on */
    sFLASH_EraseSector(FLASH_ERASE_ADDRESS);

    /* Write Tx_Buffer data to SPI FLASH memory */
    sFLASH_WriteBuffer(RxBuffer1, FLASH_WRITE_ADDRESS, RX_BUFFER_SIZE);

    /* Read data from SPI FLASH memory */
    sFLASH_ReadBuffer(RxBuffer2, FLASH_READ_ADDRESS, RX_BUFFER_SIZE);

    /* Check the corectness of written dada */
    TransferStatus1 = Buffercmp(RxBuffer1, RxBuffer2, RX_BUFFER_SIZE);
    /* TransferStatus1 = PASSED, if the transmitted and received data by SPI
       are the same */
    /* TransferStatus1 = FAILED, if the transmitted and received data by SPI
       are different */

    /* Perform an erase in the Flash followed by a read of the written data */
    /* Erase SPI FLASH Sector to write on */
    sFLASH_EraseSector(FLASH_ERASE_ADDRESS);

    /* Read data from SPI FLASH memory */
    sFLASH_ReadBuffer(RxBuffer2, FLASH_ERASE_ADDRESS, RX_BUFFER_SIZE);

    /* Check the corectness of erasing operation dada */
    for (Index = 0; Index < RX_BUFFER_SIZE; Index++)
    {
      if (RxBuffer2[Index] != 0xFF)
      {
        TransferStatus2 = FAILED;
      }
    }
    /* TransferStatus2 = PASSED, if the specified sector part is erased */
    /* TransferStatus2 = FAILED, if the specified sector part is not well erased */
    if ((TransferStatus1 != FAILED) && (TransferStatus2 != FAILED))
    {
      for (index = 0; index < MESSAGE2_SIZE; index++)
      {
        /* Wait while USART TC = 0 */
        while (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TC) == RESET);
        /* Send on byte from stm8l15x USART to HyperTherminal */
        USART_SendData8(EVAL_COM1, Message2[index]);
      }
    }
    else
    {
      for (index = 0; index < MESSAGE3_SIZE; index++)
      {
        /* Wait while USART TC = 0 */
        while (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TC) == RESET);
        /* Send on byte from stm8l15x USART to HyperTherminal */
        USART_SendData8(EVAL_COM1, Message3[index]);
      }
    }
  }
  else
  {
    /* Error: Turn on LD2 */
    STM_EVAL_LEDOn(LED2);
  }

  while (1);
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
