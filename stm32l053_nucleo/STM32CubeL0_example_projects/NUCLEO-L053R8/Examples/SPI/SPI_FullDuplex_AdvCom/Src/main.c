/**
  ******************************************************************************
  * @file    SPI/SPI_FullDuplex_AdvCom/Src/main.c 
  * @author  MCD Application Team
  * @brief   This sample code shows how to use STM32L0xx SPI HAL API to transmit 
  *          and receive a data buffer with a communication process based on
  *          Polling transfer. 
  *          The communication is done using 2 boards.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32L0xx_HAL_Examples
  * @{
  */

/** @addtogroup SPI_FullDuplex_AdvCom
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define SPI_ACK_BYTES             0xA5A5
#define SPI_NACK_BYTES            0xDEAD
#define SPI_TIMEOUT_MAX           0x1000

/* Defines used for tranfer communication*/
#define ADDRCMD_MASTER_READ                         ((uint16_t)0x1234)
#define ADDRCMD_MASTER_WRITE                        ((uint16_t)0x5678)
#define COM_LENGTH                                  ((uint16_t)0x0020)

/* Private macro ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* SPI handler declaration */
SPI_HandleTypeDef SpiHandle;
uint32_t SpiTimeout = SPI_TIMEOUT_MAX;
__IO ITStatus SpiReady = RESET;

#ifdef MASTER_BOARD
FlagStatus TestReady = RESET;
#else
GPIO_PinState CsTriggered = GPIO_PIN_RESET;
FlagStatus DataValid = RESET;
#endif

/* Buffer used for transmission */
uint8_t aTxMasterBuffer[] = "SPI - MASTER - Transmit message";
uint8_t aTxSlaveBuffer[]  = "SPI - SLAVE - Transmit message ";
/* Buffer used for reception */
uint8_t aRxBuffer[COM_LENGTH];

/* Private function prototypes -----------------------------------------------*/
#ifdef MASTER_BOARD
static void Master_Send_AddrCmd(uint16_t Address, uint16_t Length);
static ADVCOM_StatusTypeDef Master_Read_Data(uint8_t* pBuffer, uint16_t NumByteToRead);
static ADVCOM_StatusTypeDef Master_Write_Data(uint8_t* pBuffer, uint16_t NumByteToWrite);
static ADVCOM_StatusTypeDef Master_Check_Ack(void);
#else
static void Slave_Receive_AddrCmd(uint8_t *pAddrcmd);
static void Slave_Send_Ack(FlagStatus DataOk);
#endif
static void SPI_IO_Write(uint8_t* pBuffer, uint16_t NumByteToWrite);
static void SPI_IO_Read(uint8_t* pBuffer, uint16_t NumByteToRead);
static HAL_StatusTypeDef SPI_Write(SPI_HandleTypeDef * hspi,uint8_t * pData,uint16_t Size);
static HAL_StatusTypeDef SPI_Read(SPI_HandleTypeDef * hspi,uint8_t * pData,uint16_t Size);
static void SystemClock_Config(void);
static void Error_Handler(void);
static uint16_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);
static void Flush_Buffer(uint8_t* pBuffer, uint16_t BufferLength);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
#ifdef MASTER_BOARD
  ADVCOM_StatusTypeDef comstatus;
#else
  uint16_t addrcmd = 0;
  uint16_t comlength = 0;
  uint8_t pAddrcmd[4] = {0x00};
  FlagStatus DataValid = RESET;
#endif

  /* STM32L0xx HAL library initialization:
       - Configure the Flash prefetch, Flash preread and Buffer caches
       - Systick timer is configured by default as source of time base, but user 
             can eventually implement his proper time base source (a general purpose 
             timer for example or other time source), keeping in mind that Time base 
             duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
             handled in milliseconds basis.
       - Low Level Initialization
     */
  if(HAL_Init()!= HAL_OK)
  {
    /* Start Conversation Error */
    Error_Handler(); 
  }
  
  /* Configure the system clock to 2 Mhz */
  SystemClock_Config();
  
  /* Configure LED2 */
  BSP_LED_Init(LED2);
  
  /*##-1- Configure the SPI peripheral #######################################*/
  /* Set the SPI parameters */
  SpiHandle.Instance               = SPIx;
  SpiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
  SpiHandle.Init.Direction         = SPI_DIRECTION_2LINES;
  SpiHandle.Init.CLKPhase          = SPI_PHASE_2EDGE;
  SpiHandle.Init.CLKPolarity       = SPI_POLARITY_LOW;
  SpiHandle.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
  SpiHandle.Init.CRCPolynomial     = 7;
  SpiHandle.Init.DataSize          = SPI_DATASIZE_8BIT;
  SpiHandle.Init.FirstBit          = SPI_FIRSTBIT_MSB;
  SpiHandle.Init.NSS               = SPI_NSS_SOFT;
  SpiHandle.Init.TIMode            = SPI_TIMODE_DISABLE;
  
