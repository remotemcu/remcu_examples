/**
  ******************************************************************************
  * @file    Examples_LL/CRS/CRS_Synchronization_IT/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to configure Clock Recovery Service in 
  *          IT mode through the STM32L0xx CRS LL API.
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

/** @addtogroup CRS_Synchronization_IT
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define CRS_STATUS_INIT       0
#define CRS_STATUS_SYNCOK     1
#define CRS_STATUS_SYNCWARN   2
#define CRS_STATUS_SYNCMISS   3
#define CRS_STATUS_SYNCERR    4

/* Oscillator time-out values */
#define LSE_TIMEOUT_VALUE     ((uint32_t)5000)  /* 5 s */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint8_t ubButtonPress = 0;

/* Variable to save SYNC status*/
__IO uint32_t CRSSyncStatus = CRS_STATUS_INIT;

/* Variable used for Timeout management */
#if (USE_TIMEOUT == 1)
uint32_t Timeout = 0; 
#endif /* USE_TIMEOUT */

/* Private function prototypes -----------------------------------------------*/
void     SystemClock_Config(void);
void     Enable_Sync_Reference_Clock(void);
void     Enable_CRS_Clocks(void);
void     Configure_CRS(uint32_t SyncSource);
void     MCO_ConfigGPIO(void);
void     LED_Init(void);
void     LED_On(void);
void     LED_Off(void);
void     LED_Blinking(uint32_t Period);
void     UserButton_Init(void);
void     WaitForUserButtonPress(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  uint32_t felim = 0;
  
  /* Configure the system clock to 2.097 MHz */
  SystemClock_Config();

  /* Initialize LED2 */
  LED_Init();
  
  /* Initialize button in EXTI mode */
  UserButton_Init();
  
  /* Enable the reference clock used for synchronization (LSE) */
  Enable_Sync_Reference_Clock();

  /* Enable HSI48 and CRS */
  Enable_CRS_Clocks();
  
  /* For monitoring output HSI48 MCO pin(PA8) */
  MCO_ConfigGPIO();

  /* Wait for User push-button press to start transfer */
  WaitForUserButtonPress();
  
  /* Configure CRS with LSE parameters                                */ 
  /* but set the source to external via GPIO (which is not connected) */
  Configure_CRS(LL_CRS_SYNC_SOURCE_GPIO);
  
  /* Simulate a SYNC EVENT which will raise a SYNC_MISS EVENT */ 
  /* because no external sync clock is connected to GPIO      */
  LL_CRS_GenerateEvent_SWSYNC();

  /* Infinite loop */
  while (1)
  {
    switch(CRSSyncStatus)
    {
    case CRS_STATUS_SYNCOK:
      /* Power on LED2 */
      LED_On();
      /* For visual comfort on LED2 apply delay */
      LL_mDelay(500);
      /* Enable CRS SYNCWARN interrupt (in case it goes out of sync) */
      LL_CRS_EnableIT_SYNCWARN();
      break;

    case CRS_STATUS_SYNCMISS:
      /* Power off LED2 */
      LED_Off();
      /* Perform a new configuration to LSE when SYNC MISS is detected. */
      /* For visual comfort on LED2 apply delay before new configuration */
      LL_mDelay(1000);
      
      /* Change source to LSE and restart synchronization if necessary */
      if(LL_CRS_GetSyncSignalSource() != LL_CRS_SYNC_SOURCE_LSE)
      {
        /* CRS configuration can be changed only if frequency error is disabled */
        LL_CRS_DisableFreqErrorCounter();
        
        /* Change CRS SYNC signal source to LSE */
        LL_CRS_SetSyncSignalSource(LL_CRS_SYNC_SOURCE_LSE);

        /* Enable Frequency error counter */
        LL_CRS_EnableFreqErrorCounter();
      }
      else
      {
        /* Enable RCC_CRS SYNCOK interrupt */
        LL_CRS_EnableIT_SYNCOK();
      }
      break;
      
    case CRS_STATUS_SYNCWARN:
      /* Toggle LED2 when SYNC WARN is detected. */
      LED_On();
      LL_mDelay(100);
      LED_Off();
      LL_mDelay(100);
      LED_On();
      LL_mDelay(100);
      LED_Off();
      LL_mDelay(100);
      LED_On();
      LL_mDelay(100);
      LED_Off();
      LL_mDelay(100);
      /* Increase tollerance (FELIM value) until no more warning */
      LL_CRS_DisableFreqErrorCounter();
      felim = LL_CRS_GetFreqErrorLimit();
      LL_CRS_SetFreqErrorLimit(felim+1);
      LL_CRS_EnableFreqErrorCounter();
      /* Enable CRS SYNCWARN interrupt (to allow resync) */
      LL_CRS_EnableIT_SYNCWARN();
      /* Enable CRS SYNCOK interrupt (if it was in SYNCOK but then in WARN again) */
      LL_CRS_EnableIT_SYNCOK();
      break;

    case CRS_STATUS_SYNCERR:
      /* Power off LED2 */
      LED_Off();
      break;

    default:
      break;
    }
  }
}

