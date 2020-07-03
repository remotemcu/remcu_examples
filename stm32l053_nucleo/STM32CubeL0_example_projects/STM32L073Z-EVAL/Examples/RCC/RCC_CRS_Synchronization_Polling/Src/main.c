/**
  ******************************************************************************
  * @file    RCC/RCC_CRS_Synchronization_Polling/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to use the RCC HAL API to configure the
  *          system clock (SYSCLK) and modify the clock settings on run time.
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

/** @addtogroup RCC_CRS_Synchronization_Polling
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void CRS_Init(void);
void SystemClock_Config(void);
static void Error_Handler(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  uint32_t status = RCC_CRS_TIMEOUT;
  RCC_CRSSynchroInfoTypeDef syncinfo = {0};
  RCC_CRSInitTypeDef crsinitstruct = {0};

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

  /* Enable HSE oscillator and configure the PLL to reach the max system frequency (32 MHz)
     when using HSE oscillator as PLL clock source. */
  SystemClock_Config();

  /* Configure LED_GREEN and LED3 */
  BSP_LED_Init(LED_GREEN);
  BSP_LED_Init(LED3);

 /* CRS initialization (enable HSI48 and LSE oscillators and then enable CRS clock */
  CRS_Init();
  
  /* HSI48 Synchronization with LSE frequency */
  crsinitstruct.Prescaler             = RCC_CRS_SYNC_DIV1;
  crsinitstruct.Source                = RCC_CRS_SYNC_SOURCE_LSE;
  crsinitstruct.Polarity              = RCC_CRS_SYNC_POLARITY_FALLING;
  /* F(LSE)=32.768kHz then Reload=(f(Target)/f(LSE))-1= 0x5B7*/
  crsinitstruct.ReloadValue           = __HAL_RCC_CRS_RELOADVALUE_CALCULATE(HSI48_VALUE, LSE_VALUE);
  /* Felim value calculated like this FELIM = (fTARGET / fSYNC) * STEP[%] / 100% / 2 with STEP=0.14% then FELIM = 2 */
  crsinitstruct.ErrorLimitValue       = 2;
  /* Change the HSI trimming value to see the automatic calibration performed by CRS */
  crsinitstruct.HSI48CalibrationValue = 0x00;

  /* Start automatic synchronization using polling mode */
  HAL_RCCEx_CRSConfig(&crsinitstruct);
  
  /* Wait for synchronization OK event */
  do 
  {
    /* Check status of CRS synchronization */ 
    /* Timeout is based on F(LSE) then less than 1 ms*/
    status = HAL_RCCEx_CRSWaitSynchronization(1);
    
    if ((status & RCC_CRS_TIMEOUT) == RCC_CRS_TIMEOUT) 
    {
      /* Timeout issue. May have a problem with synchronization frequency */
      Error_Handler(); 
    }
  
    /* Adapt synchronization input parameters in case of SYNC error or SYNC miss event */
    if (((status & RCC_CRS_SYNCERR) == RCC_CRS_SYNCERR) || ((status & RCC_CRS_SYNCMISS) == RCC_CRS_SYNCMISS)) 
    {
      HAL_RCCEx_CRSGetSynchronizationInfo(&syncinfo);
      
      /* User can check different parameters returned in synchronization info structure*/
      /* and restart a new synchronization in changing input parameters */
      
      HAL_RCCEx_CRSConfig(&crsinitstruct);
    }
  }
  while((status & RCC_CRS_SYNCOK) != RCC_CRS_SYNCOK);
  
  /* Power on LED_GREEN */
  BSP_LED_On(LED_GREEN);
  
  /* Infinite loop */
  while (1)
  {
  }
}

/**
  * @brief  This function initializes the clock configuration for CRS.
  * @param  None
  * @retval None
  */
static void CRS_Init(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct =  {0};
  
  HAL_RCC_GetOscConfig(&RCC_OscInitStruct);

  if (RCC_OscInitStruct.HSI48State != RCC_HSI48_ON)
  {
    /* Enable HSI48 and LSE Oscillator*/
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48 | RCC_OSCILLATORTYPE_LSE;
    RCC_OscInitStruct.HSI48State     = RCC_HSI48_ON;
    RCC_OscInitStruct.LSEState       = RCC_LSE_ON;
    RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_NONE;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
      Error_Handler();
    }
  }
  
  /* Enable CRS clock*/
  __HAL_RCC_CRS_CLK_ENABLE();

  /* Output HSI48 MCO pin(PA8) */
  HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_HSI48, RCC_MCODIV_1);
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
  /* Turn LED3 on */
  BSP_LED_On(LED3);

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
#endif

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
