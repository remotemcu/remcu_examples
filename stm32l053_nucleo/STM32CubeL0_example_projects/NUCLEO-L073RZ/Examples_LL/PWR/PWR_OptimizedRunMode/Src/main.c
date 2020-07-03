/**
  ******************************************************************************
  * @file    Examples_LL/PWR/PWR_OptimizedRunMode/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to enter or exit Low Power Run mode and update
  *          the core frequency on the fly through the STM32L0xx PWR LL API.
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

/** @addtogroup STM32L0xx_LL_Examples
  * @{
  */

/** @addtogroup PWR_OptimizedRunMode
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
#define USE_LED

typedef enum {
  RUN_MODE_DOWN_TO_4MHZ = 0,
  RUN_MODE_DOWN_TO_131KHZ  = 1,
  RUN_MODE_UP_TO_4MHZ   = 2,
  RUN_MODE_UP_TO_32MHZ   = 3
}RunMode_Typedef;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
RunMode_Typedef RunMode = RUN_MODE_DOWN_TO_4MHZ;
__IO uint8_t ubExecuteRunMode = 1;
#ifdef USE_LED  
__IO uint16_t uhLedBlinkSpeed = LED_BLINK_FAST;
#endif
/* Private function prototypes -----------------------------------------------*/
void     SystemClock_Config(void);
void     Configure_PWR(void);
#ifdef USE_LED
void     LED_Init(void);
void     LED_Blinking(void);
#endif
void     UserButton_Init(void);
void     EnterRunMode_DownTo4MHz(void);
void     EnterRunMode_LowPower_DownTo131KHz(void);
void     EnterRunMode_UpTo4MHz(void);
void     EnterRunMode_UpTo32MHz(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* Configure the system clock to 32 MHz */
  SystemClock_Config();

#ifdef USE_LED
  /* Initialize LED2 */
  LED_Init();
#endif

  /* Initialize User push-button */
  UserButton_Init();

  /* Configure Power IP */
  Configure_PWR();

  while(ubExecuteRunMode)
  {
#ifdef USE_LED   
    /* Led blinking until User push-button action */
    LED_Blinking();
#endif
  }

  /* Infinite loop */
  while (1)
  {
#ifdef USE_LED   
    /* Led blinking in infinite loop*/
    LED_Blinking();
#endif
  }
}

#ifdef USE_LED
/**
  * @brief  Initialize LED2.
  * @param  None
  * @retval None
  */
void LED_Init(void)
{
  /* Enable the LED2 Clock */
  LED2_GPIO_CLK_ENABLE();

  /* Configure IO in output push-pull mode to drive external LED2 */
  LL_GPIO_SetPinMode(LED2_GPIO_PORT, LED2_PIN, LL_GPIO_MODE_OUTPUT);
  /* Reset value is LL_GPIO_OUTPUT_PUSHPULL */
  //LL_GPIO_SetPinOutputType(LED2_GPIO_PORT, LED2_PIN, LL_GPIO_OUTPUT_PUSHPULL);
  /* Reset value is LL_GPIO_SPEED_FREQ_LOW */
  //LL_GPIO_SetPinSpeed(LED2_GPIO_PORT, LED2_PIN, LL_GPIO_SPEED_FREQ_LOW);
  /* Reset value is LL_GPIO_PULL_NO */
  //LL_GPIO_SetPinPull(LED2_GPIO_PORT, LED2_PIN, LL_GPIO_PULL_NO);
}

/**
  * @brief  Set LED2 to Blinking mode (Shall be call in a Loop).
  * @param  None
  * @retval None
  */
void LED_Blinking(void)
{
  /* Toggle IO. Shall be call in a loop to toggle */
  LL_GPIO_TogglePin(LED2_GPIO_PORT, LED2_PIN);
  LL_mDelay(uhLedBlinkSpeed);
}
#endif

/**
  * @brief  Configures User push-button in GPIO or EXTI Line Mode.
  * @param  None
  * @retval None
  */
void UserButton_Init(void)
{
  /* Enable the BUTTON Clock */
  USER_BUTTON_GPIO_CLK_ENABLE();

  /* Configure GPIO for BUTTON */
  LL_GPIO_SetPinMode(USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN, LL_GPIO_MODE_INPUT);
  LL_GPIO_SetPinPull(USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN, LL_GPIO_PULL_NO);

  /* Connect External Line to the GPIO*/
  USER_BUTTON_SYSCFG_SET_EXTI();

  /* Enable a rising trigger External lines 4 to 15 Interrupt */
  USER_BUTTON_EXTI_LINE_ENABLE();
  USER_BUTTON_EXTI_FALLING_TRIG_ENABLE();

  /* Configure NVIC for USER_BUTTON_EXTI_IRQn */
  NVIC_EnableIRQ(USER_BUTTON_EXTI_IRQn); 
  NVIC_SetPriority(USER_BUTTON_EXTI_IRQn,0x03);
}

