/**
  ******************************************************************************
  * @file    COMP/COMP2_FrequencyMeasurement/main.c
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

/** @addtogroup COMP2_FrequencyMeasurement
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void CLK_Config(void);
static void COMP_Config(void);
static void DAC_Config(void);
static void TIM2_Config(void);
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

  /* DAC configuration -------------------------------------------*/
  DAC_Config(); 

  /* COMP configuration -------------------------------------------*/
  COMP_Config(); 
    
  /* TIM2 configuration -------------------------------------------*/
  TIM2_Config(); 
  
  /* Enable global interrupts */
  enableInterrupts();

  /* Infinite loop */
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
  /* Enable COMP clock */
  CLK_PeripheralClockConfig(CLK_Peripheral_COMP, ENABLE);
  /* Enable TIM2 clock */
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM2, ENABLE);
  /* Enable DAC clock */
  CLK_PeripheralClockConfig(CLK_Peripheral_DAC, ENABLE);
}

/**
  * @brief  Configure DAC peripheral 
  * @param  None
  * @retval None
  */
static void DAC_Config(void)
{
  /* Init DAC channel 1 */
  DAC_Init(DAC_Channel_1, DAC_Trigger_None, DAC_OutputBuffer_Disable);
  /* Enable DAC channel 1 */
  DAC_Cmd(DAC_Channel_1, ENABLE);
  /* Set DAC Channel1 DHR register:
  * DAC1 output = VREF * DOR / 256 = 3.3 * 85 / 256 = 1V */
  DAC_SetChannel1Data(DAC_Align_8b_R, 85);
}

/**
  * @brief  Configure COMP peripheral 
  * @param  None
  * @retval None
  */
static void COMP_Config(void)
{
  /* Close I/O Switch 23 to select PD0 as comparator 2 non inverting input:
    * External signal should be connected to PD0 */
  SYSCFG_RIIOSwitchConfig(RI_IOSwitch_23, ENABLE);

  /* Init COMP2: DAC1 output is used as COMP2 inverting input
                COMP2 output is connected to TIM2 input capture 2
                COMP2 speed is fast */
  COMP_Init(COMP_InvertingInput_DAC1, COMP_OutputSelect_TIM2IC2, COMP_Speed_Fast);
  /* COMP2 edge detection: rising edge */
  COMP_EdgeConfig(COMP_Selection_COMP2, COMP_Edge_Rising);
}

/**
  * @brief  Configure TIM2 peripheral 
  * @param  None
  * @retval None
  */
static void TIM2_Config(void)
{
  /* configure TIM2 channel 2 to capture a PWM signal */
  TIM2_PWMIConfig(TIM2_Channel_2, TIM2_ICPolarity_Rising, TIM2_ICSelection_DirectTI,
                  TIM2_ICPSC_DIV1, 0);

  /* Select the TIM2 Input Trigger: TI2FP2 */
  TIM2_SelectInputTrigger(TIM2_TRGSelection_TI2FP2);
  /* Select slave reset mode */
  TIM2_SelectSlaveMode(TIM2_SlaveMode_Reset);

  /* Enable capture 2 interrupt */
  TIM2_ITConfig(TIM2_IT_CC2, ENABLE);

  /* Enable TIM2 counter */
  TIM2_Cmd(ENABLE);
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
