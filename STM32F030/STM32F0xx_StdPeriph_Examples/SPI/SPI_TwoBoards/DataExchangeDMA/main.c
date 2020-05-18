/**
  ******************************************************************************
  * @file    SPI/SPI_TwoBoards/DataExchangeDMA/main.c
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

/** @addtogroup SPI_DataExchangeDMA
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
SPI_InitTypeDef  SPI_InitStructure;
DMA_InitTypeDef  DMA_InitStructure;
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;
uint8_t TxBuffer[] = "SPI DMA Example: Communication between two SPI using DMA";
uint8_t RxBuffer [RXBUFFERSIZE];

__IO JOYState_TypeDef PressedButton  = JOY_NONE;
__IO uint32_t CommandTransmitted = 0x00;
__IO uint32_t CommandReceived = 0x00;
__IO uint16_t NumberOfByte = 0x00;
__IO uint16_t PrescalerValue = 0;
__IO uint32_t TimeOut = 0x0;

/* Private function prototypes -----------------------------------------------*/
static void SPI_Config(void);
static void SysTickConfig(void);
static void TimeOut_UserCallback(void);
static TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength, uint8_t DataMask);
#ifdef SPI_MASTER
static void TIM_Config(void);
static JOYState_TypeDef Read_Joystick(void);
static void Fill_Buffer(uint8_t *pBuffer, uint16_t BufferLength);
#endif

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
  
  /* SPI configuration ------------------------------------------------------*/
  SPI_Config();
  
  /* SysTick configuration ---------------------------------------------------*/
  SysTickConfig();
  
  /* Initialize LEDs mounted on STM320518-EVAL board */
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);
  
  /* Master board configuration ------------------------------------------------*/
