/**
  ******************************************************************************
  * @file    COMP/COMP_PulseWidthMeasurement/Src/main.c 
  * @author  MCD Application Team
  * @brief   This example provides a short description of how to use the COMP 
  *          to measure Pulse Width.
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
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
COMP_HandleTypeDef  hcomp1;
/* Timer handler declaration */
TIM_HandleTypeDef      TimHandle;

/* Timer Input Capture Configuration Structure declaration */
TIM_IC_InitTypeDef     sICConfig;
DAC_HandleTypeDef      DacHandle;
I2C_HandleTypeDef      I2CxHandle;
static DAC_ChannelConfTypeDef sConfig;
__IO uint32_t uwMeasuredPulse = 0;

/* Captured Values */
uint32_t               uwIC2Value1 = 0;
uint32_t               uwIC2Value2 = 0;
uint32_t               uwDiffCapture = 0;

/* Capture index */
uint16_t               uhCaptureIndex = 0;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void ErrorHandler(void);
static void COMP_Config(void);
static void TIM_Config(void);
static void DAC_Config(void);
static void I2C_Config(void);
static void ErrorHandler(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  uint8_t param;
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

  /* Configure LED3 */
  BSP_LED_Init(LED3);

  /* Configure the system clock to 32 Mhz */
  SystemClock_Config();

  /* Configure the I2C */
  I2C_Config();

  /* ######################### MFX Configuration ############################ */
  param=0x80;
  if(HAL_I2C_Mem_Write(&I2CxHandle, 0x84, 0x40, I2C_MEMADD_SIZE_8BIT, &param, 0x1, 10000) != HAL_OK)
  {
    ErrorHandler();
  }

  /* Required delay to reset the MFX */
  HAL_Delay(100);

  param=0x40;    /* Enter MFX into standby to avoid conflict on PA1 pin */
  if(HAL_I2C_Mem_Write(&I2CxHandle, 0x84, 0x40, I2C_MEMADD_SIZE_8BIT, &param, 0x1, 10000) != HAL_OK)
  {
    ErrorHandler();
  }
  
  /* Configure DAC */
  DAC_Config();
  
  /* Configure COMP */
  COMP_Config(); 
  
  /* TIM2 Configuration in input capture mode */
  TIM_Config();
  
  /* Infinite loop */
  while (1)
  {}
}

/**
  * @brief  Conversion complete callback in non blocking mode 
  * @param  htim : htim handle
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
      uwMeasuredPulse = (uint32_t)(((uint64_t) uwDiffCapture * 1000000) / HAL_RCC_GetPCLK1Freq());
      uhCaptureIndex = 0;
    }
    
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = HSI
  *            SYSCLK(Hz)                     = 32000000
  *            HCLK(Hz)                       = 32000000
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /* Enable HSI Oscillator and activate PLL with HSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSEState = RCC_HSE_OFF;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL4;
  RCC_OscInitStruct.PLL.PLLDIV = RCC_PLL_DIV2;
  RCC_OscInitStruct.HSICalibrationValue = 0x10;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);  
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1);
}

/**
  * @brief  Configures the DAC channel 1 with output buffer enabled.
  * @param  None
  * @retval None
  */
static void DAC_Config(void)
{
   /*##-1- Configure the DAC peripheral #######################################*/
  DacHandle.Instance = DACx;

  if(HAL_DAC_Init(&DacHandle) != HAL_OK)
  {
    /* Error */
    ErrorHandler();
  }
  
  /*##-2- Configure DAC channel1 #############################################*/  
  sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_DISABLE;  
  
  if(HAL_DAC_ConfigChannel(&DacHandle, &sConfig, DACx_CHANNEL) != HAL_OK)
  {
    /* Channel configuration Error */
    ErrorHandler();
  }
  
  /*##-3- Set DAC Channel1 DHR register ######################################*/ 
  if(HAL_DAC_SetValue(&DacHandle, DACx_CHANNEL, DAC_ALIGN_12B_R, 868) != HAL_OK)
  {
    /* Setting value Error */
    ErrorHandler();
  }
  
  /*##-4- Enable DAC Channel1 ################################################*/ 
  if(HAL_DAC_Start(&DacHandle, DACx_CHANNEL) != HAL_OK)
  {
    /* Start Error */
    ErrorHandler();
  }
}

/**
  * @brief  Configures TIM2 channel 4 in input capture mode
  * @param  None
  * @retval None
  */