#ifdef MASTER_BOARD
  SpiHandle.Init.Mode = SPI_MODE_MASTER;
#else
  SpiHandle.Init.Mode = SPI_MODE_SLAVE;
#endif /* MASTER_BOARD */

  if(HAL_SPI_Init(&SpiHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

#ifdef MASTER_BOARD
  /* Configure user push button */
  BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_EXTI);
  BSP_LED_On(LED2);
  /* Wait for user Button press before starting the communication.*/
  while(TestReady != SET)
  {
  }
  BSP_LED_Off(LED2);

  /* Infinite loop */  
  while(1)
  {
    /* Send READ command to slave until communication status is different than non-aknowledge */
    do
    {
      comstatus = Master_Read_Data(aRxBuffer, COM_LENGTH);
    }while(comstatus == ADVCOM_NACK);
    
    /* If communication status is ok, check received datas */
    if(comstatus == ADVCOM_ACK)
    {
      /* Compare received buffer with one expected from slave */
      if(Buffercmp((uint8_t*)aTxSlaveBuffer, (uint8_t*)aRxBuffer, COM_LENGTH))
      {
        /* Transfer error in transmission process */
        Error_Handler();
      }
      else
      {
        /* Turn LED2 on: Reception is correct */
        BSP_LED_Toggle(LED2);
      }
    }
    else if(comstatus == ADVCOM_ERROR)
    {
      /* Transfer error in transmission process */
      Error_Handler();
    }
    
    /* wait for 100 ms before next communication */
    HAL_Delay(100);
    
    /* Send WRITE command to slave until communication status is different than non-aknowledge */
    do
    {
      comstatus = Master_Write_Data(aTxMasterBuffer, COM_LENGTH);
    }while(comstatus == ADVCOM_NACK);

    /* If communication status is error, call error handler */
    if(comstatus == ADVCOM_ERROR)
    {
      /* Transfer error in transmission process */
      Error_Handler();
    }
    
    /* Flush Rx buffer for next transmission */
    Flush_Buffer(aRxBuffer, COM_LENGTH);
    
    /* Toggle LED2 on: test is correct */
    BSP_LED_Toggle(LED2);
    HAL_Delay(100);
  }
  
#else
  /* enter while loop too keep treating new request from Master */
  while(1)
  {
    
    /* put Slave in receive mode, waiting for Master command */
    Slave_Receive_AddrCmd(pAddrcmd);
    
    /* If Chip Select went low */
    if(CsTriggered == GPIO_PIN_RESET)
    {
      /* Compute command and required data length */
      addrcmd = (uint16_t) ((pAddrcmd[0] << 8) | pAddrcmd[1]);
      comlength = (uint16_t) ((pAddrcmd[2] << 8) | pAddrcmd[3]);
      
      /* Check if received command correct */
      if(((addrcmd == ADDRCMD_MASTER_READ) || (addrcmd == ADDRCMD_MASTER_WRITE)) 
        && (comlength > 0))
      {
        DataValid = SET;
      }
      else
      {
        DataValid = RESET;
      }
    }
    else
    {
      /* error in communication process */
      DataValid = RESET;
    }
    
    /* Send ackowledge to Master */
    Slave_Send_Ack(DataValid);
    
    /* Treat command if Chip Select still Low and command is a valid data*/
    if((CsTriggered == GPIO_PIN_RESET) && (DataValid == SET))
    {
      /* Check if command is Master requiring data read or write, else go in error mode */
      if(addrcmd == ADDRCMD_MASTER_READ)
      {
        SPI_IO_Write(aTxSlaveBuffer, comlength);
      }
      else if(addrcmd == ADDRCMD_MASTER_WRITE)
      {
        SPI_IO_Read(aRxBuffer, comlength);
      
        /* In case, Master has sent data, compare received buffer with one expected */
        if(Buffercmp((uint8_t*)aTxMasterBuffer, (uint8_t*)aRxBuffer, COM_LENGTH))
        {
          /* Transfer error in transmission process */
          Error_Handler();
        }
        else
        {
          /* Toggle LED2 on: Reception is correct */
          BSP_LED_Toggle(LED2);
        }
      }
      
      /* Send ackowledge if Chip Select still Low */
      if(CsTriggered == GPIO_PIN_RESET)
      {
        Slave_Send_Ack(DataValid);
      }
    }
    
    /* Flush Rx buffer for next transmission */
    Flush_Buffer(aRxBuffer, COM_LENGTH);
    
    /* Reset Data valid variable for next communication*/
    DataValid = RESET;
  }
#endif /* MASTER_BOARD */
}

