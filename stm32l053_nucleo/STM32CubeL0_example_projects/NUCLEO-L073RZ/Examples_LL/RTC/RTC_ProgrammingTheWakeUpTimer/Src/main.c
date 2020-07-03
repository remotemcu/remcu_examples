/**
  ******************************************************************************
  * @file    Examples_LL/RTC/RTC_ProgrammingTheWakeUpTimer/Src/main.c
  * @author  MCD Application Team
  * @brief   This code example shows how to configure the RTC in order to work 
  *          with the WUT through the STM32L0xx RTC LL API.
  *          Peripheral initialization done using LL unitary services functions.
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

/** @addtogroup RTC_ProgrammingTheWakeUpTimer
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Oscillator time-out values */
#define LSI_TIMEOUT_VALUE          ((uint32_t)2)     /* 2 ms */
#define LSE_TIMEOUT_VALUE          ((uint32_t)5000)  /* 5 s */
#define RTC_TIMEOUT_VALUE          ((uint32_t)1000)  /* 1 s */

/* Constant used to convert in ASCII format */
#define ASCII_CONVERT              ((uint32_t)48)

/* Defines related to Clock configuration */
/* Uncomment to enable the adequate Clock Source */
#define RTC_CLOCK_SOURCE_LSI
/*#define RTC_CLOCK_SOURCE_LSE*/

#ifdef RTC_CLOCK_SOURCE_LSI
/* ck_apre=LSIFreq/(ASYNC prediv + 1) with LSIFreq=37 kHz RC */
#define RTC_ASYNCH_PREDIV          ((uint32_t)0x7F)
/* ck_spre=ck_apre/(SYNC prediv + 1) = 1 Hz */
#define RTC_SYNCH_PREDIV           ((uint32_t)0x122)
#endif

#ifdef RTC_CLOCK_SOURCE_LSE
/* ck_apre=LSEFreq/(ASYNC prediv + 1) = 256Hz with LSEFreq = 32768Hz */
#define RTC_ASYNCH_PREDIV          ((uint32_t)0x7F)
/* ck_spre=ck_apre/(SYNC prediv + 1) = 1 Hz */
#define RTC_SYNCH_PREDIV           ((uint32_t)0x00FF)
#endif

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile uint16_t error = 0;  //initialized at 0 and modified by the functions 

uint8_t bSend = 0;
uint8_t aStringToSend[20] = "\n00 : 00 : 00 ";
uint8_t RTC_InitializationMode = 0;
uint8_t CharToReceive = 0;
uint8_t CharReceived = 0;
uint8_t Alarm = 1; /* set to 1 for the first print */

#if (USE_TIMEOUT == 1)
uint32_t Timeout = 0; /* Variable used for Timeout management */
#endif /* USE_TIMEOUT */

/* Private function prototypes -----------------------------------------------*/
void     SystemClock_Config(void);
void     Configure_USART(void);
void     Configure_RTC(void);
uint32_t Enter_RTC_InitMode(void);
uint32_t Exit_RTC_InitMode(void);
uint32_t WaitForSynchro_RTC(void);
void     Set_RTC_Time(uint32_t Hour, uint32_t Minute, uint32_t Second);
void     Process(void);
void     LED_Init(void);
void     LED_Blinking(uint32_t Period);
void     UserButton_Init(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* Configure the system clock to 2.097 MHz */
  SystemClock_Config();

  /* Initialize LED2 */
  LED_Init();

  /* Initialize button in EXTI mode */
  UserButton_Init();
  
  /* Configure USART in TX and RX IT mode */
  Configure_USART();
  
  /* Configure RTC */
  Configure_RTC();
  
  /* Initialize RTC time at reset value */
  Set_RTC_Time(0, 0, 0);

  /* Infinite loop */
  while (1)
  {
    Process();
  }
}

