/**
  ******************************************************************************
  * @file    COMP/COMP_PulseWidthMeasurement/Src/main.c
  * @author  MCD Application Team
  * @brief   This example provides a short description of how to use the COMP 
  *          peripheral to measure Pulse Width.
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

/** @addtogroup COMP_PulseWidthMeasurement
  * @{
  */

/* Private typedef -----------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/**
  * @brief  Computation of digital value on range 8 bits from voltage value
  *         (unit: mV).
  *         Calculation depends on settings: digital resolution and power
  *         supply of analog voltage Vdda.
  * @param DATA: Voltage value (unit: mV)
  * @retval None
  */
#define COMPUTATION_VOLTAGE_TO_DIGITAL_8BITS(DATA)                             \
  ((DATA) * RANGE_8BITS / VDD_APPLI)

/* Private define ------------------------------------------------------------*/
#define VDD_APPLI                      ((uint32_t)3300)    /* Value of analog voltage supply Vdda (unit: mV) */
#define RANGE_8BITS                    ((uint32_t) 255)    /* Max value with a full range of 8 bits */
#define COMP_THRESHOLD_MV              ((uint32_t)1000)    /* Threshold of the comparator on the inverting input, provided by DAC. Can be customized for this example (unit: mV) */

#define TIMER_FREQUENCY_HZ             ((uint32_t)100000)  /* Timer frequency for pulse width measurement (unit: Hz). Frequency the highest (maximum TIM2CLK PCLK1), accuracy pitch the lowest. Maximum pulse length measurable is 65535*pitch (if timer set to 100kHz, pulse length measurable is min=10us, max=650ms) */

#define EXPECTED_PULSE_WIDTH_US           ((uint32_t)2000) /* Expected result of pulse width measurement (unit: us) */
#define EXPECTED_PULSE_WIDTH_TOLERANCE_US ((uint32_t) 500) /* Tolerance of expected result of pulse width measurement (unit: us) */

/* Private macro -------------------------------------------------------------*/

/**
  * @brief  Computation of digital value on range 8 bits from voltage value
  *         (unit: mV).
  *         Calculation depends on settings: digital resolution and power
  *         supply of analog voltage Vdda.
  * @param DATA: Voltage value (unit: mV)
  * @retval None
  */
#define COMPUTATION_VOLTAGE_TO_DIGITAL_8BITS(DATA)                             \
  ((DATA) * RANGE_8BITS / VDD_APPLI)

/* Private variables ---------------------------------------------------------*/
COMP_HandleTypeDef   Comp2Handle;
DAC_HandleTypeDef    DacHandle;
TIM_HandleTypeDef    TimHandle;
/* Captured Values */
uint32_t               uwIC2Value1 = 0;
uint32_t               uwIC2Value2 = 0;
uint32_t               uwDiffCapture = 0;

/* Capture index */
uint16_t               uhCaptureIndex = 0;

