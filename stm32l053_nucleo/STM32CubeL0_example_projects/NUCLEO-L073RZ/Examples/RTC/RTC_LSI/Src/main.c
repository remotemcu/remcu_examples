/**
  ******************************************************************************
  * @file    RTC/RTC_LSI/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to use STM32L0xx RTC HAL API to 
  *          use the LSI clock source auto calibration to get a precise RTC 
  *          clock.
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

/** @addtogroup RTC_LSI
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define WAKEUP_TIMER_ENABLE 0x32F2

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
RTC_HandleTypeDef RtcHandle;
TIM_HandleTypeDef Input_Handle;

uint16_t tmpCCTIM_CHANNEL_1[2] = {0, 0};
__IO uint32_t uwLsiFreq = 0;

__IO uint32_t uwCaptureNumber = 0;
__IO uint32_t uwPeriodValue = 0;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void RTC_Config(void);
static uint32_t GetLSIFrequency(void);

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

  /* Configure LED2 */
  BSP_LED_Init(LED2);

    /* Configure User push-button button */
    BSP_PB_Init(BUTTON_KEY,BUTTON_MODE_GPIO);

  /* RTC Configuration -------------------------------------------------------*/
  RTC_Config();

    /* Wait Until User push-button pressed */
    while(BSP_PB_GetState(BUTTON_KEY) != GPIO_PIN_RESET)
    {
    }

    /* Wait Until User push-button released */
    while(BSP_PB_GetState(BUTTON_KEY) != GPIO_PIN_SET)
    {
    }
  
  /* Get the LSI frequency:  TIM21 is used to measure the LSI frequency */
  uwLsiFreq = GetLSIFrequency();
  
  /* Update the Calendar Configuration with the LSI exact value */
  RtcHandle.Init.HourFormat = RTC_HOURFORMAT_24;
  RtcHandle.Init.AsynchPrediv = 0x7F;
  RtcHandle.Init.SynchPrediv = (uwLsiFreq/128) - 1;
  RtcHandle.Init.OutPut = RTC_OUTPUT_DISABLE;
  RtcHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  RtcHandle.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  
  if(HAL_RTC_Init(&RtcHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Infinite loop */
  while (1)
  {
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
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  while (1)
  {
  }
}

/**
  * @brief  Configure the RTC peripheral by selecting the clock source.
  * @param  None
  * @retval None
  */
static void RTC_Config(void)
{
  /*##-1- Configure the RTC peripheral #######################################*/
  /* Configure RTC prescaler and RTC data registers */
  /* RTC configured as follows:
      - Hour Format    = Format 24
      - Asynch Prediv  = Value according to source clock
      - Synch Prediv   = Value according to source clock
      - OutPut         = Output Disable
      - OutPutPolarity = High Polarity
      - OutPutType     = Open Drain */
  RtcHandle.Instance = RTC; 
  RtcHandle.Init.HourFormat = RTC_HOURFORMAT_24;
  RtcHandle.Init.AsynchPrediv = RTC_ASYNCH_PREDIV; /* 0x7F */
  RtcHandle.Init.SynchPrediv = RTC_SYNCH_PREDIV; /* (39KHz / 128) - 1 = 0x130 */
  RtcHandle.Init.OutPut = RTC_OUTPUT_DISABLE;
  RtcHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  RtcHandle.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  
  if(HAL_RTC_Init(&RtcHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler(); 
  }
  
  /*##-2- Check if data stored in BackUp register1: Wakeup timer enable #######*/
  /* Read the Back Up Register 1 Data */
  if (HAL_RTCEx_BKUPRead(&RtcHandle, RTC_BKP_DR1) == WAKEUP_TIMER_ENABLE)
  {
    /* if the wakeup timer is enabled then deactivate it to disable the wakeup timer interrupt */
    if(HAL_RTCEx_DeactivateWakeUpTimer(&RtcHandle) != HAL_OK)
    {
      /* Initialization Error */
      Error_Handler(); 
    }
  }
  
  /*##-3- Configure the RTC Wakeup peripheral #################################*/
  /* Setting the Wakeup time to 1 s
       If RTC_WAKEUPCLOCK_CK_SPRE_16BITS is selected, the frequency is 1Hz, 
       this allows to get a wakeup time equal to 1 s if the counter is 0x0 */
  HAL_RTCEx_SetWakeUpTimer_IT(&RtcHandle, 0x0, RTC_WAKEUPCLOCK_CK_SPRE_16BITS);
  
  /*##-4- Write 'wakeup timer enabled' tag in RTC Backup data Register 1 #######*/
  HAL_RTCEx_BKUPWrite(&RtcHandle, RTC_BKP_DR1, WAKEUP_TIMER_ENABLE);

}

/**
  * @brief  Configures TIM21 to measure the LSI oscillator frequency. 
  * @param  None
  * @retval LSI Frequency
  */
static uint32_t GetLSIFrequency(void)
{
  TIM_IC_InitTypeDef    TIMInput_Config;

  /* Configure the TIM peripheral *********************************************/ 
  /* Set TIMx instance */  
  Input_Handle.Instance = TIM21;
  
  /* TIM21 configuration: Input Capture mode ---------------------
     The LSI oscillator is connected to TIM21 TIM_CHANNEL_1.
     The Rising edge is used as active edge.
     The TIM21 Capture/Compare register associated to TIM_CHANNEL_1 
     is used to compute the frequency value. 
  ------------------------------------------------------------ */
  Input_Handle.Init.Prescaler         = 0; 
  Input_Handle.Init.CounterMode       = TIM_COUNTERMODE_UP;  
  Input_Handle.Init.Period            = 0xFFFF; 
  Input_Handle.Init.ClockDivision     = 0;     
  if(HAL_TIM_IC_Init(&Input_Handle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  /* Connect internally the TIM21 Input Capture of TIM_CHANNEL_1 to the LSI clock output */
  HAL_TIMEx_RemapConfig(&Input_Handle, TIM21_TI1_LSI);
  
  /* Configure the Input Capture of TIM_CHANNEL_1 */
  TIMInput_Config.ICPolarity  = TIM_ICPOLARITY_RISING;
  TIMInput_Config.ICSelection = TIM_ICSELECTION_DIRECTTI;
  TIMInput_Config.ICPrescaler = TIM_ICPSC_DIV8;
  TIMInput_Config.ICFilter    = 0;
  if(HAL_TIM_IC_ConfigChannel(&Input_Handle, &TIMInput_Config, TIM_CHANNEL_1) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Start the TIM Input Capture measurement in interrupt mode */
  if(HAL_TIM_IC_Start_IT(&Input_Handle, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }

  /* Wait until the TIM21 gets 2 LSI edges */
  while(uwCaptureNumber != 2)
  {
  }

  /* Disable TIM21 Capture/Compare channel Interrupt Request */
  HAL_TIM_IC_Stop_IT(&Input_Handle, TIM_CHANNEL_1);
  
  /* Deinitialize the TIM21 peripheral registers to their default reset values */
  HAL_TIM_IC_DeInit(&Input_Handle);

  return uwLsiFreq;
}

/**
  * @brief  Input Capture callback in non blocking mode 
  * @param  htim : TIM IC handle
  * @retval None
*/
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
  /* Get the Input Capture value */
  tmpCCTIM_CHANNEL_1[uwCaptureNumber++] = HAL_TIM_ReadCapturedValue(&Input_Handle, TIM_CHANNEL_1);
  
  if (uwCaptureNumber >= 2)
  {
    if ( tmpCCTIM_CHANNEL_1[0] > tmpCCTIM_CHANNEL_1[1] )
    {
      /* Compute the period length */
      uwPeriodValue = (uint16_t)(0xFFFF - tmpCCTIM_CHANNEL_1[0] + tmpCCTIM_CHANNEL_1[1] + 1);
    }
    else
    {
      /* Compute the period length */
      uwPeriodValue = (uint16_t)(tmpCCTIM_CHANNEL_1[1] - tmpCCTIM_CHANNEL_1[0]);
    }
    /* Frequency computation */ 
    uwLsiFreq = (uint32_t) SystemCoreClock / uwPeriodValue;
    uwLsiFreq *= 8;
  }
}

/**
  * @brief  RTC wakeup timer callback 
  * @param  htim : TIM IC handle
  * @retval None
*/
void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc)
{    
  /* Toggle LED2 */
  BSP_LED_Toggle(LED2);
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