/**
  * @brief  Function to configure and initialize PWR IP.
  * @param  None
  * @retval None
  */
void Configure_PWR(void)
{
  /* Enable Power Clock */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
}

/**
  * @brief  Function to decrease Frequency at 4MHz in Run Mode.
  * @param  None
  * @retval None
  */
void EnterRunMode_DownTo4MHz(void)
{
  /* 1 - Set Frequency to 4MHz (MSI) to set VOS to Range 2 */
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_MSI);    
  /* Disable PLL to decrease power consumption */
  LL_RCC_PLL_Disable();
  LL_RCC_MSI_SetRange(LL_RCC_MSIRANGE_6);
  /* Switch clock source on MSI */
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_MSI);    
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_MSI) 
  {
  };
  
  /* Disable HSI and wait for deactivation */
  LL_RCC_HSI_Disable();
  while(LL_RCC_HSI_IsReady() != 0) 
  {
  };
  
  /* Disable PLL and wait for deactivation */
  LL_RCC_PLL_Disable();
  while(LL_RCC_PLL_IsReady() != 0) 
  {
  };

  /* Set systick to 1ms in using frequency set to 4MHz */
  LL_Init1msTick(4 * 1000000);
  /* Update CMSIS variable */
  LL_SetSystemCoreClock(4 * 1000000);

  /* 2 - Adjust Flash Wait state after decrease Clock Frequency */
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);

  /* 3 - Set Voltage scaling to LL_PWR_REGU_VOLTAGE_SCALE3. Decrease VCore  */
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE3);
}

/**
  * @brief  Function to decrease Frequency at 131KHZ in Low Power Run Mode.
  * @param  None
  * @retval None
  */
void EnterRunMode_LowPower_DownTo131KHz(void)
{
  /* 1 - Set Frequency to 131KHz to activate Low Power Run Mode: 131KHz */
  LL_RCC_MSI_SetRange(LL_RCC_MSIRANGE_1);
  /* Set systick to 1ms in using frequency set to 131KHz */
  LL_Init1msTick(131 * 1000);
  /* Update CMSIS variable */
  LL_SetSystemCoreClock(131 * 1000);

  /* 2 - Adjust Flash Wait state after decrease Clock Frequency */
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);

  /* Voltage Scaling already set to LL_PWR_REGU_VOLTAGE_SCALE3. VCore already decreased */

  /* 3 - Activate Low Power Run Mode */
  /* Enable ultra low power mode */
  LL_PWR_EnableUltraLowPower();
  LL_PWR_EnterLowPowerRunMode();
}

/**
  * @brief  Function to increase Frequency at 4MHz in Run Mode.
  * @param  None
  * @retval None
  */
void EnterRunMode_UpTo4MHz(void)
{
  /* 1 - Deactivate Low Power Run Mode to increase Frequency up to 4MHz */
  /* Disable ultra low power mode */
  LL_PWR_DisableUltraLowPower();
  LL_PWR_ExitLowPowerRunMode();

  /* Voltage Scaling already set to LL_PWR_REGU_VOLTAGE_SCALE3. VCore already decreased */

  /* 2 - Adjust Flash Wait state before increase Clock Frequency */
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);

  /* 3 - Set Frequency to 4MHz (MSI) */
  LL_RCC_MSI_SetRange(LL_RCC_MSIRANGE_6);
  /* Set systick to 1ms in using frequency set to 4MHz */
  LL_Init1msTick(4 * 1000000);
  /* Update CMSIS variable */
  LL_SetSystemCoreClock(4 * 1000000);
}

/**
  * @brief  Function to decrease Frequency at 32MHz in Run Mode.
  * @param  None
  * @retval None
  */
void EnterRunMode_UpTo32MHz(void)
{
  /* 1 - Set Voltage scaling to LL_PWR_REGU_VOLTAGE_SCALE1 before increase Clock Frequency */
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);

  /* 2 - Wait Voltage Scaling 1 before increase frequency */
  while(LL_PWR_IsActiveFlag_VOS() !=  0)
  {
  };

  /* 3 - Adjust Flash Wait state before increase Clock Frequency */
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);

  /* 4 - Set Frequency to 32MHz (PLL) */
  /* Enable HSI and wait for activation */
  LL_RCC_HSI_Enable();
  while(LL_RCC_HSI_IsReady() != 1) 
  {
  };

  /* Enable PLL and wait for activation */ 
  LL_RCC_PLL_Enable();
  while(LL_RCC_PLL_IsReady() != 1) 
  {
  };

  /* Switch on PLL. Previous configuration done by SystemClock_Config is used */
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL) 
  {
  };

  /* Disable MSI and wait for deactivation */
  LL_RCC_MSI_Disable();
  while(LL_RCC_MSI_IsReady() != 0) 
  {
  };
  
  /* Set systick to 1ms in using frequency set to 32MHz */
  LL_Init1msTick(32 * 1000000);

  /* Update CMSIS variable */
  LL_SetSystemCoreClock(32 * 1000000);
}