/**
  * Brief   This function sets the TIME in RTC.
  * Param   Hour   New Hour to set 
  * Param   Minute New Minute to set
  * Param   Second New Second to set
  * Retval  None
  */
void Set_RTC_Time(uint32_t Hour, uint32_t Minute, uint32_t Second)
{
  /* Disable RTC registers write protection */
  LL_RTC_DisableWriteProtection(RTC);
  
  /* Enter in initialization mode */
  if (Enter_RTC_InitMode() != RTC_ERROR_NONE)   
  {
    /* Initialization Error */
    LED_Blinking(LED_BLINK_FAST);
  }
  
  /* New time in TR */
  LL_RTC_TIME_Config(RTC, 
                     LL_RTC_TIME_FORMAT_PM, 
                     Hour, 
                     Minute, 
                     Second);
  
  /* Exit of initialization mode */
  if (Exit_RTC_InitMode() != RTC_ERROR_NONE)   
  {
    /* Initialization Error */
    LED_Blinking(LED_BLINK_FAST);
  }
  
  /* Enable RTC registers write protection */
  LL_RTC_EnableWriteProtection(RTC);
}

/**
  * Brief   This function configures RTC.
  * Param   None
  * Retval  None
  */
void Configure_RTC(void)
{
  /*##-1- Enables the PWR Clock and Enables access to the backup domain #######*/
  /* To change the source clock of the RTC feature (LSE, LSI), you have to:
     - Enable the power clock
     - Enable write access to configure the RTC clock source (to be done once after reset).
     - Reset the Back up Domain
     - Configure the needed RTC clock source */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
  LL_PWR_EnableBkUpAccess();
  
  /*##-2- Configure LSE/LSI as RTC clock source ###############################*/
#ifdef RTC_CLOCK_SOURCE_LSE
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
    LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_LSE);
    
    /*##-3- Enable RTC peripheral Clocks #######################################*/
    /* Enable RTC Clock */ 
    LL_RCC_EnableRTC();
  }
#elif defined(RTC_CLOCK_SOURCE_LSI)
  /* Enable LSI */
  LL_RCC_LSI_Enable();
#if (USE_TIMEOUT == 1)
  Timeout = LSI_TIMEOUT_VALUE;
#endif /* USE_TIMEOUT */
  while (LL_RCC_LSI_IsReady() != 1)
  {
#if (USE_TIMEOUT == 1)
    if (LL_SYSTICK_IsActiveCounterFlag()) 
    {
      Timeout --;
    }
    if (Timeout == 0)
    {
      /* LSI activation error */
      LED_Blinking(LED_BLINK_ERROR);
    }  
#endif /* USE_TIMEOUT */
  }
  LL_RCC_ForceBackupDomainReset();
  LL_RCC_ReleaseBackupDomainReset();
  LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_LSI);

  /*##-3- Enable RTC peripheral Clocks #######################################*/
  /* Enable RTC Clock */ 
  LL_RCC_EnableRTC();

#else
#error "configure clock for RTC"
#endif

  /*##-4- Disable RTC registers write protection ##############################*/
  LL_RTC_DisableWriteProtection(RTC);
  
  /* Set prescaler according to source clock */
  LL_RTC_SetAsynchPrescaler(RTC, RTC_ASYNCH_PREDIV);
  LL_RTC_SetSynchPrescaler(RTC, RTC_SYNCH_PREDIV);

  /* Disable wake up timer to modify it */
  LL_RTC_WAKEUP_Disable(RTC);
  
  /* Wait until it is allow to modify wake up reload value */
#if (USE_TIMEOUT == 1)
  Timeout = RTC_TIMEOUT_VALUE;
