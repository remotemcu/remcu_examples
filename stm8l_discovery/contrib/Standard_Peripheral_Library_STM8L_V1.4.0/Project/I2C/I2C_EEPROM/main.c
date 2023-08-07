/**
  ******************************************************************************
  * @file    I2C/I2C_EEPROM/main.c
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
#include "stm8_eval_i2c_ee.h"

/** @addtogroup STM8L15x_StdPeriph_Examples
  * @{
  */

/**
  * @addtogroup I2C_EEPROM
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/* Private define ------------------------------------------------------------*/

#define EEPROM_WRITE_ADDRESS1         0x0
#define EEPROM_READ_ADDRESS1          0x0
#define TX_BUFFER1_SIZE             (countof(TxBuffer1) - 1)
#define RX_BUFFER1_SIZE               50

#define EEPROM_WRITE_ADDRESS2         0x100
#define EEPROM_READ_ADDRESS2          0x100
#define TX_BUFFER2_SIZE             (countof(TxBuffer2)-1)

#define MESSAGE1_SIZE               (countof(Message1)-1)
#define MESSAGE2_SIZE               (countof(Message2)-1)
#define MESSAGE3_SIZE               (countof(Message3)-1)
/* Private macro -------------------------------------------------------------*/
#define countof(a) (sizeof(a) / sizeof(*(a)))

/* Private variables ---------------------------------------------------------*/
extern uint8_t RxBuffer1[RX_BUFFER1_SIZE];
uint8_t RxBuffer3[RX_BUFFER1_SIZE] = {0};

CONST uint8_t TxBuffer2[] = "STM8L15x I2C Firmware Library Example";
uint8_t RxBuffer2[TX_BUFFER2_SIZE];
__IO TestStatus TransferStatus1 = FAILED, TransferStatus2 = FAILED;
extern __IO ITStatus UsartTransferStatus;

CONST uint8_t Message1[] = "\n\rRxBuffer1 Filled \nI2C write/read operations started";
CONST uint8_t Message2[] = "\n\rI2C operations done with Success";
CONST uint8_t Message3[] = "\n\rI2C operations Failed";

__IO uint16_t NumDataRead = 0;
/* Private functions ---------------------------------------------------------*/
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
void main(void)
{
  uint8_t index = 0;
  /* Select HSE as system clock source */
  CLK_SYSCLKSourceSwitchCmd(ENABLE);
  CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSE);
  /* High speed external clock prescaler: 1*/
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);

  while (CLK_GetSYSCLKSource() != CLK_SYSCLKSource_HSE)
  {}

  /* Enable I2C1 clock */
  CLK_PeripheralClockConfig(CLK_Peripheral_I2C1, ENABLE);

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

  /* Initialize the I2C EEPROM driver ---------------------------------------*/
  sEE_Init();

  /* First write in the memory followed by a read of the written data --------*/
  /* Write on I2C EEPROM from EEPROM_WRITE_ADDRESS1 */
  sEE_WriteBuffer(RxBuffer1, EEPROM_WRITE_ADDRESS1, RX_BUFFER1_SIZE);

  /* Set the Number of data to be read */
  NumDataRead = RX_BUFFER1_SIZE;

  /* Read from I2C EEPROM from EEPROM_READ_ADDRESS1 */
  sEE_ReadBuffer(RxBuffer3, EEPROM_READ_ADDRESS1, (uint16_t*)&NumDataRead);

  /* Starting from this point, if the requested number of data is higher than 1,
     then only the DMA is managing the data transfer. Meanwhile, CPU is free to 
     perform other tasks:
    
    // Add your code here: 
    //...
    //...

     For simplicity reasons, this example is just waiting till the end of the 
     transfer. */


  /* Wait till DMA transfer is complete (Transfer complete interrupt handler
    resets the variable holding the number of data to be read) */
  while (NumDataRead > 0)
  {}

  /* Check if the data written to the memory is read correctly */
  TransferStatus1 = Buffercmp(RxBuffer1, RxBuffer3, RX_BUFFER1_SIZE);
  /* TransferStatus1 = PASSED, if the transmitted and received data
     to/from the EEPROM are the same */
  /* TransferStatus1 = FAILED, if the transmitted and received data
     to/from the EEPROM are different */

  /* Wait for EEPROM standby state */
  sEE_WaitEepromStandbyState();

  /* Second write in the memory followed by a read of the written data -------*/
  /* Write on I2C EEPROM from EEPROM_WRITE_ADDRESS2 */
  sEE_WriteBuffer((uint8_t*)TxBuffer2, EEPROM_WRITE_ADDRESS2, TX_BUFFER2_SIZE);

  /* Set the Number of data to be read */
  NumDataRead = TX_BUFFER2_SIZE;

  /* Read from I2C EEPROM from EEPROM_READ_ADDRESS2 */
  sEE_ReadBuffer(RxBuffer2, EEPROM_READ_ADDRESS2, (uint16_t*)&NumDataRead);

  /* Starting from this point, if the requested number of data is higher than 1,
     then only the DMA is managing the data transfer. Meanwhile, CPU is free to 
     perform other tasks:
    
    // Add your code here: 
    //...
    //...

     For simplicity reasons, this example is just waiting till the end of the 
     transfer. */


  /* Wait till DMA transfer is complete (Transfer complete interrupt handler
    resets the variable holding the number of data to be read) */
  while (NumDataRead > 0)
  {}

  /* Check if the data written to the memory is read correctly */
  TransferStatus2 = Buffercmp((uint8_t*)TxBuffer2, RxBuffer2, TX_BUFFER2_SIZE);
  /* TransferStatus2 = PASSED, if the transmitted and received data
     to/from the EEPROM are the same */
  /* TransferStatus2 = FAILED, if the transmitted and received data
     to/from the EEPROM are different */
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

  /* Free all used resources */
  sEE_DeInit();

  while (1);
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval PASSED: pBuffer1 identical to pBuffer2
  *   FAILED: pBuffer1 differs from pBuffer2
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
