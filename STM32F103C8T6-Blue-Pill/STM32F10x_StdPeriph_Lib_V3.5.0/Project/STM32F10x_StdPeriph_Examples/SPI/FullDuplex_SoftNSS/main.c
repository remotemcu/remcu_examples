/**
  ******************************************************************************
  * @file    SPI/FullDuplex_SoftNSS/main.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "platform_config.h"

/** @addtogroup STM32F10x_StdPeriph_Examples
  * @{
  */

/** @addtogroup SPI_FullDuplex_SoftNSS
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/* Private define ------------------------------------------------------------*/
#define BufferSize 32

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
SPI_InitTypeDef   SPI_InitStructure;
uint8_t SPIy_Buffer_Tx[BufferSize] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                                      0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E,
                                      0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15,
                                      0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C,
                                      0x1D, 0x1E, 0x1F, 0x20};
uint8_t SPIz_Buffer_Tx[BufferSize] = {0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,
                                      0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E,
                                      0x5F, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 
                                      0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C,
                                      0x6D, 0x6E, 0x6F, 0x70};
uint8_t SPIy_Buffer_Rx[BufferSize], SPIz_Buffer_Rx[BufferSize];
__IO uint8_t TxIdx = 0, RxIdx = 0, k = 0;
volatile TestStatus TransferStatus1 = FAILED, TransferStatus2 = FAILED;
volatile TestStatus TransferStatus3 = FAILED, TransferStatus4 = FAILED;

/* Private functions ---------------------------------------------------------*/
void RCC_Configuration(void);
void GPIO_Configuration(uint16_t SPIy_Mode, uint16_t SPIz_Mode);
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f10x_xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f10x.c file
     */     
       
  /* System clocks configuration ---------------------------------------------*/
  RCC_Configuration();

  /* 1st phase: SPIy Master and SPIz Slave */
  /* GPIO configuration ------------------------------------------------------*/
  GPIO_Configuration(SPI_Mode_Master, SPI_Mode_Slave);
  
  /* SPIy Config -------------------------------------------------------------*/
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_LSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPIy, &SPI_InitStructure);

  /* SPIz Config -------------------------------------------------------------*/
  SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;
  SPI_Init(SPIz, &SPI_InitStructure);

  /* Enable SPIy */
  SPI_Cmd(SPIy, ENABLE);
  /* Enable SPIz */
  SPI_Cmd(SPIz, ENABLE);

  /* Transfer procedure */
  while (TxIdx < BufferSize)
  {
    /* Wait for SPIy Tx buffer empty */
    while (SPI_I2S_GetFlagStatus(SPIy, SPI_I2S_FLAG_TXE) == RESET);
    /* Send SPIz data */
    SPI_I2S_SendData(SPIz, SPIz_Buffer_Tx[TxIdx]);
    /* Send SPIy data */
    SPI_I2S_SendData(SPIy, SPIy_Buffer_Tx[TxIdx++]);
    /* Wait for SPIz data reception */
    while (SPI_I2S_GetFlagStatus(SPIz, SPI_I2S_FLAG_RXNE) == RESET);
    /* Read SPIz received data */
    SPIz_Buffer_Rx[RxIdx] = SPI_I2S_ReceiveData(SPIz);
    /* Wait for SPIy data reception */
    while (SPI_I2S_GetFlagStatus(SPIy, SPI_I2S_FLAG_RXNE) == RESET);
    /* Read SPIy received data */
    SPIy_Buffer_Rx[RxIdx++] = SPI_I2S_ReceiveData(SPIy);
  }

  /* Check the correctness of written dada */
  TransferStatus1 = Buffercmp(SPIz_Buffer_Rx, SPIy_Buffer_Tx, BufferSize);
  TransferStatus2 = Buffercmp(SPIy_Buffer_Rx, SPIz_Buffer_Tx, BufferSize);
  /* TransferStatus1, TransferStatus2 = PASSED, if the transmitted and received data
     are equal */
  /* TransferStatus1, TransferStatus2 = FAILED, if the transmitted and received data
     are different */

  /* 2nd phase: SPIy Slave and SPIz Master */
  /* GPIO configuration ------------------------------------------------------*/
  GPIO_Configuration(SPI_Mode_Slave , SPI_Mode_Master);
  
  /* SPIy Re-configuration ---------------------------------------------------*/
  SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;
  SPI_Init(SPIy, &SPI_InitStructure);

  /* SPIz Re-configuration ---------------------------------------------------*/
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_Init(SPIz, &SPI_InitStructure);

  /* Reset TxIdx, RxIdx indexes and receive tables values */
  TxIdx = 0;
  RxIdx = 0;
  for (k = 0; k < BufferSize; k++)  SPIz_Buffer_Rx[k] = 0;
  for (k = 0; k < BufferSize; k++)  SPIy_Buffer_Rx[k] = 0;

  /* Transfer procedure */
  while (TxIdx < BufferSize)
  {
    /* Wait for SPIz Tx buffer empty */
    while (SPI_I2S_GetFlagStatus(SPIz, SPI_I2S_FLAG_TXE) == RESET);
    /* Send SPIy data */
    SPI_I2S_SendData(SPIy, SPIy_Buffer_Tx[TxIdx]);
    /* Send SPIz data */
    SPI_I2S_SendData(SPIz, SPIz_Buffer_Tx[TxIdx++]);
    /* Wait for SPIy data reception */
    while (SPI_I2S_GetFlagStatus(SPIy, SPI_I2S_FLAG_RXNE) == RESET);
    /* Read SPIy received data */
    SPIy_Buffer_Rx[RxIdx] = SPI_I2S_ReceiveData(SPIy);
    /* Wait for SPIz data reception */
    while (SPI_I2S_GetFlagStatus(SPIz, SPI_I2S_FLAG_RXNE) == RESET);
    /* Read SPIz received data */
    SPIz_Buffer_Rx[RxIdx++] = SPI_I2S_ReceiveData(SPIz);
  }

  /* Check the correctness of written dada */
  TransferStatus3 = Buffercmp(SPIz_Buffer_Rx, SPIy_Buffer_Tx, BufferSize);
  TransferStatus4 = Buffercmp(SPIy_Buffer_Rx, SPIz_Buffer_Tx, BufferSize);
  /* TransferStatus3, TransferStatus4 = PASSED, if the transmitted and received data
     are equal */
  /* TransferStatus3, TransferStatus4 = FAILED, if the transmitted and received data
     are different */

  while (1)
  {}
}

