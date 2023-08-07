/**
  ******************************************************************************
  * @file    IWDG/IWDG_Example/main.c
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

/** @addtogroup IWDG_Example
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define RELOAD_VALUE   254
#define LSI_PERIOD_NUMBERS         10
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint32_t LsiFreq = 0;
extern __IO uint16_t CaptureState;
extern __IO uint32_t Capture;
/* Private function prototypes -----------------------------------------------*/
static void CLK_Config(void);
static void GPIO_Config(void);
void IWDG_Config(void);
uint32_t LSIMeasurment(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void main(void)
{
  /* Clock Configuration */
  CLK_Config(); 
  
  /* GPIO Configuration */
  GPIO_Config();

  /* Check if the system has resumed from IWDG reset */
  if (RST_GetFlagStatus(RST_FLAG_IWDGF) != RESET)
  {
    /* IWDGF flag set */
    /* Turn on LED1 */
    STM_EVAL_LEDOn(LED1);

    /* Clear IWDGF Flag */
    RST_ClearFlag(RST_FLAG_IWDGF);
  }
  else
  {
    /* IWDGF flag is not set */
    /* Turn off LED1 */
    STM_EVAL_LEDOff(LED1);
  }

  /* Get measured LSI frequency */
  LsiFreq = LSIMeasurment();  
  
  /* IWDG Configuration */
  IWDG_Config();
  
  while (1)
  {
    /* Reload IWDG counter */
    IWDG_ReloadCounter();  
  }
}

/**
  * @brief  Configure system clock to run at 16MHz
  * @param  None
  * @retval None
  */
static void CLK_Config(void)
{
  /* High Speed Internal clock divider: 1 */
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);

  /* Enable LSI clock */
  CLK_LSICmd(ENABLE);
  
  /* Wait for LSI clock to be ready */
  while (CLK_GetFlagStatus(CLK_FLAG_LSIRDY) == RESET)
  {}

  /* Wait for BEEP switch busy flag to be reset */
  while (CLK_GetFlagStatus(CLK_FLAG_BEEPSWBSY) == SET)
  {}
  
  /* Select LSI clock as source for BEEP */
  CLK_BEEPClockConfig(CLK_BEEPCLKSource_LSI);

  /* Enable TIM2 clock */
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM2, ENABLE);
  
  /* Enable BEEP clock to get write access for BEEP registers */
  CLK_PeripheralClockConfig(CLK_Peripheral_BEEP, ENABLE);
}

/**
  * @brief  Configures the IWDG to generate a Reset if it is not refreshed at the
  *         correct time. 
  * @param  None
  * @retval None
  */
void IWDG_Config(void)
{
  /* Enable IWDG (the LSI oscillator will be enabled by hardware) */
  IWDG_Enable(); 
  
  /* IWDG timeout equal to 214 ms (the timeout may varies due to LSI frequency
     dispersion) */
  /* Enable write access to IWDG_PR and IWDG_RLR registers */
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
  
  /* IWDG configuration: IWDG is clocked by LSI = 38KHz */
  IWDG_SetPrescaler(IWDG_Prescaler_32);
  
  /* IWDG timeout equal to 214.7 ms (the timeout may varies due to LSI frequency dispersion) */
  /* IWDG timeout = (RELOAD_VALUE + 1) * Prescaler / LSI 
                  = (254 + 1) * 32 / 38 000 
                  = 214.7 ms */
  IWDG_SetReload((uint8_t)RELOAD_VALUE);
  
  /* Reload IWDG counter */
  IWDG_ReloadCounter();
}

/**
  * @brief  Initialize LED1 and Key Button mounted on the evaluation board
  * @param  None
  * @retval None
  */
static void GPIO_Config(void) 
{
  /* Configure LED1 */
  STM_EVAL_LEDInit(LED1);

  /* Configure Key button */
  STM_EVAL_PBInit(BUTTON_KEY, BUTTON_MODE_EXTI);
  
  /* enable interrupts */
  enableInterrupts();
}

/**
  * @brief  Measure the LSI frequency using timer IC1 and update the calibration registers.
  * @note   It is recommended to use a timer clock frequency of at least 10MHz in order 
  *         to obtain a better in the LSI frequency measurement.
  * @param  None
  * @retval None
  */
uint32_t LSIMeasurment(void)
{
  uint8_t icfilter = 0;
  uint32_t LSICurrentPeriod = 0;
  uint32_t LSIMeasuredFrequencyCumul = 0;
  uint16_t LSIMeasuredFrequency = 0;
  uint8_t LSIPeriodCounter = 0;

 /* Enable the LSI measurement: LSI clock connected to timer Input Capture 1 */
  BEEP_LSClockToTIMConnectCmd(ENABLE);
  
  /* TIM2 configuration: Input Capture mode */
  /* Configure TIM2 channel 1 in input capture mode */
  /* The signal in input is divided and not filtered */
  /* The capture occurs when a rising edge is detected on TIM2 channel 1 */
  TIM2_ICInit(TIM2_Channel_1, TIM2_ICPolarity_Rising, TIM2_ICSelection_DirectTI,
              TIM2_ICPSC_DIV8, icfilter);

  LSIPeriodCounter = 0;
  /**************************** START of LSI Measurement **********************/
  while (LSIPeriodCounter <= LSI_PERIOD_NUMBERS)
  {
    CaptureState = 1;
    /* Clear all TM2 flags */
    TIM2_GenerateEvent(TIM2_EventSource_Update);
    TIM2->SR1 = 0;
    TIM2->SR2 = 0;
    /* Enable capture 1 interrupt */
    TIM2_ITConfig(TIM2_IT_CC1, ENABLE);
    /* Enable TIM2 */
    TIM2_Cmd(ENABLE);

    while (CaptureState != 255);

    if (LSIPeriodCounter != 0)
    {
      /* Compute the frequency value */
      LSICurrentPeriod = (uint32_t) 8 * (HSI_VALUE / Capture);
      /* Add the current frequency to previous cumulation */
      LSIMeasuredFrequencyCumul = LSIMeasuredFrequencyCumul + LSICurrentPeriod;
    }
    LSIPeriodCounter++;
  }
  /**************************** END of LSI Measurement ************************/

  /* Compute the average of LSI frequency value */
  LSIMeasuredFrequency = (uint16_t) (LSIMeasuredFrequencyCumul / LSI_PERIOD_NUMBERS);
  
  /* Disconnect LSI clock from Timer 2 channel 1 */
  BEEP_LSClockToTIMConnectCmd(ENABLE);
  
  /* Return the LSI frequency */
  return (uint16_t)(LSIMeasuredFrequency);
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
