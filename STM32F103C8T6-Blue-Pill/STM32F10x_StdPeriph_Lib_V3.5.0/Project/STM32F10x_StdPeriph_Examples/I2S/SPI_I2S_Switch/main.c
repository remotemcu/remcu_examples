/**
  ******************************************************************************
  * @file    I2S/SPI_I2S_Switch/main.c 
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

/* Local includes ------------------------------------------------------------*/

/** @addtogroup STM32F10x_StdPeriph_Examples
  * @{
  */

/** @addtogroup I2S_SPI_I2S_Switch
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/* Private define ------------------------------------------------------------*/
#define BufferSize   32

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
I2S_InitTypeDef I2S_InitStructure;
SPI_InitTypeDef SPI_InitStructure;
uint16_t I2S3_Buffer_Tx[BufferSize] = {0x0102, 0x0304, 0x0506, 0x0708, 0x090A, 0x0B0C,
                                  0x0D0E, 0x0F10, 0x1112, 0x1314, 0x1516, 0x1718,
                                  0x191A, 0x1B1C, 0x1D1E, 0x1F20, 0x2122, 0x2324,
                                  0x2526, 0x2728, 0x292A, 0x2B2C, 0x2D2E, 0x2F30,
                                  0x3132, 0x3334, 0x3536, 0x3738, 0x393A, 0x3B3C,
                                  0x3D3E, 0x3F40};

uint16_t SPI3_Buffer_Tx[BufferSize] = {0x5152, 0x5354, 0x5556, 0x5758, 0x595A, 0x5B5C,
                                  0x5D5E, 0x5F60, 0x6162, 0x6364, 0x6566, 0x6768,
                                  0x696A, 0x6B6C, 0x6D6E, 0x6F70, 0x7172, 0x7374,
                                  0x7576, 0x7778, 0x797A, 0x7B7C, 0x7D7E, 0x7F80,
                                  0x8182, 0x8384, 0x8586, 0x8788, 0x898A, 0x8B8C,
                                  0x8D8E, 0x8F90};

__IO uint16_t I2S2_Buffer_Rx[BufferSize];
__IO uint16_t SPI2_Buffer_Rx[BufferSize];
__IO uint8_t TxIdx = 0, RxIdx = 0;
volatile TestStatus TransferStatus1 = FAILED, TransferStatus2 = FAILED;
volatile TestStatus TransferStatus3 = FAILED;
ErrorStatus HSEStartUpStatus;

/* Private functions ---------------------------------------------------------*/
void RCC_Configuration(void);
void GPIO_Configuration(void);
TestStatus Buffercmp(uint16_t* pBuffer1, uint16_t* pBuffer2, uint16_t BufferLength);

