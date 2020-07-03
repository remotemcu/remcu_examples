/**
  ******************************************************************************
  * @file    COMP/COMP_AnalogWatchdog/Src/main.c  
  * @author  MCD Application Team
  * @brief   This sample code shows how to make an analog watchdog using 
  *          the STM32L0xx COMPs peripherals in window mode.
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
__IO uint32_t State = 0;

/* COMP handlers declaration */
COMP_HandleTypeDef   Comp1Handle;
COMP_HandleTypeDef   Comp2Handle;

RCC_ClkInitTypeDef RCC_ClockFreq;
RCC_ClkInitTypeDef RCC_ClkInitStruct;
RCC_OscInitTypeDef RCC_OscInitStruct;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Comp_Config(void);
static void State_Check(void);
static void Error_Handler(void);

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
  
  /*******************************************************************************
  *                          Common Configuration Routines                       *
  *******************************************************************************/  
 
  /* Configure LED2 */
  BSP_LED_Init(LED2);

  /* Configure the system clock to 2 Mhz */
  SystemClock_Config();
  
  /* configure COMP1 and COMP2 with interrupts enabled */
  Comp_Config();
  
  /* Check input voltage level: within the thresholds, above the upper threshold
     or under the lower threshold */
  State_Check();
  
  /* Infinite loop */
  while (1)
  {
    if (State == STATE_OVER_THRESHOLD)
    {
      /* Restore clock configuration */
      SystemClock_Config();
      
      /* Restore GPIO configuration */
      BSP_LED_Init(LED2);
      
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
      /* Input voltage is within the thresholds: higher and lower thresholds */
      
      /* Turn off LD2 */
      BSP_LED_Off(LED2);
      
      /* Enter Stop Mode */
      HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
    }
    else /* (State == STATE_UNDER_THRESHOLD) */
    {
      /* Restore clock configuration */
      SystemClock_Config();
      
      /* Restore GPIO configuration */
      BSP_LED_Init(LED2);
      
      while(State == STATE_UNDER_THRESHOLD)
      {
        /* add your code here */
        /* Toggle LED2 */
        BSP_LED_Toggle(LED2);
        HAL_Delay(1000);
      }
    }
  }
}

/**
  * @brief  Configure COMP1 and COMP2 with interrupt
  * @param  None
  * @retval None
  */
static void Comp_Config(void)
{
 
  /* COMP1 Init: the higher threshold is set to VREFINT ~ 1.22V
     but can be changed to other available possibilities */
  Comp1Handle.Instance = COMP1;
  Comp1Handle.Init.InvertingInput  = COMP_INPUT_MINUS_VREFINT;
  Comp1Handle.Init.OutputPol       = COMP_OUTPUTPOL_NONINVERTED;
  Comp1Handle.Init.Mode            = COMP_POWERMODE_ULTRALOWPOWER;
  Comp1Handle.Init.WindowMode      = COMP_WINDOWMODE_COMP1_INPUT_PLUS_COMMON;
  Comp1Handle.Init.TriggerMode     = COMP_TRIGGERMODE_IT_RISING_FALLING;
  if(HAL_COMP_Init(&Comp1Handle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler(); 
  }
  
  
  /* COMP2 Init: the lower threshold is set to VREFINT/4 ~ 1.22 / 4 ~ 0.305 V 
     but can be changed to other available possibilities */
  Comp2Handle.Instance               = COMP2;
  Comp2Handle.Init.InvertingInput    = COMP_INPUT_MINUS_1_4VREFINT;
  Comp2Handle.Init.NonInvertingInput = COMP_INPUT_PLUS_IO2; 
  Comp2Handle.Init.OutputPol         = COMP_OUTPUTPOL_NONINVERTED;
  Comp2Handle.Init.Mode              = COMP_POWERMODE_ULTRALOWPOWER;
  Comp2Handle.Init.TriggerMode       = COMP_TRIGGERMODE_IT_RISING_FALLING;
  if(HAL_COMP_Init(&Comp2Handle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler(); 
  }
  
  /* Enable VREFINT */
  HAL_COMPEx_EnableVREFINT();
    
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
}

/**
  * @brief  check input voltage level: within the thresholds, above the upper 
  *         threshold or under the lower threshold
  * @param  None
  * @retval None
  */
static void State_Check(void)
{
  /* Check if COMP2 output level is high */
  if ((HAL_COMP_GetOutputLevel(&Comp1Handle) == COMP_OUTPUT_LEVEL_HIGH) 
   && (HAL_COMP_GetOutputLevel(&Comp2Handle) == COMP_OUTPUT_LEVEL_HIGH))
  {
    /* A rising edge is detected so the input voltage is higher than VREFINT */
    State = STATE_OVER_THRESHOLD;
  }
  else if ((HAL_COMP_GetOutputLevel(&Comp1Handle) == COMP_OUTPUT_LEVEL_LOW)
       && (HAL_COMP_GetOutputLevel(&Comp2Handle) == COMP_OUTPUT_LEVEL_HIGH))
  {
    /* A falling edge is detected so the input voltage is lower than VREFINT */
    State = STATE_WITHIN_THRESHOLD;
  }
  else if ((HAL_COMP_GetOutputLevel(&Comp1Handle) == COMP_OUTPUT_LEVEL_LOW)
       && (HAL_COMP_GetOutputLevel(&Comp2Handle) == COMP_OUTPUT_LEVEL_LOW))
  {
    State = STATE_UNDER_THRESHOLD;
  }
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
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  
  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  
  /* Enable MSI Oscillator */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_5;
  RCC_OscInitStruct.MSICalibrationValue=0x00;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  
  /* Select MSI as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
}


/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  while(1)
  {
    /* Turn on the LED2 */
    BSP_LED_On(LED2);
  }
}


/**
  * @brief COMP callback
  * @param None
  * @retval None
  */
void HAL_COMP_TriggerCallback(COMP_HandleTypeDef *hcomp)
{

    /* Check input voltage level: within the thresholds, above the upper threshold
       or under the lower threshold */
    State_Check();
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
