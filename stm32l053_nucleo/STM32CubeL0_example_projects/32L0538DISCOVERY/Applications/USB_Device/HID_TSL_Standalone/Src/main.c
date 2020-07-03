/**
  ******************************************************************************
  * @file    USB_Device/HID_TSL_Standalone/Src/main.c
  * @author  MCD Application Team 
  * @brief   USB device HID demo main file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2016 STMicroelectronics International N.V. All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"  

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* define for the linear */
#define LINEAR_DETECT ((MyLinRots[0].p_Data->StateId == TSL_STATEID_DETECT) || \
                       (MyLinRots[0].p_Data->StateId == TSL_STATEID_DEB_RELEASE_DETECT))
#define LINEAR_POSITION (MyLinRots[0].p_Data->Position)

#define CURSOR_STEP     3
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
USBD_HandleTypeDef USBD_Device;
/* TSC handler declaration */
TSC_HandleTypeDef TscHandle;
/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void TSL_Config(void);
static void Process_Sensors(tsl_user_status_t status);
static void GetPointerData(uint8_t *pbuf);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  tsl_user_status_t tsl_status;
  
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
  
  /* Configure the system clock to get correspondent USB clock source */
  SystemClock_Config();
  
  /* Configure Key button for remote wakeup */
  BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_EXTI);
  
  /* Init Device Library */
  USBD_Init(&USBD_Device, &HID_Desc, 0);
  
  /* Register the HID class */
  USBD_RegisterClass(&USBD_Device, &USBD_HID);
  
  /* Start Device Process */
  USBD_Start(&USBD_Device);

  /* Configure the TSL*/
  TSL_Config();

  /* In an infinite loop, send periodically host mouse pointer position (emulated) */  
  while (1)
  {
    /* Execute STMTouch Driver state machine */
    tsl_status = tsl_user_Exec();
    if (tsl_status != TSL_USER_STATUS_BUSY)
    {
      Process_Sensors(tsl_status);
    }
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow:
  *         HSI48 used as USB clock source 
  *              - System Clock source            = HSI
  *              - HSI Frequency(Hz)              = 16000000
  *              - SYSCLK(Hz)                     = 16000000
  *              - HCLK(Hz)                       = 16000000
  *              - AHB Prescaler                  = 1
  *              - APB1 Prescaler                 = 1
  *              - APB2 Prescaler                 = 1
  *              - Flash Latency(WS)              = 0
  *              - Main regulator output voltage  = Scale1 mode
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;
  static RCC_CRSInitTypeDef RCC_CRSInitStruct;
  
  /* Enable HSI Oscillator to be used as System clock source
     Enable HSI48 Oscillator to be used as USB clock source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_HSI48;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  HAL_RCC_OscConfig(&RCC_OscInitStruct); 
 
  /* Select HSI48 as USB clock source */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInitStruct.UsbClockSelection = RCC_USBCLKSOURCE_HSI48;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
  
  /* Select HSI as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clock dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0);
    
  /*Configure the clock recovery system (CRS)**********************************/
  
  /*Enable CRS Clock*/
  __HAL_RCC_CRS_CLK_ENABLE();
  
  /* Default Synchro Signal division factor (not divided) */
  RCC_CRSInitStruct.Prescaler = RCC_CRS_SYNC_DIV1;  
  /* Set the SYNCSRC[1:0] bits according to CRS_Source value */
  RCC_CRSInitStruct.Source = RCC_CRS_SYNC_SOURCE_USB;  
  /* HSI48 is synchronized with USB SOF at 1KHz rate */
  RCC_CRSInitStruct.ReloadValue =  __HAL_RCC_CRS_RELOADVALUE_CALCULATE(48000000, 1000);
  RCC_CRSInitStruct.ErrorLimitValue = RCC_CRS_ERRORLIMIT_DEFAULT;  
  /* Set the TRIM[5:0] to the default value*/
  RCC_CRSInitStruct.HSI48CalibrationValue = 0x20;   
  /* Start automatic synchronization */ 
  HAL_RCCEx_CRSConfig (&RCC_CRSInitStruct);
   
  /* Enable Power Controller clock */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* The voltage scaling allows optimizing the power consumption when the device is 
  clocked below the maximum system frequency, to update the voltage scaling value 
  regarding system frequency refer to product datasheet. */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
}

/**
  * @brief  Gets Pointer Data.
  * @param  pbuf: Pointer to report
  * @retval None
  */
static void GetPointerData(uint8_t *pbuf)
{
  int8_t  x = 0, y = 0 ;
  
  if (LINEAR_POSITION < 4)
  {
    /* Mouse Cursor move to the LEFT*/
    x -= CURSOR_STEP;
  }
  
  else if (LINEAR_POSITION < 8)
  {
    /* Mouse Cursor move to the RIGHT*/
    x += CURSOR_STEP;
  }
  else if (LINEAR_POSITION < 12)
  {
    /* Mouse Cursor move to the UP*/
    y -= CURSOR_STEP;
  }
  else
  {
    /* Mouse Cursor move to the DOWN*/
    y += CURSOR_STEP;
  }
  
  pbuf[0] = 0;
  pbuf[1] = x;
  pbuf[2] = y;
  pbuf[3] = 0;
}

/**
  * @brief  Display sensors information on LEDs
  * @param  status TSL user status
  * @retval None
  */
static void Process_Sensors(tsl_user_status_t status)
{
	uint8_t HID_Buffer[4];

  if (LINEAR_DETECT)
  {
    GetPointerData(HID_Buffer);
    /* send data though IN endpoint*/
    if((HID_Buffer[1] != 0) || (HID_Buffer[2] != 0))
    {
      USBD_HID_SendReport(&USBD_Device, HID_Buffer, 4);
    }
  }
}

/**
  * @brief  TSL Configuration
  *         Configure the TSC peripheral
  * @param  None
  * @retval None
  */
static void TSL_Config(void)
{
  /* Configure the TSC peripheral */
  TscHandle.Instance = TSC;
  TscHandle.Init.AcquisitionMode         = TSC_ACQ_MODE_NORMAL;
  TscHandle.Init.CTPulseHighLength       = TSC_CTPH_2CYCLES;
  TscHandle.Init.CTPulseLowLength        = TSC_CTPL_2CYCLES;
  TscHandle.Init.IODefaultMode           = TSC_IODEF_OUT_PP_LOW;
  TscHandle.Init.MaxCountInterrupt       = DISABLE;
  TscHandle.Init.MaxCountValue           = TSC_MCV_8191;
  TscHandle.Init.PulseGeneratorPrescaler = TSC_PG_PRESC_DIV2;
  TscHandle.Init.SpreadSpectrum          = DISABLE;
  TscHandle.Init.SpreadSpectrumDeviation = 127;
  TscHandle.Init.SpreadSpectrumPrescaler = TSC_SS_PRESC_DIV1;
  TscHandle.Init.SynchroPinPolarity      = TSC_SYNC_POLARITY_FALLING;
  /* All channel, shield and sampling IOs must be declared below */
  TscHandle.Init.ChannelIOs              = TSC_GROUP1_IO3 | TSC_GROUP2_IO3 | TSC_GROUP3_IO2;
  TscHandle.Init.SamplingIOs             = TSC_GROUP1_IO4 | TSC_GROUP2_IO4 | TSC_GROUP3_IO3;
  TscHandle.Init.ShieldIOs               = 0;
  HAL_TSC_Init(&TscHandle);

  /* Initialize the STMTouch driver */
  tsl_user_Init();
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