#ifdef MASTER_BOARD
/**
  * @brief  Send a read command to slave, then receive datas through SPI
  * @param  pBuffer : pointer to the buffer containing the data to be read through SPI.
  * @param  NumByteToWrite: Number of bytes to read.
  * @retval   ADVCOM_ERROR, ADVCOM_ACK or ADVCOM_NACK
  */
static ADVCOM_StatusTypeDef Master_Read_Data(uint8_t* pBuffer, uint16_t NumByteToRead)
{
  ADVCOM_StatusTypeDef ret = ADVCOM_ERROR;
  uint16_t addrcmd = ADDRCMD_MASTER_READ;

  /* Set chip select Low at the start of the transmission */
  SPI_CS_LOW();
  
  /* Call function to send command and number of bytes required to be read */
  Master_Send_AddrCmd(addrcmd, NumByteToRead);
  
  /* Wait 5 ms */
  HAL_Delay(5);
  
  /* Check for ackowledge bytes sent by Slave */
  ret = Master_Check_Ack();

  /* Set chip select High at the end of the transmission */ 
  SPI_CS_HIGH();
  
  /* Wait 1 ms */
  HAL_Delay(1);
  
  if(ret != ADVCOM_ACK)
  {
    return ret;
  }

  /* Set chip select Low at the start of the transmission */
  SPI_CS_LOW();
  
  /* Wait 5 ms */
  HAL_Delay(5);
  
  /* Call SPI read function to receive data from slave */
  SPI_IO_Read(pBuffer, NumByteToRead);
  
  /* Check for ackowledge bytes sent by Slave */
  ret = Master_Check_Ack();

  /* Set chip select High at the end of the transmission */ 
  SPI_CS_HIGH();

  return ret;
}

/**
  * @brief  Send a write command to slave, then send datas through SPI
  * @param  pBuffer : pointer to the buffer containing the data to be written through SPI.
  * @param  NumByteToWrite: Number of bytes to write.
  * @retval   ADVCOM_ERROR, ADVCOM_ACK or ADVCOM_NACK
  */
static ADVCOM_StatusTypeDef Master_Write_Data(uint8_t* pBuffer, uint16_t NumByteToWrite)
{
  ADVCOM_StatusTypeDef ret = ADVCOM_ERROR;
  uint16_t addrcmd = ADDRCMD_MASTER_WRITE;
    
  /* Set chip select Low at the start of the transmission */
  SPI_CS_LOW();
  
  /* Call function to send command and number of bytes required to be write */
  Master_Send_AddrCmd(addrcmd, NumByteToWrite);
  
  HAL_Delay(5);
  
  /* Check for ackowledge bytes sent by Slave */
  ret = Master_Check_Ack();
  
  /* Set chip select High at the end of the transmission */ 
  SPI_CS_HIGH();
  
  /* Wait 1 ms */
  HAL_Delay(1);
  
  if(ret != ADVCOM_ACK)
  {
    return ret;
  }
  
  /* Set chip select Low at the start of the transmission */
  SPI_CS_LOW();
  
  /* Wait 5 ms */
  HAL_Delay(5);
  
  /* Call SPI write function to send data to slave */
  SPI_IO_Write(pBuffer, NumByteToWrite);
  
  /* Check for ackowledge bytes sent by Slave */
  ret = Master_Check_Ack();

  /* Set chip select High at the end of the transmission */ 
  SPI_CS_HIGH();
  return ret;
}