/**
  * @brief  This function enables the reference clock used for sync (LSE)
  * @param  None
  * @retval None
  */
void Enable_Sync_Reference_Clock(void)
{
  /* To enable LSE, before it is necessary to:
     - Enable the power clock
     - Reset the Back up Domain */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
  LL_PWR_EnableBkUpAccess();
  
  /* Enable LSE only if disabled.*/
  if (LL_RCC_LSE_IsReady() == 0)
  {
    LL_RCC_ForceBackupDomainReset();
    LL_RCC_ReleaseBackupDomainReset();
    LL_RCC_LSE_Enable();
#if (USE_TIMEOUT == 1)
    Timeout = LSE_TIMEOUT_VALUE;
#endif /* USE_TIMEOUT */
    while (LL_RCC_LSE_IsReady() != 1)
    {
#if (USE_TIMEOUT == 1)
      if (LL_SYSTICK_IsActiveCounterFlag()) 
      {
        Timeout --;
      }
      if (Timeout == 0)
      {
        /* LSE activation error */
        LED_Blinking(LED_BLINK_ERROR);
      }  
#endif /* USE_TIMEOUT */
    }
  }
}   
  
/**
  * @brief  This function enables CRS and HSI48 clocks 
  * @param  None
  * @retval None
  */
void Enable_CRS_Clocks(void)
{
  /* CRS initialization (enable HSI48 oscillator and enable CRS clock) */

  /* Enable CRS clock*/
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_CRS);  

  /* L0xx devices need to enable the VREFINT when using HSI48 */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  LL_SYSCFG_VREFINT_EnableHSI48();

  /* Enable HSI48 only if disabled.*/
  if (LL_RCC_HSI48_IsReady() == 0)
  {
    LL_RCC_HSI48_Enable();
#if (USE_TIMEOUT == 1)
    Timeout = LSE_TIMEOUT_VALUE;
#endif /* USE_TIMEOUT */
    while (LL_RCC_HSI48_IsReady() != 1)
    {
#if (USE_TIMEOUT == 1)
      if (LL_SYSTICK_IsActiveCounterFlag()) 
      {
        Timeout --;
      }
      if (Timeout == 0)
      {
        /* LSE activation error */
        LED_Blinking(LED_BLINK_ERROR);
      }  
#endif /* USE_TIMEOUT */
    }
  }
}  

/**
  * @brief  This function configures CRS
  * @param  None
  * @retval None
  */
