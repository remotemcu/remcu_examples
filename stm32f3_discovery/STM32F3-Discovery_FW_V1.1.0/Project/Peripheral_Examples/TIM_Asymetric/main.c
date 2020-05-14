/**
  ******************************************************************************
  * @file    TIM_Asymetric/main.c 
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    20-September-2012
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
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

/** @addtogroup STM32F3_Discovery_Peripheral_Examples
  * @{
  */

/** @addtogroup TIM_Asymetric
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define INITIAL_PHASE         (uint32_t) 15000
#define INITIAL_LENGTH        (uint32_t) 5000
#define PWM_FREQUENCY         (uint32_t) 22000
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
TIM_OCInitTypeDef         TIM_OCInitStructure;
/* Private function prototypes -----------------------------------------------*/
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
       file (startup_stm32f30x.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f30x.c file
     */ 

  /* TIM Configuration */
  TIM_Config();

  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);

  TIM_OCStructInit(&TIM_OCInitStructure);

  /* ---------------------------------------------------------------------------
     TIM8 is configured to generate an Asymetric signal with a programmable 
	 Phase-Shifted signal on TIM8_CH2:
	 - TIM8 Channel 1 is configured in PWM2 mode
	 - TIM8 Channel 2 is configured in Asymetric PWM2 mode
	 - The counter mode is center aligned mode
	 - The pulse length and the phase shift are programmed consecutively in TIM8_CCR2 and TIM8_CCR1.

	 TIM1 is configured to generating the reference signal on Channel1 used by TIM8:
	  - TIM1 is generating a PWM signal with frequency equal to 1.5KHz
	  - TIM1 is used as master for TIM8, the update event of TIM1 genarates the Reset counter
	    of TIM8 to synchronize the two signals: the reference signal (TIM1_CH1) and 
            the shifted signal (TIM8_CH2). 
	   
    In this example TIM1 and TIM8 input clock (TIM18CLK) is set to APB2 clock (PCLK2)    
    TIM1 and TIM8 signals are at frequency of  (SystemCoreClock / (PWM_FREQUENCY + 1))  
               
    TIM8 is gerating a signal with the following caracteristics:
	  - Pulse lenght = (TIM8_CCR1 + TIM8_CCR2) / TIM8_CLK
	  - Phase shift = TIM8_CCR1/TIM8_CLK
	  with TIM8_CLK = (SystemCoreClock / (Period + 1)), as the prescaler is equal to zero.
	
    Note: 
     SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f30x.c file.
     Each time the core clock (HCLK) changes, user had to call SystemCoreClockUpdate()
     function to update SystemCoreClock variable value. Otherwise, any configuration
     based on this variable will be incorrect.    
  --------------------------------------------------------------------------- */ 
  /* Initialize Timers: TIM1 & TIM8 */  
  /* Time base configuration for TIM8 and TIM1 */
  TIM_TimeBaseStructure.TIM_Period = PWM_FREQUENCY;
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;

  /* First wrong parameter: Counting Mode,TIM8 counting direction has to be set to center-aligned mode */
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_CenterAligned1; 
  
  TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);
  
  TIM_TimeBaseStructure.TIM_Period = 2 * PWM_FREQUENCY;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

  /* Channels 1&2 configuration on TIM8 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = INITIAL_PHASE;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OC1Init(TIM8, &TIM_OCInitStructure); /* Channel1 config done */
  
  /* Second wrong parameter: PWM Mode */
  /* The same PWM mode has to be configured for the two coupled channels */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Asymmetric_PWM2;

  TIM_OCInitStructure.TIM_Pulse = INITIAL_LENGTH;
  TIM_OC2Init(TIM8, &TIM_OCInitStructure); /* Channel2 config done */
  
  /* Channel1 configuration on TIM1 */
  TIM_OCInitStructure.TIM_Pulse = PWM_FREQUENCY;
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OC1Init(TIM1, &TIM_OCInitStructure);

  /* Enable outputs on both TIM1 & TIM8*/
  TIM_CtrlPWMOutputs(TIM8, ENABLE);
  TIM_CtrlPWMOutputs(TIM1, ENABLE);

  /* Synchronization between TIM1 and TIM8
  The aim is to generate a reference signal on TIM1_CH1
  The Phase-Shifted siganl generated on TIM8_CH2 is compared to the reference
  signal 
  */
  TIM_SelectSlaveMode(TIM8, TIM_SlaveMode_Reset); /* Configure TIM8 in slave
  mode: an active edge on  trigger input generates a reset on tIM8 */
  
  TIM_SelectInputTrigger(TIM8, TIM_TS_ITR0); /* Connect TIM1 to TIM8
  TIM1 is the Master
  TIM8 is the Slave */
  
  TIM_SelectOutputTrigger(TIM1,TIM_TRGOSource_Update); /* Select the Update
  to be the Master Trigger-Out TRGO signal origine */
  
  TIM_CCPreloadControl(TIM8, ENABLE); /* Enable Shadaw register on TIM8 
  CCRx register are not accessed directly: their content is updated each Update
  event */
  
  /* TIM8 enable counter */
  TIM_Cmd(TIM1, ENABLE);
  TIM_Cmd(TIM8, ENABLE);

  /* Infinite loop */
  while (1)
  {
  }
}

/**
  * @brief  Configure the TIM IRQ Handler.
  * @param  None
  * @retval None
  */
static void TIM_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* TIM8 clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8 | RCC_APB2Periph_TIM1, ENABLE);
  
  /* GPIOA & GPIOB clock enable*/
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB, ENABLE);
  
  /* Configure AF mux to connect 
      TIM8_CH1 to PB6
      TIM8_CH2 to PB8 */
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_5);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_10);
  
  /* Configure AF mux to connect TIM1_CH1 to PA8 */
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_6);

  /* Configure PB6 and PB8 as Alternate Function */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  /* Configure PA8 as Alternate Function */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_Init(GPIOA, &GPIO_InitStructure); 
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
