/**
  ******************************************************************************
  * @file    I2C/I2C_TwoBoards/I2C_DataExchange/Master/main.c
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

/** @addtogroup I2C_TwoBoards
  * @{
  */

/** @addtogroup I2C_DataExchange
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
extern uint8_t HEADER_ADDRESS_Read = (((SLAVE_ADDRESS & 0xFF00) >> 7) | 0xF1);
extern uint8_t HEADER_ADDRESS_Write;
/* Private variables ---------------------------------------------------------*/
__IO uint8_t Rx_Idx = 0, Tx_Idx = 0;
__IO uint8_t NumByteToRead = BUFFERSIZE;
__IO uint8_t NumOfBytes = BUFFERSIZE;
uint8_t i = 0;
__IO uint8_t RxBuffer[BUFFERSIZE];
TestStatus TransferStatus1 = FAILED;
extern __IO uint8_t TxBuffer[BUFFERSIZE];
/* Private function prototypes -----------------------------------------------*/
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);
void Delay(__IO uint32_t nCount);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void main()
{

  /* I2C  clock Enable*/
  CLK_PeripheralClockConfig(CLK_Peripheral_I2C1, ENABLE);

#ifdef FAST_I2C_MODE
  /* system_clock / 1 */
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);
#else
  /* system_clock / 2 */
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_2);
#endif

  /* Initialize LEDs mounted on STM8L1526-EVAL board */
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);

  /* Initialize I2C peripheral */
  I2C_Init(I2C1, I2C_SPEED, 0xA0,
           I2C_Mode_I2C, I2C_DutyCycle_2,
           I2C_Ack_Enable, I2C_AcknowledgedAddress_7bit);

  /* Enable Buffer and Event Interrupt*/
  I2C_ITConfig(I2C1, (I2C_IT_TypeDef)(I2C_IT_EVT | I2C_IT_BUF) , ENABLE);

  enableInterrupts();

  /* TXBuffer initialization */
  for (i = 0; i < BUFFERSIZE; i++)
    TxBuffer[i] = i;

  /* Send START condition */
  I2C_GenerateSTART(I2C1, ENABLE);
  while (NumOfBytes);
  while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));

  /* Add a delay to be sure that communication is finished */
  Delay(0xFFFF);

  /*****  reception phase ***/
  /*  Wait while the bus is busy */
  while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));

  /* Send START condition */
  I2C_GenerateSTART(I2C1, ENABLE);

  /* Test on EV5 and clear it */
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

#ifdef TEN_BITS_ADDRESS
  /* Send Header to Slave for write */
  I2C_SendData(I2C1, HEADER_ADDRESS_Write);

  /* Test on EV9 and clear it*/
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_ADDRESS10));

  /* Send slave Address */
  I2C_Send7bitAddress(I2C1, (uint8_t)SLAVE_ADDRESS, I2C_Direction_Transmitter);

  /* Test on EV6 and clear it */
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

  /* Repeated Start */
  I2C_GenerateSTART(I2C1, ENABLE);

  /* Test on EV5 and clear it */
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

  /* Send Header to Slave for Read */
  I2C_SendData(I2C1, HEADER_ADDRESS_Read);

  /* Test on EV6 and clear it */
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

#else
  /* Send slave Address for write */
  I2C_Send7bitAddress(I2C1, SLAVE_ADDRESS, I2C_Direction_Receiver);

  /* Test on EV6 and clear it */
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
#endif /* TEN_BITS_ADDRESS */

  /* While there is data to be read */
  while (NumByteToRead)
  {
#ifdef SAFE_PROCEDURE
    if (NumByteToRead != 3) /* Receive bytes from first byte until byte N-3 */
    {
      while ((I2C_GetLastEvent(I2C1) & 0x04) != 0x04); /* Poll on BTF */

      /* Read a byte from the Slave */
      RxBuffer[Rx_Idx] = I2C_ReceiveData(I2C1);

      /* Point to the next location where the byte read will be saved */
      Rx_Idx++;

      /* Decrement the read bytes counter */
      NumByteToRead--;
    }

    if (NumByteToRead == 3)  /* it remains to read three data: data N-2, data N-1, Data N */
    {
      /* Data N-2 in DR and data N -1 in shift register */
      while ((I2C_GetLastEvent(I2C1) & 0x04) != 0x04); /* Poll on BTF */

      /* Clear ACK */
      I2C_AcknowledgeConfig(I2C1, DISABLE);

      /* Disable general interrupts */
      disableInterrupts();

      /* Read Data N-2 */
      RxBuffer[Rx_Idx] = I2C_ReceiveData(I2C1);

      /* Point to the next location where the byte read will be saved */
      Rx_Idx++;

      /* Program the STOP */
      I2C_GenerateSTOP(I2C1, ENABLE);

      /* Read DataN-1 */
      RxBuffer[Rx_Idx] = I2C_ReceiveData(I2C1);

      /* Enable General interrupts */
      enableInterrupts();

      /* Point to the next location where the byte read will be saved */
      Rx_Idx++;

      while ((I2C_GetLastEvent(I2C1) & 0x40) != 0x40); /* Poll on RxNE */

      /* Read DataN */
      RxBuffer[Rx_Idx] = I2C_ReceiveData(I2C1);

      /* Reset the number of bytes to be read by master */
      NumByteToRead = 0;

    }
#else
    if (NumByteToRead == 1)
    {
      /* Disable Acknowledgement */
      I2C_AcknowledgeConfig(I2C1, DISABLE);

      /* Send STOP Condition */
      I2C_GenerateSTOP(I2C1, ENABLE);

      /* Poll on RxNE Flag */
      while ((I2C_GetFlagStatus(I2C1, I2C_FLAG_RXNE) == RESET));
      /* Read a byte from the Slave */
      RxBuffer[Rx_Idx] = I2C_ReceiveData(I2C1);

      /* Point to the next location where the byte read will be saved */
      Rx_Idx++;

      /* Decrement the read bytes counter */
      NumByteToRead--;
    }

    /* Test on EV7 and clear it */
    if (I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED) )
    {
      /* Read a byte */
      RxBuffer[Rx_Idx] = I2C_ReceiveData(I2C1);

      /* Point to the next location where the byte read will be saved */
      Rx_Idx++;

      /* Decrement the read bytes counter */
      NumByteToRead--;
    }
#endif /* SAFE_PROCEDURE */
  }

  /* check if sent and received data are not corrupted */
  TransferStatus1 = Buffercmp((uint8_t*)TxBuffer, (uint8_t*) RxBuffer, BUFFERSIZE);

  if (TransferStatus1 != FAILED)
  {
    while (1)
    {
      /* Toggle LED1*/
      STM_EVAL_LEDToggle(LED1);
      /* Insert delay */
      Delay(0x7FFF);
    }
  }
  else
  {
    while (1)
    {
      /* Toggle LED4*/
      STM_EVAL_LEDToggle(LED4);
      /* Insert delay */
      Delay(0x7FFF);
    }
  }
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

/**
  * @brief  Inserts a delay time.
  * @param  nCount: specifies the delay time length.
  * @retval None
  */
void Delay(__IO uint32_t nCount)
{
  for (; nCount != 0; nCount--);
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

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/