#endif /* USE_TIMEOUT */
  while (LL_RTC_IsActiveFlag_WUTW(RTC) != 1)
  {
#if (USE_TIMEOUT == 1)
    if (LL_SYSTICK_IsActiveCounterFlag()) 
    {
      Timeout --;
    }
    if (Timeout == 0)
    {
      /* LSI activation error */
      LED_Blinking(LED_BLINK_ERROR);
    }  
#endif /* USE_TIMEOUT */
  }
  
  /* Setting the Wakeup time to 1 s
       If LL_RTC_WAKEUPCLOCK_CKSPRE is selected, the frequency is 1Hz, 
       this allows to get a wakeup time equal to 1 s if the counter is 0x0 */
  LL_RTC_WAKEUP_SetAutoReload(RTC, 0);
  LL_RTC_WAKEUP_SetClock(RTC, LL_RTC_WAKEUPCLOCK_CKSPRE);
  
  /* Enable wake up counter and wake up interrupt */
  LL_RTC_WAKEUP_Enable(RTC);
  LL_RTC_EnableIT_WUT(RTC);
  
  /* Enable RTC registers write protection */
  LL_RTC_EnableWriteProtection(RTC);
  
  /* Configure exti and nvic for RTC IT */
  LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_20);
  LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_20);
  NVIC_SetPriority(RTC_IRQn, 0);
  NVIC_EnableIRQ(RTC_IRQn);
}

/**
  * @brief  Enter in initialization mode
  * @note In this mode, the calendar counter is stopped and its value can be updated
  * @param  None
  * @retval RTC_ERROR_NONE if no error
  */
uint32_t Enter_RTC_InitMode(void)
{
  /* Set Initialization mode */
  LL_RTC_EnableInitMode(RTC);
  
#if (USE_TIMEOUT == 1)
    Timeout = RTC_TIMEOUT_VALUE;
#endif /* USE_TIMEOUT */

  /* Check if the Initialization mode is set */
  while (LL_RTC_IsActiveFlag_INIT(RTC) != 1)
  {
#if (USE_TIMEOUT == 1)
      if (LL_SYSTICK_IsActiveCounterFlag())
    {
        Timeout --;
    }
      if (Timeout == 0)
    {
      return RTC_ERROR_TIMEOUT;
    }  
#endif /* USE_TIMEOUT */
  }
  
  return RTC_ERROR_NONE;
}

/**
  * @brief  Exit Initialization mode 
  * @param  None
  * @retval RTC_ERROR_NONE if no error
  */
uint32_t Exit_RTC_InitMode(void)
{
  LL_RTC_DisableInitMode(RTC);
  
  /* Wait for synchro */
  /* Note: Needed only if Shadow registers is enabled           */
  /*       LL_RTC_IsShadowRegBypassEnabled function can be used */
  return (WaitForSynchro_RTC());
}

/**
  * @brief  Wait until the RTC Time and Date registers (RTC_TR and RTC_DR) are
  *         synchronized with RTC APB clock.
  * @param  None
  * @retval RTC_ERROR_NONE if no error (RTC_ERROR_TIMEOUT will occur if RTC is 
  *         not synchronized)
  */
uint32_t WaitForSynchro_RTC(void)
{
  /* Clear RSF flag */
  LL_RTC_ClearFlag_RS(RTC);

#if (USE_TIMEOUT == 1)
    Timeout = RTC_TIMEOUT_VALUE;
#endif /* USE_TIMEOUT */

  /* Wait the registers to be synchronised */
  while(LL_RTC_IsActiveFlag_RS(RTC) != 1)
  {
#if (USE_TIMEOUT == 1)
      if (LL_SYSTICK_IsActiveCounterFlag())
    {
        Timeout --;
    }
      if (Timeout == 0)
    {
      return RTC_ERROR_TIMEOUT;
    }  
#endif /* USE_TIMEOUT */
  }
  return RTC_ERROR_NONE;
}

/**
  * Brief   This function processes RTC with USART.
  * Param   None
  * Retval  None
  */
