/**
  ******************************************************************************
  * @file    DMA/SPI_RAM/main.c 
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
#include "stm32_eval.h"

/** @addtogroup STM32F10x_StdPeriph_Examples
  * @{
  */

/** @addtogroup DMA_SPI_RAM
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
typedef enum { FAILED = 0, PASSED = !FAILED} TestStatus;

/* Private define ------------------------------------------------------------*/
#define BufferSize         32
#define CRCPolynomial      7

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
SPI_InitTypeDef    SPI_InitStructure;
DMA_InitTypeDef    DMA_InitStructure;
uint8_t SPI_MASTER_Buffer_Rx[BufferSize], SPI_SLAVE_Buffer_Rx[BufferSize];
volatile uint8_t SPI_MASTERCRCValue = 0, SPI_SLAVECRCValue = 0;
volatile TestStatus TransferStatus1 = FAILED, TransferStatus2 = FAILED;

uint8_t SPI_MASTER_Buffer_Tx[BufferSize] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,
                                            0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,
                                            0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,
                                            0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,0x20};

uint8_t SPI_SLAVE_Buffer_Tx[BufferSize] = {0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,
                                           0x59,0x5A,0x5B,0x5C,0x5D,0x5E,0x5F,0x60,
                                           0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,
                                           0x69,0x6A,0x6B,0x6C,0x6D,0x6E,0x6F,0x70};

/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);
void GPIO_Configuration(void);
TestStatus Buffercmp(uint8_t* pBuffer, uint8_t* pBuffer1, uint16_t BufferLength);

/* Private functions ---------------------------------------------------------*/

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
  
  /* Configure the STM32_EVAL LED */
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED2);
  /* Turn Off LED1 and LED2 */
  STM_EVAL_LEDOff(LED1);
  STM_EVAL_LEDOff(LED2);     
  
  /* System Clocks Configuration */
  RCC_Configuration();
       
  /* Configure the GPIO ports */
  GPIO_Configuration();

  /* SPI_MASTER configuration ------------------------------------------------*/
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = CRCPolynomial;
  SPI_Init(SPI_MASTER, &SPI_InitStructure);
  
  /* SPI_SLAVE configuration -------------------------------------------------*/
  SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;
  SPI_Init(SPI_SLAVE, &SPI_InitStructure);

  /* SPI_MASTER_Rx_DMA_Channel configuration ---------------------------------*/
  DMA_DeInit(SPI_MASTER_Rx_DMA_Channel);
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SPI_MASTER_DR_Base;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)SPI_MASTER_Buffer_Rx;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = BufferSize;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(SPI_MASTER_Rx_DMA_Channel, &DMA_InitStructure);

  /* SPI_MASTER_Tx_DMA_Channel configuration ---------------------------------*/
  DMA_DeInit(SPI_MASTER_Tx_DMA_Channel);  
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SPI_MASTER_DR_Base;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)SPI_MASTER_Buffer_Tx;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
  DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
  DMA_Init(SPI_MASTER_Tx_DMA_Channel, &DMA_InitStructure);

  /* SPI_SLAVE_Rx_DMA_Channel configuration ----------------------------------*/
  DMA_DeInit(SPI_SLAVE_Rx_DMA_Channel);  
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SPI_SLAVE_DR_Base;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)SPI_SLAVE_Buffer_Rx;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
  DMA_Init(SPI_SLAVE_Rx_DMA_Channel, &DMA_InitStructure);

  /* SPI_SLAVE_Tx_DMA_Channel configuration ----------------------------------*/
  DMA_DeInit(SPI_SLAVE_Tx_DMA_Channel);  
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SPI_SLAVE_DR_Base;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)SPI_SLAVE_Buffer_Tx;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
  DMA_Init(SPI_SLAVE_Tx_DMA_Channel, &DMA_InitStructure);

  /* Enable SPI_MASTER DMA Tx request */
  SPI_I2S_DMACmd(SPI_MASTER, SPI_I2S_DMAReq_Tx, ENABLE);
  /* Enable SPI_MASTER DMA Rx request */
  SPI_I2S_DMACmd(SPI_MASTER, SPI_I2S_DMAReq_Rx, ENABLE);
  /* Enable SPI_SLAVE DMA Tx request */
  SPI_I2S_DMACmd(SPI_SLAVE, SPI_I2S_DMAReq_Tx, ENABLE);
  /* Enable SPI_SLAVE DMA Rx request */
  SPI_I2S_DMACmd(SPI_SLAVE, SPI_I2S_DMAReq_Rx, ENABLE);
 
  /* Enable SPI_MASTER CRC calculation */
  SPI_CalculateCRC(SPI_MASTER, ENABLE);
  /* Enable SPI_SLAVE CRC calculation */
  SPI_CalculateCRC(SPI_SLAVE, ENABLE);

  /* Enable SPI_SLAVE */
  SPI_Cmd(SPI_SLAVE, ENABLE);
  /* Enable SPI_MASTER */
  SPI_Cmd(SPI_MASTER, ENABLE);

  /* Enable DMA channels */
  DMA_Cmd(SPI_MASTER_Rx_DMA_Channel, ENABLE);
  DMA_Cmd(SPI_SLAVE_Rx_DMA_Channel, ENABLE);
  DMA_Cmd(SPI_SLAVE_Tx_DMA_Channel, ENABLE);
  DMA_Cmd(SPI_MASTER_Tx_DMA_Channel, ENABLE);
   
  /* Transfer complete */
  while(!DMA_GetFlagStatus(SPI_MASTER_Rx_DMA_FLAG));
  while(!DMA_GetFlagStatus(SPI_SLAVE_Rx_DMA_FLAG));
  while(!DMA_GetFlagStatus(SPI_SLAVE_Tx_DMA_FLAG));
  while(!DMA_GetFlagStatus(SPI_MASTER_Tx_DMA_FLAG));

  /* Wait for SPI_MASTER data reception: CRC transmitted by SPI_SLAVE */
  while(SPI_I2S_GetFlagStatus(SPI_MASTER, SPI_I2S_FLAG_RXNE) == RESET);
  /* Wait for SPI_SLAVE data reception: CRC transmitted by SPI_MASTER */
  while(SPI_I2S_GetFlagStatus(SPI_SLAVE, SPI_I2S_FLAG_RXNE) == RESET);

  /* Check the correctness of written dada */
  TransferStatus1 = Buffercmp(SPI_SLAVE_Buffer_Rx, SPI_MASTER_Buffer_Tx, BufferSize);
  TransferStatus2 = Buffercmp(SPI_MASTER_Buffer_Rx, SPI_SLAVE_Buffer_Tx, BufferSize);
  /* TransferStatus1, TransferStatus2 = PASSED, if the data transmitted and received 
     are correct */
  /* TransferStatus1, TransferStatus2 = FAILED, if the data transmitted and received  
     are different */

  /* Test on the SPI_MASTER CRCR ERROR flag */
  if ((SPI_I2S_GetFlagStatus(SPI_MASTER, SPI_FLAG_CRCERR)) != RESET)
  {
    TransferStatus1 = FAILED;
  }
  /* Test on the SPI_SLAVE CRCR ERROR flag */
  if ((SPI_I2S_GetFlagStatus(SPI_SLAVE, SPI_FLAG_CRCERR)) != RESET)
  {
    TransferStatus2 = FAILED;
  } 

  /* Read SPI_MASTER received CRC value */
  SPI_MASTERCRCValue = SPI_I2S_ReceiveData(SPI_MASTER);
  /* Read SPI_SLAVE received CRC value */
  SPI_SLAVECRCValue = SPI_I2S_ReceiveData(SPI_SLAVE);

  if (TransferStatus1 != FAILED)
  {
    /* OK */
    /* Turn on LD1 */
    STM_EVAL_LEDOn(LED1);
  }
  else
  { 
    /* KO */
    /* Turn Off LD1 */
    STM_EVAL_LEDOff(LED1);
  }
    
  if (TransferStatus2 != FAILED)
  {	
    /* OK */
    /* Turn on LD2 */
    STM_EVAL_LEDOn(LED2);
  }
  else
  { 
    /* KO */
    /* Turn Off LD2 */
    STM_EVAL_LEDOff(LED2);
  }

  while (1)
  {
  }
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

  /* Enable peripheral clocks ------------------------------------------------*/
  /* Enable DMA1 or/and DMA2 clock */
  RCC_AHBPeriphClockCmd(SPI_MASTER_DMA_CLK | SPI_SLAVE_DMA_CLK, ENABLE);

