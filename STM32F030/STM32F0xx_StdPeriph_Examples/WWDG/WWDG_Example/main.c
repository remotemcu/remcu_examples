/**
  ******************************************************************************
  * @file    WWDG/WWDG_Example/main.c  
  * @author  MCD Application Team
  * @version V1.4.0
  * @date    24-July-2014
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

/** @addtogroup STM32F0xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup WWDG_Example
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint32_t TimingDelay = 0;

/* Private function prototypes -----------------------------------------------*/
void Delay(__IO uint32_t nTime);
/* Private functions ---------------------------------------------------------*/
static void WWDG_Config(void);

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f0xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f0xx.c file
     */ 
  /* Initialize LEDs and Tamper Button mounted on EVAL board */       
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_PBInit(BUTTON_TAMPER, BUTTON_MODE_EXTI);

  /* Check if the system has resumed from WWDG reset */
  if (RCC_GetFlagStatus(RCC_FLAG_WWDGRST) != RESET)
  { 
    /* WWDGRST flag set */
    /* Turn on LED1 */
    STM_EVAL_LEDOn(LED1);

    /* Clear reset flags */
    RCC_ClearFlag();
  }
  else
  {
    /* WWDGRST flag is not set */
    /* Turn off LED1 */
    STM_EVAL_LEDOff(LED1);
   }
  
  /* Setup SysTick Timer for 1 msec interrupts  */
  if (SysTick_Config(SystemCoreClock / 1000))
  { 
    /* Capture error */ 
    while (1)
    {}
  }
  
  /* Configure WWDG */
  WWDG_Config();
   
  while (1)
  {
    /* Toggle LED2 */
    STM_EVAL_LEDToggle(LED2);

    /* Insert 33 ms delay */
    Delay(33);

    /* Update WWDG counter */
    WWDG_SetCounter(127);
  }
}

/**
  * @brief  WWDG configuration
  * @param  None
  * @retval None
  */
static void WWDG_Config(void)
{
  /* Enable WWDG clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);

  /* WWDG clock counter = (PCLK1 (48MHz)/4096)/8 = 1464Hz (~683 us)  */
  WWDG_SetPrescaler(WWDG_Prescaler_8);

  /* Set Window value to 80; WWDG counter should be refreshed only when the counter
    is below 80 (and greater than 64) otherwise a reset will be generated */
  WWDG_SetWindowValue(80);

  /* Enable WWDG and set counter value to 127, WWDG timeout = ~683 us * 64 = 43.7 ms 
     In this case the refresh window is: ~683 * (127-80)= 32.1ms < refresh window < ~683 * 64 = 43.7ms
     */
  WWDG_Enable(127);
  
}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in 10 ms.
  * @retval None
  */
void Delay(__IO uint32_t nTime)
{
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
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
