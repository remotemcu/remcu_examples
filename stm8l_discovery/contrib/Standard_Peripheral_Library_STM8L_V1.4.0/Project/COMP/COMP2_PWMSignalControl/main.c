/**
  ******************************************************************************
  * @file    COMP/COMP2_PWMSignalControl/main.c
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

/** @addtogroup COMP2_PWMSignalControl
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define TIM1_PERIOD                  65535
#define TIM1_PRESCALER                   0
#define TIM1_REPTETION_COUNTER           0

/* Channel 1 duty cycle = 100 * CCR1_VAL / TIM1_PERIOD + 1
                        = 32767 * 100 / (65535 + 1) = 50 %*/
#define CCR1_VAL                     32767

/* Channel 2 duty cycle = 100 * CCR2_VAL / TIM1_PERIOD + 1
                        = 16383 * 100 / (65535 + 1) = 25 %*/
#define CCR2_VAL                     16383

/* Channel 3 duty cycle = 100 * CCR3_VAL / TIM1_PERIOD + 1
                        = 8191 * 100 / (65535 + 1) = 12.5 %*/
#define CCR3_VAL                      8191

#define DEADTIME                       117

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void CLK_Config(void);
static void COMP_Config(void);
static void DAC_Config(void);
static void GPIO_Config(void);
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

  /* TIM1 configuration -------------------------------------------*/
  TIM1_Config(); 
  
  /* DAC configuration -------------------------------------------*/
  DAC_Config(); 

  /* COMP configuration -------------------------------------------*/
  COMP_Config(); 

  /* TIM1 counter enable */
  TIM1_Cmd(ENABLE);

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
  /* Enable TIM1 clock */
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM1, ENABLE);
  /* Enable DAC clock */
  CLK_PeripheralClockConfig(CLK_Peripheral_DAC, ENABLE);
}

/**
  * @brief  Configure GPIO for TIM1
  * @param  None
  * @retval None
  */
static void GPIO_Config(void)
{
  /* TIM1 Channels 1, 2, 3 and 1N configuration: PD2, PD4, PD5 and PD7 */
  GPIO_Init(GPIOD, GPIO_Pin_2 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7, GPIO_Mode_Out_PP_Low_Fast);

  /* TIM1 Channels 2N and 3N configuration: PE1 and PE2 */
  GPIO_Init(GPIOE, GPIO_Pin_1 | GPIO_Pin_2, GPIO_Mode_Out_PP_Low_Fast);
}

/**
  * @brief  Configure TIM1 peripheral
  * @param  None
  * @retval None
  */
static void TIM1_Config(void)
{
  /* TIM1 configuration:
   - TIM1CLK is set to 2 MHz, the TIM2 Prescaler is equal to 0 so the TIM1 counter
   clock used is 2 MHz / (0+1) = 2 MHz
   - Channels output frequency = TIM1CLK / (TIM1_PERIOD + 1) * (TIM1_PRESCALER + 1)
                = 2 000 000 / 65536 * 1 = 30.51 Hz             */
  /* Time Base configuration */
  TIM1_TimeBaseInit(TIM1_PRESCALER, TIM1_CounterMode_Up, TIM1_PERIOD, TIM1_REPTETION_COUNTER);

  /* Channels 1, 1N, 2, 2N, 3 and 3N Configuration in PWM2 mode */
  TIM1_OC1Init(TIM1_OCMode_PWM2, TIM1_OutputState_Enable, TIM1_OutputNState_Enable, CCR1_VAL,
               TIM1_OCPolarity_Low, TIM1_OCNPolarity_Low, TIM1_OCIdleState_Set, TIM1_OCNIdleState_Set);
  TIM1_OC2Init(TIM1_OCMode_PWM2, TIM1_OutputState_Enable, TIM1_OutputNState_Enable, CCR2_VAL,
               TIM1_OCPolarity_Low, TIM1_OCNPolarity_Low, TIM1_OCIdleState_Set, TIM1_OCNIdleState_Set);
  TIM1_OC3Init(TIM1_OCMode_PWM2, TIM1_OutputState_Enable, TIM1_OutputNState_Enable, CCR3_VAL,
               TIM1_OCPolarity_Low, TIM1_OCNPolarity_Low, TIM1_OCIdleState_Set, TIM1_OCNIdleState_Set);

  /* Automatic Output Enable, Break, dead time and lock configuration
  Enable the break feature with polarity high.
    Non-inverted Channels and inverted channels rising edge are delayed 
    by 58.5 µs = DEADTIME / TIM1CLK = 117 / 2 MHz
    As the lock level is level 1, Channels Idle state, break enable and polarity 
    bits can no longer be written.
  */
  TIM1_BDTRConfig(TIM1_OSSIState_Enable, TIM1_LockLevel_1, DEADTIME,
                  TIM1_BreakState_Enable, TIM1_BreakPolarity_High, TIM1_AutomaticOutput_Enable);

  /* TIM1 Main Output Enable */
  TIM1_CtrlPWMOutputs(ENABLE);
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
  * DAC1 output = VREF * DOR / 256 = 3.3 * 116 / 256 = 1.5 V */
  DAC_SetChannel1Data(DAC_Align_8b_R, 116);

  /* Close I/O Switch 24 to select PE5 as comparator 2 non inverting input:
    * External signal should be connected to PE5 */
  SYSCFG_RIIOSwitchConfig(RI_IOSwitch_24, ENABLE);
}

/**
  * @brief  Configure COMP peripheral 
  * @param  None
  * @retval None
  */
static void COMP_Config(void)
{
  /* Init COMP2: DAC1 output is used as COMP2 inverting input
                COMP2 output is connected to TIM1 break
                COMP2 speed is fast */
  /* If non inverting input exceeds inverting input (input voltage applyed on PE5
   exceeds DAC1 output) COMP2 output is set at high level and since COMP2 output
    is connected to break input, the non-inverted channels and inverted channels
  of TIM1 switch to high level (TIM1_OCIdleState_Set, TIM1_OCNIdleState_Set).
    Once COMP2 output is at low level, the non-inverted channels and inverted 
  channels return to PWM state at the next update (TIM1_AutomaticOutput_Enable).*/
  COMP_Init(COMP_InvertingInput_DAC1, COMP_OutputSelect_TIM1BRK, COMP_Speed_Fast);

  /* COMP2 edge detection: rising edge */
  COMP_EdgeConfig(COMP_Selection_COMP2, COMP_Edge_Rising);
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