static void TIM_Config(void)
{
  /*##-1- Configure the TIM peripheral #######################################*/ 
  /* Set TIMx instance */
  TimHandle.Instance = TIMx;
 
  /* Initialize TIMx peripheral as follow:
       + Period = 0xFFFF
       + Prescaler = 0
       + ClockDivision = 0
       + Counter direction = Up
  */
  TimHandle.Init.Period        = 0xFFFF;
  TimHandle.Init.Prescaler     = 0;
  TimHandle.Init.ClockDivision = 0;
  TimHandle.Init.CounterMode   = TIM_COUNTERMODE_UP;  
  if(HAL_TIM_IC_Init(&TimHandle) != HAL_OK)
  {
    /* Error */
    ErrorHandler();
  }
  
  HAL_TIMEx_RemapConfig(&TimHandle, TIM2_TI4_COMP1);
   
  /*##-2- Configure the Input Capture channel ################################*/ 
  /* Configure the Input Capture of channel 4 */
  sICConfig.ICPolarity  = TIM_ICPOLARITY_BOTHEDGE;
  sICConfig.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sICConfig.ICPrescaler = TIM_ICPSC_DIV1;
  sICConfig.ICFilter    = 0;   
  if(HAL_TIM_IC_ConfigChannel(&TimHandle, &sICConfig, TIM_CHANNEL_4) != HAL_OK)
  {
    /* Configuration Error */
    ErrorHandler();
  }
  
  /*##-3- Start the Input Capture in interrupt mode ##########################*/
  if(HAL_TIM_IC_Start_IT(&TimHandle, TIM_CHANNEL_4) != HAL_OK)
  {
    /* Starting Error */
    ErrorHandler();
  }
}

/**
  * @brief  Configures Comparator
  * @param  None
  * @retval None
  */
static void COMP_Config(void)
{
  /*##-1- Configure the COMP peripheral ######################################*/  
  hcomp1.Instance = COMP1;
  
  hcomp1.Init.InvertingInput    = COMP_INPUT_MINUS_DAC1_CH1;
  hcomp1.Init.NonInvertingInput = COMP_INPUT_PLUS_IO1;
  hcomp1.Init.OutputPol         = COMP_OUTPUTPOL_NONINVERTED;
  hcomp1.Init.Mode              = COMP_POWERMODE_MEDIUMSPEED;
  hcomp1.Init.WindowMode        = COMP_WINDOWMODE_DISABLE;
  hcomp1.Init.TriggerMode       = COMP_TRIGGERMODE_IT_RISING_FALLING;

  if(HAL_COMP_Init(&hcomp1) != HAL_OK)
  {
    /* Error */
    ErrorHandler();
  }
  
  /*##-2- Start teh COMP1 and enable the interrupt ###########################*/  
  if(HAL_COMP_Start(&hcomp1) != HAL_OK)
  {
    /* Error */
    ErrorHandler();
  }
}

/**
  * @brief SYSTICK callback
  * @param None
  * @retval None
  */
void HAL_SYSTICK_Callback(void)
{
  HAL_IncTick();
}


/**
  * @brief  I2C Configuration
  * @param  None
  * @retval None
  */
static void I2C_Config(void)
{
  I2CxHandle.Instance              = I2C1;
  HAL_I2C_DeInit(&I2CxHandle);
  /*##-1- Configure the I2C peripheral #######################################*/
  I2CxHandle.Instance              = I2C1;
  I2CxHandle.Init.AddressingMode   = I2C_ADDRESSINGMODE_7BIT;
  I2CxHandle.Init.Timing           = 0x00B1112E;
  I2CxHandle.Init.DualAddressMode  = I2C_DUALADDRESS_DISABLE;
  I2CxHandle.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  I2CxHandle.Init.GeneralCallMode  = I2C_GENERALCALL_DISABLE;
  I2CxHandle.Init.NoStretchMode    = I2C_NOSTRETCH_DISABLE;
  I2CxHandle.Init.OwnAddress1      = 0x0;
  I2CxHandle.Init.OwnAddress2      = 0x0;
  if(HAL_I2C_Init(&I2CxHandle) != HAL_OK)
  {
    /* Initialization Error */
    ErrorHandler();
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void ErrorHandler(void)
{
  while(1)
  {
    /* Toggle LED3 */
    BSP_LED_Toggle(LED3);
      
    /* Add a delay */
    HAL_Delay(100);
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