/**
  * @brief   Main program
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

  /* GPIO configuration ------------------------------------------------------*/
  GPIO_Configuration();
 
  /* Deinitializes the SPI2 and SPI3 peripheral registers --------------------*/
  SPI_I2S_DeInit(SPI2);
  SPI_I2S_DeInit(SPI3);
  
  /* I2S peripheral configuration */
  I2S_InitStructure.I2S_Standard = I2S_Standard_Phillips;
  I2S_InitStructure.I2S_DataFormat = I2S_DataFormat_16bextended;
  I2S_InitStructure.I2S_MCLKOutput = I2S_MCLKOutput_Disable;
  I2S_InitStructure.I2S_AudioFreq = I2S_AudioFreq_48k;
  I2S_InitStructure.I2S_CPOL = I2S_CPOL_Low;

  /* I2S3 Master Transmitter to I2S2 Slave Receiver communication ------------*/
  /* I2S3 configuration */
  I2S_InitStructure.I2S_Mode = I2S_Mode_MasterTx;
  I2S_Init(SPI3, &I2S_InitStructure);

  /* I2S2 configuration */
  I2S_InitStructure.I2S_Mode = I2S_Mode_SlaveRx;
  I2S_Init(SPI2, &I2S_InitStructure);

  /* Enable the I2S2 */
  I2S_Cmd(SPI2, ENABLE);

  /* Enable the I2S3 */
  I2S_Cmd(SPI3, ENABLE);

  /* Begin the communication in I2S mode */
  while (RxIdx < BufferSize)
  {
    /* Wait the Tx buffer to be empty */
    while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET)
    {}
    /* Send a data from I2S3 */
    SPI_I2S_SendData(SPI3, I2S3_Buffer_Tx[TxIdx++]);

    /* Wait the Rx buffer to be full */
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET)
    {}
    /* Store the I2S2 received data in the relative data table */
    I2S2_Buffer_Rx[RxIdx++] = SPI_I2S_ReceiveData(SPI2);
  }

  TransferStatus1 = Buffercmp((uint16_t *)I2S2_Buffer_Rx, I2S3_Buffer_Tx, BufferSize);
  /* TransferStatus1 = PASSED, if the data transmitted from I2S3 and received by
                               I2S2 are the same 
     TransferStatus1 = FAILED, if the data transmitted from I2S3 and received by
                               I2S2 are different */

  /* Reset TxIdx, RxIdx indexes */
  TxIdx = 0;
  RxIdx = 0;
  
  /* Switch to SPI mode communication ----------------------------------------*/
  /* SPI3 configuration */
  SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI3, &SPI_InitStructure);

  /* SPI2 configuration ------------------------------------------------------*/
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_RxOnly;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;
  SPI_Init(SPI2, &SPI_InitStructure);

  /* Enable SPI2 */
  SPI_Cmd(SPI2, ENABLE);
  /* Enable SPI3 */
  SPI_Cmd(SPI3, ENABLE);

  /* Begin the communication in SPI mode */
  while (RxIdx < BufferSize)
  {
    /* Wait the Tx buffer to be empty */
    while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET)
    {}
    /* Send a data from SPI3 */
    SPI_I2S_SendData(SPI3, SPI3_Buffer_Tx[TxIdx++]);

    /* Wait the Rx buffer to be full */
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET)
    {}
    /* Store the SPI2 received data in the relative data table */
    SPI2_Buffer_Rx[RxIdx++] = SPI_I2S_ReceiveData(SPI2);
  }

  TransferStatus2 = Buffercmp((uint16_t *)SPI2_Buffer_Rx, SPI3_Buffer_Tx, BufferSize);
  /* TransferStatus2 = PASSED, if the data transmitted from SPI3 and received by
                               SPI2 are the same
     TransferStatus2 = FAILED, if the data transmitted from SPI3 and received by
                               SPI2 are different */

  /* Reset TxIdx, RxIdx indexes and receive table values */
  for (TxIdx = 0; TxIdx < BufferSize; TxIdx++)
  {
    I2S2_Buffer_Rx[TxIdx] = 0;
  }
  
  TxIdx = 0;
  RxIdx = 0;
      
  /* I2S3 Slave Transmitter to I2S2 Master Receiver communication ------------*/
  /* I2S3 configuration */
  I2S_InitStructure.I2S_Mode = I2S_Mode_SlaveTx;
  I2S_Init(SPI3, &I2S_InitStructure);

  /* I2S2 configuration */
  I2S_InitStructure.I2S_Mode = I2S_Mode_MasterRx;
  I2S_Init(SPI2, &I2S_InitStructure);

  /* Wait the Tx buffer to be empty */
  while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET)
  {}
  /* Prepare the first data to be sent from the slave */
  SPI_I2S_SendData(SPI3, I2S3_Buffer_Tx[TxIdx++]);

  /* Enable the I2S3 */
  I2S_Cmd(SPI3, ENABLE);

  /* Enable the I2S2 */
  I2S_Cmd(SPI2, ENABLE);

  /* Begin the communication in I2S mode */
  while (RxIdx < BufferSize)
  {
    /* Wait the Rx buffer to be full */
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET)
    {}
    /* Store the I2S2 received data in the relative data table */
    I2S2_Buffer_Rx[RxIdx++] = SPI_I2S_ReceiveData(SPI2);

    /* Wait the Tx buffer to be empty */
    while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET)
    {}
    /* Send a data from I2S3 */
    SPI_I2S_SendData(SPI3, I2S3_Buffer_Tx[TxIdx++]);
  }

  TransferStatus3 = Buffercmp((uint16_t *)I2S2_Buffer_Rx, I2S3_Buffer_Tx, BufferSize);
  /* TransferStatus3 = PASSED, if the data transmitted from I2S3 and received by
                               I2S2 are the same
     TransferStatus3 = FAILED, if the data transmitted from I2S3 and received by
                               I2S2 are different */
 
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
  /* RCC system reset(for debug purpose) */
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if(HSEStartUpStatus == SUCCESS)
  {
    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_2);
	
    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1); 
  
    /* PCLK2 = HCLK */
    RCC_PCLK2Config(RCC_HCLK_Div1); 

    /* PCLK1 = HCLK/2 */
    RCC_PCLK1Config(RCC_HCLK_Div2);

    /* ADCCLK = PCLK2/4 */
    RCC_ADCCLKConfig(RCC_PCLK2_Div4); 
  
