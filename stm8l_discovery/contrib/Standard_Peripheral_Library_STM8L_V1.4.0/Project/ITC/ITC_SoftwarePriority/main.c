/**
  ******************************************************************************
  * @file    ITC/ITC_SoftwarePriority/main.c
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

/** @addtogroup ITC_SoftwarePriority
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void Delay (__IO uint16_t nCount);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void main(void)
{
  /* Initialize LEDs mounted on STM8L152X-EVAL board */
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDInit(LED3);

  /* Initialize key and Joystick down buttons mounted on STM8L152X-EVAL board */
  STM_EVAL_PBInit(BUTTON_KEY, BUTTON_MODE_EXTI);
  STM_EVAL_PBInit(BUTTON_DOWN, BUTTON_MODE_EXTI);

#ifdef USE_STM8L1526_EVAL

  /* Set PC1 sensitivity to falling edge and low level */
  EXTI_SetPinSensitivity(EXTI_Pin_1, EXTI_Trigger_Falling_Low);
  /* Set PD4 sensitivity to falling edge and low level */
  EXTI_SetPinSensitivity(EXTI_Pin_4, EXTI_Trigger_Falling_Low);

  /*
    Change the software priority between key and joystick down buttons:
    - By default the Pin_1 (key) and Pin_4 have the same priority level: level3
    - We configure the Pin_4 (joystick_down) to have a higher sw priority than Pin_1 (key)
  */
  /* Set the EXTI1 IRQ (which is connected to key button) level to 1 */
  ITC_SetSoftwarePriority(EXTI1_IRQn, ITC_PriorityLevel_1);

  /* Set the EXTI4 IRQ (which is connected to joystick down button) software level to 2 */
  /* Thus joystick down button IRQ software priority is higher than key one */
  /* Pressing joystick down button interrupts the execution of the EXTI1 Interrupt Service Routine */
  ITC_SetSoftwarePriority(EXTI4_IRQn, ITC_PriorityLevel_2);

#elif defined USE_STM8L1528_EVAL

  /* Set PH6 sensitivity to falling edge and low level */
  EXTI_SetPinSensitivity(EXTI_Pin_6, EXTI_Trigger_Falling_Low);
  /* Set PG1 sensitivity to falling edge and low level */
  EXTI_SetPinSensitivity(EXTI_Pin_1, EXTI_Trigger_Falling_Low);

  /*
  Change the software priority between key and joystick down buttons:
  - By default the Pin_6 (key) and Pin_1 have the same priority level: level3
  - We configure the Pin_1 (joystick_down) to have a higher sw priority than Pin_6 (key)
  */
  /* Set the EXTI6 IRQ (which is connected to key button) level to 1 */
  ITC_SetSoftwarePriority(EXTI6_IRQn, ITC_PriorityLevel_1);

  /* Set the EXTI1 IRQ (which is connected to joystick down button) software level to 2 */
  /* Thus joystick down button IRQ software priority is higher than key one */
  /* Pressing joystick down button interrupts the execution of the EXTI6 Interrupt Service Routine */
  ITC_SetSoftwarePriority(EXTI1_IRQn, ITC_PriorityLevel_2);

#endif /* USE_STM8L152X_EVAL */

  /* enable interrupts by setting level 0 */
  enableInterrupts();

  /* LD2 and LD3 are on */
  STM_EVAL_LEDOn(LED2);
  STM_EVAL_LEDOn(LED3);

  while (1);
}

/**
  * @brief  Inserts a delay time.
  * @param  nCount: specifies the delay time length.
  * @retval None
  */
void Delay(__IO uint16_t nCount)
{
  /* Decrement nCount value */
  while (nCount != 0)
  {
    nCount--;
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