#ifdef SPI_MASTER
  /* Initialize push-buttons mounted on STM320518-EVAL board */
  STM_EVAL_PBInit(BUTTON_RIGHT, BUTTON_MODE_GPIO);
  STM_EVAL_PBInit(BUTTON_LEFT, BUTTON_MODE_GPIO);
  STM_EVAL_PBInit(BUTTON_UP, BUTTON_MODE_GPIO);
  STM_EVAL_PBInit(BUTTON_DOWN, BUTTON_MODE_GPIO);
  STM_EVAL_PBInit(BUTTON_SEL, BUTTON_MODE_GPIO);
  
  /* Initializes the SPI communication */
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_Init(SPIx, &SPI_InitStructure);
  
  /* Initialize the FIFO threshold */
  SPI_RxFIFOThresholdConfig(SPIx, SPI_RxFIFOThreshold_QF);
  
  /* TIM configuration ------------------------------------------------------*/
  TIM_Config(); 
  
  /* Enable the SPI peripheral */
  SPI_Cmd(SPIx, ENABLE);
  
  /* Enable NSS output for master mode */
  SPI_SSOutputCmd(SPIx, ENABLE);
  
  /* TIM Capture Compare DMA Request enable */
  TIM_DMACmd(TIMx, TIMx_DMA_CHANNEL, ENABLE);
  
  while (1)
  {
    /* DMA channel Rx of SPI Configuration */
    DMA_InitStructure.DMA_BufferSize = (uint16_t)1;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SPIx_DR_ADDRESS;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) &CommandReceived;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_Init(SPIx_RX_DMA_CHANNEL, &DMA_InitStructure);
    
    /* DMA TIM trigger channel Configuration */
    DMA_InitStructure.DMA_BufferSize = (uint16_t)1;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SPIx_DR_ADDRESS;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) &CommandTransmitted;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
    DMA_Init(TIMx_CHANNEL_DMA_CHANNEL, &DMA_InitStructure);
    
    /* Enable the SPI Rx DMA request */
    SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Rx, ENABLE);
    
    CommandTransmitted = 0x00;
    CommandReceived = 0x00;
    
    /* Clear the RxBuffer */
    Fill_Buffer(RxBuffer, TXBUFFERSIZE);
    
    PressedButton = Read_Joystick();
    while (PressedButton == JOY_NONE)
    {
      PressedButton = Read_Joystick();
    }
    
    switch (PressedButton)
    {
      /* JOY_RIGHT button pressed */
    case JOY_RIGHT:
      CommandTransmitted = CMD_RIGHT;
      NumberOfByte = CMD_RIGHT_SIZE;
      break;
      /* JOY_LEFT button pressed */ 
    case JOY_LEFT:
      CommandTransmitted = CMD_LEFT;
      NumberOfByte = CMD_LEFT_SIZE;
      break;
      /* JOY_UP button pressed */
    case JOY_UP:
      CommandTransmitted = CMD_UP;
      NumberOfByte = CMD_UP_SIZE;
      break;
      /* JOY_DOWN button pressed */
    case JOY_DOWN:
      CommandTransmitted = CMD_DOWN;
      NumberOfByte = CMD_DOWN_SIZE;
      break;
      /* JOY_SEL button pressed */
    case JOY_SEL:
      CommandTransmitted = CMD_SEL;
      NumberOfByte = CMD_SEL_SIZE;
      break;
    default:
      break;
    }
    
    /* Enable the DMA channel */
    DMA_Cmd(SPIx_RX_DMA_CHANNEL, ENABLE);
    
    /* Enable DMA1 TIM Trigger Channel */
    DMA_Cmd(TIMx_CHANNEL_DMA_CHANNEL, ENABLE);
    
    /* TIM enable counter */
    TIM_Cmd(TIMx, ENABLE);
    
    /* Wait the SPI DMA Rx transfer complete or time out*/
    TimeOut = USER_TIMEOUT;
    while ((DMA_GetFlagStatus(SPIx_RX_DMA_FLAG_TC) == RESET)&&(TimeOut != 0x00))
    {}
    if(TimeOut == 0)
    {
      TimeOut_UserCallback();
    }
    /* The BSY flag can be monitored to ensure that the SPI communication is complete.
    This is required to avoid corrupting the last transmission before disabling 
    the SPI or entering the Stop mode. The software must first wait until TXE=1
    and then until BSY=0.*/
    TimeOut = USER_TIMEOUT;
    while ((SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET)&&(TimeOut != 0x00))
    {}
    if(TimeOut == 0)
    {
      TimeOut_UserCallback();
    }
    
    TimeOut = USER_TIMEOUT;
    while ((SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_BSY) == SET)&&(TimeOut != 0x00))
    {}
    if(TimeOut == 0)
    {
      TimeOut_UserCallback();
    } 
    
    /* Clear DMA1 global flags*/
    DMA_ClearFlag(TIMx_CHANNEL_DMA_FLAG_GL);
    DMA_ClearFlag(SPIx_RX_DMA_FLAG_GL);
    
    /* disable the DMA channels */
    DMA_Cmd(SPIx_RX_DMA_CHANNEL, DISABLE);
    DMA_Cmd(TIMx_CHANNEL_DMA_CHANNEL, DISABLE);
    
    /* disable the SPI Rx DMA request */
    SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Rx, DISABLE);
    
    /* TIM disable counter */
    TIM_Cmd(TIMx, DISABLE);
    
    if (CommandReceived == CMD_ACK)
    {
      /* DMA channel Rx of SPI Configuration */
      DMA_InitStructure.DMA_BufferSize = (uint16_t)NumberOfByte;
      DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SPIx_DR_ADDRESS;
      DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)RxBuffer;
      DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
      DMA_InitStructure.DMA_Priority = DMA_Priority_High;
      DMA_Init(SPIx_RX_DMA_CHANNEL, &DMA_InitStructure);
      
      /* DMA channel Tx of SPI Configuration */
      DMA_InitStructure.DMA_BufferSize = (uint16_t)NumberOfByte;
      DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SPIx_DR_ADDRESS;
      DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)TxBuffer;
      DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
      DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
      DMA_Init(TIMx_CHANNEL_DMA_CHANNEL, &DMA_InitStructure);
      
      /* Enable the SPI Rx DMA request */
      SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Rx, ENABLE);
      
      /* Enable the DMA channel */
      DMA_Cmd(SPIx_RX_DMA_CHANNEL, ENABLE);
      
      /* Enable DMA1 TIM Trigger Channel */
      DMA_Cmd(TIMx_CHANNEL_DMA_CHANNEL, ENABLE);
      
      /* TIM enable counter */
      TIM_Cmd(TIMx, ENABLE);
      
      /* Wait the SPI Rx DMA transfer complete or time out */
      TimeOut = USER_TIMEOUT;
      while ((DMA_GetFlagStatus(SPIx_RX_DMA_FLAG_TC) == RESET)&&(TimeOut != 0x00))
      {}
      if(TimeOut == 0)
      {
        TimeOut_UserCallback();
      }
      /* The BSY flag can be monitored to ensure that the SPI communication is complete.
      This is required to avoid corrupting the last transmission before disabling 
      the SPI or entering the Stop mode. The software must first wait until TXE=1
      and then until BSY=0.*/
      TimeOut = USER_TIMEOUT;
      while ((SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET)&&(TimeOut != 0x00))
      {}
      if(TimeOut == 0)
      {
        TimeOut_UserCallback();
      }
      
      TimeOut = USER_TIMEOUT;
      while ((SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_BSY) == SET)&&(TimeOut != 0x00))
      {}
      if(TimeOut == 0)
      {
        TimeOut_UserCallback();
      }
      
      /* Clear DMA1 global flags */
      DMA_ClearFlag(TIMx_CHANNEL_DMA_FLAG_GL);
      DMA_ClearFlag(SPIx_RX_DMA_FLAG_GL);
      
      /* Disable the DMA channels */
      DMA_Cmd(SPIx_RX_DMA_CHANNEL, DISABLE);
      DMA_Cmd(TIMx_CHANNEL_DMA_CHANNEL, DISABLE);
      
      /* Disable the SPI Rx and Tx DMA requests */
      SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Rx, DISABLE);
      
      /* TIM disable counter */
      TIM_Cmd(TIMx, DISABLE);
      
      switch (NumberOfByte)
      {
        /* CMD_RIGHT command received */
      case CMD_RIGHT_SIZE:
        if ((Buffercmp(TxBuffer, RxBuffer, CMD_RIGHT_SIZE, SPI_DATAMASK) != FAILED) && (CommandReceived == CMD_ACK))
        {
          /* Turn ON LED2 and LED3 */
          STM_EVAL_LEDOn(LED2);
          STM_EVAL_LEDOn(LED3);
          /* Turn OFF LED4 */
          STM_EVAL_LEDOff(LED4);
        }
        break;
        /* CMD_LEFT command received */
      case CMD_LEFT_SIZE:
        if ((Buffercmp(TxBuffer, RxBuffer, CMD_LEFT_SIZE, SPI_DATAMASK) != FAILED) && (CommandReceived == CMD_ACK))
        {
          /* Turn ON LED4 */
          STM_EVAL_LEDOn(LED4);
          /* Turn OFF LED2 and LED3 */
          STM_EVAL_LEDOff(LED2);
          STM_EVAL_LEDOff(LED3);
        }
        break;
        /* CMD_UP command received */
      case CMD_UP_SIZE:
        if ((Buffercmp(TxBuffer, RxBuffer, CMD_UP_SIZE, SPI_DATAMASK) != FAILED) && (CommandReceived == CMD_ACK))
        {
          /* Turn ON LED2 */
          STM_EVAL_LEDOn(LED2);
          /* Turn OFF LED3 and LED4 */
          STM_EVAL_LEDOff(LED3);
          STM_EVAL_LEDOff(LED4);
        }
        break;
        /* CMD_DOWN command received */
      case CMD_DOWN_SIZE:
        if ((Buffercmp(TxBuffer, RxBuffer, CMD_DOWN_SIZE, SPI_DATAMASK) != FAILED) && (CommandReceived == CMD_ACK))
        {
          /* Turn ON LED3 */
          STM_EVAL_LEDOn(LED3);
          /* Turn OFF LED2 and LED4 */
          STM_EVAL_LEDOff(LED2);
          STM_EVAL_LEDOff(LED4);
        }
        break;
        /* CMD_SEL command received */
      case CMD_SEL_SIZE:
        if ((Buffercmp(TxBuffer, RxBuffer, CMD_SEL_SIZE, SPI_DATAMASK) != FAILED) && (CommandReceived == CMD_ACK))
        {
          /* Turn ON LED2, LED3 and LED4 */
          STM_EVAL_LEDOn(LED2);
          STM_EVAL_LEDOn(LED3);
          STM_EVAL_LEDOn(LED4);
        }
        break;
      default:
        break;
      }
    }
  }
