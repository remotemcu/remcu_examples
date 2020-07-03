/**
  ******************************************************************************
  * @file    COMP/COMP_AnalogWatchdog/Src/main.c
  * @author  MCD Application Team
  * @brief   This example provides a short description of how to use the COMP 
  *          peripheral in window mode to make an analog watchdog.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32L0xx_HAL_Examples
  * @{
  */

/** @addtogroup COMP_AnalogWatchdog
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define STATE_OVER_THRESHOLD    0x00000001
#define STATE_WITHIN_THRESHOLD  0x00000002
#define STATE_UNDER_THRESHOLD   0x00000003

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
COMP_HandleTypeDef   Comp1Handle;
COMP_HandleTypeDef   Comp2Handle;
__IO uint32_t State = 0;
/* Variable to indicate that MCU entered in Stop mode */
__IO uint32_t EnterInStopMode = 0;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void Error_Handler(void);
static void COMP_Config(void);
static void InputVoltageLevel_Check(void);
static void StopSequence_Config(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* STM32L0xx HAL library initialization:
       - Configure the Flash prefetch, Flash preread and Buffer caches
       - Systick timer is configured by default as source of time base, but user 
             can eventually implement his proper time base source (a general purpose 
             timer for example or other time source), keeping in mind that Time base 
             duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
             handled in milliseconds basis.
       - Low Level Initialization
     */
  HAL_Init();

  /* Configure the system clock to 2 MHz */
  SystemClock_Config();
  
  /*******************************************************************************
  *                          Common Configuration Routines                       *
  *******************************************************************************/  
 
  /******* Initialize LEDs available on STM32L073-Nucleo Rev C board ******************/
  
  BSP_LED_Init(LED2);
  
  /*## Configure peripherals #################################################*/
  /* Configure the COMP peripherals */
  COMP_Config();
  
  /*## Enable peripherals ####################################################*/
  /* Start COMP1 */
  if(HAL_COMP_Start(&Comp1Handle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler(); 
  }
  
  /* Start COMP2 */
  if(HAL_COMP_Start(&Comp2Handle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler(); 
  }

  /* Check input voltage level: within the thresholds, above the upper threshold
     or under the lower threshold */
  InputVoltageLevel_Check();
  
  /* Infinite loop */
  while (1)
  {
    if (State == STATE_OVER_THRESHOLD)
    {
      /* Restoration done only in case of MCU was in stop mode */
      if (EnterInStopMode == 1)
      {
        /* Restore config: clock, GPIO... */
        SystemClock_Config();
        
        /* Restore GPIO configuration */
        BSP_LED_Init(LED2);
        
        EnterInStopMode = 0;
      }

     
      while(State == STATE_OVER_THRESHOLD)
      {
        /* add your code here */
        
        /* Toggle LED2 */
        BSP_LED_Toggle(LED2);
        HAL_Delay(50);
      }
    }
    else if (State == STATE_WITHIN_THRESHOLD)
    {
      

      /* Turn off LED2 */
      BSP_LED_Off(LED2);

      
      /* Input voltage is within the thresholds: higher and lower thresholds */
      /* Enter STOP mode with regulator in low power */
      StopSequence_Config();
    }
    else /* (State == STATE_UNDER_THRESHOLD) */
    {
      /* Restoration done only in case of MCU was in stop mode */
      if (EnterInStopMode == 1)
      {
        /* Restore config: clock, GPIO... */
        SystemClock_Config();
        
        /* Restore GPIO configuration */
        BSP_LED_Init(LED2);
        EnterInStopMode = 0;
      }
      
      BSP_LED_Init(LED2);
      
      while(State == STATE_UNDER_THRESHOLD)
      {
        /* add your code here */
        
        /* add your code here */
        /* Toggle LED2 */
        BSP_LED_Toggle(LED2);
        HAL_Delay(1000);
      }
    }
  }
}

/**
  * @brief  COMP configuration
  * @param  None
  * @retval None
  */
static void COMP_Config(void)
{
  
  /*##-1- Configure the COMPx peripheral ###################################*/


    /* COMP1 Init: the higher threshold is fixed by hardware to VREFINT         */
    /* (~ 1.22V).                                                               */
  Comp1Handle.Instance                   = COMP1;
  Comp1Handle.Init.NonInvertingInput     = COMP_INPUT_PLUS_IO1;
  Comp1Handle.Init.InvertingInput        = COMP_INPUT_MINUS_VREFINT;
  Comp1Handle.Init.LPTIMConnection       = COMP_LPTIMCONNECTION_DISABLED;
  Comp1Handle.Init.OutputPol             = COMP_OUTPUTPOL_NONINVERTED;
  Comp1Handle.Init.Mode                  = COMP_POWERMODE_ULTRALOWPOWER;
  Comp1Handle.Init.WindowMode            = COMP_WINDOWMODE_COMP1_INPUT_PLUS_COMMON;
  Comp1Handle.Init.TriggerMode           = COMP_TRIGGERMODE_IT_RISING_FALLING;


  if(HAL_COMP_Init(&Comp1Handle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler(); 
  }
  
  
  /* COMP2 Init: the lower threshold is set to VREFINT/4 (1.22V /4 ~ 0.305 V) */
  /* but can be changed to other available possibilities.                     */
  Comp2Handle.Instance                   = COMP2;
  

  Comp2Handle.Instance                   = COMP2;
  Comp2Handle.Init.InvertingInput        = COMP_INPUT_MINUS_1_4VREFINT;
  Comp2Handle.Init.NonInvertingInput     = COMP_INPUT_PLUS_IO2;
  Comp2Handle.Init.LPTIMConnection       = COMP_LPTIMCONNECTION_DISABLED;
  Comp2Handle.Init.OutputPol             = COMP_OUTPUTPOL_NONINVERTED;
  Comp2Handle.Init.Mode                  = COMP_POWERMODE_ULTRALOWPOWER;
  Comp2Handle.Init.WindowMode            = COMP_WINDOWMODE_COMP1_INPUT_PLUS_COMMON;
  Comp2Handle.Init.TriggerMode           = COMP_TRIGGERMODE_IT_RISING_FALLING;


  if(HAL_COMP_Init(&Comp2Handle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler(); 
  }
  
  /* Enable VREFINT */
   HAL_COMPEx_EnableVREFINT();

}

/**
  * @brief  Prepare the system to enter STOP mode.
  * @param  None
  * @retval None
  */
static void StopSequence_Config(void)
{
  GPIO_InitTypeDef      GPIO_InitStruct;
  
  /* PWR Peripheral clock enable */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* Enable GPIOs clock */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();

  /* Configure all GPIO port pins in Analog mode */
  GPIO_InitStruct.Pin = GPIO_PIN_All;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
  GPIO_InitStruct.Pin = GPIOH_PIN_AVAILABLE;
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);



  /* Request to enter STOP mode with regulator in low power */
  HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
  EnterInStopMode = 1;
}

/**
  * @brief  check input voltage level: within the thresholds, above the upper 
  *         threshold or under the lower threshold
  * @param  None
  * @retval None
  */
static void InputVoltageLevel_Check(void)
{
  /*  Check if COMP1 and COMP2 output level is high */
  if (((HAL_COMP_GetOutputLevel(&Comp1Handle)) == COMP_OUTPUT_LEVEL_HIGH) 
   && ((HAL_COMP_GetOutputLevel(&Comp2Handle)) == COMP_OUTPUT_LEVEL_HIGH))
  {
    /* A rising edge is detected so the input voltage is higher than VREFINT */
    State = STATE_OVER_THRESHOLD;
  }
  else if (((HAL_COMP_GetOutputLevel(&Comp1Handle)) == COMP_OUTPUT_LEVEL_LOW)
       && ((HAL_COMP_GetOutputLevel(&Comp2Handle)) == COMP_OUTPUT_LEVEL_HIGH))
  {
    /* A falling edge is detected so the input voltage is lower than VREFINT */
    State = STATE_WITHIN_THRESHOLD;
  }
  else if (((HAL_COMP_GetOutputLevel(&Comp1Handle)) == COMP_OUTPUT_LEVEL_LOW)
       && ((HAL_COMP_GetOutputLevel(&Comp2Handle)) == COMP_OUTPUT_LEVEL_LOW))
  {
    State = STATE_UNDER_THRESHOLD;
  }
}

/**
  * @brief  Comparator interrupt callback.
  * @param  hcomp: COMP handle
  * @retval None
  */
void HAL_COMP_TriggerCallback(COMP_HandleTypeDef *hcomp)
{
  /* Check input voltage level: within the thresholds, */
  /* above the upper threshold or under the lower threshold */
  InputVoltageLevel_Check();
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = MSI
  *            SYSCLK(Hz)                     = 2000000
  *            HCLK(Hz)                       = 2000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            Flash Latency(WS)              = 0
  *            Main regulator output voltage  = Scale3 mode
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  
  /* Enable MSI Oscillator */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_5;
  RCC_OscInitStruct.MSICalibrationValue=0x00;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct)!= HAL_OK)
  {
    /* Initialization Error */
    while(1); 
  }
  
  /* Select MSI as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0)!= HAL_OK)
  {
    /* Initialization Error */
    while(1); 
  }
  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  
}


/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  
  while (1)
  {
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
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