void Process(void)
{
  register uint32_t time = 0;
  static uint32_t new_hour = 0, new_minute = 0, new_second = 0;
  __IO uint32_t temp_read = 0;
  
  switch(RTC_InitializationMode)
  {
  case 0:
    /* check alarm and synchronisation flag */
    if (Alarm && (LL_RTC_IsActiveFlag_RS(RTC) == 1))
    {
      Alarm = 0;
      
      /* get time */
      time = LL_RTC_TIME_Get(RTC); 
      /* need to read date also to unlock TR register */
      temp_read = LL_RTC_ReadReg(RTC, DR); 
      ((void)(temp_read));  /* To avoid warning */
      
      aStringToSend[1] = (uint8_t)((__LL_RTC_GET_HOUR(time) >> 4) + ASCII_CONVERT);/* hour tens */
      aStringToSend[2] = (uint8_t)((__LL_RTC_GET_HOUR(time) & 0x0F) + ASCII_CONVERT);/* hour units */
      aStringToSend[6] = (uint8_t)((__LL_RTC_GET_MINUTE(time) >> 4) + ASCII_CONVERT);/* minute tens */
      aStringToSend[7] = (uint8_t)((__LL_RTC_GET_MINUTE(time) & 0x0F) + ASCII_CONVERT);/* minute units */
      aStringToSend[11] = (uint8_t)((__LL_RTC_GET_SECOND(time) >> 4) + ASCII_CONVERT);/* second tens */
      aStringToSend[12] = (uint8_t)((__LL_RTC_GET_SECOND(time) & 0x0F) + ASCII_CONVERT);/* second units */
      
      /* start USART transmission */
      LL_USART_TransmitData8(USART1, aStringToSend[bSend++]); /* Will initiate TC if TXE */
    }
  break;
  case 1:      
    {
      if(!bSend)
      {
        strcpy((char *)aStringToSend,"\nSend hour tens\n");
        LL_USART_TransmitData8(USART1, aStringToSend[bSend++]); /* Will initiate TC if TXE */
        RTC_InitializationMode=2;
      }
    }
    break;
  case 2: /* Hour tens */
    {
      if(CharReceived)
      {
        CharReceived=0;
        CharToReceive -= ASCII_CONVERT;
        if(CharToReceive<3)
        {
          new_hour = CharToReceive << 4;
          RTC_InitializationMode=3;
        }
        else RTC_InitializationMode=1;
      }
    }
    break;
  case 3:      
    {
      if(!bSend)
      {
        strcpy((char *)aStringToSend,"\nSend hour units\n");
        LL_USART_TransmitData8(USART1, aStringToSend[bSend++]); /* Will initiate TC if TXE */
        RTC_InitializationMode=4;
      }
    }
    break;
  case 4: /* Hour units */
    {
       if(CharReceived)
      {
        CharReceived = 0;
        CharToReceive -= ASCII_CONVERT;
        if((((new_hour >> 4) == 2) && (CharToReceive < 4)) ||
            (((new_hour >> 4) < 2) && (CharToReceive < 10)))
        {
          new_hour |= CharToReceive;
          RTC_InitializationMode=5;
        }
        else RTC_InitializationMode=3;
      }
    }
    break;
  case 5:      
    {
      if(!bSend)
      {
        strcpy((char *)aStringToSend,"\nSend minute tens\n");
        LL_USART_TransmitData8(USART1, aStringToSend[bSend++]); /* Will initiate TC if TXE */
        RTC_InitializationMode=6;
      }
    }
    break;
  case 6: /* Minute tens */
    {
      if(CharReceived)
      {
        CharReceived = 0;
        CharToReceive -= ASCII_CONVERT;
        if(CharToReceive < 6)
        {
          new_minute = CharToReceive << 4;
          RTC_InitializationMode = 7;
        }
        else RTC_InitializationMode = 5;
      }
    }
    break;
  case 7:      
    {
      if(!bSend)
      {
        strcpy((char *)aStringToSend,"\nSend minute units\n");
        LL_USART_TransmitData8(USART1, aStringToSend[bSend++]); /* Will initiate TC if TXE */
        RTC_InitializationMode=8;
      }
    }
    break;
  case 8: /* Minute units */
    {
      if(CharReceived)
      {
        CharReceived=0;
        CharToReceive -= ASCII_CONVERT;
        if(CharToReceive < 10)
        {
          new_minute |= CharToReceive;
          RTC_InitializationMode = 9;
        }
        else RTC_InitializationMode = 7;
      }
    }
    break;
  case 9:      
    {
      if(!bSend)
      {
        strcpy((char *)aStringToSend,"\nSend second tens\n");
        LL_USART_TransmitData8(USART1, aStringToSend[bSend++]); /* Will initiate TC if TXE */
        RTC_InitializationMode=10;
      }
    }
    break;
  case 10: /* Second tens */
    {
       if(CharReceived)
      {
        CharReceived = 0;
        CharToReceive -= ASCII_CONVERT;
        if(CharToReceive < 6)
        {
          new_second = CharToReceive << 4;
          RTC_InitializationMode = 11;
        }
        else RTC_InitializationMode = 9;
      }
    }
    break;
  case 11:      
    {
      if(!bSend)
      {
        strcpy((char *)aStringToSend,"\nSend second units\n");
        LL_USART_TransmitData8(USART1, aStringToSend[bSend++]); /* Will initiate TC if TXE */
        RTC_InitializationMode = 12;
      }
    }
    break;
  case 12: /* Second Units */
    {
      if(CharReceived)
      {
        CharReceived = 0;
        CharToReceive -= ASCII_CONVERT;
        if(CharToReceive < 10)
        {
          new_second |= CharToReceive;
          RTC_InitializationMode = 13;
        }
        else RTC_InitializationMode = 11;
      }
    }
    break;
  case 13:
    {
      Set_RTC_Time(new_hour, new_minute, new_second);
      strcpy((char *)aStringToSend,"\n-- : -- : --      ");
      RTC_InitializationMode = 0;
    }
    break;
  }  
}