#endif /* SPI_MASTER */
  
  /* Slave board configuration -----------------------------------------------*/
#ifdef SPI_SLAVE
  
  /* Initializes the SPI communication */
  SPI_I2S_DeInit(SPIx);
  SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;
  SPI_Init(SPIx, &SPI_InitStructure);
  
  /* Initialize the FIFO threshold */
  SPI_RxFIFOThresholdConfig(SPIx, SPI_RxFIFOThreshold_QF);
  
  CommandTransmitted = CMD_ACK;
  
  /* Infinite Loop */
  while (1)
  {
    /* DMA channel Rx of SPI Configuration */
    DMA_InitStructure.DMA_BufferSize = 1;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SPIx_DR_ADDRESS;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) &CommandReceived;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_Init(SPIx_RX_DMA_CHANNEL, &DMA_InitStructure);
    
    /* DMA channel Tx of SPI Configuration */
    DMA_InitStructure.DMA_BufferSize = 1;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SPIx_DR_ADDRESS;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) &CommandTransmitted;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
    DMA_Init(SPIx_TX_DMA_CHANNEL, &DMA_InitStructure);
    
    /* Enable the SPI Rx and Tx DMA requests */
    SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Rx, ENABLE);
    SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Tx, ENABLE);
    
    /* Enable the SPI peripheral */
    SPI_Cmd(SPIx, ENABLE);
    
    CommandReceived = 0x00;
    
    /* Enable the DMA channels */
    DMA_Cmd(SPIx_RX_DMA_CHANNEL, ENABLE);
    DMA_Cmd(SPIx_TX_DMA_CHANNEL, ENABLE);
    
    /* Wait the SPI DMA transfers complete or time out */
    while (DMA_GetFlagStatus(SPIx_RX_DMA_FLAG_TC) == RESET)
    {}
    
    TimeOut = USER_TIMEOUT;
    while ((DMA_GetFlagStatus(SPIx_TX_DMA_FLAG_TC) == RESET)&&(TimeOut != 0x00))
    {}
    if(TimeOut == 0)
    {
      TimeOut_UserCallback();
    }  
    
    /* The BSY flag can be monitored to ensure that the SPI communication is complete.
    This is required to avoid corrupting the last transmission before disabling 
    the SPI or entering the Stop mode. The software must first wait until TXE=1
    and then until BSY=0.*/
    TimeOut = USER_TIMEOUT;
    while ((SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET)&&(TimeOut != 0x00))
    {}
    if(TimeOut == 0)
    {
      TimeOut_UserCallback();
    }
    
    TimeOut = USER_TIMEOUT;
    while ((SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_BSY) == SET)&&(TimeOut != 0x00))
    {}
    if(TimeOut == 0)
    {
      TimeOut_UserCallback();
    } 
    
    /* Clear DMA1 global flags */
    DMA_ClearFlag(SPIx_TX_DMA_FLAG_GL);
    DMA_ClearFlag(SPIx_RX_DMA_FLAG_GL);
    
    /* Disable the DMA channels */
    DMA_Cmd(SPIx_RX_DMA_CHANNEL, DISABLE);
    DMA_Cmd(SPIx_TX_DMA_CHANNEL, DISABLE);
    
    /* Disable the SPI peripheral */
    SPI_Cmd(SPIx, DISABLE);
    
    /* Disable the SPI Rx and Tx DMA requests */
    SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Rx, DISABLE);
    SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Tx, DISABLE);
    
    
    switch (CommandReceived)
    {
      /* CMD_RIGHT command received */
    case CMD_RIGHT:
      NumberOfByte = CMD_RIGHT_SIZE;
      break;
      /* CMD_LEFT command received */  
    case CMD_LEFT:
      NumberOfByte = CMD_LEFT_SIZE;
      break;
      /* CMD_UP command received */ 
    case CMD_UP:
      NumberOfByte = CMD_UP_SIZE;
      break;
      /* CMD_DOWN command received */
    case CMD_DOWN:
      NumberOfByte = CMD_DOWN_SIZE;
      break;
      /* CMD_SEL command received */
    case CMD_SEL:
      NumberOfByte = CMD_SEL_SIZE;
      break;
    default:
      break;
    }   
    
    /* DMA channel Rx of SPI Configuration */
    DMA_InitStructure.DMA_BufferSize = NumberOfByte;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SPIx_DR_ADDRESS;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)RxBuffer;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_Init(SPIx_RX_DMA_CHANNEL, &DMA_InitStructure);
    
    /* DMA channel Tx of SPI Configuration */
    DMA_InitStructure.DMA_BufferSize = NumberOfByte;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SPIx_DR_ADDRESS;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)TxBuffer;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
    DMA_Init(SPIx_TX_DMA_CHANNEL, &DMA_InitStructure);
    
    /* Enable the SPI Rx and Tx DMA requests */
    SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Rx, ENABLE);
    SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Tx, ENABLE);
    
    /* Enable the SPI peripheral */
    SPI_Cmd(SPIx, ENABLE);
    
    /* Enable the DMA channels */
    DMA_Cmd(SPIx_RX_DMA_CHANNEL, ENABLE);
    DMA_Cmd(SPIx_TX_DMA_CHANNEL, ENABLE);
    
    /* Wait the SPI DMA transfers complete or time out */
    while (DMA_GetFlagStatus(SPIx_RX_DMA_FLAG_TC) == RESET)
    {}
    
    TimeOut = USER_TIMEOUT;
    while ((DMA_GetFlagStatus(SPIx_TX_DMA_FLAG_TC) == RESET)&&(TimeOut != 0x00))
    {}
    if(TimeOut == 0)
    {
      TimeOut_UserCallback();
    } 
    
    /* The BSY flag can be monitored to ensure that the SPI communication is complete.
    This is required to avoid corrupting the last transmission before disabling 
    the SPI or entering the Stop mode. The software must first wait until TXE=1
    and then until BSY=0.*/
    TimeOut = USER_TIMEOUT;
    while ((SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET)&&(TimeOut != 0x00))
    {}
    if(TimeOut == 0)
    {
      TimeOut_UserCallback();
    }
    
    TimeOut = USER_TIMEOUT;
    while ((SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_BSY) == SET)&&(TimeOut != 0x00))
    {}
    if(TimeOut == 0)
    {
      TimeOut_UserCallback();
    }
    
    switch (NumberOfByte)
    {
      /* CMD_RIGHT command received */
    case CMD_RIGHT_SIZE:
      if (Buffercmp(TxBuffer, RxBuffer, CMD_RIGHT_SIZE, SPI_DATAMASK) != FAILED)
      {
        /* Turn ON LED2 and LED3 */
        STM_EVAL_LEDOn(LED2);
        STM_EVAL_LEDOn(LED3);
        /* Turn OFF LED4 */
        STM_EVAL_LEDOff(LED4);
      }
      break;
      /* CMD_LEFT command received */ 
    case CMD_LEFT_SIZE:
      if (Buffercmp(TxBuffer, RxBuffer, CMD_LEFT_SIZE, SPI_DATAMASK) != FAILED)
      {
        /* Turn ON LED4 */
        STM_EVAL_LEDOn(LED4);
        /* Turn OFF LED2 and LED3 */
        STM_EVAL_LEDOff(LED2);
        STM_EVAL_LEDOff(LED3);
      }
      break;
      /* CMD_UP command received */
    case CMD_UP_SIZE:
      if (Buffercmp(TxBuffer, RxBuffer, CMD_UP_SIZE, SPI_DATAMASK) != FAILED)
      {
        /* Turn ON LED2 */
        STM_EVAL_LEDOn(LED2);
        /* Turn OFF LED3 and LED4 */
        STM_EVAL_LEDOff(LED3);
        STM_EVAL_LEDOff(LED4);
      }
      break;
      /* CMD_DOWN command received */
    case CMD_DOWN_SIZE:
      if (Buffercmp(TxBuffer, RxBuffer, CMD_DOWN_SIZE, SPI_DATAMASK) != FAILED)
      {
        /* Turn ON LED3 */
        STM_EVAL_LEDOn(LED3);
        /* Turn OFF LED2 and LED4 */
        STM_EVAL_LEDOff(LED2);
        STM_EVAL_LEDOff(LED4);
      }
      break;
      /* CMD_SEL command received */
    case CMD_SEL_SIZE:
      if (Buffercmp(TxBuffer, RxBuffer, CMD_SEL_SIZE, SPI_DATAMASK) != FAILED)
      {
        /* Turn ON LED2, LED3 and LED4 */
        STM_EVAL_LEDOn(LED2);
        STM_EVAL_LEDOn(LED3);
        STM_EVAL_LEDOn(LED4);
      }
      break;
    default:
      break;
    }
    
    /* Clear DMA1 global flags */
    DMA_ClearFlag(SPIx_TX_DMA_FLAG_GL);
    DMA_ClearFlag(SPIx_RX_DMA_FLAG_GL);
    
    /* Disable the DMA channels */
    DMA_Cmd(SPIx_RX_DMA_CHANNEL, DISABLE);
    DMA_Cmd(SPIx_TX_DMA_CHANNEL, DISABLE);
    
    /* Disable the SPI peripheral */
    SPI_Cmd(SPIx, DISABLE);
    
    /* Disable the SPI Rx and Tx DMA requests */
    SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Rx, DISABLE);
    SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Tx, DISABLE);
  }