#ifndef STM32F10X_CL  
    /* PLLCLK = 8MHz * 9 = 72 MHz */
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);

#else
    /* Configure PLLs *********************************************************/
    /* PLL2 configuration: PLL2CLK = (HSE / 5) * 8 = 40 MHz */
    RCC_PREDIV2Config(RCC_PREDIV2_Div5);
    RCC_PLL2Config(RCC_PLL2Mul_8);

    /* Enable PLL2 */
    RCC_PLL2Cmd(ENABLE);

    /* Wait till PLL2 is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_PLL2RDY) == RESET)
    {}

    /* PLL configuration: PLLCLK = (PLL2 / 5) * 9 = 72 MHz */ 
    RCC_PREDIV1Config(RCC_PREDIV1_Source_PLL2, RCC_PREDIV1_Div5);
    RCC_PLLConfig(RCC_PLLSource_PREDIV1, RCC_PLLMul_9);

    /* PPL3 configuration: PLL3CLK = (HSE / 5) * 11 = PLL3_VCO = 110 MHz */
    RCC_PLL3Config(RCC_PLL3Mul_11);
    /* Enable PLL3 */
    RCC_PLL3Cmd(ENABLE);    
    /* Wait till PLL3 is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_PLL3RDY) == RESET)
    {}

    /* Configure I2S clock source: On Connectivity Line Devices, the I2S can be 
        clocked by PLL3 VCO instead of SYS_CLK in order to guarantee higher 
        precision */
    RCC_I2S3CLKConfig(RCC_I2S3CLKSource_PLL3_VCO);
    RCC_I2S2CLKConfig(RCC_I2S2CLKSource_PLL3_VCO);  
#endif

    /* Enable PLL */ 
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {
    }

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till PLL is used as system clock source */
    while(RCC_GetSYSCLKSource() != 0x08)
    {
    }
  }  
  
  /* Enable peripheral clocks --------------------------------------------------*/
  /* GPIOA, GPIOB and AFIO clocks enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
                         RCC_APB2Periph_AFIO, ENABLE);

#ifdef USE_STM3210C_EVAL 
  /* GPIOC Clock enable (for the SPI3 remapped pins)  */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE);  
#endif /* USE_STM3210C_EVAL */  
  
  /* SPI2 and SPI3 clocks enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2 | RCC_APB1Periph_SPI3, ENABLE);  
}

/**
  * @brief  Configures the different GPIO ports.
  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

#ifdef USE_STM3210E_EVAL
  /* Disable the JTAG interface and enable the SWJ interface
      This operation is not necessary for Connectivity Line devices since
      SPI3 I/Os can be remapped on other GPIO pins */
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
#endif /* USE_STM3210E_EVAL */ 

  /* Configure SPI2 pins: CK, WS and SD ---------------------------------*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

#ifdef USE_STM3210C_EVAL
  
  /* Remap SPI3 on PC10-PC11-PC12-PA4 GPIO pins ------------------------*/
  GPIO_PinRemapConfig(GPIO_Remap_SPI3, ENABLE);
  
  /* Configure SPI3 pins: CK and SD ------------------------------------*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_12;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  /* Configure SPI3 pins: WS -------------------------------------------*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
#elif defined (USE_STM3210E_EVAL)
  
  /* Configure SPI3 pins: CK and SD ------------------------------------*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_5;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* Configure SPI3 pins: WS -------------------------------------------*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
#endif /* USE_STM3210C_EVAL */ 
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval PASSED: pBuffer1 identical to pBuffer2
  *         FAILED: pBuffer1 differs from pBuffer2
  */
TestStatus Buffercmp(uint16_t* pBuffer1, uint16_t* pBuffer2, uint16_t BufferLength)
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
