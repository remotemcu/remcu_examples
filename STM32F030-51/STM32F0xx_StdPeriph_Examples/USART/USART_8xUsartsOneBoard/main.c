/**
  ******************************************************************************
  * @file    USART/USART_8xUsartsOneBoard/main.c 
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

/** @addtogroup USART_8xUsartsOneBoard
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static __IO uint32_t TimingDelay;

/* Buffer used for transmission */
uint8_t aTxBuffer[BUFFER_SIZE] = "USART Example: 8xUsarts Tx/Rx Communication";
uint8_t aRxBuffer[USART_MAX_INDEX][BUFFER_SIZE];
/* Buffer used for reception */
USART_InitTypeDef USART_InitStructure;
__IO uint8_t TxCounter = 0;
extern __IO uint8_t ReceiveState;
USART_TypeDef* UsartInstance[USART_MAX_INDEX] = {USART1, USART2, USART3, USART4, USART5, USART6, USART7, USART8};
uint8_t UsartIndex = 0;
/* Private function prototypes -----------------------------------------------*/
static uint16_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);
static void RCC_Configuration(void);
static void GPIO_Configuration(void);
static void NVIC_Configuration(void);
static void Delay(__IO uint32_t nTime);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  uint8_t *buffptr;
  
  /*!< At this stage the microcontroller clock setting is already configured, 
  this is done through SystemInit() function which is called from startup
  file (startup_stm32f0xx.s) before to branch to application main.
  To reconfigure the default setting of SystemInit() function, refer to
  system_stm32f0xx.c file
  */ 
  if (SysTick_Config(SystemCoreClock / 1000))
  { 
    /* error */ 
    while (1);
  }
  /* Configure clock GPIO, USARTs */
  RCC_Configuration();
  
  /* Configure GPIO Pin Tx/Rx for Usart communication */
  GPIO_Configuration();
  
  /* Configure NVIC */
  NVIC_Configuration();
  
  /* 8xUSARTs configuration --------------------------------------------------*/
  /* 8xUSARTs  configured as follow:
  - BaudRate = 9600 baud  
  - Word Length = 8 Bits
  - One Stop Bit
  - No parity
  - Hardware flow control disabled (RTS and CTS signals)
  - Receive and transmit enabled
  */
  USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  
  /* Prepare all uart to receive a data packet */
  for(UsartIndex = 0; UsartIndex < USART_MAX_INDEX; UsartIndex++)
  {
    USART_Init(UsartInstance[UsartIndex], &USART_InitStructure);
    
    /* Enable 8xUSARTs Receive interrupts */
    USART_ITConfig(UsartInstance[UsartIndex], USART_IT_RXNE, ENABLE);
    
    /* Enable the 8xUSARTs */
    USART_Cmd(UsartInstance[UsartIndex], ENABLE);
  }
  
  /* Infinite Loop */
  while(1)
  {
    /* Set aRxBuffer to 0 */
    memset(aRxBuffer, 0x0, BUFFER_SIZE * USART_MAX_INDEX);
    
    /* When data has been transferred start the next transfer */
    for(UsartIndex = 0; UsartIndex < USART_MAX_INDEX; UsartIndex++)
    { 
      /* Initialize the global variable to handle the data transfer */
      TxCounter = 0;
      ReceiveState = 0;
      
      /* Delay to let time to see counter incrementation on the screen */
      Delay(2000);
      /* Set the data buffptr to use for the transfer */
      if (UsartIndex == 0)
      {
        buffptr = aTxBuffer;
      }
      else
      {
        buffptr = aRxBuffer[UsartIndex];
      }
      
      while(TxCounter < BUFFER_SIZE)
      {   
        /* Send one byte from USART1 to USARTx */
        USART_SendData(UsartInstance[UsartIndex], buffptr[TxCounter++]);
        
        /* Loop until USART1 DR register is empty */ 
        while(USART_GetFlagStatus(UsartInstance[UsartIndex], USART_FLAG_TXE) == RESET)
        {}
      }
      
      while(ReceiveState == 0);
      /* Compares two buffers */
      if(UsartIndex == (USART_MAX_INDEX - 1))
      {
        if( Buffercmp((uint8_t*) aRxBuffer[0], (uint8_t*) aTxBuffer, BUFFER_SIZE))
        {
          /* Error */
          while(1);
        }
      }
      else
      {
        if( Buffercmp((uint8_t*) aRxBuffer[UsartIndex+1], (uint8_t*) aTxBuffer, BUFFER_SIZE))
        {
          /* Error */
          while(1);
        }
      }
      ReceiveState = 0;
    }
    /* Insert delay */
    Delay(2000);
  }
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}

/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
static void RCC_Configuration(void)
{   
  /* Enable GPIO clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA|RCC_AHBPeriph_GPIOB|RCC_AHBPeriph_GPIOC|RCC_AHBPeriph_GPIOD|RCC_AHBPeriph_GPIOF, ENABLE);
  
  /* Enable 8xUSARTs Clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2|RCC_APB1Periph_USART3|RCC_APB1Periph_USART4|RCC_APB1Periph_USART5, ENABLE);  
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6|RCC_APB2Periph_USART7|RCC_APB2Periph_USART8|RCC_APB2Periph_USART1, ENABLE);
}

/**
  * @brief  Configures the different GPIO ports.
  * @param  None
  * @retval None
  */
static void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* USART1 Pins configuration ************************************************/
  /* Connect pin to Periph */
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1); 
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);    
  
  /* Configure pins as AF pushpull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);  

  /* USART2 Pins configuration ************************************************/  
  /* Connect pin to Periph */
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_0);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_0);    
  
  /* Configure pins as AF pushpull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &GPIO_InitStructure);   
  
  /* USART3 Pins configuration  ***********************************************/  
  /* Connect pin to Periph */
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_0);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_0);    
  
  /* Configure pins as AF pushpull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &GPIO_InitStructure); 
  
  /* USART4 Pins configuration  ***********************************************/  
  /* Connect pin to Periph */
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_0);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_0);    
 
  /* Configure pins as AF pushpull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStructure); 
  
  /* USART5 Pins configuration*************************************************/  
  /* Connect pin to Periph */
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_4);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_4);    
  
  /* Configure pins as AF pushpull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  /* USART6 Pins configuration*************************************************/  
  /* Connect pin to Periph */
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource9, GPIO_AF_1);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource10, GPIO_AF_1);    
  
  /* Configure pins as AF pushpull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOF, &GPIO_InitStructure); 
  
  /* USART7 Pins configuration  *************************************************/  
  /* Connect pin to Periph */
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource2, GPIO_AF_1);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource3, GPIO_AF_1);    
  
  /* Configure pins as AF pushpull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOF, &GPIO_InitStructure); 
    
  /* USART8 Pins configuration  *************************************************/  
  /* Connect pin to Periph */
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_1);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_1);    
  
  /* Configure pins as AF pushpull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStructure); 
}

/**
  * @brief  Configures the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* USART1 IRQ Channel configuration */
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* USART2 IRQ Channel configuration */
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* USART3_8 IRQ Channel configuration */
  NVIC_InitStructure.NVIC_IRQChannel = USART3_8_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
static void Delay(__IO uint32_t nTime)
{ 
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval 0  : pBuffer1 identical to pBuffer2
  *        >0 : pBuffer1 differs from pBuffer2
  */
static uint16_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    if ((*pBuffer1) != *pBuffer2)
    {
      return BufferLength;
    }
    pBuffer1++;
    pBuffer2++;
  }

  return 0;
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