/**
  * @brief  System Clock Configuration
      *         The system Clock is configured as follows :
      *            System Clock source            = PLL (HSI)
      *            SYSCLK(Hz)                     = 32000000
      *            HCLK(Hz)                       = 32000000
      *            AHB Prescaler                  = 1
      *            APB1 Prescaler                 = 4
      *            APB2 Prescaler                 = 2
      *            HSI Frequency(Hz)              = 16000000
      *            PLL_MUL                        = 4
      *            PLL_DIV                        = 2
      *            Flash Latency(WS)              = 1
  * @retval None
  */
void SystemClock_Config(void)
{
    LL_RCC_PLL_Disable();
    /* Set new latency */
    LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);
 
    /* HSI configuration and activation */
    LL_RCC_HSI_Enable();
    LL_RCC_HSI_DisableDivider();
    while(LL_RCC_HSI_IsReady() != 1) 
    {
    };
    
    /* Main PLL configuration and activation */
    LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI, LL_RCC_PLL_MUL_4, LL_RCC_PLL_DIV_2);

    LL_RCC_PLL_Enable();
    while(LL_RCC_PLL_IsReady() != 1) 
    {
    };
    
    /* Sysclk activation on the main PLL */
    LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);

    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

    while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)  
    {
    };
    
    /* Set APB1 & APB2 prescaler*/
    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_4);
    LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_2);

    /* Set systick to 1ms in using frequency set to 32MHz */
    LL_Init1msTick(32000000);
  
    /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
    LL_SetSystemCoreClock(32000000);    
}
/******************************************************************************/
/*   USER IRQ HANDLER TREATMENT                                               */
/******************************************************************************/
/**
  * @brief  Function to manage BUTTON IRQ Handler
  * @param  None
  * @retval None
  */
void UserButton_Callback(void)
{
  switch(RunMode)
  {
  case RUN_MODE_DOWN_TO_4MHZ:
    {
      /* Decrease core frequency and voltage
       * Frequency: 32MHz -> 4MHz
       * Voltage Scaling Range 2
       */
      EnterRunMode_DownTo4MHz();
#ifdef USE_LED
      uhLedBlinkSpeed = LED_BLINK_MEDIUM;
#endif
      /* Set Next RunMode to execute */
      RunMode = RUN_MODE_DOWN_TO_131KHZ;
      break;
    }
  case RUN_MODE_DOWN_TO_131KHZ:
    {
      /* Decrease core frequency and enter Low Power Run mode
       * Frequency: 131KHz
       * Voltage Scaling Range 2
       * LowPowerRunMode activated
       */
      EnterRunMode_LowPower_DownTo131KHz();
#ifdef USE_LED
      uhLedBlinkSpeed = LED_BLINK_SLOW;
#endif
      /* Set Next RunMode to execute */
      RunMode = RUN_MODE_UP_TO_4MHZ;
      break;
    }
  case RUN_MODE_UP_TO_4MHZ:
    {
      /* Increase core frequency and exit Low Power Run mode 
       * Frequency: 131KHz -> 4MHz
       * Voltage Scaling Range 2
       * LowPowerRunMode deactivated
       */
      EnterRunMode_UpTo4MHz();
#ifdef USE_LED
      uhLedBlinkSpeed = LED_BLINK_MEDIUM;
#endif
      /* Set Next RunMode to execute */
      RunMode = RUN_MODE_UP_TO_32MHZ;
      break;
    }
  case RUN_MODE_UP_TO_32MHZ:
    {
      /* Increase core frequency and voltage:
       * Frequency: 4MHz -> 32MHz
       * Voltage Scaling Range 1
       */
      EnterRunMode_UpTo32MHz();
#ifdef USE_LED
      uhLedBlinkSpeed = LED_BLINK_FAST;
#endif
      /* Exit Test */
      NVIC_DisableIRQ(USER_BUTTON_EXTI_IRQn); 
      USER_BUTTON_EXTI_LINE_DISABLE();
      
      ubExecuteRunMode = 0;
      break;
    }
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
     ex: printf("Wrong parameters value: file %s on line %d", file, line) */

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