/**
  * @brief  Computes Address/Command data and transmission length then send it to Master thtough SPI
  * @param  Address: command to be sent Read or Write
  * @param  Length: next communication Length
  * @retval   ADVCOM_ERROR, ADVCOM_ACK or ADVCOM_NACK
  */
static void Master_Send_AddrCmd(uint16_t Address, uint16_t Length)
{
  uint8_t addrcmd[4]={0};

  addrcmd[0] = (uint8_t) (Address >> 8);
  addrcmd[1] = (uint8_t) Address;
  addrcmd[2] = (uint8_t) (Length >> 8);
  addrcmd[3] = (uint8_t) Length;
  
  SPI_IO_Write(addrcmd, 4);
}

/**
  * @brief  Check acknowledge bytes from Slave
  * @param  None
  * @retval  None
  */
static ADVCOM_StatusTypeDef Master_Check_Ack(void)
{
  ADVCOM_StatusTypeDef ret = ADVCOM_ERROR;
  uint16_t ackbytes = 0x0000;

  /* Master in Receive state to wait for ACK from Slave */
  SPI_IO_Read((uint8_t *)&ackbytes, sizeof(ackbytes));
  
  if(ackbytes == SPI_ACK_BYTES)
  {
    ret = ADVCOM_ACK;
  }
  else if(ackbytes == SPI_NACK_BYTES)
  {
    ret = ADVCOM_NACK;
  }

  return ret;
}

#else
/**
  * @brief  Reads SPI frame from Master
  * @param  pAddrcmd: pointer to buffer
  * @retval  None
  */
static void Slave_Receive_AddrCmd(uint8_t *pAddrcmd)
{
  /* Put SPI slave on infinite Timeout as we don't know when command will be sent */
  SpiTimeout = 0xFFFFFFFF;

  /* Read 4 bytes */
  SPI_IO_Read(pAddrcmd, sizeof(pAddrcmd));

  /* Set timeout back to its original value */
  SpiTimeout = SPI_TIMEOUT_MAX;
}

/**
  * @brief  Sends acknowledge byte to let Master knows Slave is ready
  * @param  DataOk : to send ack or nack bytes
  * @retval  None
  */
static void Slave_Send_Ack(FlagStatus DataOk)
{
  uint16_t ackbyte = 0x0000;
  
  if(DataOk == SET)
  {
    ackbyte = SPI_ACK_BYTES;
  }
  else
  {
    ackbyte = SPI_NACK_BYTES;
  }
  /* Send ACK or NACK bytes */
  SPI_IO_Write((uint8_t *)&ackbyte, sizeof(ackbyte));
  
  /* wait until CS go high */
  while(CsTriggered != GPIO_PIN_SET)
  {
  }
  
  /* wait 5 ms */
  HAL_Delay(5);
}

#endif /* MASTER_BOARD */

/**
  * @brief  Writes data through SPI. This function calls low layer fonction SPI_Write which redirects
                to SPI HAL function according to selected mode (Polling, IT)
  * @param  pBuffer : pointer to the buffer containing the data to be written through SPI.
  * @param  NumByteToWrite: Number of bytes to write.
  * @retval None
  */
static void SPI_IO_Write(uint8_t* pBuffer, uint16_t NumByteToWrite)
{
#if defined(SPI_MODE_IT)
  /* Reset volatile varaible before transmission*/
  SpiReady = RESET;
#endif

  /* Call SPI write generic function */
  if(SPI_Write(&SpiHandle, pBuffer, NumByteToWrite) != HAL_OK)
  {
    Error_Handler();
  }

#if defined(SPI_MODE_IT)
  /* wait until transmission is finished */
  while(SpiReady == RESET);
#endif
  
}

/**
    * @brief  Read data through SPI. This function calls low layer fonction SPI_Read which redirects
                  to SPI HAL function according to selected mode (Polling, IT)
    * @param  pBuffer : pointer to the buffer containing the data to be read through SPI.
    * @param  NumByteToRead: Number of bytes to read.
    * @retval None
    */