#endif /* SPI_SLAVE */
}

/**
  * @brief  Configures the SPI Peripheral.
  * @param  None
  * @retval None
  */
static void SPI_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable the SPI peripheral */
  RCC_APB2PeriphClockCmd(SPIx_CLK, ENABLE);
  
  /* Enable the DMA peripheral */
  RCC_AHBPeriphClockCmd(DMAx_CLK, ENABLE);
  
  /* Enable the TIM peripheral */
  RCC_APB1PeriphClockCmd(TIMx_CLK, ENABLE);

  /* Enable SCK, MOSI, MISO and NSS GPIO clocks */
  RCC_AHBPeriphClockCmd(SPIx_SCK_GPIO_CLK | SPIx_MISO_GPIO_CLK | SPIx_MOSI_GPIO_CLK |
                        SPIx_NSS_GPIO_CLK , ENABLE);
  
  /* Enable TIM DMA trigger clock */ 
  RCC_AHBPeriphClockCmd(TIMx_TRIGGER_GPIO_CLK, ENABLE);
  
  /* SPI pin mappings */
  GPIO_PinAFConfig(SPIx_SCK_GPIO_PORT, SPIx_SCK_SOURCE, SPIx_SCK_AF);
  GPIO_PinAFConfig(SPIx_MOSI_GPIO_PORT, SPIx_MOSI_SOURCE, SPIx_MOSI_AF);
  GPIO_PinAFConfig(SPIx_MISO_GPIO_PORT, SPIx_MISO_SOURCE, SPIx_MISO_AF);
  GPIO_PinAFConfig(SPIx_NSS_GPIO_PORT, SPIx_NSS_SOURCE, SPIx_NSS_AF);
  
  /* TIM capture compare pin mapping */
  GPIO_PinAFConfig(TIMx_TRIGGER_GPIO_PORT, TIMx_TRIGGER_SOURCE, TIMx_TRIGGER_AF);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;

  /* SPI SCK pin configuration */
  GPIO_InitStructure.GPIO_Pin = SPIx_SCK_PIN;
  GPIO_Init(SPIx_SCK_GPIO_PORT, &GPIO_InitStructure);

  /* SPI  MOSI pin configuration */
  GPIO_InitStructure.GPIO_Pin =  SPIx_MOSI_PIN;
  GPIO_Init(SPIx_MOSI_GPIO_PORT, &GPIO_InitStructure);

  /* SPI MISO pin configuration */
  GPIO_InitStructure.GPIO_Pin = SPIx_MISO_PIN;
  GPIO_Init(SPIx_MISO_GPIO_PORT, &GPIO_InitStructure);
  
  /* SPI NSS pin configuration */
  GPIO_InitStructure.GPIO_Pin = SPIx_NSS_PIN;
  GPIO_Init(SPIx_NSS_GPIO_PORT, &GPIO_InitStructure);
  
  /* Configure the TIM channelx capture compare as DMA Trigger */
  GPIO_InitStructure.GPIO_Pin =  TIMx_TRIGGER_PIN;
  GPIO_Init(TIMx_TRIGGER_GPIO_PORT, &GPIO_InitStructure);

  /* SPI configuration -------------------------------------------------------*/
  SPI_I2S_DeInit(SPIx);
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_DataSize = SPI_DATASIZE;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;

  /* DMA Configuration -------------------------------------------------------*/
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize =  DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
}

