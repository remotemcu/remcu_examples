/**
  ******************************************************************************
  * @file    ADC/ADC_LowPower/Src/main.c
  * @author  MCD Application Team
  * @brief   This example provides a short description of how to use the ADC
  *          peripheral to perform conversions with ADC low power modes:
  *          auto-wait and auto-power off.
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

/** @addtogroup ADC_LowPower
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define VDD_APPLI                      ((uint32_t)3300)    /* Value of analog voltage supply Vdda (unit: mV) */
#define RANGE_12BITS                   ((uint32_t)4095)    /* Max value with a full range of 12 bits */
#define TIMER_FREQUENCY_HZ             ((uint32_t)1000)    /* Timer frequency (unit: Hz). With SysClk set to 2MHz, timer frequency TIMER_FREQUENCY_HZ range is min=1Hz, max=33.3kHz. */

/* Uncomment the line below to not use the ADC Low Power features */ 
#define ADC_LOWPOWER    /* ADC low power modes auto-wait and auto-power off are enabled */

/* Private macro -------------------------------------------------------------*/
/**
  * @brief  Computation of voltage (unit: mV) from ADC measurement digital
  *         value on range 12 bits.
  *         Calculation validity conditioned to settings: 
  *          - ADC resolution 12 bits (need to scale value if using a different 
  *            resolution).
  *          - Power supply of analog voltage Vdda 3.3V (need to scale value 
  *            if using a different analog voltage supply value).
  * @param ADC_DATA: Digital value measured by ADC
  * @retval None
  */
#define COMPUTATION_DIGITAL_12BITS_TO_VOLTAGE(ADC_DATA)                        \
  ( (ADC_DATA) * VDD_APPLI / RANGE_12BITS)

/* Private variables ---------------------------------------------------------*/
/* ADC handler declaration */
ADC_HandleTypeDef    AdcHandle;

/* TIM handler declaration */
TIM_HandleTypeDef    TimHandle;
 