static void SPI_IO_Read(uint8_t* pBuffer, uint16_t NumByteToRead)
{
#if defined(SPI_MODE_IT)
  /* Reset volatile varaible before transmission*/
  SpiReady = RESET;
#endif

  /* Call SPI read generic function */
  if(SPI_Read(&SpiHandle, pBuffer, NumByteToRead) != HAL_OK)
  {
    Error_Handler();
  }
  
#if defined(SPI_MODE_IT)
  /* wait until transmission is finished */
  while(SpiReady == RESET);
#endif
  
}

/**
  * @brief  Write data through SPI. This function calls SPI HAL function according to selected 
                mode (Polling, IT)
  * @param  hspi: pointer to spi handler.
  * @param  pData : pointer to the buffer containing the data to be written through SPI.
  * @param  Size: Number of bytes to write.
  * @retval None
  */
static HAL_StatusTypeDef SPI_Write(SPI_HandleTypeDef * hspi,uint8_t * pData,uint16_t Size)
{
/* .........*/
#if defined(SPI_MODE_IT)
  return HAL_SPI_Transmit_IT(hspi, pData, Size);
#else
  return HAL_SPI_Transmit( hspi, pData, Size, SpiTimeout);
#endif
}

/**
  * @brief  Read data through SPI. This function calls HAL SPI function according to selected 
                mode (Polling, IT)
  * @param  hspi: pointer to spi handler.
  * @param  pData : pointer to the buffer containing the data to be read through SPI.
  * @param  Size: Number of bytes to read.
  * @retval None
  */
static HAL_StatusTypeDef SPI_Read(SPI_HandleTypeDef * hspi,uint8_t * pData,uint16_t Size)
{
/* .........*/
#if defined (SPI_MODE_IT)
  return HAL_SPI_Receive_IT(hspi, pData, Size);
#else
  return HAL_SPI_Receive( hspi, pData, Size, SpiTimeout);
#endif
}

/**
  * @brief  EXTI line detection callbacks.
  * @param  GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  
#if defined(MASTER_BOARD)
  if(KEY_BUTTON_PIN == GPIO_Pin)
  {
    TestReady = SET;
  }
#else
  /* Check if CS has been set High or Low */
  if(SPIx_NSS_PIN == GPIO_Pin)
  {
    CsTriggered = HAL_GPIO_ReadPin(SPIx_NSS_GPIO_PORT, GPIO_Pin);
  }
#endif
}

#if defined(SPI_MODE_IT)
/**
  * @brief  TxRx Transfer completed callback
  * @param  hspi: SPI handle. 
  * @note   This example shows a simple way to report end of Interrupt TxRx transfer, and 
  *         you can add your own implementation. 
  * @retval None
  */
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{ 
  /* Set variable value to SET : transmission is finished*/
  SpiReady = SET;
}

/**
  * @brief  TxRx Transfer completed callback
  * @param  hspi: SPI handle. 
  * @note   This example shows a simple way to report end of Interrupt TxRx transfer, and 
  *         you can add your own implementation. 
  * @retval None
  */
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
  
  /* Set variable value to SET : transmission is finished*/
  SpiReady = SET;
}

/**
  * @brief  SPI error callbacks
  * @param  hspi: SPI handle
  * @note   This example shows a simple way to report transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
 void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi)
{ 
  /* Set variable value to SET : transmission is finished*/
  SpiReady = RESET;

  /* call error handler */
  Error_Handler();
}
#endif

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = MSI
  *            SYSCLK(Hz)                     = 2000000
  *            HCLK(Hz)                       = 2000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            Flash Latency(WS)              = 0
  *            Main regulator output voltage  = Scale3 mode
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  
  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  
  /* Enable MSI Oscillator */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_5;
  RCC_OscInitStruct.MSICalibrationValue=0x00;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  
  /* Select MSI as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* Infinite loop */ 
  while(1)
  {
  }
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval 0  : pBuffer1 identical to pBuffer2
  *         >0 : pBuffer1 differs from pBuffer2
  */
static uint16_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    if((*pBuffer1) != *pBuffer2)
    {
      return BufferLength;
    }
    pBuffer1++;
    pBuffer2++;
  }

  return 0;
}

/**
  * @brief  Flushes the buffer
  * @param  pBuffer: buffers to be flushed.
  * @param  BufferLength: buffer's length
  * @retval None
  */
static void Flush_Buffer(uint8_t* pBuffer, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    *pBuffer = 0;

    pBuffer++;
  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
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
