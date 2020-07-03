/**
  ******************************************************************************
  * @file    RTC/RTC_TimeStamp/Src/main.c 
  * @author  MCD Application Team
  * @brief   This sample code shows how to use STM32L0xx RTC HAL API to
  *          ensure Time Stamp configuration.
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

/** @addtogroup RTC_TimeStamp
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* RTC handler declaration */
RTC_HandleTypeDef RtcHandle;
I2C_HandleTypeDef I2CxHandle;

/* Buffers used for displaying Time and Date */
uint8_t aShowTime[50] = {0}, aShowTimeStamp[50] = {0};
uint8_t aShowDate[50] = {0}, aShowDateStamp[50] = {0};
 
FlagStatus TamperStatus = RESET;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void RTC_TimeStampConfig(void);
static void RTC_CalendarShow(void);
static void I2C_Config(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{ 
  uint8_t param;
  GPIO_InitTypeDef  GPIO_InitStruct;

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
  
  /* Turn LED3 on */
  BSP_LED_On(LED3);
  
  /* Configure the system clock to 2 Mhz */
  SystemClock_Config();
  
  /* Configure the I2C */
  I2C_Config();

  /* ######################### MFX Configuration ############################ */
  /* PA1 GPIO pin configuration to wakeup MFX  */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  GPIO_InitStruct.Pin       = GPIO_PIN_1;
  GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);

  /* Required delay to wake-up MFX */
  HAL_Delay(100);

  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);

  param=0x80;
  if(HAL_I2C_Mem_Write(&I2CxHandle, 0x84, 0x40, I2C_MEMADD_SIZE_8BIT, &param, 0x1, 10000) != HAL_OK)
  {
    Error_Handler();
  }

  /* Required delay to reset the MFX */
  HAL_Delay(100);

  param=0x40;    /* Enter MFX into standby to avoid conflict on PC13 pin */
  if(HAL_I2C_Mem_Write(&I2CxHandle, 0x84, 0x40, I2C_MEMADD_SIZE_8BIT, &param, 0x1, 10000) != HAL_OK)
  {
    Error_Handler();
  }
  
 /*##-1- Configure the RTC peripheral #######################################*/
  /* Configure RTC prescaler and RTC data registers */
  /* RTC configured as follow:
      - Hour Format    = Format 12
      - Asynch Prediv  = Value according to source clock
      - Synch Prediv   = Value according to source clock
      - OutPut         = Output Disable
      - OutPutPolarity = High Polarity
      - OutPutType     = Open Drain */
  RtcHandle.Instance = RTC; 
  RtcHandle.Init.HourFormat = RTC_HOURFORMAT_12;
  RtcHandle.Init.AsynchPrediv = RTC_ASYNCH_PREDIV;
  RtcHandle.Init.SynchPrediv = RTC_SYNCH_PREDIV;
  RtcHandle.Init.OutPut = RTC_OUTPUT_DISABLE;
  RtcHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  RtcHandle.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  
  if(HAL_RTC_Init(&RtcHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler(); 
  }
  
    /*##-2-  Configure RTC Timestamp ############################################*/
    RTC_TimeStampConfig();
 
  /* Infinite loop */  
  while (1)
  {
    /*##-3- Display the updated Time and Date ################################*/
    RTC_CalendarShow();
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  while(1)
  {
    /* Turn LED3 on */
    BSP_LED_Toggle(LED3);
    
    /* Delay */
    HAL_Delay(100);
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
  RCC_OscInitStruct.MSICalibrationValue = 0x00;
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
  * @brief  Configure the current time and date.
  * @param  None
  * @retval None
  */
static void RTC_TimeStampConfig(void)
{
  RTC_DateTypeDef sdatestructure;
  RTC_TimeTypeDef stimestructure;
  RTC_TamperTypeDef stamperstructure;
  
  /*##-1- Configure the Tamper peripheral ####################################*/
  /* Configure Tamper registers */
  /* RTC Tamper configured as follow:
  - Pamper = Tamper1
  - Pin selection = PC13
  - Trigger = Falling Edge
  - TimeStamp On Tamper Detection = Enable */ 
  stamperstructure.Filter = RTC_TAMPERFILTER_DISABLE;
  stamperstructure.Tamper = RTC_TAMPER_1;
  stamperstructure.Trigger = RTC_TAMPERTRIGGER_FALLINGEDGE;
  stamperstructure.SamplingFrequency = RTC_TAMPERSAMPLINGFREQ_RTCCLK_DIV4096;
  stamperstructure.PrechargeDuration = RTC_TAMPERPRECHARGEDURATION_1RTCCLK ;
  stamperstructure.TamperPullUp = RTC_TAMPER_PULLUP_DISABLE;
  stamperstructure.TimeStampOnTamperDetection = RTC_TIMESTAMPONTAMPERDETECTION_ENABLE;
  HAL_RTCEx_SetTamper(&RtcHandle, &stamperstructure);
  
  /*##-2- Configure the Time Stamp peripheral ################################*/
  /*  RTC TimeStamp flag Generation: TimeStamp Rising Edge on PC13 Pin */
  HAL_RTCEx_SetTimeStamp_IT(&RtcHandle, RTC_TIMESTAMPEDGE_RISING, RTC_TIMESTAMPPIN_DEFAULT);
  
  /* Clear the TIMESTAMP interrupt pending bit */
  __HAL_RTC_TIMESTAMP_CLEAR_FLAG(&RtcHandle,RTC_FLAG_TSF);
  
  /*##-3- Configure the Date #################################################*/
  /* Set Date: Monday April 14th 2014 */
  sdatestructure.Year = 0x14;
  sdatestructure.Month = RTC_MONTH_APRIL;
  sdatestructure.Date = 0x14;
  sdatestructure.WeekDay = RTC_WEEKDAY_MONDAY;
  
  if(HAL_RTC_SetDate(&RtcHandle,&sdatestructure, RTC_FORMAT_BCD) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler(); 
  } 
  
  /*##-4- Configure the Time #################################################*/
  /* Set Time: 08:10:00 */
  stimestructure.Hours = 0x08;
  stimestructure.Minutes = 0x10;
  stimestructure.Seconds = 0x00;
  stimestructure.TimeFormat = RTC_HOURFORMAT12_AM;
  stimestructure.DayLightSaving = RTC_DAYLIGHTSAVING_NONE ;
  stimestructure.StoreOperation = RTC_STOREOPERATION_RESET;
  
  if(HAL_RTC_SetTime(&RtcHandle,&stimestructure, RTC_FORMAT_BCD) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler(); 
  }
}

/**
  * @brief  Tamper callback 
  * @param  hrtc : hrtc handle
  * @retval None
  */
void HAL_RTCEx_TimeStampEventCallback(RTC_HandleTypeDef *hrtc)
{
   RTC_DateTypeDef sTimeStampDateget;
   RTC_TimeTypeDef sTimeStampget;
  
  /* Clear the Tamper Flag */
  __HAL_RTC_TAMPER_CLEAR_FLAG(hrtc,RTC_FLAG_TAMP1F);
  
  HAL_RTCEx_GetTimeStamp(&RtcHandle, &sTimeStampget, &sTimeStampDateget, RTC_FORMAT_BIN);

#ifdef DISPLAY_ON_DUBUGGER  
  /* Display time Format : hh:mm:ss */
  sprintf((char*)aShowTimeStamp,"%.2d:%.2d:%.2d", sTimeStampget.Hours, sTimeStampget.Minutes, sTimeStampget.Seconds);
  /* Display date Format : mm-dd */
  sprintf((char*)aShowDateStamp,"%.2d-%.2d-%.2d", sTimeStampDateget.Month, sTimeStampDateget.Date, 2014);
#endif
}

/**
  * @brief  Display the current time and date.
  * @param  showtime : pointer to buffer
  * @param  showdate : pointer to buffer
  * @retval None
  */
static void RTC_CalendarShow(void)
{
  RTC_DateTypeDef sdatestructureget;
  RTC_TimeTypeDef stimestructureget;
  
  /* Get the RTC current Time */
  HAL_RTC_GetTime(&RtcHandle, &stimestructureget, RTC_FORMAT_BIN);
  /* Get the RTC current Date */
  HAL_RTC_GetDate(&RtcHandle, &sdatestructureget, RTC_FORMAT_BIN);
  
#ifdef DISPLAY_ON_DUBUGGER
  /* Display time Format : hh:mm:ss */
  sprintf((char*)aShowTime,"%.2d:%.2d:%.2d", stimestructureget.Hours, stimestructureget.Minutes, stimestructureget.Seconds);
  /* Display date Format : mm-dd-yy */
  sprintf((char*)aShowDate,"%.2d-%.2d-%.2d", sdatestructureget.Month, sdatestructureget.Date, 2000 + sdatestructureget.Year);
#endif
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
    Error_Handler();
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