__IO uint32_t uwMeasuredPulse = 0;    /* Result of the pulse width measurement (unit: us) */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void Error_Handler(void);
static void COMP_Config(void);
static void TIM_Config(void);
static void DAC_Config(void);
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
  
  /*## Configure peripherals #################################################*/
  /* Initialize LED on board */
  BSP_LED_Init(LED2);
  
  /*## Configure peripherals #################################################*/
  /* Configure the COMPx peripheral */
  COMP_Config();
  
  /* Configure the DAC peripheral */
  DAC_Config();

  /* Configure the TIMx peripheral */
  TIM_Config();

  /*## Enable peripherals ####################################################*/
    
  /* Start COMP1 */
  if(HAL_COMP_Start(&Comp2Handle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler(); 
  }
  
  /* Set DAC Channel DHR register */
  if(HAL_DAC_SetValue(&DacHandle, DACx_CHANNEL_TO_COMP_INPUT_NEG, DAC_ALIGN_8B_R, COMPUTATION_VOLTAGE_TO_DIGITAL_8BITS(COMP_THRESHOLD_MV)) != HAL_OK)
  {
    /* Setting value Error */
    Error_Handler();
  }
  
  /* Enable DAC Channel */ 
  if(HAL_DAC_Start(&DacHandle, DACx_CHANNEL_TO_COMP_INPUT_NEG) != HAL_OK)
  {
    /* Start Error */
    Error_Handler();
  }


  /* Start the Input Capture in interrupt mode */
  if(HAL_TIM_IC_Start_IT(&TimHandle, TIM_CHANNEL_4) != HAL_OK)
  {
    /* Starting Error */
    Error_Handler();
  }
  
  /* Infinite loop */
  while (1)
  {
    /* Note: For debug, to watch value of pulse measurement, add a breakpoint */
    /*       into function "HAL_TIM_IC_CaptureCallback()" (see comment placed */
    /*       at the end of this function).                                    */ 
    
    /* Turn-on/off LED2 in function of pulse width measurement */
    /*  - Turn-off if pulse width is out of the expected range 2ms +- 1ms */
    /*  - Turn-on if pulse width is within expected range 2ms +- 1ms */ 
    /* Note: In case of usage of the test signal from DAC channel, since this */
    /*       signal is periodic and comparator can measure the length of the  */
    /*       complement of the pulse: comparison with the complement of the   */
    /*       pulse width (period - pulse width).                              */
    /* Note: Variable of pulse width measurement is set into timer            */
    /*       interrupt callback "HAL_TIM_IC_CaptureCallback()"                */
    if ((uwMeasuredPulse > (EXPECTED_PULSE_WIDTH_US - EXPECTED_PULSE_WIDTH_TOLERANCE_US)) &&
        (uwMeasuredPulse < (EXPECTED_PULSE_WIDTH_US + EXPECTED_PULSE_WIDTH_TOLERANCE_US))   )
    {
      BSP_LED_On(LED2);
    }
    else
    {
      BSP_LED_Off(LED2);
    }
  
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
  * @brief  COMP configuration
  * @param  None
  * @retval None
  */
static void COMP_Config(void)
{
  
  /*##-1- Configure the COMPx peripheral ###################################*/ 
  
  Comp2Handle.Instance                   = COMP1;
  
  Comp2Handle.Init.NonInvertingInput     = COMP_INPUT_PLUS_IO1;
  Comp2Handle.Init.InvertingInput             = COMP_INPUT_MINUS_DAC1_CH1;
  Comp2Handle.Init.LPTIMConnection            = COMP_LPTIMCONNECTION_DISABLED;
  Comp2Handle.Init.OutputPol                  = COMP_OUTPUTPOL_NONINVERTED;
  Comp2Handle.Init.Mode                  = COMP_POWERMODE_MEDIUMSPEED;
  Comp2Handle.Init.WindowMode                 = COMP_WINDOWMODE_DISABLE;
  Comp2Handle.Init.TriggerMode                = COMP_TRIGGERMODE_IT_RISING_FALLING;

  if(HAL_COMP_Init(&Comp2Handle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler(); 
  }
  

}


/**
  * @brief Configure the TIMx peripheral
  *        channel 4 in input capture mode
  * @param  None
  * @retval None
  */
static void TIM_Config(void)
{
  /* Timer Input Capture Configuration Structure declaration */
  TIM_IC_InitTypeDef     sICConfig;

  /* Configure the TIM peripheral */
  /* Set TIMx instance */
  TimHandle.Instance = TIMx;
  
  /* Configure timer frequency */
  /* Set timer period to the maximum */
  /* Set timer prescaler to system core clock (unit: Hz) divided by the       */
  /* selected frequency (unit: Hz) to have timer incremented at the selected  */
  /* frequency.                                                               */
  /* Note: Timer clock source frequency is retrieved with function            */
  /*       HAL_RCC_GetPCLK1Freq().                                            */
  /*       Alternate possibility, depending on prescaler settings:            */
  /*       use variable "SystemCoreClock" holding HCLK frequency, updated by  */
  /*       function HAL_RCC_ClockConfig().                                    */
  TimHandle.Init.Period = 0xFFFF;
  TimHandle.Init.Prescaler = ((HAL_RCC_GetPCLK1Freq() / TIMER_FREQUENCY_HZ) -1);
  TimHandle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  TimHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
  if(HAL_TIM_IC_Init(&TimHandle) != HAL_OK)
  {
    /* Error */
    Error_Handler();
  }
  
  HAL_TIMEx_RemapConfig(&TimHandle, TIM2_TI4_COMP1);

  /* Configure the Input Capture of channel 4 */
  sICConfig.ICPolarity  = TIM_ICPOLARITY_BOTHEDGE;  /* For pulse width measurement: capture from COMP rising to falling edge */
  sICConfig.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sICConfig.ICPrescaler = TIM_ICPSC_DIV1;
  sICConfig.ICFilter    = 0;   
  if(HAL_TIM_IC_ConfigChannel(&TimHandle, &sICConfig, TIM_CHANNEL_4) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  }

}

/**
  * @brief  Configure the DAC peripheral
  * @param  None
  * @retval None
  */
static void DAC_Config(void)
{
  DAC_ChannelConfTypeDef sConfig;

  /* Configuration of DACx peripheral */
  DacHandle.Instance = DACx;

  if(HAL_DAC_Init(&DacHandle) != HAL_OK)
  {
    /* Error */
    Error_Handler();
  }
  
  /* Configuration of DACx selected channel */
  sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_DISABLE;  
  
  if(HAL_DAC_ConfigChannel(&DacHandle, &sConfig, DACx_CHANNEL_SIGNAL_FOR_TEST) != HAL_OK)
  {
    /* Channel configuration Error */
    Error_Handler();
  }
  
}

/**
  * @brief  Input Capture callback in non blocking mode
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
  {
    if(uhCaptureIndex == 0)
    {
      /* Get the 1st Input Capture value */
      uwIC2Value1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4);
      uhCaptureIndex = 1;
    }
    else if(uhCaptureIndex == 1)
    {
      /* Get the 2nd Input Capture value */
      uwIC2Value2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4); 
      
      /* Capture computation */
      if (uwIC2Value2 > uwIC2Value1)
      {
        uwDiffCapture = (uwIC2Value2 - uwIC2Value1); 
      }
      else if (uwIC2Value2 < uwIC2Value1)
      {
        uwDiffCapture = ((0xFFFF - uwIC2Value1) + uwIC2Value2) + 1;
      }
      else
      {
        uwDiffCapture = 0;
      }
      /* Compute the pulse width in us */
      uwMeasuredPulse = (uint32_t)(((uint64_t) uwDiffCapture * (htim->Init.Prescaler +1) * 1000000) / HAL_RCC_GetPCLK1Freq());
      uhCaptureIndex = 0;
      
      /* For debug: To watch value of pulse measurement:                      */
      /*  - Set breakpoint here                                               */
      /*  - Add variable "uwMeasuredPulse" into the debugger watch window     */
      __NOP();  /* Set breakpoint here */
      
    }

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
