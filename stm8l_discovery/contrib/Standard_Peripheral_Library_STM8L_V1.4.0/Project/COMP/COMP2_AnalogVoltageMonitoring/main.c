/**
  ******************************************************************************
  * @file    COMP/COMP2_AnalogVoltageMonitoring/main.c
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
#include "main.h"


/** @addtogroup STM8L15x_StdPeriph_Examples
  * @{
  */

/** @addtogroup COMP2_AnalogVoltageMonitoring
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint16_t ADCVal = 0;
__IO uint8_t State = STATE_OVER_THRESHOLD;
/* Private function prototypes -----------------------------------------------*/
static void CLK_Config(void);
static void COMP_Config(void);
static void ADC_Config(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void main(void)
{
  /*************** Initialize LEDs available on STM8L15X-EVAL board ***********/
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED2);

  /* CLK configuration -------------------------------------------*/
  CLK_Config(); 
  
  /* ADC configuration -------------------------------------------*/
  ADC_Config(); 

  /* COMP configuration -------------------------------------------*/
  COMP_Config(); 

  while (1)
  {
    if (State != STATE_UNDER_THRESHOLD) /* Input voltage is over the threshold VREFINT */
    {
      /* LD1 ON and LD2 OFF: MCU in run mode */
      STM_EVAL_LEDOn(LED1);
      STM_EVAL_LEDOff(LED2);

      /* Disable global Interrupts */
      disableInterrupts();
      /* Disable COMP clock */
      CLK_PeripheralClockConfig(CLK_Peripheral_COMP, DISABLE);

      /* Enable ADC1 clock */
      CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, ENABLE);
      /* Enable end of conversion ADC1 Interrupt */
      ADC_ITConfig(ADC1, ADC_IT_EOC, DISABLE);
      /* Enable ADC1 */
      ADC_Cmd(ADC1, ENABLE);

      /* Start ADC1 Software Conversion */
      ADC_SoftwareStartConv(ADC1);
      /* Wait for first end of conversion */
      while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
      ADCVal = ADC_GetConversionValue(ADC1);
      /* Enable global Interrupts */
      enableInterrupts();

      /* Enable end of conversion ADC1 Interrupt */
      ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);

      while (State == STATE_OVER_THRESHOLD)
      {}
    }
    else /* Input voltage is under the threshold */
    {
      /* LD1 OFF and LD2 ON: MCU in halt mode */
      STM_EVAL_LEDOff(LED1);
      STM_EVAL_LEDOn(LED2);

      /* Disable global Interrupts */
      disableInterrupts();
      /* Clear EOC and OVR flags */
      ADC_ClearFlag(ADC1, (ADC_FLAG_TypeDef) (ADC_FLAG_EOC | ADC_FLAG_OVER));
      /* Disable ADC1 */
      ADC_Cmd(ADC1, DISABLE);
      /* Disable ADC1 clock */
      CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, DISABLE);

      /* Enable COMP clock */
      CLK_PeripheralClockConfig(CLK_Peripheral_COMP, ENABLE);
      /* Enable COMP2 Interrupt */
      COMP_ITConfig(COMP_Selection_COMP2, ENABLE);

      /* Enable global Interrupts */
      enableInterrupts();
      /* Check COMP2 output level before entering halt mode */
      if (COMP_GetOutputLevel(COMP_Selection_COMP2) == COMP_OutputLevel_Low)
      {
        /* Enter halt mode */
        halt();
      }
    }
  }
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
  
  /* Enable COMP clock */
  CLK_PeripheralClockConfig(CLK_Peripheral_COMP, ENABLE);
}

/**
  * @brief  Configure Comparator peripheral 
  * @param  None
  * @retval None
  */
static void COMP_Config(void)
{
  /* Close I/O Switch 23 to select PD0 as comparator 2 non inverting input:
     External signal should be connected to PD0 */
  SYSCFG_RIIOSwitchConfig(RI_IOSwitch_23, ENABLE);

  /* Init COMP2: VREFINT is used as COMP2 inverting input
                COMP2 output is connected to TIM2 input capture 2 (default configuration)
                COMP2 speed is fast */
  COMP_Init(COMP_InvertingInput_VREFINT, COMP_OutputSelect_TIM2IC2, COMP_Speed_Fast);

  /* COMP2 edge detection: rising edge */
  COMP_EdgeConfig(COMP_Selection_COMP2, COMP_Edge_Rising);
}

/**
  * @brief  Configure ADC peripheral 
  * @param  None
  * @retval None
  */
static void ADC_Config(void)
{
  /* Initialise and configure ADC1 */
  ADC_Init(ADC1, ADC_ConversionMode_Continuous, ADC_Resolution_12Bit, ADC_Prescaler_1);

  /* Configure ADC1 to measure the analog voltage available on slow channels */
  /* Slow channel group configuration: Channel 22 (PD0) */
  ADC_SamplingTimeConfig(ADC1, ADC_Group_SlowChannels, ADC_SamplingTime_192Cycles);

  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);

  /* Enable ADC1 Channel 22: PD0 */
  ADC_ChannelCmd(ADC1, ADC_Channel_22, ENABLE);
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