/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
void RCC_Configuration(void)
{
  /* PCLK2 = HCLK/2 */
  RCC_PCLK2Config(RCC_HCLK_Div2); 

/* Enable peripheral clocks --------------------------------------------------*/
#ifdef USE_STM3210C_EVAL
  /* Enable GPIO clock for SPIy and SPIz */
  RCC_APB2PeriphClockCmd(SPIy_GPIO_CLK | SPIz_GPIO_CLK | RCC_APB2Periph_AFIO, ENABLE);

  /* Enable SPIy Periph clock */
  RCC_APB1PeriphClockCmd(SPIy_CLK, ENABLE);
                           
#else
  /* Enable SPIy clock and GPIO clock for SPIy and SPIz */
  RCC_APB2PeriphClockCmd(SPIy_GPIO_CLK | SPIz_GPIO_CLK | SPIy_CLK, ENABLE);
#endif
  /* Enable SPIz Periph clock */
  RCC_APB1PeriphClockCmd(SPIz_CLK, ENABLE);
}

/**
  * @brief  Configures the different SPIy and SPIz GPIO ports.
  * @param  SPIy_Mode: Specifies the SPIy operating mode. 
  *            This parameter can be:
  *              -  SPIy_Mode_Master
  *              -  SPIy_Mode_Slave                 
  * @param  SPIz_Mode: Specifies the SPIz operating mode. 
  *            This parameter can be:
  *              -  SPIz_Mode_Master
  *              -  SPIz_Mode_Slave 
  * @retval None
  */
void GPIO_Configuration(uint16_t SPIy_Mode, uint16_t SPIz_Mode)
{
  GPIO_InitTypeDef GPIO_InitStructure;

#ifdef USE_STM3210C_EVAL
  /* Enable SPI3 Pins Software Remapping */
  GPIO_PinRemapConfig(GPIO_Remap_SPI3, ENABLE);
#endif

  /* Configure SPIy pins: SCK, MISO and MOSI ---------------------------------*/
  GPIO_InitStructure.GPIO_Pin = SPIy_PIN_SCK | SPIy_PIN_MOSI;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  if(SPIy_Mode == SPI_Mode_Master)
  {
    /* Configure SCK and MOSI pins as Alternate Function Push Pull */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  }
  else
  {
    /* Configure SCK and MOSI pins as Input Floating */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  }
  GPIO_Init(SPIy_GPIO, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = SPIy_PIN_MISO;

  if(SPIy_Mode == SPI_Mode_Master)
  {
    /* Configure MISO pin as Input Floating  */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  }
  else
  {
    /* Configure MISO pin as Alternate Function Push Pull */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  }
  GPIO_Init(SPIy_GPIO, &GPIO_InitStructure);
  
  /* Configure SPIz pins: SCK, MISO and MOSI ---------------------------------*/
  GPIO_InitStructure.GPIO_Pin = SPIz_PIN_SCK | SPIz_PIN_MOSI;

  if(SPIz_Mode == SPI_Mode_Slave)
  {
    /* Configure SCK and MOSI pins as Input Floating */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  }
  else
  { 
    /* Configure SCK and MOSI pins as Alternate Function Push Pull */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  }
  GPIO_Init(SPIz_GPIO, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = SPIz_PIN_MISO;
  if(SPIz_Mode == SPI_Mode_Slave)
  {
    /* Configure MISO pin as Alternate Function Push Pull */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  }
  else
  { /* Configure MISO pin as Input Floating  */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  }
  GPIO_Init(SPIz_GPIO, &GPIO_InitStructure);
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
  {}
}

#endif
/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
