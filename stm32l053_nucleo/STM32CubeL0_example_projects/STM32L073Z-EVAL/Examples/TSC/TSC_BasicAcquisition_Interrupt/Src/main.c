/**
  ******************************************************************************
  * @file    TSC/TSC_BasicAcquisition_Interrupt/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to use the TSC to perform an acquisition
  *          of two channels in interrupt mode.
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

/** @addtogroup TSC_BasicAcquisition_Interrupt
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* These values can be modified, board layout dependency */
#define TSCx_TS1_MINTHRESHOLD 400
#define TSCx_TS1_MAXTHRESHOLD 466
#define TSCx_TS2_MINTHRESHOLD 400 
#define TSCx_TS2_MAXTHRESHOLD 485
#define TSCx_TS3_MINTHRESHOLD 400 
#define TSCx_TS3_MAXTHRESHOLD 480

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* TSC handler declaration */
TSC_HandleTypeDef TscHandle;

/* Array used to store the three acquisition value (one per channel) */
__IO uint32_t uhTSCAcquisitionValue[3];

uint8_t IdxBank = 0;
TSC_IOConfigTypeDef IoConfig;
  
/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void Error_Handler(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{ 
  /* STM32L0xx HAL library initialization:
       - Configure the Flash prefetch
       - Systick timer is configured by default as source of time base, but user 
         can eventually implement his proper time base source (a general purpose 
         timer for example or other time source), keeping in mind that Time base 
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
         handled in milliseconds basis.
       - Low Level Initialization
     */
  HAL_Init();
  
  /* Configure the system clock to 32 MHz */
  SystemClock_Config();

  /* Configure LEDs */
  BSP_LED_Init(LED4);
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED3);

  /*##-1- Configure the TSC peripheral #######################################*/
  TscHandle.Instance                     = TSCx;
  TscHandle.Init.AcquisitionMode         = TSC_ACQ_MODE_NORMAL;
  TscHandle.Init.CTPulseHighLength       = TSC_CTPH_1CYCLE;
  TscHandle.Init.CTPulseLowLength        = TSC_CTPL_1CYCLE;
  TscHandle.Init.IODefaultMode           = TSC_IODEF_IN_FLOAT;
  TscHandle.Init.MaxCountInterrupt       = ENABLE;
  TscHandle.Init.MaxCountValue           = TSC_MCV_16383;
  TscHandle.Init.PulseGeneratorPrescaler = TSC_PG_PRESC_DIV64;
  TscHandle.Init.SpreadSpectrum          = DISABLE;
  TscHandle.Init.SpreadSpectrumDeviation = 127;
  TscHandle.Init.SpreadSpectrumPrescaler = TSC_SS_PRESC_DIV1;
  TscHandle.Init.SynchroPinPolarity      = TSC_SYNC_POLARITY_FALLING;
  TscHandle.Init.ChannelIOs              = 0; /* Not needed yet. Will be set with HAL_TSC_IOConfig() */
  TscHandle.Init.SamplingIOs             = 0; /* Not needed yet. Will be set with HAL_TSC_IOConfig() */
  TscHandle.Init.ShieldIOs               = 0; /* Not needed yet. Will be set with HAL_TSC_IOConfig() */

  if (HAL_TSC_Init(&TscHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /*##-2- Configure the touch-sensing IOs ####################################*/
  IoConfig.ChannelIOs  = TSC_GROUP6_IO2; /* Start with the first channel */
  IoConfig.SamplingIOs = (TSC_GROUP6_IO3 | TSC_GROUP3_IO3 | TSC_GROUP1_IO3 | TSC_GROUP8_IO2);
  IoConfig.ShieldIOs   = TSC_GROUP8_IO1;
  
  if (HAL_TSC_IOConfig(&TscHandle, &IoConfig) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /*##-3- Discharge the touch-sensing IOs ####################################*/
  HAL_TSC_IODischarge(&TscHandle, ENABLE);
  HAL_Delay(1); /* 1 ms is more than enough to discharge all capacitors */

  /*##-4- Start the acquisition process ######################################*/
  if (HAL_TSC_Start_IT(&TscHandle) != HAL_OK)
  {
    /* Acquisition Error */
    Error_Handler();
  }
  
  /* Infinite loop */
  while (1)
  {
	  /*Suspend Tick increment to prevent wakeup by Systick interrupt. 
      Otherwise the Systick interrupt will wake up the device within 1ms (HAL time base)*/
    HAL_SuspendTick();
  	
    /* The acquisition process is now performed in the HAL_TSC_ConvCpltCallback() function */
    HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
    
    /* Resume Tick interrupt if disabled prior to sleep mode entry*/
    HAL_ResumeTick();
  }
}

/**
  * @brief  Acquisition completed callback in non blocking mode 
  * @param  htsc: pointer to a TSC_HandleTypeDef structure that contains
  *         the configuration information for the specified TSC.
  * @retval None
  */
void HAL_TSC_ConvCpltCallback(TSC_HandleTypeDef* htsc)
{  
  /*##-5- Discharge the touch-sensing IOs ####################################*/
  HAL_TSC_IODischarge(&TscHandle, ENABLE);
  /* Note: a delay can be added here */

  /*##-6- Check if the acquisition is correct (no max count) #################*/
  if (HAL_TSC_GroupGetStatus(&TscHandle, TSC_GROUP6_IDX) == TSC_GROUP_COMPLETED)
  {
    /*##-7- Read the acquisition value #######################################*/
    uhTSCAcquisitionValue[IdxBank] = HAL_TSC_GroupGetValue(&TscHandle, TSC_GROUP6_IDX);  
    /* Note: Show the touch activity on LEDs.
       The threshold values are indicative and may need to be adjusted */
    if ((uhTSCAcquisitionValue[0] > TSCx_TS1_MINTHRESHOLD) && (uhTSCAcquisitionValue[0] < TSCx_TS1_MAXTHRESHOLD)) // Channel 1
    {
      BSP_LED_On(LED4);
    }
    else
    {
      BSP_LED_Off(LED4);
    }
  }

  if (HAL_TSC_GroupGetStatus(&TscHandle, TSC_GROUP3_IDX) == TSC_GROUP_COMPLETED)
  {
    /*##-7- Read the acquisition value #######################################*/
    uhTSCAcquisitionValue[IdxBank] = HAL_TSC_GroupGetValue(&TscHandle, TSC_GROUP3_IDX);  
    /* Note: Show the touch activity on LEDs.
       The threshold values are indicative and may need to be adjusted */
    if ((uhTSCAcquisitionValue[1] > TSCx_TS2_MINTHRESHOLD) && (uhTSCAcquisitionValue[1] < TSCx_TS2_MAXTHRESHOLD)) // Channel 1
    {
      BSP_LED_On(LED3);
    }
    else
    {
      BSP_LED_Off(LED3);
    }
  }

  if (HAL_TSC_GroupGetStatus(&TscHandle, TSC_GROUP1_IDX) == TSC_GROUP_COMPLETED)
  {
    /*##-7- Read the acquisition value #######################################*/
    uhTSCAcquisitionValue[IdxBank] = HAL_TSC_GroupGetValue(&TscHandle, TSC_GROUP1_IDX);  
    /* Note: Show the touch activity on LEDs.
       The threshold values are indicative and may need to be adjusted */
    if ((uhTSCAcquisitionValue[2] > TSCx_TS3_MINTHRESHOLD) && (uhTSCAcquisitionValue[2] < TSCx_TS3_MAXTHRESHOLD)) // Channel 1
    {
      BSP_LED_On(LED2);
    }
    else
    {
      BSP_LED_Off(LED2);
    }
  }

  /*##-8- Configure the next channels to be acquired #########################*/
  switch (IdxBank)
  {
    case 0:
      IoConfig.ChannelIOs = TSC_GROUP3_IO2; /* Second channel */
      IdxBank = 1;
      break;
    case 1:
      IoConfig.ChannelIOs = TSC_GROUP1_IO2; /* Third channel */
      IdxBank = 2;
      break;
    case 2:
      IoConfig.ChannelIOs = TSC_GROUP6_IO2; /* First channel */
      IdxBank = 0;
      break;
    default:
      break;
  }
  
  if (HAL_TSC_IOConfig(&TscHandle, &IoConfig) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
 
  /*##-9- Re-start the acquisition process ###################################*/
  if (HAL_TSC_Start_IT(&TscHandle) != HAL_OK)
  {
    /* Acquisition Error */
    Error_Handler();
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 32000000
  *            HCLK(Hz)                       = 32000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            Flash Latency(WS)              = 1
  *            Main regulator output voltage  = Scale1 mode
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct ={0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  
  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  
  /* Enable HSE Oscillator */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLSource   = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLState    = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLMUL      = RCC_PLL_MUL8;
  RCC_OscInitStruct.PLL.PLLDIV      = RCC_PLL_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct)!= HAL_OK)
  {
    /* Initialization Error */
    while(1)
    {}
  }
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1)!= HAL_OK)
  {
    /* Initialization Error */
    while(1)
    {}
  }
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
    BSP_LED_Toggle(LED3);
    HAL_Delay(200);
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
    BSP_LED_Toggle(LED3);
    HAL_Delay(100);
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
