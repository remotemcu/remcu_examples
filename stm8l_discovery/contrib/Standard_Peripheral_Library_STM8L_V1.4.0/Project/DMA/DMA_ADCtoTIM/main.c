/**
  ******************************************************************************
  * @file    DMA/DMA_ADCtoTIM/main.c
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

/** @addtogroup DMA_ADCtoTIM
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define ADC1_DR_ADDRESS      ((uint16_t)(ADC1_BASE + 0x04))
#define TIM1_CCR1_ADDRESS    ((uint16_t)(TIM1_BASE + 0x16))
#define BUFFER_SIZE          ((uint8_t) 0x01)
#define BUFFER_ADDRESS       ((uint16_t)(&Buffer))

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint16_t Buffer = 0;

/* Private function prototypes -----------------------------------------------*/
static void CLK_Config(void);
static void ADC_Config(void);
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

  /* ADC configuration -------------------------------------------*/
  ADC_Config(); 

  /* DMA configuration -------------------------------------------*/
  DMA_Config(); 

  /* TIM1 configuration -------------------------------------------*/
  TIM1_Config(); 

  /* Enable ADC1 DMA requests*/
  ADC_DMACmd(ADC1, ENABLE);

  /* Enable TIM1 DMA requests*/
  TIM1_DMACmd(TIM1_DMASource_Update, ENABLE);

  /* Start ADC1 Conversion using Software trigger*/
  ADC_SoftwareStartConv(ADC1);

  while (1)
  {}
}

/**
  * @brief  Configure peripherals clock 
  * @param  None
  * @retval None
  */
static void CLK_Config(void)
{
  /* Enable ADC1 clock */
  CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, ENABLE);

  /* Enable TIM1 clock */
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM1, ENABLE);

  /* Enable DMA1 clock */
  CLK_PeripheralClockConfig(CLK_Peripheral_DMA1, ENABLE);
}

/**
  * @brief  Configure ADC peripheral
  * @param  None
  * @retval None
  */
static void ADC_Config(void)
{
  /* Initialise and configure ADC1 */
  ADC_Init(ADC1, ADC_ConversionMode_Continuous, ADC_Resolution_12Bit, ADC_Prescaler_2);
  ADC_SamplingTimeConfig(ADC1, ADC_Group_SlowChannels, ADC_SamplingTime_384Cycles);

  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);

  /* Enable ADC1 Channels 3 */
  ADC_ChannelCmd(ADC1, ADC_Channel_3, ENABLE); /* connected to potentiometer */
}

/**
  * @brief  Configure DMA peripheral
  * @param  None
  * @retval None
  */
static void DMA_Config(void)
{
  /* Connect ADC1 to DMA1 channel 0 */
  SYSCFG_REMAPDMAChannelConfig(REMAP_DMA1Channel_ADC1ToChannel0);
  DMA_Init(DMA1_Channel0, BUFFER_ADDRESS, ADC1_DR_ADDRESS, BUFFER_SIZE, DMA_DIR_PeripheralToMemory,
           DMA_Mode_Circular, DMA_MemoryIncMode_Inc, DMA_Priority_High, DMA_MemoryDataSize_HalfWord);

  /* Connect DMA1 channel 2 to TIM1 */
  DMA_Init(DMA1_Channel2, BUFFER_ADDRESS, TIM1_CCR1_ADDRESS, BUFFER_SIZE, DMA_DIR_MemoryToPeripheral,
           DMA_Mode_Circular, DMA_MemoryIncMode_Inc, DMA_Priority_High, DMA_MemoryDataSize_HalfWord);

  /* DMA1 Channel0 enable */
  DMA_Cmd(DMA1_Channel0, ENABLE);

  /* DMA1 Channel2 enable */
  DMA_Cmd(DMA1_Channel2, ENABLE);

  /* DMA1 enable */
  DMA_GlobalCmd(ENABLE);
}

/**
  * @brief  Configure TIM1 peripheral
  * @param  None
  * @retval None
  */
static void TIM1_Config(void)
{
  /* configure TIM1 channel 1 as PWM Output */
  TIM1_OC1Init(TIM1_OCMode_PWM1, TIM1_OutputState_Enable, TIM1_OutputNState_Disable,
               0x7FF/* TIM1_Pulse */, TIM1_OCPolarity_Low, TIM1_OCNPolarity_Low,
               TIM1_OCIdleState_Reset, TIM1_OCNIdleState_Reset);

  /* Set TIM1 Autoreload value*/
  TIM1_SetAutoreload(0xFFF);

  /* Enable TIM1 */
  TIM1_Cmd(ENABLE);

  /* GPIO configuration: TIM1 channel 1 (PD2)*/
  GPIO_Init(GPIOD, GPIO_Pin_2 , GPIO_Mode_Out_PP_Low_Fast);

  /* Enable TIM1 Outputs*/
  TIM1_CtrlPWMOutputs(ENABLE);  
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
