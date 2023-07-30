/**
  ******************************************************************************
  * @file    DMA_ChannelsPriority/main.c
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

/** @addtogroup STM8L15x_StdPeriph_Examples
  * @{
  */
/** @addtogroup DMA_ChannelsPriority
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Uncomment one of the lines below to choose the configuration*/
/*#define PRIORITIES_CONFIG_HW */
#define PRIORITIES_CONFIG_SW_1
/*#define PRIORITIES_CONFIG_SW_2 */

#define TIM1_ARRL_ADDRESS      ((uint16_t)(TIM1_BASE + 0x14))
#define TIM2_ARRL_ADDRESS      ((uint16_t)(TIM2_BASE + 0x10))
#define TIM3_ARRL_ADDRESS      ((uint16_t)(TIM3_BASE + 0x10))
#define TIM4_ARR_ADDRESS       ((uint16_t)(TIM4_BASE + 0x09))
#define RAM_Buffer1_ADDRESS    ((uint16_t)(&RAMBuffer1))
#define RAM_Buffer2_ADDRESS    ((uint16_t)(&RAMBuffer2))

uint8_t RAMBuffer1 = 0x00;
uint8_t RAMBuffer2 = 0x55;

DMA_Priority_TypeDef DMA_Channel0_Priority;
DMA_Priority_TypeDef DMA_Channel1_Priority;
DMA_Priority_TypeDef DMA_Channel2_Priority;
DMA_Priority_TypeDef DMA_Channel3_Priority;

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint8_t TIM1RegValue    = 0;
__IO uint8_t TIM2RegValue    = 0;
__IO uint8_t TIM3RegValue    = 0;
__IO uint8_t TIM4RegValue    = 0;
__IO uint8_t RAMBuffer1Value = 0;
__IO uint8_t RAMBuffer2Value = 0;


/* Private function prototypes -----------------------------------------------*/
void Init_RAMBuffers(void);
void Init_TimersARRRegisters(void);
void Init_TimersDMARequestSources(void);
void Init_DMAChannels(void);
void Enable_Timers(void);
void Enable_DMAchannels(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */

void main(void)
{

  /* Enable DMA1 clock */
  CLK_PeripheralClockConfig(CLK_Peripheral_DMA1, ENABLE);

  /* Enable Timers clock */
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM1, ENABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM2, ENABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM3, ENABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, ENABLE);

  SYSCFG_REMAPDMAChannelConfig(REMAP_DMA1Channel_TIM4ToChannel3);

  /* Initialise Timers autoreload registers (ARR)*/
  Init_TimersARRRegisters();

  /* Read registers values for display*/
  TIM1RegValue    = TIM1->ARRL;
  TIM2RegValue    = TIM2->ARRL;
  TIM3RegValue    = TIM3->ARRL;
  TIM4RegValue    = TIM4->ARR;
  RAMBuffer1Value = RAMBuffer1;
  RAMBuffer2Value = RAMBuffer2;

  /* Initialise RAMBuffer1 and RAMBuffer2*/
  Init_RAMBuffers();

  /* Initialise Timers DMA requests */
  Init_TimersDMARequestSources();


#ifdef  PRIORITIES_CONFIG_SW_1

  /* DMA Channels Priorities */
  DMA_Channel0_Priority = DMA_Priority_Low;
  DMA_Channel1_Priority = DMA_Priority_High;
  DMA_Channel2_Priority = DMA_Priority_VeryHigh;
  DMA_Channel3_Priority = DMA_Priority_Medium;

  /* Initialise DMA channels with their correspondent Priorities */
  Init_DMAChannels();

  /* Enable Timers 1, 2, 3 and 4*/
  Enable_Timers();

  /* Enable DMA Channels 0, 1, 2 and 3*/
  Enable_DMAchannels();

  /* Enable the DMA */
  DMA_GlobalCmd(ENABLE);



  while ((DMA_GetFlagStatus(DMA1_FLAG_TC0) == RESET) &&
         (DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET) &&
         (DMA_GetFlagStatus(DMA1_FLAG_TC2) == RESET) &&
         (DMA_GetFlagStatus(DMA1_FLAG_TC3) == RESET));



  /*
    The expected result is
    
    RAMBuffer1 = 0x11
    TIM2_ARRL  = 0x11
    TIM3_ARRL  = 0x44  
  RAMBuffer2 = 0x44

    */

#elif defined (PRIORITIES_CONFIG_SW_2)

  /* DMA configuration*/
  DMA_Channel0_Priority = DMA_Priority_Medium;
  DMA_Channel1_Priority = DMA_Priority_VeryHigh;
  DMA_Channel2_Priority = DMA_Priority_High;
  DMA_Channel3_Priority = DMA_Priority_Low;

  /* Initialise DMA channels with their correspondent Priorities */
  Init_DMAChannels();

  /* Enable Timers 1, 2, 3 and 4*/
  Enable_Timers();

  /* Enable DMA Channels 0, 1, 2 and 3*/
  Enable_DMAchannels();

  /* Enable the DMA */
  DMA_GlobalCmd(ENABLE);


  while ((DMA_GetFlagStatus(DMA1_FLAG_TC0) == RESET) &&
         (DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET) &&
         (DMA_GetFlagStatus(DMA1_FLAG_TC2) == RESET) &&
         (DMA_GetFlagStatus(DMA1_FLAG_TC3) == RESET));


  /*
  The expected result is

  RAMBuffer1 = 0x11
  TIM2_ARRL  = 0x00
  TIM3_ARRL  = 0x55
  RAMBuffer2 = 0x44
  */

#elif defined (PRIORITIES_CONFIG_HW)

  /* DMA configuration*/
  DMA_Channel0_Priority = DMA_Priority_Low;
  DMA_Channel1_Priority = DMA_Priority_Low;
  DMA_Channel2_Priority = DMA_Priority_Low;
  DMA_Channel3_Priority = DMA_Priority_Low;

  /* Initialise DMA channels with their correspondent Priorities */
  Init_DMAChannels();

  /* Enable Timers 1, 2, 3 and 4*/
  Enable_Timers();

  /* Enable DMA Channels 0, 1, 2 and 3*/
  Enable_DMAchannels();

  /* Enable the DMA */
  DMA_GlobalCmd(ENABLE);


  while ((DMA_GetFlagStatus(DMA1_FLAG_TC0) == RESET) &&
         (DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET) &&
         (DMA_GetFlagStatus(DMA1_FLAG_TC2) == RESET) &&
         (DMA_GetFlagStatus(DMA1_FLAG_TC3) == RESET));


  /*
  The expected result is

  RAMBuffer1 = 0x11
  TIM2_ARRL  = 0x00
  TIM3_ARRL  = 0x55
  RAMBuffer2 = 0x44
  */

#endif /* PRIORITIES_CONFIG_SW_1 */

  /* Read registers values for display*/
  TIM1RegValue    = TIM1->ARRL;
  TIM2RegValue    = TIM2->ARRL;
  TIM3RegValue    = TIM3->ARRL;
  TIM4RegValue    = TIM4->ARR;
  RAMBuffer1Value = RAMBuffer1;
  RAMBuffer2Value = RAMBuffer2;



  while (1);

}