/**
  * @brief  This function configures USART1.
  * @param  None
  * @retval None
  */
void Configure_USART(void)
{
  /* Enables GPIO clock and configures the USART1 pins (TX on PA.09, RX on PA.10) */

  /* Enable the peripheral clock of GPIOA */
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);

  /* Configure Tx Pin as : Alternate function, High Speed, Push pull, Pull up */
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_9, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetAFPin_8_15(GPIOA, LL_GPIO_PIN_9, LL_GPIO_AF_4);
  LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_9, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_9, LL_GPIO_OUTPUT_PUSHPULL);
  LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_9, LL_GPIO_PULL_UP);

  /* Configure Rx Pin as : Alternate function, High Speed, Push pull, Pull up */
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_10, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetAFPin_8_15(GPIOA, LL_GPIO_PIN_10, LL_GPIO_AF_4);
  LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_10, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_10, LL_GPIO_OUTPUT_PUSHPULL);
  LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_10, LL_GPIO_PULL_UP);

  /* Enable the peripheral clock for USART1 */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);

  /* Configure USART1 */
  
  /* Disable USART prior modifying configuration registers */
  LL_USART_Disable(USART1);
  /* TX/RX direction */
  LL_USART_SetTransferDirection(USART1, LL_USART_DIRECTION_TX_RX);
  /* 8 data bit, 1 start bit, 1 stop bit, no parity */
  LL_USART_ConfigCharacter(USART1, LL_USART_DATAWIDTH_8B, LL_USART_PARITY_NONE, LL_USART_STOPBITS_1);
  /* Oversampling by 16 */
  LL_USART_SetOverSampling(USART1, LL_USART_OVERSAMPLING_16);
  /* Set Baudrate to 115200 using APB2 frequency set to 2097000 Hz */
  /* This frequency can also be calculated through LL RCC macro */
  /* Ex :
      pllclk = __LL_RCC_CALC_PLLCLK_FREQ(__LL_RCC_CALC_MSI_FREQ(LL_RCC_MSIRANGESEL_RUN, LL_RCC_MSIRANGE_6), LL_RCC_PLLM_DIV1, 40, LL_RCC_PLLR_DIV2);
      hclk = __LL_RCC_CALC_HCLK_FREQ(pllclk, LL_RCC_GetAHBPrescaler());
      periphclk = __LL_RCC_CALC_PCLK2_FREQ(hclk, LL_RCC_GetAPB2Prescaler());
  
      periphclk is expected to be equal to 2097000 Hz
      
      In this example, Peripheral Clock is equal to SystemCoreClock
  */
  LL_USART_SetBaudRate(USART1, SystemCoreClock, LL_USART_OVERSAMPLING_16, 115200); 
  LL_USART_Enable(USART1);
  
  /* Polling USART initialisation */
  while((!(LL_USART_IsActiveFlag_TEACK(USART1))) || (!(LL_USART_IsActiveFlag_REACK(USART1))))
  { 
  }
  /* Configure IT */
  /*  Set priority for USART1_IRQn */
  /*  Enable USART1_IRQn */
  NVIC_SetPriority(USART1_IRQn, 0);  
  NVIC_EnableIRQ(USART1_IRQn);

  /* Clear TC Flag */
  LL_USART_ClearFlag_TC(USART1); 

  /* Enable RXNE and Error interrupts */
  LL_USART_EnableIT_RXNE(USART1);
  /* Enable TC interrupt */
  LL_USART_EnableIT_TC(USART1); 
  LL_USART_EnableIT_ERROR(USART1);
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
  * @brief  Set LED2 to Blinking mode for an infinite loop (toggle period based on value provided as input parameter).
  * @param  Period : Period of time (in ms) between each toggling of LED
  *   This parameter can be user defined values. Pre-defined values used in that example are :
  *     @arg LED_BLINK_FAST : Fast Blinking
  *     @arg LED_BLINK_SLOW : Slow Blinking
  *     @arg LED_BLINK_FAST : Error specific Blinking
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
  * @brief  Configures User push-button in EXTI mode.
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
  
  /* Enable a rising trigger EXTI line 13 Interrupt */
  USER_BUTTON_EXTI_LINE_ENABLE();
  USER_BUTTON_EXTI_FALLING_TRIG_ENABLE();
  
  /* Configure NVIC for USER_BUTTON_EXTI_IRQn */
  NVIC_EnableIRQ(USER_BUTTON_EXTI_IRQn); 
  NVIC_SetPriority(USER_BUTTON_EXTI_IRQn,0x03);  
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
  * @brief  Function to manage IRQ Handler
  * @param  None
  * @retval None
  */
void UserButton_Callback(void)
{
  RTC_InitializationMode = 1;  
}

/**
  * @brief  Function called from USART IRQ Handler when RXNE flag is set
  *         Function is in charge of reading character received on USART RX line.
  * @param  None
  * @retval None
  */
void USART_CharReception_Callback(void)
{
    CharToReceive = LL_USART_ReceiveData8(USART1); /* Receive data, clear flag */
    CharReceived = 1;      
}

/**
  * @brief  Function called for achieving next TX Byte bSending
  * @param  None
  * @retval None
  */
void USART_CharTransmitComplete_Callback(void)
{
    if(bSend == sizeof(aStringToSend))
    {
      bSend=0;
    }
    else
    {
      /* clear transfer complete flag and fill TDR with a new char */
      LL_USART_TransmitData8(USART1, aStringToSend[bSend++]);
    }
}

/**
  * @brief  Function handle the RTC wakeup interruption.
  * @param  None
  * @retval None
  */
void RTC_Wakup_Treatement(void)
{
    /* Toggle Green LED */
    LL_GPIO_TogglePin(LED2_GPIO_PORT, LED2_PIN);
    Alarm = 1;
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
