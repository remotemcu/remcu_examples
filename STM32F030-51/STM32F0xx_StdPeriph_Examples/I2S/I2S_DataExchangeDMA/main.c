/**
  ******************************************************************************
  * @file    I2S/I2S_DataExchangeDMA/main.c 
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

/** @addtogroup I2S_DataExchangeDMA
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;
/* Private define ------------------------------------------------------------*/
/* DR adresses */
#define SPI1_DR_Address          0x4001300C
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
const uint16_t I2S_Buffer_Tx[32] = {0x0102, 0x0304, 0x0506, 0x0708, 0x090A, 0x0B0C,
                                    0x0D0E, 0x0F10, 0x1112, 0x1314, 0x1516, 0x1718,
                                    0x191A, 0x1B1C, 0x1D1E, 0x1F20, 0x2122, 0x2324,
                                    0x2526, 0x2728, 0x292A, 0x2B2C, 0x2D2E, 0x2F30,
                                    0x3132, 0x3334, 0x3536, 0x3738, 0x393A, 0x3B3C,
                                    0x3D3E, 0x3F40};
uint16_t I2S_Buffer_Rx[32]={0};
TestStatus TransferStatus = FAILED;
__IO uint32_t TxStatus= 0, RxStatus=0;

/* Private function prototypes -----------------------------------------------*/
#if defined (I2S_SLAVE_RECEIVER)
static TestStatus Buffercmp(uint16_t* pBuffer1, uint16_t* pBuffer2, uint16_t BufferLength);
#endif
static void I2S_Config(void);
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

  /* I2S peripheral Configuration */
  I2S_Config();
 
  /* Initialize the LEDs */
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED1);

#if defined (I2S_MASTER_TRANSMITTER) 
  
  /* Enable the Tamper button */
  STM_EVAL_PBInit(BUTTON_TAMPER, BUTTON_MODE_GPIO); 
  
  while (STM_EVAL_PBGetState(BUTTON_TAMPER) != RESET)
  {} 
  
  /* Enable the DMA1_Channel3 transfer complete */  
  DMA_ITConfig(DMA1_Channel3, DMA_IT_TC, ENABLE);
  
  /* Enable the DMA channel Tx */
  DMA_Cmd(DMA1_Channel3, ENABLE);  

  /* Enable the I2S TX DMA request */
  SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);
  
  /* Enable the SPI1 Master peripheral */
  I2S_Cmd(SPI1, ENABLE);

  /* Wait for successful ransfer complete*/
  while(TxStatus == 0);
  
  /* Disable the DMA1_Channel3 transfer complete interrupt */  
  DMA_ITConfig(DMA1_Channel3, DMA_IT_TC, DISABLE);

  while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);
  
  /* Disable the SPI1 Master peripheral */
  I2S_Cmd(SPI1, DISABLE);
  
  /* Disable DMA Channel 3*/
  DMA_Cmd(DMA1_Channel3, DISABLE);
  
  /* disables the SPI1/I2S1 DMA interface */
  SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx,DISABLE);
#elif defined (I2S_SLAVE_RECEIVER)  
  
  /* Wait for received data: Transfer complete */
  while(RxStatus == 0);

  while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);
  
  /* Disable the SPI1 Slave peripheral */
  I2S_Cmd(SPI1, DISABLE);
  
  /* Disable the DMA1_Channel2 transfer complete */  
  DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, DISABLE);
  
  /* Enable DMA channel 2 Rx */
  DMA_Cmd(DMA1_Channel2, DISABLE);
  
  /* Enable the I2S1 RX DMA request */
  SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Rx, DISABLE);
  
  /* Check if the data transmitted from Master Board and received by
  Slave Board are the same */
  TransferStatus = Buffercmp(I2S_Buffer_Rx, (uint16_t*)I2S_Buffer_Tx, 32);
  
  if (TransferStatus == PASSED) /* successful transfer */ 
  {
    /* Green Led On */
    STM_EVAL_LEDOn(LED1);
    STM_EVAL_LEDOff(LED3);
  }
  else /* unsuccessful transfer */ 
  {
    /* Red Led On */
    STM_EVAL_LEDOn(LED3);
    STM_EVAL_LEDOff(LED1);
  }
#endif
   /* Infinite loop */
  while (1)
  {}

}

/**
  * @brief  Configures I2S peripheral.
  * @param  None
  * @retval None
  */
static void I2S_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  I2S_InitTypeDef I2S_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure; 
  DMA_InitTypeDef DMA_InitStructure;
  
  /* Enable SPI1 APB clocks */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
  
  /* Enable GPIOA clocks */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA|RCC_AHBPeriph_GPIOB, ENABLE);     
  
  /* Enable the DMA1 AHB clocks */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

  /* I2S1 Pins configuration */
#ifdef USE_STM320518_EVAL
  /* Configure pins as AF */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);   
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 |GPIO_Pin_5;
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
  
  /* Connect pin to Periph */
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource15, GPIO_AF_0); 
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_0);    
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_0); 
#else 
  /* I2S1 Pins configuration */
  /* Configure pins as AF */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7  ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);   
  
  /* Connect pin to Periph */
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_AF_0); 
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_0);    
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_0); 
#endif /* USE_STM320518_EVAL */
    
  /* I2S peripheral configuration */
  I2S_InitStructure.I2S_Standard = I2S_Standard_Phillips;
  I2S_InitStructure.I2S_DataFormat = I2S_DataFormat_16bextended;
  I2S_InitStructure.I2S_MCLKOutput = I2S_MCLKOutput_Disable;
  I2S_InitStructure.I2S_AudioFreq = I2S_AudioFreq_48k;
  I2S_InitStructure.I2S_CPOL = I2S_CPOL_Low;
  
  /* Common DMA configuration */
  DMA_InitStructure.DMA_BufferSize = 32;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

#if defined (I2S_MASTER_TRANSMITTER)   
  /* I2S Master Transmitter configuration */
  I2S_InitStructure.I2S_Mode = I2S_Mode_MasterTx;
  I2S_Init(SPI1, &I2S_InitStructure);
  /* DMA1 channel 5 (SPI1_Tx) configuration */
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SPI1_DR_Address;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)I2S_Buffer_Tx;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
  DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
  DMA_Init(DMA1_Channel3, &DMA_InitStructure);

#elif defined (I2S_SLAVE_RECEIVER)
  /* I2S Slave Receiver configuration */
  I2S_InitStructure.I2S_Mode = I2S_Mode_SlaveRx;  
  I2S_Init(SPI1, &I2S_InitStructure);
  /* DMA1 channel 2 (SPI1_Rx) Configuration */
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SPI1_DR_Address;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)I2S_Buffer_Rx;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_Init(DMA1_Channel2, &DMA_InitStructure);
  
   DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, ENABLE);
  /* Enable the DMA channels */
  DMA_Cmd(DMA1_Channel2, ENABLE);
  /* Enable the I2S1 RX DMA request */
  SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Rx, ENABLE);     
    
  /* Enable the SPI1 Master peripheral */
  I2S_Cmd(SPI1, ENABLE);
 
#endif 
    /* SPI1 IRQ Channel configuration */
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel2_3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
}

#if defined (I2S_SLAVE_RECEIVER)      
/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval PASSED: pBuffer1 identical to pBuffer2
  *         FAILED: pBuffer1 differs from pBuffer2
  */
static TestStatus Buffercmp(uint16_t* pBuffer1, uint16_t* pBuffer2, uint16_t BufferLength)
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
#endif

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
