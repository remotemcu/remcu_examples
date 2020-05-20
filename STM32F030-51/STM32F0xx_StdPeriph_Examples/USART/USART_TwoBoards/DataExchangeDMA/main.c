/**
  ******************************************************************************
  * @file    USART/USART_TwoBoards/DataExchangeDMA/main.c 
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

/** @addtogroup USART_DataExchangeDMA
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
DMA_InitTypeDef  DMA_InitStructure;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t TxBuffer[] = "USART DMA Example: Communication between two USART using DMA";
uint8_t RxBuffer [RXBUFFERSIZE];

uint8_t CmdBuffer [0x02] = {0x00, 0x00}; /* {Command, Number of byte to receive or to transmit} */
uint8_t AckBuffer [0x02] = {0x00, 0x00}; /* {Command, ACK} */

__IO uint32_t TimeOut = 0x0;   
__IO JOYState_TypeDef PressedButton = JOY_NONE;

/* Private function prototypes -----------------------------------------------*/
static void TimeOut_UserCallback(void);
static void USART_Config(void);
static void SysTickConfig(void);
static JOYState_TypeDef Read_Joystick(void);
static TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);
static void Fill_Buffer(uint8_t *pBuffer, uint16_t BufferLength);

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
  /* USART configuration -----------------------------------------------------*/
  USART_Config();
  
  /* SysTick configuration ---------------------------------------------------*/
  SysTickConfig();
  
  /* Initialize LEDs mounted on EVAL board */
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);
  
  /* Initialize push-buttons mounted on EVAL board */
  STM_EVAL_PBInit(BUTTON_RIGHT, BUTTON_MODE_GPIO);
  STM_EVAL_PBInit(BUTTON_LEFT, BUTTON_MODE_GPIO);
  STM_EVAL_PBInit(BUTTON_UP, BUTTON_MODE_GPIO);
  STM_EVAL_PBInit(BUTTON_DOWN, BUTTON_MODE_GPIO);
  STM_EVAL_PBInit(BUTTON_SEL, BUTTON_MODE_GPIO);
  
  while (1)
  {
    /* Clear Buffers */
    Fill_Buffer(CmdBuffer, 0x02);
    Fill_Buffer(AckBuffer, 0x02);
    Fill_Buffer(RxBuffer, TXBUFFERSIZE);
    
    /* Waiting transaction code in case of USART receiver */
    /* DMA channel Rx of USART Configuration */
    DMA_DeInit(USARTx_RX_DMA_CHANNEL);
    DMA_InitStructure.DMA_PeripheralBaseAddr = USARTx_RDR_ADDRESS;
    DMA_InitStructure.DMA_BufferSize = (uint16_t)2;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)CmdBuffer;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
    DMA_Init(USARTx_RX_DMA_CHANNEL, &DMA_InitStructure);
    
    /* Enable the USART Rx DMA request */
    USART_DMACmd(USARTx, USART_DMAReq_Rx, ENABLE);
    
    /* Enable the DMA channel */
    DMA_Cmd(USARTx_RX_DMA_CHANNEL, ENABLE);
    
    PressedButton = Read_Joystick();
    
    /* Waiting Joystick pressed in case to transmit data or received Transaction command */ 
    while((PressedButton == JOY_NONE) && (CmdBuffer[0x00] == 0x00))
    {
      PressedButton = Read_Joystick();
    }
    
    /* USART in Mode Transmitter ---------------------------------------------*/
    if ((PressedButton != JOY_NONE) && (CmdBuffer[0x00] == 0x00))
    {
      /* Configure the USART to receive the ACK command Table */
      /* DMA channel Rx of USART Configuration */
      DMA_DeInit(USARTx_RX_DMA_CHANNEL);
      DMA_InitStructure.DMA_PeripheralBaseAddr = USARTx_RDR_ADDRESS;
      DMA_InitStructure.DMA_BufferSize = (uint16_t)2;
      DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)AckBuffer;
      DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
      DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
      DMA_Init(USARTx_RX_DMA_CHANNEL, &DMA_InitStructure);
      
      /* Configure the USART to send the command table */    
      /* DMA channel Tx of USART Configuration */
      DMA_DeInit(USARTx_TX_DMA_CHANNEL);
      DMA_InitStructure.DMA_PeripheralBaseAddr = USARTx_TDR_ADDRESS;
      DMA_InitStructure.DMA_BufferSize = (uint16_t)2;
      DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)CmdBuffer;
      DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
      DMA_InitStructure.DMA_Priority = DMA_Priority_High;
      DMA_Init(USARTx_TX_DMA_CHANNEL, &DMA_InitStructure);
      
      switch (PressedButton)
      {
        /* JOY_RIGHT button pressed */
        case JOY_RIGHT:
          CmdBuffer[0x00] = CMD_RIGHT;
          CmdBuffer[0x01] = CMD_RIGHT_SIZE;
          break;
        /* JOY_LEFT button pressed */
        case JOY_LEFT:
          CmdBuffer[0x00] = CMD_LEFT;
          CmdBuffer[0x01]  = CMD_LEFT_SIZE;
          break;
        /* JOY_UP button pressed */
        case JOY_UP:
          CmdBuffer[0x00] = CMD_UP;
          CmdBuffer[0x01] = CMD_UP_SIZE;
          break;
        /* JOY_DOWN button pressed */          
        case JOY_DOWN:
          CmdBuffer[0x00] = CMD_DOWN;
          CmdBuffer[0x01] = CMD_DOWN_SIZE;
          break;
        /* JOY_SEL button pressed */
        case JOY_SEL:
          CmdBuffer[0x00] = CMD_SEL;
          CmdBuffer[0x01] = CMD_SEL_SIZE;
          break;
        default:
          break;
      }
      
      /* Enable the USART DMA requests */
      USART_DMACmd(USARTx, USART_DMAReq_Rx, ENABLE);
      USART_DMACmd(USARTx, USART_DMAReq_Tx, ENABLE);
      
      /* Clear the TC bit in the SR register by writing 0 to it */
      USART_ClearFlag(USARTx, USART_FLAG_TC);
      
      /* Enable the DMA USART Tx Channel */
      DMA_Cmd(USARTx_TX_DMA_CHANNEL, ENABLE);
      /* Enable the DMA USART Rx channel */
      DMA_Cmd(USARTx_RX_DMA_CHANNEL, ENABLE);
      
      /* Wait the USART DMA Tx transfer complete or time out */
      TimeOut = USER_TIMEOUT;
      
      while ((DMA_GetFlagStatus(USARTx_TX_DMA_FLAG_TC) == RESET)&&(TimeOut != 0x00))
      {
      }
      
      if(TimeOut == 0)
      {
        TimeOut_UserCallback();
      } 
      /* The software must wait until TC=1. The TC flag remains cleared during all data
         transfers and it is set by hardware at the last frame’s end of transmission*/
      TimeOut = USER_TIMEOUT;
      while ((USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET)&&(TimeOut != 0x00))
      {
      }
      if(TimeOut == 0)
      {
        TimeOut_UserCallback();
      }      
      
      /* Wait the USART DMA Rx transfer complete or time out */
      TimeOut = USER_TIMEOUT;
      
      while ((DMA_GetFlagStatus(USARTx_RX_DMA_FLAG_TC) == RESET)&&(TimeOut != 0x00))
      {}
      
      if(TimeOut == 0)
      {
        TimeOut_UserCallback();
      }
      
      /* Clear DMA1 global flags */
      DMA_ClearFlag(USARTx_TX_DMA_FLAG_GL);
      DMA_ClearFlag(USARTx_RX_DMA_FLAG_GL);
      
      /* Disable the DMA channels */
      DMA_Cmd(USARTx_TX_DMA_CHANNEL, DISABLE);
      DMA_Cmd(USARTx_RX_DMA_CHANNEL, DISABLE);
      
      /* Disable the USART Tx DMA request */
      USART_DMACmd(USARTx, USART_DMAReq_Tx, DISABLE);
      /* Disable the USART Rx DMA requests */
      USART_DMACmd(USARTx,  USART_DMAReq_Rx, DISABLE);
      
      /* DMA channel Tx of USART Configuration */
      DMA_DeInit(USARTx_TX_DMA_CHANNEL);
      DMA_InitStructure.DMA_PeripheralBaseAddr = USARTx_TDR_ADDRESS;
      DMA_InitStructure.DMA_BufferSize = (uint16_t)CmdBuffer[0x01];
      DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)TxBuffer;
      DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
      DMA_InitStructure.DMA_Priority = DMA_Priority_High;
      DMA_Init(USARTx_TX_DMA_CHANNEL, &DMA_InitStructure);
      
      USART_DMACmd(USARTx, USART_DMAReq_Tx, ENABLE);
      
      /* Clear the TC bit in the SR register by writing 0 to it */
      USART_ClearFlag(USARTx, USART_FLAG_TC);
      
      /* Enable DMA1 USART Tx Channel */
      DMA_Cmd(USARTx_TX_DMA_CHANNEL, ENABLE);
      
      /* Wait the USART DMA Tx transfer complete or time out */
      TimeOut = USER_TIMEOUT;
      while ((DMA_GetFlagStatus(USARTx_TX_DMA_FLAG_TC) == RESET)&&(TimeOut != 0x00))
      {
      }
      if(TimeOut == 0)
      {
        TimeOut_UserCallback();
      }
      
      /* The software must wait until TC=1. The TC flag remains cleared during all data
         transfers and it is set by hardware at the last frame’s end of transmission*/
      TimeOut = USER_TIMEOUT;
      while ((USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET)&&(TimeOut != 0x00))
      {
      }
      if(TimeOut == 0)
      {
        TimeOut_UserCallback();
      }
       
      /* Clear DMA global flags */
      DMA_ClearFlag(USARTx_TX_DMA_FLAG_GL);
      DMA_ClearFlag(USARTx_RX_DMA_FLAG_GL);
      
      DMA_Cmd(USARTx_TX_DMA_CHANNEL, DISABLE);
      
      /* Disable the USART Tx DMA requests */
      USART_DMACmd(USARTx, USART_DMAReq_Tx, DISABLE);
      
      CmdBuffer[0x00] = 0x00;
    }
    
    /* USART in Mode Receiver-------------------------------------------------*/
    /* USART Receive Transaction command and the number of Bytes to receive */ 
    if (CmdBuffer[0x00] != 0x00)
    {
      /* Wait the USART DMA Rx transfer complete or time out */
      TimeOut = USER_TIMEOUT;
      while ((DMA_GetFlagStatus(USARTx_RX_DMA_FLAG_TC) == RESET)&&(TimeOut != 0x00))
      {}
      if(TimeOut == 0)
      {
        TimeOut_UserCallback();
      }
      
      /* Clear DMA1 global flags */
      DMA_ClearFlag(USARTx_TX_DMA_FLAG_GL);
      DMA_ClearFlag(USARTx_RX_DMA_FLAG_GL);
      
      /* Disable the DMA channels */
      DMA_Cmd(USARTx_RX_DMA_CHANNEL, DISABLE);
      
      /* Disable the USART Rx DMA requests */
      USART_DMACmd(USARTx, USART_DMAReq_Rx, DISABLE);
      
      /* At this Steep the USART send the ACK command (after Receive the transaction 
         command and the number of data to receive this parameter is mandatory 
         to configure the DMA_BufferSize in the second reception phase */    
      
      AckBuffer[0x00] = CmdBuffer[0x00];
      AckBuffer[0x01] = CMD_ACK;
      
      /* DMA channel Tx of USART Configuration */
      DMA_DeInit(USARTx_TX_DMA_CHANNEL);
      DMA_InitStructure.DMA_PeripheralBaseAddr = USARTx_TDR_ADDRESS;
      DMA_InitStructure.DMA_BufferSize = (uint16_t)2;
      DMA_InitStructure.DMA_MemoryBaseAddr =  (uint32_t)AckBuffer;
      DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
      DMA_InitStructure.DMA_Priority = DMA_Priority_High;
      DMA_Init(USARTx_TX_DMA_CHANNEL, &DMA_InitStructure);
      
      USART_DMACmd(USARTx, USART_DMAReq_Tx, ENABLE);
      
      /* Clear the TC bit in the SR register by writing 0 to it */
      USART_ClearFlag(USARTx, USART_FLAG_TC);
      
      /* Enable DMA1 USART Tx Channel */
      DMA_Cmd(USARTx_TX_DMA_CHANNEL, ENABLE);
      
      /* Wait the USART DMA Tx transfer complete or time out */
      TimeOut = USER_TIMEOUT;
      
      while ((DMA_GetFlagStatus(USARTx_TX_DMA_FLAG_TC) == RESET)&&(TimeOut != 0x00))
      {}
      
      if(TimeOut == 0)
      {
        TimeOut_UserCallback();
      }
      
      /* The software must wait until TC=1. The TC flag remains cleared during all data
         transfers and it is set by hardware at the last frame’s end of transmission*/
      TimeOut = USER_TIMEOUT;
      while ((USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET)&&(TimeOut != 0x00))
      {
      }
      if(TimeOut == 0)
      {
        TimeOut_UserCallback();
      } 
      
      /* Clear DMA1 global flags */
      DMA_ClearFlag(USARTx_TX_DMA_FLAG_GL);
      DMA_ClearFlag(USARTx_RX_DMA_FLAG_GL);
      
      DMA_Cmd(USARTx_TX_DMA_CHANNEL, DISABLE);
      
      /* Disable the USART Tx DMA requests */
      USART_DMACmd(USARTx, USART_DMAReq_Tx, DISABLE);
      
      /* The transmitter After receive the ACK command it sends the defined data
         in his TxBuffer */      
      /* DMA channel Rx of USART Configuration */
      DMA_DeInit(USARTx_RX_DMA_CHANNEL);
      DMA_InitStructure.DMA_PeripheralBaseAddr = USARTx_RDR_ADDRESS;
      DMA_InitStructure.DMA_BufferSize = (uint16_t)CmdBuffer[0x01];
      DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) RxBuffer;
      DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
      DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
      DMA_Init(USARTx_RX_DMA_CHANNEL, &DMA_InitStructure);
      
      USART_DMACmd(USARTx, USART_DMAReq_Rx , ENABLE);
      
      /* Enable the DMA channel */
      DMA_Cmd(USARTx_RX_DMA_CHANNEL, ENABLE);
      
      /* Wait the USART DMA Rx transfer complete or time out */
      TimeOut = USER_TIMEOUT;
      
      while ((DMA_GetFlagStatus(USARTx_RX_DMA_FLAG_TC) == RESET)&&(TimeOut != 0x00))
      {
      }
      
      if(TimeOut == 0)
      {
        TimeOut_UserCallback();
      }
      
      /* Clear DMA1 global flags */
      DMA_ClearFlag(USARTx_TX_DMA_FLAG_GL);
      DMA_ClearFlag(USARTx_RX_DMA_FLAG_GL);
      
      /* Disable the DMA channels */
      DMA_Cmd(USARTx_RX_DMA_CHANNEL, DISABLE);
      
      /* Disable the USART Rx DMA requests */
      USART_DMACmd(USARTx, USART_DMAReq_Rx, DISABLE);
      
      switch (CmdBuffer[0x01])
      {
        /* CMD_RIGHT command received */
        case CMD_RIGHT_SIZE:
          if (Buffercmp(TxBuffer, RxBuffer, CMD_RIGHT_SIZE) != FAILED)
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
          if (Buffercmp(TxBuffer, RxBuffer, CMD_LEFT_SIZE) != FAILED)
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
          if (Buffercmp(TxBuffer, RxBuffer, CMD_UP_SIZE) != FAILED)
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
          if (Buffercmp(TxBuffer, RxBuffer, CMD_DOWN_SIZE) != FAILED)
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
          if (Buffercmp(TxBuffer, RxBuffer, CMD_SEL_SIZE) != FAILED) 
          {
            /* Turn ON all LED2, LED3 and LED4 */
            STM_EVAL_LEDOn(LED2);
            STM_EVAL_LEDOn(LED3);
            STM_EVAL_LEDOn(LED4);
          }
          break;
        default:
          break;
      }
      CmdBuffer[0x00] = 0x00;
    }
  }
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
* @brief  Configures the USART Peripheral.
* @param  None
* @retval None
*/
static void USART_Config(void)
{
  USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* Enable GPIO clock */
  RCC_AHBPeriphClockCmd(USARTx_TX_GPIO_CLK | USARTx_RX_GPIO_CLK, ENABLE);
  
  /* Enable USART clock */
  USARTx_APBPERIPHCLOCK(USARTx_CLK, ENABLE);
  
  /* Enable the DMA periph */
  RCC_AHBPeriphClockCmd(DMAx_CLK, ENABLE);
  
  /* Connect PXx to USARTx_Tx */
  GPIO_PinAFConfig(USARTx_TX_GPIO_PORT, USARTx_TX_SOURCE, USARTx_TX_AF);
  
  /* Connect PXx to USARTx_Rx */
  GPIO_PinAFConfig(USARTx_RX_GPIO_PORT, USARTx_RX_SOURCE, USARTx_RX_AF);
  
  /* Configure USART Tx and Rx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  
  GPIO_InitStructure.GPIO_Pin = USARTx_TX_PIN;
  GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = USARTx_RX_PIN;
  GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStructure);
  
  /* USARTx configuration ----------------------------------------------------*/
  /* USARTx configured as follow:
  - BaudRate = 230400 baud  
  - Word Length = 8 Bits
  - one Stop Bit
  - No parity
  - Hardware flow control disabled (RTS and CTS signals)
  - Receive and transmit enabled
  */
  USART_InitStructure.USART_BaudRate = 230400;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  /* When using Parity the word length must be configured to 9 bits */
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USARTx, &USART_InitStructure);
  
  /* DMA Configuration -------------------------------------------------------*/
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    
  /* Enable USART */
  USART_Cmd(USARTx, ENABLE);
}

/**
* @brief  Configure a SysTick Base time to 10 ms.
* @param  None
* @retval None
*/
static void SysTickConfig(void)
{
  /* Set SysTick Timer for 10ms interrupts  */
  if (SysTick_Config(SystemCoreClock / 100))
  {
    /* Capture error */
    while (1);
  }
  /* Configure the SysTick handler priority */
  NVIC_SetPriority(SysTick_IRQn, 0x0);
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
* @brief  Compares two buffers.
* @param  pBuffer1, pBuffer2: buffers to be compared.
* @param  BufferLength: buffer's length
* @retval PASSED: pBuffer1 identical to pBuffer2
*         FAILED: pBuffer1 differs from pBuffer2
*/
static TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
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