void Configure_CRS(uint32_t SyncSource)
{
  /* Enable and set RCC_CRS interrupt*/
  NVIC_SetPriority(RCC_IRQn, 0x03);
  NVIC_EnableIRQ(RCC_IRQn);
  
  /* HSI48 Synchronization:
     - HSI trimming value set to 0x00 to see the automatic calibration performed by CRS
     - Reload value calculated like this F(LSE)=32.768kHz then Reload=(f(Target)/f(LSE))-1= 0x5B7 
     - Felim value calculated like this FELIM = (fTARGET / fSYNC) * STEP[%] / 100% / 2 with STEP=0.14% then FELIM = 2 
  */
  LL_CRS_ConfigSynchronization(0x00,
                               2,
                               __LL_CRS_CALC_CALCULATE_RELOADVALUE(HSI48_VALUE, LSE_VALUE),
                               LL_CRS_SYNC_DIV_1 | SyncSource | LL_CRS_SYNC_POLARITY_FALLING
                              );

  /* Enable Automatic trimming */
  LL_CRS_EnableAutoTrimming();

  /* Enable Frequency error counter */
  LL_CRS_EnableFreqErrorCounter();

  /* Enable RCC_CRS interrupts */
  /* Each interruption can be enabled independently in using following functions:
  - LL_CRS_EnableIT_SYNCOK();
  - LL_CRS_EnableIT_SYNCWARN();
  - LL_CRS_EnableIT_ERR();
  - LL_CRS_EnableIT_ESYNC();
  */
  LL_CRS_WriteReg(CRS, CR, LL_CRS_ReadReg(CRS, CR) | LL_CRS_CR_SYNCOKIE | LL_CRS_CR_SYNCWARNIE | LL_CRS_CR_ERRIE | LL_CRS_CR_ESYNCIE);
}

/**
  * @brief  Configure MCO pin (PA8).
  * @param  None
  * @retval None
  */
void MCO_ConfigGPIO(void)
{
  /* MCO Clock Enable */
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);

  /* Configure the MCO pin in alternate function mode */
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_8, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_8, LL_GPIO_OUTPUT_PUSHPULL);
  LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_8, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_8, LL_GPIO_PULL_NO);
  LL_GPIO_SetAFPin_8_15(GPIOA, LL_GPIO_PIN_8, LL_GPIO_AF_0);

  /* Select MCO clock source and prescaler */
  LL_RCC_ConfigMCO(LL_RCC_MCO1SOURCE_HSI48, LL_RCC_MCO1_DIV_1);
}

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
  * @brief  Turn-on LED2.
  * @param  None
  * @retval None
  */
void LED_On(void)
{
  /* Turn LED2 on */
  LL_GPIO_SetOutputPin(LED2_GPIO_PORT, LED2_PIN);
}

/**
  * @brief  Turn-off LED2.
  * @param  None
  * @retval None
  */
void LED_Off(void)
{
  /* Turn LED2 off */
  LL_GPIO_ResetOutputPin(LED2_GPIO_PORT, LED2_PIN);
}

/**
  * @brief  Set LED2 to Blinking mode for an infinite loop (toggle period based on value provided as input parameter).
  * @param  Period : Period of time (in ms) between each toggling of LED
  *   This parameter can be user defined values. Pre-defined values used in that example are :
  *     @arg LED_BLINK_FAST : Fast Blinking
  *     @arg LED_BLINK_SLOW : Slow Blinking
  *     @arg LED_BLINK_ERROR : Error specific Blinking
  * @retval None
  */
void LED_Blinking(uint32_t Period)
{
  /* Toggle IO in an infinite loop */
  while (1)
  {
    LL_GPIO_TogglePin(LED2_GPIO_PORT, LED2_PIN);  
    LL_mDelay(Period);
  }
}

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
  
  /* Configure NVIC for USER_BUTTON_EXTI_IRQn */
  NVIC_EnableIRQ(USER_BUTTON_EXTI_IRQn); 
  NVIC_SetPriority(USER_BUTTON_EXTI_IRQn,0x03);  
  
  /* Connect External Line to the GPIO*/
  USER_BUTTON_SYSCFG_SET_EXTI();
  
  /* Enable a rising trigger EXTI line 13 Interrupt */
  USER_BUTTON_EXTI_LINE_ENABLE();
  USER_BUTTON_EXTI_FALLING_TRIG_ENABLE();
}