#ifdef USE_STM3210C_EVAL
  /* Enable GPIO clock for SPI_MASTER and SPI_SLAVE */
  RCC_APB2PeriphClockCmd(SPI_MASTER_GPIO_CLK | SPI_SLAVE_GPIO_CLK |
  RCC_APB2Periph_AFIO, ENABLE);

  /* Enable SPI_MASTER Periph clock */
  RCC_APB1PeriphClockCmd(SPI_MASTER_CLK, ENABLE);
                           
#else
  /* Enable SPI_MASTER clock and GPIO clock for SPI_MASTER and SPI_SLAVE */
  RCC_APB2PeriphClockCmd(SPI_MASTER_GPIO_CLK | SPI_SLAVE_GPIO_CLK |
                         SPI_MASTER_CLK, ENABLE);
#endif
  /* Enable SPI_SLAVE Periph clock */
  RCC_APB1PeriphClockCmd(SPI_SLAVE_CLK, ENABLE);
}

/**
  * @brief  Configures the different GPIO ports.
  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

#ifdef USE_STM3210C_EVAL
  /* Enable SPI3 Pins Software Remapping */
  GPIO_PinRemapConfig(GPIO_Remap_SPI3, ENABLE);
#endif

  /* Configure SPI_MASTER pins: SCK and MOSI */
  GPIO_InitStructure.GPIO_Pin = SPI_MASTER_PIN_SCK | SPI_MASTER_PIN_MOSI;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(SPI_MASTER_GPIO, &GPIO_InitStructure);
  /* Configure SPI_MASTER pins:  MISO */
  GPIO_InitStructure.GPIO_Pin = SPI_MASTER_PIN_MISO;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(SPI_MASTER_GPIO, &GPIO_InitStructure);

  /* Configure SPI_SLAVE pins: SCK and MOSI */
  GPIO_InitStructure.GPIO_Pin = SPI_SLAVE_PIN_SCK | SPI_SLAVE_PIN_MOSI;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(SPI_SLAVE_GPIO, &GPIO_InitStructure);
  /* Configure SPI_SLAVE pins: MISO  */
  GPIO_InitStructure.GPIO_Pin = SPI_SLAVE_PIN_MISO ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(SPI_SLAVE_GPIO, &GPIO_InitStructure);
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer, pBuffer1: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval PASSED: pBuffer identical to pBuffer1
  *         FAILED: pBuffer differs from pBuffer1
  */
TestStatus Buffercmp(uint8_t* pBuffer, uint8_t* pBuffer1, uint16_t BufferLength)
{
  while(BufferLength--)
  {
    if(*pBuffer != *pBuffer1)
    {
      return FAILED;
    }
    
    pBuffer++;
    pBuffer1++;
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

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
