/**
  ******************************************************************************
  * @file    COMP/COMP_AnalogWatchdog/main.c
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

/** @addtogroup COMP_AnalogWatchdog
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint8_t State = 0;
/* Private function prototypes -----------------------------------------------*/
static void CLK_Config(void);
static void COMP_Config(void);
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

  /******* Initialize LEDs available on STM8L15X-EVAL board ******/
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);

   /* COMP configuration -------------------------------------------*/
  COMP_Config(); 
  
  /* Enable global interrupts */
  enableInterrupts();

  /* check input voltage level: within the thresolds, above the upper threshold
     or under the lower threshold */
  CheckState();

  while (1)
  {
    if (State == STATE_OVER_THRESHOLD)
    {
      /* Turn on LD1 and LD3 and turn off LD2 and LD4 */
      STM_EVAL_LEDOn(LED1);
      STM_EVAL_LEDOff(LED2);
      STM_EVAL_LEDOn(LED3);
      STM_EVAL_LEDOff(LED4);

      while (State == STATE_OVER_THRESHOLD)
      {
        /* add your code here */
      }
    }
    else if (State == STATE_WITHIN_THRESHOLD)
    {
      /* Input voltage is within the thresholds: higher and lower thresholds */
      /* Turn of LD1, LD2, LD3 and LD4 */
      STM_EVAL_LEDOff(LED1);
      STM_EVAL_LEDOff(LED2);
      STM_EVAL_LEDOff(LED3);
      STM_EVAL_LEDOff(LED4);

      /* Enter Halt mode */
      halt();
    }
    else /* (State == STATE_UNDER_THRESHOLD) */
    {
      /* Turn on LD2 & LD4 and turn off LD1 & LD3 */
      STM_EVAL_LEDOff(LED1);
      STM_EVAL_LEDOn(LED2);
      STM_EVAL_LEDOff(LED3);
      STM_EVAL_LEDOn(LED4);

      while (State == STATE_UNDER_THRESHOLD)
      {
        /* add your code here */
      }
    }
  }
}

/**
  * @brief  Configure peripheral clock 
  * @param  None
  * @retval None
  */
static void CLK_Config(void)
{
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
  /* Connect internal reference voltage (VREFINT) to COMP1 inverting input:
     The higher threshold is set to VREFINT ~ 1.22 V */
  COMP_VrefintToCOMP1Connect(ENABLE);

  /* Close I/O Switch 22 to slect PD1 as COMP1 and COMP2 non inverting inputs:
    * Input voltage should be connected to PD1 */
  SYSCFG_RIIOSwitchConfig(RI_IOSwitch_22, ENABLE);

  /* Enable Window mode */
  COMP_WindowCmd(ENABLE);

  /* Init COMP2: - COMP2 inverting input is connected to 1/4 VREFINT:
  * The lower threshold is set to VREFINT/4 ~ 1.22 / 4 ~ 0.305 V
  * and can be changed to other available possibilities
   * - COMP2 output is connected to TIM2 input capture 2 (default config)
   * - COMP2 speed is slow  */
  COMP_Init(COMP_InvertingInput_1_4VREFINT, COMP_OutputSelect_TIM2IC2, COMP_Speed_Slow);

  /* COMP2 edge detection: rising and falling edges */
  COMP_EdgeConfig(COMP_Selection_COMP2, COMP_Edge_Rising_Falling);

  /* COMP1 edge detection: rising and falling edges */
  COMP_EdgeConfig(COMP_Selection_COMP1, COMP_Edge_Rising_Falling);

  /* Enable COMP1 Interrupt */
  COMP_ITConfig(COMP_Selection_COMP1, ENABLE);

  /* Enable COMP2 Interrupt */
  COMP_ITConfig(COMP_Selection_COMP2, ENABLE);
}

/**
  * @brief  check input voltage level: within the thresolds, above the upper threshold
            or under the lower threshold
  * @param  None
  * @retval None
  */
void CheckState(void)
{
  /* Check if COMP1 and COMP2 output levels are high */
  if ((COMP_GetOutputLevel(COMP_Selection_COMP1) == COMP_OutputLevel_High)
      && (COMP_GetOutputLevel(COMP_Selection_COMP2) == COMP_OutputLevel_High))
  {
    /* The input voltage is higher than VREFINT */
    State = STATE_OVER_THRESHOLD;
  }
  /* Ceck if COMP1 output level is low and COMP2 output is high */
  else if ((COMP_GetOutputLevel(COMP_Selection_COMP1) == COMP_OutputLevel_Low)
           && (COMP_GetOutputLevel(COMP_Selection_COMP2) == COMP_OutputLevel_High))
  {
    /* The input voltage is lower than VREFINT and higher than VREFINT/4 */
    State = STATE_WITHIN_THRESHOLD;
  }
  /* Check if COMP1 and COMP2 output levels are low */
  else if ((COMP_GetOutputLevel(COMP_Selection_COMP1) == COMP_OutputLevel_Low)
           && (COMP_GetOutputLevel(COMP_Selection_COMP2) == COMP_OutputLevel_Low))
  {
    /* The input voltage is lower than VREFINT/4 */
    State = STATE_UNDER_THRESHOLD;
  }
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