/**
  * @brief  Wait for User push-button press to start transfer.
  * @param  None
  * @retval None
  */
  /*  */
void WaitForUserButtonPress(void)
{
  while (ubButtonPress == 0)
  {
    LL_GPIO_TogglePin(LED2_GPIO_PORT, LED2_PIN);
    LL_mDelay(LED_BLINK_FAST);
  }
  /* Ensure that LED2 is turned Off */
  LED_Off();
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = MSI
  *            SYSCLK(Hz)                     = 2097000
  *            HCLK(Hz)                       = 2097000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            Flash Latency(WS)              = 0
  *            Main regulator output voltage  = Scale3 mode
  * @retval None
  */
void SystemClock_Config(void)
{
  /* MSI configuration and activation */
  LL_RCC_PLL_Disable();
  /* Set new latency */
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);

  LL_RCC_MSI_Enable();
  while(LL_RCC_MSI_IsReady() != 1) 
  {
  };
  LL_RCC_MSI_SetRange(LL_RCC_MSIRANGE_5);  
  LL_RCC_MSI_SetCalibTrimming(0x0);

  /* Sysclk activation  */
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_MSI);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_MSI) 
  {
  };
  
  /* Set APB1 & APB2 prescaler*/
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

  /* Set systick to 1ms in using frequency set to 2MHz */
  LL_Init1msTick(2097000);

  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(2097000);  

  /* Enable Power Control clock */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
  /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE3);
}


/******************************************************************************/
/*   USER IRQ HANDLER TREATMENT                                               */
/******************************************************************************/
/**
  * @brief  Function to manage User button callback interruption
  * @param  None
  * @retval None
  */
void UserButton_Callback(void)
{
  /* Update User push-button variable : to be checked in waiting loop in main program */
  ubButtonPress = 1;
}

/**
  * @brief  Handle CRS interruption
  * @param  none 
  * @retval none
  */
void CRS_Handler_Callback(void)
{
  /* Check CRS SYNCOK flag  */
  if (LL_CRS_IsActiveFlag_SYNCOK() && LL_CRS_IsEnabledIT_SYNCOK())
  {
    CRSSyncStatus = CRS_STATUS_SYNCOK;

    /* Disable CRS SYNCOK interrupt since synchronization succeeded */
    LL_CRS_DisableIT_SYNCOK();
      
    /* Clear CRS SYNC event OK bit */
    LL_CRS_ClearFlag_SYNCOK();
  }

  /* Check CRS SYNCWARN flag  */
  if (LL_CRS_IsActiveFlag_SYNCWARN() && LL_CRS_IsEnabledIT_SYNCWARN())
  {
    CRSSyncStatus = CRS_STATUS_SYNCWARN;

    /* Disable temporary CRS SYNCWARN interrupt to let the main loop to be execute */
    LL_CRS_DisableIT_SYNCWARN();

    /* Clear CRS SYNCWARN bit */
    LL_CRS_ClearFlag_SYNCWARN();
  }

  /* Check CRS SYNCERR flag  */
  if (LL_CRS_IsActiveFlag_SYNCERR() && LL_CRS_IsEnabledIT_ERR())
  {
    CRSSyncStatus = CRS_STATUS_SYNCERR;

    /* Clear CRS Error bit */
    LL_CRS_ClearFlag_ERR();
  }

  /* Check CRS SYNC Missed flag  */
  if (LL_CRS_IsActiveFlag_SYNCMISS() && LL_CRS_IsEnabledIT_ERR())
  {
    CRSSyncStatus = CRS_STATUS_SYNCMISS;
    
    /* Clear CRS SYNC Missed bit */
    LL_CRS_ClearFlag_ERR();
  }
  
  /* Check CRS Expected SYNC flag  */
  if (LL_CRS_IsActiveFlag_ESYNC() && LL_CRS_IsEnabledIT_ESYNC())
  {
    /* frequency error counter reached a zero value */
    LL_CRS_ClearFlag_ESYNC();
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
