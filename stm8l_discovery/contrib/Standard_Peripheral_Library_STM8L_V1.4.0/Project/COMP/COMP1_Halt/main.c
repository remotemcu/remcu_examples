/**
  ******************************************************************************
  * @file    COMP/COMP1_Halt/main.c
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
#include "timing_delay.h"

/** @addtogroup STM8L15x_StdPeriph_Examples
  * @{
  */

/** @addtogroup COMP1_Halt
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
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
  
  /* Init TIM2 to generate 1 ms time base update interrupt */
  TimingDelay_Init();

  /* Key button configuration */
  STM_EVAL_PBInit(BUTTON_KEY, BUTTON_MODE_EXTI);

  /* Initialize Leds mounted on STM8L152X-EVAL board */
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);

   /* COMP configuration -------------------------------------------*/
  COMP_Config(); 

  /* Enable Interrupts */
  enableInterrupts();

  /* Infinite loop */
  while (1)
  {
    /* Toggle LDE1..4 */
    STM_EVAL_LEDToggle(LED1);
    STM_EVAL_LEDToggle(LED2);
    STM_EVAL_LEDToggle(LED3);
    STM_EVAL_LEDToggle(LED4);

    /* Insert delay */
    Delay(50);
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
  /* Connect internal reference voltage to COMP1 inverting input */
  COMP_VrefintToCOMP1Connect(ENABLE);
  /* close the analog switch number 14 */
  SYSCFG_RIAnalogSwitchConfig(RI_AnalogSwitch_14, ENABLE);
  /* close the analog switch number 1 */
  SYSCFG_RIAnalogSwitchConfig(RI_AnalogSwitch_1, ENABLE);
  /* close the I/O switch number 4 */
  SYSCFG_RIIOSwitchConfig(RI_IOSwitch_4, ENABLE);

  /* Enable COMP1 Interrupt */
  COMP_ITConfig(COMP_Selection_COMP1, ENABLE);
  /* Configure the event detection */
  COMP_EdgeConfig(COMP_Selection_COMP1, COMP_Edge_Rising);
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