/**
* @brief  Basic management of the timeout situation.
* @param  None.
* @retval None.
*/
static void TimeOut_UserCallback(void)
{
  /* User can add his own implementation to manage TimeOut Communication failure */
  /* Block communication and all processes */
  while (1)
  {   
  }
}

/**
  * @brief  Configure a SysTick Base time to 10 ms.
  * @param  None
  * @retval None
  */
static void SysTickConfig(void)
{
  /* Setup SysTick Timer for 10ms interrupts  */
  if (SysTick_Config(SystemCoreClock / 100))
  {
    /* Capture error */
    while (1);
  }

  /* Configure the SysTick handler priority */
  NVIC_SetPriority(SysTick_IRQn, 0x0);
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval PASSED: pBuffer1 identical to pBuffer2
  *         FAILED: pBuffer1 differs from pBuffer2
  */
static TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength, uint8_t DataMask)
{
  while (BufferLength--)
  {
    if (((*pBuffer1) & DataMask) != *pBuffer2)
    {
      return FAILED;
    }
    pBuffer1++;
    pBuffer2++;
  }

  return PASSED;
}

#ifdef SPI_MASTER
/**
  * @brief  Configures the TIM Peripheral.
  * @param  None
  * @retval None
  */
static void TIM_Config(void)
{
  /* ---------------------------------------------------------------------------
  TIM2 Configuration: generate 1 PWM signals with 50% duty cycle.
    The TIM2CLK frequency is set to SystemCoreClock (Hz), to get TIM2 counter
    clock at 32 MHz the Prescaler is computed as following:
     - Prescaler = (TIM2CLK / TIM2 counter clock) - 1
    SystemCoreClock is set to 48 MHz for Ultra Low Power Medium-density devices.

    The TIM2 is running at 4 KHz: TIM2 Frequency = TIM2 counter clock/(TIM2_ARR + 1)
                                                  = 48 MHz / (TIM2_ARR +1) = 4 KHz
                                                 ==> TIM2_ARR + 1 = 12000
    TIM2 Channel2 duty cycle = (TIM2_CCR4/ TIM2_ARR)* 100 = 50%
    ==> TIM2_CCR4 = TIM2_ARR/2 = 6000             
  ----------------------------------------------------------------------------*/
  /* Compute the prescaler value */
  PrescalerValue = (uint16_t) (SystemCoreClock / 48000000) - 1;
  
  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = TIM_ARR;
  TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);

  /* TIM PWM1 Mode configuration: Channel */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = TIM_CCR;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
 
  TIMx_CHANNEL_INIT(TIMx, &TIM_OCInitStructure);
}
/**
* @brief  Reads key from evaluationboard.
* @param  None
* @retval Return JOY_RIGHT, JOY_LEFT, JOY_SEL, JOY_UP, JOY_DOWN or JOY_NONE
*/
static JOYState_TypeDef Read_Joystick(void)
{
  /* "JOY_RIGHT" key is pressed */
  if (STM_EVAL_PBGetState(BUTTON_RIGHT))
  {
    while (STM_EVAL_PBGetState(BUTTON_RIGHT) == RESET)
    {}
    return JOY_RIGHT;
  }
  /* "JOY_LEFT" key is pressed */
  if (STM_EVAL_PBGetState(BUTTON_LEFT))
  {
    while (STM_EVAL_PBGetState(BUTTON_LEFT) == RESET)
    {}
    return JOY_LEFT;
  }
  /* "JOY_UP" key is pressed */
  if (STM_EVAL_PBGetState(BUTTON_UP))
  {
    while (STM_EVAL_PBGetState(BUTTON_UP) == RESET)
    {}
    return JOY_UP;
  }
  /* "JOY_DOWN" key is pressed */
  if (STM_EVAL_PBGetState(BUTTON_DOWN))
  {
    while (STM_EVAL_PBGetState(BUTTON_DOWN) == RESET)
    {}
    return JOY_DOWN;
  }
  /* "JOY_SEL" key is pressed */
  if (STM_EVAL_PBGetState(BUTTON_SEL))
  {
    while (STM_EVAL_PBGetState(BUTTON_SEL) == RESET)
    {}
    return JOY_SEL;
  }
  /* No key is pressed */
  else
  {
    return JOY_NONE;
  }
}

/**
  * @brief  Fills buffer.
  * @param  pBuffer: pointer on the Buffer to fill
  * @param  BufferLength: size of the buffer to fill
  * @retval None
  */
static void Fill_Buffer(uint8_t *pBuffer, uint16_t BufferLength)
{
  uint16_t index = 0;

  /* Put in global buffer same values */
  for (index = 0; index < BufferLength; index++ )
  {
    pBuffer[index] = 0x00;
  }
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