__IO uint16_t  uhADCxConvertedValue = 0;
__IO uint16_t  uhADCxConvertedVoltage = 0;
__IO uint8_t   ubADC_overrun_status = RESET;      /* Variable to report ADC overrun event from ADC error callback to main program */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void Error_Handler(void);
static void ADC_Config(void);
static void TIM_Config(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
 /* This sample code shows how to convert an analog input and read the converted
    data using polling mode. */

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

  /* Initialize LEDs on board */
  BSP_LED_Init(LED2);

  /* Configure User push-button in non-interrupt mode */
  BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_GPIO);


  /* Configure the ADC peripheral */
  ADC_Config();
  
  /* Run the ADC calibration in single-ended mode */  
  if (HAL_ADCEx_Calibration_Start(&AdcHandle, ADC_SINGLE_ENDED) != HAL_OK)
  {
    /* Calibration Error */
    Error_Handler();
  }

  /* Configure the TIM peripheral */
  TIM_Config();  

  /*## Enable peripherals ####################################################*/

  /* Timer counter enable */
  if (HAL_TIM_Base_Start(&TimHandle) != HAL_OK)
  {
    /* Counter Enable Error */
    Error_Handler();
  }
  
  /* For this example purpose, enable ADC overrun interruption. */
  /* In this ADC LowPower example, ADC overrun is not considered as an error, */
  /* but as a way to watch the ADC low power modes effectiveness.             */
  /* Note: Enabling overrun has no usefulness except for this example purpose:*/
  /*       ADC overrun cannot occur with ADC low power mode "auto-wait"       */
  /*       Usually, in normal application, overrun is enabled automatically   */
  /*       by HAL ADC driver with functions "HAL_ADC_Start_IT()" or           */
  /*       "HAL_ADC_Start_DMA()", but this is not compliant with low power    */
  /*       modes. Refer to comments of parameter "LowPowerAutoWait" in HAL    */
  /*       ADC driver definition file.                                        */
  __HAL_ADC_ENABLE_IT(&AdcHandle, (ADC_IT_OVR));

  /* Start ADC conversion */
  HAL_ADC_Start(&AdcHandle);
  
  /* Wait for the first ADC conversion to be completed (timeout unit: ms) */
  HAL_ADC_PollForConversion(&AdcHandle, (1000/TIMER_FREQUENCY_HZ));
  
  /* Infinite loop */
  while (1)
  {
    /* Wait for at least 2 ADC conversions elapsed time, to let time for      */
    /* potential overrun event to occur (unit: ms)                            */
    HAL_Delay(2* (1000/TIMER_FREQUENCY_HZ));
  
    /* Manage LED2 status in function of ADC overrun event */
    if (ubADC_overrun_status != RESET)
    {
      /* Turn on LED2 to indicate ADC overrun event */
      BSP_LED_On(LED2);
      
      /* Reset overrun status variable for next iteration loop */ 
      ubADC_overrun_status = RESET;
    }
    else
    {
      /* Turn off LED2 to indicate no ADC overrun event */
      BSP_LED_Off(LED2);
    }
  
    /* Press User push-button on STM32L073-Nucleo Rev C to get the converted data */
    while(BSP_PB_GetState(BUTTON_KEY) != GPIO_PIN_RESET);
    while(BSP_PB_GetState(BUTTON_KEY) != GPIO_PIN_SET);

    /* Get ADC1 converted data */
    /* If ADC low power mode auto-wait is enabled, this release the ADC */
    /* from idle mode: a new conversion will start at the next trigger  */
    /* event.                                                           */
    uhADCxConvertedValue = HAL_ADC_GetValue(&AdcHandle);
    
    /* Compute the voltage */
    uhADCxConvertedVoltage = COMPUTATION_DIGITAL_12BITS_TO_VOLTAGE(uhADCxConvertedValue);

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
  * @brief  ADC configuration
  * @param  None
  * @retval None
  */
static void ADC_Config(void)
{
  ADC_ChannelConfTypeDef   sConfig;
  
  /* Configuration of ADCx init structure: ADC parameters and regular group */
  AdcHandle.Instance = ADCx;

  AdcHandle.Init.ClockPrescaler        = ADC_CLOCK_ASYNC_DIV4;
  AdcHandle.Init.Resolution            = ADC_RESOLUTION_12B;
  AdcHandle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
  AdcHandle.Init.ScanConvMode          = ADC_SCAN_DIRECTION_FORWARD;    /* Sequencer will convert the number of channels configured below, successively from the lowest to the highest channel number */
  AdcHandle.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;
#if defined(ADC_LOWPOWER)
  AdcHandle.Init.LowPowerAutoWait      = ENABLE;                        /* Enable the dynamic low power Auto Delay: new conversion start only when the previous conversion (for regular group) or previous sequence (for injected group) has been treated by user software. */
  AdcHandle.Init.LowPowerAutoPowerOff  = ENABLE;                        /* Enable the auto-off mode: the ADC automatically powers-off after a conversion and automatically wakes-up when a new conversion is triggered (with startup time between trigger and start of sampling). */
#else
  AdcHandle.Init.LowPowerAutoWait      = DISABLE;
  AdcHandle.Init.LowPowerAutoPowerOff  = DISABLE;
#endif
  AdcHandle.Init.ContinuousConvMode    = DISABLE;                       /* Continuous mode disabled to have only 1 conversion at each conversion trig */
  AdcHandle.Init.DiscontinuousConvMode = DISABLE;                       /* Parameter discarded because sequencer is disabled */
  AdcHandle.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_Tx_TRGO;  /* Trig of conversion start done by external event */
  AdcHandle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_RISING;
  AdcHandle.Init.DMAContinuousRequests = ENABLE;
  AdcHandle.Init.Overrun               = ADC_OVR_DATA_PRESERVED;            /* Overrun set to data preserved to trig overrun event as an error in this exemple */
  AdcHandle.Init.SamplingTime          = ADC_SAMPLETIME_39CYCLES_5;

  if (HAL_ADC_Init(&AdcHandle) != HAL_OK)
  {
    /* ADC initialization error */
    Error_Handler();
  }
 
  /* Configuration of channel on ADCx regular group on sequencer rank 1 */
  /* Note: Considering IT occurring after each ADC conversion (ADC IT         */
  /*       enabled), select sampling time and ADC clock with sufficient       */
  /*       duration to not create an overhead situation in IRQHandler.        */
  sConfig.Channel      = ADCx_CHANNELa;
  sConfig.Rank         = ADC_RANK_CHANNEL_NUMBER;
  
  if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
  {
    /* Channel Configuration Error */
    Error_Handler();
  }
}

/**
  * @brief  TIM configuration
  * @param  None
  * @retval None
  */
static void TIM_Config(void)
{
  TIM_MasterConfigTypeDef sMasterConfig;
  
  /* Time Base configuration */
  TimHandle.Instance = TIMx;
  
  /* Configure timer frequency */
  /* Note: Setting of timer prescaler to 30 to increase the maximum range     */
  /*       of the timer, to fit within timer range of 0xFFFF.                 */
  /*       Setting of reload period to SysClk/30 to maintain a base           */
  /*       frequency of 1us.                                                  */
  /*       With SysClk set to 2MHz, timer frequency (defined by label         */
  /*       TIMER_FREQUENCY_HZ range) is min=1Hz, max=33.3kHz.                 */
  /* Note: Timer clock source frequency is retrieved with function            */
  /*       HAL_RCC_GetPCLK1Freq().                                            */
  /*       Alternate possibility, depending on prescaler settings:            */
  /*       use variable "SystemCoreClock" holding HCLK frequency, updated by  */
  /*       function HAL_RCC_ClockConfig().                                    */
  TimHandle.Init.Period = ((HAL_RCC_GetPCLK1Freq() / (30 * TIMER_FREQUENCY_HZ)) - 1);
  TimHandle.Init.Prescaler = (30-1);
  TimHandle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  TimHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
  
  if (HAL_TIM_Base_Init(&TimHandle) != HAL_OK)
  {
    /* Timer initialization Error */
    Error_Handler();
  }

  /* Timer TRGO selection */
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

  if (HAL_TIMEx_MasterConfigSynchronization(&TimHandle, &sMasterConfig) != HAL_OK)
  {
    /* Timer TRGO selection Error */
    Error_Handler();
  }
}

/**
  * @brief  ADC error callback in non blocking mode
  *        (ADC conversion with interruption or transfer by DMA)
  * @param  hadc: ADC handle
  * @retval None
  */
void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc)
{
  /* In this ADC LowPower example, ADC overrun is not considered as an error, */
  /* but as a way to watch the ADC low power modes effectiveness.             */
  /* Differentiation of ADC error overrun versus other potential errors:      */
  if (HAL_ADC_GetError(hadc) == HAL_ADC_ERROR_OVR)
  {
    /* Update variable to report ADC overrun event to main program */
    ubADC_overrun_status = SET;
  }
  else
  {
    /* In case of ADC error, call main error handler */
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
  /* User may add here some code to deal with a potential error */
  
  /* In case of error, LED2 is toggling at a frequency of 1Hz */
  while(1)
  {
    /* Toggle LED2 */
    BSP_LED_Toggle(LED2);
    HAL_Delay(500);
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

#endif

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
