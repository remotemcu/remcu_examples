/**
  ******************************************************************************
  * @file    DMA/DMA_ADCTIMTransfer/main.c  
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

/** @addtogroup DMA_ADCTIMTransfer
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define ADC1_DR_ADDRESS     0x40012440
#define TIM3_CCR1_ADDRESS   0x40000434

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void ADC_Config(void);
static void DMA_Config(void);
static void TIM_Config(void);

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

  /* DMA1 channel3 configuration */
  DMA_Config();

  /* TIM3 channel1 configuration */
  TIM_Config();

  /* ADC channel11 configuration */
  ADC_Config();

  while (1)
  {
  }
}

/**
  * @brief  Configures DMA1 channel3 to transfer data from 
  *                    ADC1_DR_ADDRESS to TIM3_CCR1_ADDRESS
  * @param  None
  * @retval None
  */
static void DMA_Config(void)
{
  DMA_InitTypeDef           DMA_InitStructure;
  
  /* Enable DMA1 clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

  DMA_DeInit(DMA1_Channel3);
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)TIM3_CCR1_ADDRESS;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADC1_DR_ADDRESS;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
  DMA_InitStructure.DMA_BufferSize = 1;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel3, &DMA_InitStructure);

  /* Enable DMA1 Channel3 */
  DMA_Cmd(DMA1_Channel3, ENABLE);
}

/**
  * @brief  Configures the ADC1 channel11 in continuous mode.
  * @param  None
  * @retval None
  */
static void ADC_Config(void)
{
  ADC_InitTypeDef           ADC_InitStructure;
  GPIO_InitTypeDef          GPIO_InitStructure;
  /* Enable the HSI */
  RCC_HSICmd(ENABLE);

  /* Enable the GPIOC Clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

  /* Configure ADC Channel11 (PC.01) as analog input */
#ifdef USE_STM320518_EVAL
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 ;
#else
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 ;/* Configure ADC Channel10 as analog input */
#endif /* USE_STM320518_EVAL */
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStructure);  

  /* Check that HSI oscillator is ready */
  while( RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);

  ADC_StructInit(&ADC_InitStructure);

  /* Enable ADC1 clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Upward;
  ADC_Init(ADC1, &ADC_InitStructure);

  /* ADC1 regular channel11 configuration */ 
#ifdef USE_STM320518_EVAL
  ADC_ChannelConfig(ADC1, ADC_Channel_11,ADC_SampleTime_28_5Cycles);
#else
  /* ADC1 regular channel10 configuration */ 
  ADC_ChannelConfig(ADC1, ADC_Channel_10,ADC_SampleTime_28_5Cycles);   
#endif /* USE_STM320518_EVAL */

  /* Enable the ADC peripheral */
  ADC_Cmd(ADC1, ENABLE);     
  
  /* Wait the ADRDY flag */
  while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADRDY)); 

  /* Enable the wait conversion mode */
  ADC_WaitModeCmd(ADC1, ENABLE);

  /* Start ADC1 Software Conversion */
  ADC_StartOfConversion(ADC1);
}

/**
  * @brief  Configures the TIM3 channel1 in PWM mode
  * @param  None
  * @retval None
  */
static void TIM_Config(void)
{
  TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
  TIM_OCInitTypeDef         TIM_OCInitStructure;
  GPIO_InitTypeDef          GPIO_InitStructure;
  
  /* Enable GPIOA clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

  /* GPIOA Configuration: PA6(TIM3 CH1) as alternate function push-pull */
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_1);

  /* Enable TIM3 clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  /* Time Base configuration */
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Period = 0xFF0;
  TIM_TimeBaseStructure.TIM_Prescaler = 0x0;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  /* Channel1 Configuration in PWM mode */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0xf0;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OC1Init(TIM3, &TIM_OCInitStructure);

  /* Enable TIM3 DMA interface */
  TIM_DMACmd(TIM3, TIM_DMA_Update, ENABLE);

  /* Enable TIM3 */  
  TIM_Cmd(TIM3, ENABLE);
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
