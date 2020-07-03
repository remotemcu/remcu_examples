/**
  ******************************************************************************
  * @file    TIM/TIM_ExtTriggerSynchro/Src/main.c 
  * @author  MCD Application Team
  * @brief   This sample code shows how to synchronize TIM peripherals in cascade
  *          mode with an external trigger.
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

/** @addtogroup TIM_ExtTriggerSynchro
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Timer2 handler declaration: Slave for an external trigger/Master */
TIM_HandleTypeDef    TimMasterHandle;
/* Timer21 handler declaration: Slave/Master */
TIM_HandleTypeDef    TimSlaveMasterHandle;
/* Timer22 handler declaration: Slave */
TIM_HandleTypeDef    TimSlaveHandle;

/* Timer Output Compare Configuration Structure declaration */
TIM_OC_InitTypeDef   sOCConfig;
/* Timer Input Capture Configuration Structure declaration */
TIM_IC_InitTypeDef   sICConfig;

/* Timer Master Configuration Structure declaration */
TIM_MasterConfigTypeDef  sMasterConfig;
/* Timer Slave Configuration Structure declaration */
TIM_SlaveConfigTypeDef   sSlaveConfig;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void ErrorHandler(void);

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
  
  /* Configure the system clock */
  SystemClock_Config();
  
 /* Set Timers instance */
  TimMasterHandle.Instance      = TIM2;
  TimSlaveMasterHandle.Instance = TIM21;
  TimSlaveHandle.Instance       = TIM22;
 
  /*======= Master1/Slave for an external trigger configuration : TIM2 =======*/
  /* Initialize TIM2 peripheral in Output Compare mode*/
  TimMasterHandle.Init.Period            = 79;
  TimMasterHandle.Init.Prescaler         = 1;
  TimMasterHandle.Init.ClockDivision     = 0;
  TimMasterHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
  if(HAL_TIM_OC_Init(&TimMasterHandle) != HAL_OK)
  {
    /* Initialization Error */
    ErrorHandler();
  }  
  
  /* Configure the output: Channel_1 */
  sOCConfig.OCMode     = TIM_OCMODE_TOGGLE;
  sOCConfig.OCPolarity = TIM_OCPOLARITY_HIGH;    
  if(HAL_TIM_OC_ConfigChannel(&TimMasterHandle, &sOCConfig, TIM_CHANNEL_1) != HAL_OK)
  {
    /* Configuration Error */
    ErrorHandler();
  }

  /* Configure the Input: channel_2 */
  sICConfig.ICPolarity  = TIM_ICPOLARITY_RISING;
  sICConfig.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sICConfig.ICPrescaler = TIM_ICPSC_DIV1;
  sICConfig.ICFilter = 0;  
  if(HAL_TIM_IC_ConfigChannel(&TimMasterHandle, &sICConfig, TIM_CHANNEL_2) != HAL_OK)
  {
    /* Configuration Error */
    ErrorHandler();
  }
  
  /* Configure TIM2 in Gated Slave mode for the external trigger (Filtered Timer
     Input 2) */
  sSlaveConfig.InputTrigger = TIM_TS_TI2FP2;
  sSlaveConfig.SlaveMode    = TIM_SLAVEMODE_GATED;
  if( HAL_TIM_SlaveConfigSynchro(&TimMasterHandle, &sSlaveConfig) != HAL_OK)
  {
    /* Configuration Error */
    ErrorHandler();
  }
  
  /* Configure TIM2 in Master Enable mode & use the update event as Trigger
     Output (TRGO) */
  sMasterConfig.MasterSlaveMode     = TIM_MASTERSLAVEMODE_ENABLE;
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_ENABLE;
  if( HAL_TIMEx_MasterConfigSynchronization(&TimMasterHandle, &sMasterConfig) != HAL_OK)
  {
    /* Configuration Error */
    ErrorHandler();
  }  
  /*=== End of Master1/Slave for an external trigger configuration : TIM2 ====*/

  
  /*=================== Slave/Master configuration : TIM21 ====================*/
  /* Initialize TIM21 peripheral in Output Compare mode*/
  TimSlaveMasterHandle.Init.Period            = 79;
  TimSlaveMasterHandle.Init.Prescaler         = 1;
  TimSlaveMasterHandle.Init.ClockDivision     = 0;
  TimSlaveMasterHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
  if(HAL_TIM_OC_Init(&TimSlaveMasterHandle) != HAL_OK)
  {
    /* Initialization Error */
    ErrorHandler();
  }
  
  /* Configure the Output Compare channel_1  */
  sOCConfig.OCMode     = TIM_OCMODE_TOGGLE;
  sOCConfig.OCPolarity = TIM_OCPOLARITY_HIGH;
  if(HAL_TIM_OC_ConfigChannel(&TimSlaveMasterHandle, &sOCConfig, TIM_CHANNEL_1) != HAL_OK)
  {
    /* Configuration Error */
    ErrorHandler();
  }
  
  /* Configure TIM21 in Gated Slave mode for the internal trigger 0(ITR0) */  
  sSlaveConfig.InputTrigger = TIM_TS_ITR0;
  sSlaveConfig.SlaveMode    = TIM_SLAVEMODE_GATED;
  if( HAL_TIM_SlaveConfigSynchro(&TimSlaveMasterHandle, &sSlaveConfig) != HAL_OK)
  {
    /* Configuration Error */
    ErrorHandler();
  }
  
  /* Configure TIM21 in Master Enable mode & use the update event as Trigger
     Output (TRGO) */
  sMasterConfig.MasterSlaveMode     = TIM_MASTERSLAVEMODE_ENABLE;
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_ENABLE;
  if( HAL_TIMEx_MasterConfigSynchronization(&TimSlaveMasterHandle, &sMasterConfig) != HAL_OK)
  {
    /* Configuration Error */
    ErrorHandler();
  }
  /*=============== End of Slave/Master configuration : TIM21 =================*/
  
  
  /*====================== Slave configuration : TIM22 ========================*/
  /* Initialize TIM22 peripheral in Output Compare mode*/
  TimSlaveHandle.Init.Period            = 79;
  TimSlaveHandle.Init.Prescaler         = 1;
  TimSlaveHandle.Init.ClockDivision     = 0;
  TimSlaveHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
  if(HAL_TIM_OC_Init(&TimSlaveHandle) != HAL_OK)
  {
    /* Initialization Error */
    ErrorHandler();
  } 
 
  /* Configure the Output Compare channel_1  */
  sOCConfig.OCMode     = TIM_OCMODE_TOGGLE;
  sOCConfig.OCPolarity = TIM_OCPOLARITY_HIGH;
  if(HAL_TIM_OC_ConfigChannel(&TimSlaveHandle, &sOCConfig, TIM_CHANNEL_1) != HAL_OK)
  {
    /* Configuration Error */
    ErrorHandler();
  }  
  
  /* Configure TIM22 in Gated Slave mode for the internal trigger 2(ITR2) */ 
  sSlaveConfig.SlaveMode    = TIM_SLAVEMODE_GATED;
  sSlaveConfig.InputTrigger = TIM_TS_ITR0;
  if(HAL_TIM_SlaveConfigSynchro(&TimSlaveHandle, &sSlaveConfig) != HAL_OK)
  {
    /* Configuration Error */
    ErrorHandler();
  }  
  /*================== End of Slave configuration : TIM22 =====================*/
  
  
  /* 1- Start Timer1 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx */
  /* Start Channel2 in Input Capture */
  if(HAL_TIM_IC_Start(&TimMasterHandle, TIM_CHANNEL_2) != HAL_OK)
  {
    /* Start Error */
    ErrorHandler();
  }  
  /* Start the Output Compare */
  if(HAL_TIM_OC_Start(&TimMasterHandle, TIM_CHANNEL_1) != HAL_OK)
  {
    /* Start Error */
    ErrorHandler();
  }

  /* 2- Start Timer3 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx */
  /* Start the Output Compare */
  if(HAL_TIM_OC_Start(&TimSlaveMasterHandle, TIM_CHANNEL_1) != HAL_OK)
  {
    /* Start Error */
    ErrorHandler();
  }
  
  /* 3- Start Timer3 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx */
  /* Start the Output Compare */
  if(HAL_TIM_OC_Start(&TimSlaveHandle, TIM_CHANNEL_1) != HAL_OK)
  {
    /* Start Error */
    ErrorHandler();
  }
  
  /* Infinite loop */
  while (1)
  {
  }

}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSI)
  *            SYSCLK(Hz)                     = 32000000
  *            HCLK(Hz)                       = 32000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            HSI Frequency(Hz)              = 16000000
  *            PLL_MUL                        = 4
  *            PLL_DIV                        = 2
  *            Flash Latency(WS)              = 1
  *            Main regulator output voltage  = Scale1 mode
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
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void ErrorHandler(void)
{
  /* Infinite loop */
  while(1)
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
#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