/**
  * @brief  Init RAM Buffers.
  * @param  None.
  * @retval None
  */
void Init_RAMBuffers(void)
{
  RAMBuffer1 = 0x00;
  RAMBuffer2 = 0x55;
}

/**
  * @brief  Init Timers Autorleoad registers.
  * @param  None.
  * @retval None
  */
void Init_TimersARRRegisters(void)
{
  TIM1->ARRH = 0x00;
  TIM2->ARRH = 0x00;
  TIM3->ARRH = 0x00;

  TIM1->ARRL = 0x11;
  TIM2->ARRL = 0x22;
  TIM3->ARRL = 0x33;
  TIM4->ARR  = 0x44;
}

/**
  * @brief  Init Timers DMA requests
  * @param  None.
  * @retval None
  */
void Init_TimersDMARequestSources(void)
{
  TIM1_DMACmd(TIM1_DMASource_Update, ENABLE);
  TIM2_DMACmd(TIM2_DMASource_Update, ENABLE);
  TIM3_DMACmd(TIM3_DMASource_Update, ENABLE);
  TIM4_DMACmd(TIM4_DMASource_Update, ENABLE);
}

/**
  * @brief  Enable Timers.
  * @param  None.
  * @retval None.
  */
void Enable_Timers(void)
{
  TIM1_Cmd(ENABLE);
  TIM2_Cmd(ENABLE);
  TIM3_Cmd(ENABLE);
  TIM4_Cmd(ENABLE);
}


/**
  * @brief  Configure DMA Channels.
  * @param  None.
  * @retval None
  */
void Init_DMAChannels(void)
{

  /* channel0 : from RAMBuffer2 to TIM3_ARRL*/
  DMA_Init( DMA1_Channel0, RAM_Buffer2_ADDRESS, TIM3_ARRL_ADDRESS,
            1/* DMA_BufferSize*/, DMA_DIR_MemoryToPeripheral,
            DMA_Mode_Normal, DMA_MemoryIncMode_Inc,
            DMA_Channel0_Priority, DMA_MemoryDataSize_Byte );

  /* channel1 : from RAMBuffer1 to TIM2_ARRL*/
  DMA_Init( DMA1_Channel1, RAM_Buffer1_ADDRESS, TIM2_ARRL_ADDRESS,
            1/*DMA_BufferSize*/, DMA_DIR_MemoryToPeripheral,
            DMA_Mode_Normal, DMA_MemoryIncMode_Inc,
            DMA_Channel1_Priority, DMA_MemoryDataSize_Byte );

  /* channel2 : from TIM1_ARRL to RAMBuffer1*/
  DMA_Init( DMA1_Channel2, RAM_Buffer1_ADDRESS, TIM1_ARRL_ADDRESS,
            1/*DMA_BufferSize*/, DMA_DIR_PeripheralToMemory,
            DMA_Mode_Normal, DMA_MemoryIncMode_Inc,
            DMA_Channel2_Priority, DMA_MemoryDataSize_Byte );

  /* channel3 : from TIM4_ARR to RAMBuffer2*/
  DMA_Init( DMA1_Channel3, RAM_Buffer2_ADDRESS, TIM4_ARR_ADDRESS,
            1/*DMA_BufferSize*/, DMA_DIR_PeripheralToMemory,
            DMA_Mode_Normal, DMA_MemoryIncMode_Inc,
            DMA_Channel3_Priority, DMA_MemoryDataSize_Byte );

}

/**
  * @brief  Enable DMA Channels.
  * @param  None.
  * @retval None
  */
void Enable_DMAchannels(void)
{
  /* Enable DMA1 channel 0*/
  DMA_Cmd(DMA1_Channel0, ENABLE);

  /* Enable DMA1 channel 1*/
  DMA_Cmd(DMA1_Channel1, ENABLE);

  /* Enable DMA1 channel 2*/
  DMA_Cmd(DMA1_Channel2, ENABLE);

  /* Enable DMA1 channel 3*/
  DMA_Cmd(DMA1_Channel3, ENABLE);
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
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
