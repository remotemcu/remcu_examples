/**
  ******************************************************************************
  * @file    TIM1/TIM1_Synchro/main.c
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

/** @addtogroup TIM1_Synchro
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define TIM1_PERIOD                        255
#define TIM1_PRESCALER                      15
#define TIM1_REPETITION_COUNTER               4

/* TIM1 Channel 1 duty cycle = 100 * TIM1_CCR1_VAL / TIM1_PERIOD + 1
                             = 100 * 127 / (255 + 1) = 50 %*/
#define TIM1_CCR1_VAL                      127

/* TIM2 Channel 1 duty cycle = 100 * TIM2_CCR1_VAL / TIM2_PERIOD + 1
                             = 100 * 1 / (2 + 1) = 33.33 %*/
#define TIM2_CCR1_VAL                        1
#define TIM2_PERIOD                          2

/* TIM3 Channel 1 duty cycle = 100 * TIM3_CCR1_VAL / TIM3_PERIOD + 1
                             = 100 * 1 / (3 + 1) = 25 %*/
#define TIM3_CCR1_VAL                        1
#define TIM3_PERIOD                          3

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void CLK_Config(void);
static void GPIO_Config(void);
static void TIM_Config(void);
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
  
   /* TIM configuration -------------------------------------------*/
  TIM_Config();

  while (1)
  {}
}

/**
  * @brief  Configure peripherals Clock   
  * @param  None
  * @retval None
  */
static void CLK_Config(void)
{
  /* Enable TIM1 clock */
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM1, ENABLE);
  /* Enable TIM2 clock */
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM2, ENABLE);
  /* Enable TIM3 clock */
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM3, ENABLE);
}

/**
  * @brief  Configure TIM Channels GPIO 
  * @param  None
  * @retval None
  */
static void GPIO_Config(void)
{
  /* TIM1 Channel 1 configuration: PD2 */
  GPIO_Init(GPIOD, GPIO_Pin_2, GPIO_Mode_Out_PP_Low_Fast);

  /* TIM2 Channel 1 and TIM3 Channel 1 configuration: PB0 and PB1 */
  GPIO_Init(GPIOB, GPIO_Pin_0 | GPIO_Pin_1, GPIO_Mode_Out_PP_Low_Fast);
}

/**
  * @brief  Configure TIM1,2,3 peripherals 
  * @param  None
  * @retval None
  */
static void TIM_Config(void)
{
  /* TIM1 configuration:
   - TIM1CLK is set to 2 MHz, the TIM2 Prescaler is equal to 15 so the TIM1 counter
   clock used is 2 MHz / (15 + 1) = 125 000 Hz
   - TIM1 Channel 1 output frequency = TIM1CLK / (TIM1_PERIOD + 1) * (TIM1_PRESCALER + 1)
                                    = 2 000 000 / 256 * 16 = 488.28 Hz */
  /* Time Base configuration */
  TIM1_TimeBaseInit(TIM1_PRESCALER, TIM1_CounterMode_Up, TIM1_PERIOD, TIM1_REPETITION_COUNTER);

  /* TIM1 Channel 1 Configuration in PWM2 mode */
  TIM1_OC1Init(TIM1_OCMode_PWM2, TIM1_OutputState_Enable, TIM1_OutputNState_Disable, TIM1_CCR1_VAL,
               TIM1_OCPolarity_Low, TIM1_OCNPolarity_Low, TIM1_OCIdleState_Set, TIM1_OCNIdleState_Set);

  /* Master Mode selection: Update event */
  TIM1_SelectOutputTrigger(TIM1_TRGOSource_Update);

  /* TIM2 configuration:
   - TIM2 is connected to TIM1 Update so TIM2CLK is equal to 
     TIM1 output clock / (TIM1_REPETITION_COUNTER + 1) = 488.28 / 5 = 97.65 Hz
    - TIM2 Prescaler is equal to 1 so the TIM2 counter clock used is 97.65 / 1 = 97.65 Hz
    - TIM2 Channel 1 output frequency = TIM2CLK / (TIM2_PERIOD + 1) * TIM2_Prescaler
                                     = 97.65 / 3 * 1 = 32.55 Hz */
  /* Time Base configuration */
  TIM2_TimeBaseInit(TIM2_Prescaler_1, TIM2_CounterMode_Up, TIM2_PERIOD);

  TIM2_OC1Init(TIM2_OCMode_PWM2, TIM2_OutputState_Enable, TIM2_CCR1_VAL,
               TIM2_OCPolarity_Low, TIM2_OCIdleState_Reset);

  /* TIM2 Slave Mode selection: Gated mode */
  TIM2_SelectSlaveMode(TIM2_SlaveMode_Gated);
  TIM2_SelectInputTrigger(TIM2_TRGSelection_TIM1);

  /* TIM3 configuration:
   - TIM3 is connected to TIM1 Update so TIM3CLK is equal to 
     TIM1 output clock / (TIM1_REPETITION_COUNTER + 1) = 488.28 / 5 = 97.65 Hz
    - TIM3 Prescaler is equal to 1 so the TIM3 counter clock used is 97.65 / 1 = 97.65 Hz
    - TIM3 Channel 1 output frequency = TIM3CLK / (TIM3_PERIOD + 1) * TIM3Prescaler
                                     = 97.65 / 4 * 1 = 24.41 Hz */
  /* Time Base configuration */
  TIM3_TimeBaseInit(TIM3_Prescaler_1, TIM3_CounterMode_Up, TIM3_PERIOD);

  TIM3_OC1Init(TIM3_OCMode_PWM2, TIM3_OutputState_Enable, TIM3_CCR1_VAL,
               TIM3_OCPolarity_Low, TIM3_OCIdleState_Reset);

  /* TIM3 Slave Mode selection: Gated mode */
  TIM3_SelectSlaveMode(TIM3_SlaveMode_Gated);
  TIM3_SelectInputTrigger(TIM3_TRGSelection_TIM1);

  /* Main Output Enable */
  TIM1_CtrlPWMOutputs(ENABLE);
  TIM2_CtrlPWMOutputs(ENABLE);
  TIM3_CtrlPWMOutputs(ENABLE);

  /* TIM2 counter enable */
  TIM2_Cmd(ENABLE);
  /* TIM3 counter enable */
  TIM3_Cmd(ENABLE);
  /* TIM1 counter enable */
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
