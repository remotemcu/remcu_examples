/**
  ******************************************************************************
  * @file    DAC/DAC_NoiseTriangleGenerator/main.c
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
#include "stm8_eval.h"

/** @addtogroup STM8L15x_StdPeriph_Examples
  * @{
  */

/** @addtogroup DAC_NoiseTriangleGenerator
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void DAC_Config(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void main(void)
{
  /* DAC configuration -------------------------------------------*/
  DAC_Config();
  
  /* Init Eval Board used Buttons*/
  STM_EVAL_PBInit(BUTTON_RIGHT, BUTTON_MODE_EXTI);
  STM_EVAL_PBInit(BUTTON_LEFT, BUTTON_MODE_EXTI);

  /* Enable Interrupts */
  enableInterrupts();

  /* Infinite loop */
  while(1)
  {
    /* Enable Software Trigger for Both DAC channels*/
    DAC_DualSoftwareTriggerCmd(ENABLE);
  }
}

/**
  * @brief  Configure DAC peripheral
  * @param  None
  * @retval None
  */
static void DAC_Config(void)
{
  /* Enable the Clock of DAC */
  CLK_PeripheralClockConfig(CLK_Peripheral_DAC, ENABLE);
  
  DAC_DeInit();

  /* Init DAC channel 1 and DAC channel 2 outputs */
  DAC_Init(DAC_Channel_1, DAC_Trigger_Software, DAC_OutputBuffer_Enable);
  DAC_Init(DAC_Channel_2, DAC_Trigger_Software, DAC_OutputBuffer_Enable);

  /* DAC channel 1 Triangle wave generator enabled */
  DAC_WaveGenerationCmd(DAC_Channel_1, DAC_Wave_Triangle, ENABLE);

  /* DAC channel 1 Noise  wave generator enabled */
  DAC_WaveGenerationCmd(DAC_Channel_2, DAC_Wave_Noise, ENABLE);

  /* Select DAC channel 1 Triangle wave generator amplitude  */
  DAC_SetTriangleWaveAmplitude(DAC_Channel_1, DAC_TriangleAmplitude_4095);

  /* Select DAC channel 2 Mask  */
  DAC_SetNoiseWaveLFSR(DAC_Channel_2, DAC_LFSRUnmask_Bits11_0);

  /* Enable DAC channel 1 and 2 outputs  */
  DAC_Cmd(DAC_Channel_1, ENABLE);
  DAC_Cmd(DAC_Channel_2, ENABLE);

  DAC_SetDualChannelData(DAC_Align_12b_R, 0, 0);
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
