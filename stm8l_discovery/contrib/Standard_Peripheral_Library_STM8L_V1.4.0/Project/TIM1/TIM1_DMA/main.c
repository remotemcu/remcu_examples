/**
  ******************************************************************************
  * @file    TIM1/TIM1_DMA/main.c
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

/** @addtogroup TIM1_DMA
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define TIM1_CCR1_ADDRESS       0x52C6
#define TIM1_PERIOD               4095
#define TIM1_REPTETION_COUNTER       2
#define TIM1_PRESCALER               0
#define BUFFERSIZE                   3

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint16_t SrcBuffer[3] = {2049, 1023, 511};

/* Private function prototypes -----------------------------------------------*/
static void CLK_Config(void);
static void GPIO_Config(void);
static void DMA_Config(void);
static void TIM1_Config(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void main(void)
{
   /* CLK configuration -------------------------------------------*/
  CLK_Config(); 

   /* GPIO configuration -------------------------------------------*/
  GPIO_Config(); 

   /* DMA configuration -------------------------------------------*/
  DMA_Config();
  
   /* TIM1 configuration -------------------------------------------*/
  TIM1_Config();

  /* Infinite loop */
  while (1);
}

/**
  * @brief  Configure peripherals Clock   
  * @param  None
  * @retval None
  */
static void CLK_Config(void)
{
  /* Clock prescaler: 1 */
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);

  /* Enable TIM1 clock */
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM1, ENABLE);
  /* Enable DMA1 clock */
  CLK_PeripheralClockConfig(CLK_Peripheral_DMA1, ENABLE);
}

/**
  * @brief  Configure TIM Channels GPIO 
  * @param  None
  * @retval None
  */
static void GPIO_Config(void)
{
  /* GPIOD configuration: TIM1 channel 1 (PD2) */
  GPIO_Init(GPIOD, GPIO_Pin_2, GPIO_Mode_Out_PP_Low_Fast);
}

/**
  * @brief  Configure DMA peripheral 
  * @param  None
  * @retval None
  */
static void DMA_Config(void)
{
  /* DMA1 configuration */
  DMA_Init(DMA1_Channel2, (uint16_t)SrcBuffer, TIM1_CCR1_ADDRESS, BUFFERSIZE,
           DMA_DIR_MemoryToPeripheral, DMA_Mode_Circular, DMA_MemoryIncMode_Inc,
           DMA_Priority_High, DMA_MemoryDataSize_HalfWord);

  /* DMA1 global enable */
  DMA_GlobalCmd(ENABLE);

  /* DMA1 Channel2 enable */
  DMA_Cmd(DMA1_Channel2, ENABLE);
}

/**
  * @brief  Configure TIM1 peripheral 
  * @param  None
  * @retval None
  */
static void TIM1_Config(void)
{
  /* Time base configuration */
  TIM1_TimeBaseInit(TIM1_PRESCALER, TIM1_CounterMode_Up, TIM1_PERIOD, TIM1_REPTETION_COUNTER);

  /* PWM2 Mode configuration: Channel1 */
  TIM1_OC1Init(TIM1_OCMode_PWM2, TIM1_OutputState_Enable, TIM1_OutputNState_Disable,
               SrcBuffer[0], TIM1_OCPolarity_Low, TIM1_OCNPolarity_Low,
               TIM1_OCIdleState_Set, TIM1_OCNIdleState_Set);
  TIM1_OC1PreloadConfig(DISABLE);

  /* TIM1 Update DMA Request enable */
  TIM1_DMACmd(TIM1_DMASource_Update, ENABLE);

  /* Main Output Enable */
  TIM1_CtrlPWMOutputs(ENABLE);

  /* Enable TIM1 */
  TIM1_Cmd(ENABLE);
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
