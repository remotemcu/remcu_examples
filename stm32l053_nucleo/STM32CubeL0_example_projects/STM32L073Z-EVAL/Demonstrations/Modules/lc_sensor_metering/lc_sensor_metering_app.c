/**
******************************************************************************
* @file    lc_sensor_metering_app.c
* @author  MCD Application Team
* @brief   thermometer system information.
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

#define _LC_SENSOR_METERING_APP_C

/* Includes ------------------------------------------------------------------*/
#include "string.h"
#include "stdio.h"
#include "k_config.h"
#include "k_module.h"
#include "k_menu.h"
#include "k_storage.h"
#include "lc_sensor_metering_app.h"

/** @addtogroup LC_SENSOR_METERING_MODULE
* @{
*/

/** @defgroup LC_SENSOR_SYSTEM_INFO
* @brief system info routines 
* @{
*/

/* Private typedef ----------------------------------------------------------*/    
/* Private constants ----------------------------------------------------------*/



LcSensorModeTypeEnum  LCSensorMode = CONFIG_LC_SENSOR_OFF;
GPIO_TypeDef   GpioConfigLcSensor[6];

/* Function prototypes ------------------------------------------------------*/
KMODULE_RETURN _LcSensorCountingDemoExec(void);
KMODULE_RETURN _LcSensorCountingConfig(void);
KMODULE_RETURN _LcSensorCountingUnConfig(void);
void LcSensorCountingDemoStd(void);
void LcSensorCountingDemoLowPower(void);
void LcSensorCountingMenuUserAction(uint8_t sel);
static void CopyGpioConfigLcSensor(uint32_t  index, GPIO_TypeDef * gpio);
static void RestoreGpioConfigLcSensor(uint32_t  index, GPIO_TypeDef * gpio);
static void RestoreGpioContext(void);
static void SaveGpioContext(void);
static void DisplayMsgWhenInPowerMode(void);





const tMenuItem LcSensorModeStdItems[] ={
  {"", 14, 30, TYPE_EXEC, MODULE_NONE, LcSensorCountingDemoStd, LcSensorCountingMenuUserAction, NULL, NULL },
};
const tMenu LcSensorModeStd = { "LC sensor (Std)", LcSensorModeStdItems, countof(LcSensorModeStdItems), TYPE_EXEC, 1, 1};

const tMenuItem LcSensorModeLowPowerItems[] ={
  {"", 14, 30, TYPE_EXEC, MODULE_NONE, LcSensorCountingDemoLowPower, LcSensorCountingMenuUserAction, NULL, NULL },
};
const tMenu LcSensorModeLowPower = { "LC sensor (LP)", LcSensorModeLowPowerItems, countof(LcSensorModeLowPowerItems), TYPE_EXEC, 1, 1};


const tMenuItem LcSensorCountingMenuMenuItems[] =
{
  {"MODE STANDARD",           14, 30,SEL_SUBMENU, MODULE_NONE, NULL, NULL, (const tMenu*)&LcSensorModeStd, NULL },
  {"MODE LOW POWER",       14, 30,SEL_SUBMENU, MODULE_NONE, NULL, NULL, (const tMenu*)&LcSensorModeLowPower, NULL },
  {"RETURN",            0,  0,SEL_EXIT, MODULE_NONE, NULL, NULL, NULL, NULL }
};


const tMenu LcSensorCountingMenu = {
  "LC sensor metering", LcSensorCountingMenuMenuItems, countof(LcSensorCountingMenuMenuItems), TYPE_TEXT, 1, 1 };

/* Private typedef -----------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
const K_ModuleItem_Typedef LcSensorMeteringModuleItem =
{
  MODULE_LC_SENSOR_METERING,
  _LcSensorCountingConfig,
  _LcSensorCountingDemoExec,
  _LcSensorCountingUnConfig,
  NULL
};  


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static GPIO_InitTypeDef  GPIO_InitStruct;
extern RTC_HandleTypeDef RtcHandle;
/* COMP handle declaration */
COMP_HandleTypeDef  hcomp2;
/* DAC handle declaration */
DAC_HandleTypeDef       DacHandle;
static DAC_ChannelConfTypeDef sConfig;
/* LPTIM handle declaration */
LPTIM_HandleTypeDef     LptimHandle;
/* TIM handle declaration */
TIM_HandleTypeDef    TimHandle;
/* Clocks structure declaration */
RCC_PeriphCLKInitTypeDef        RCC_PeriphCLKInitStruct;

/* Prescaler declaration */
uint32_t uwPrescalerValue = 0;
uint32_t DAC_VALUE = 0x850; /* Set to medium value with margin (0x800 + 50) */
uint32_t CPT_int1 = 0;
uint32_t CPT_int2 = 0;
uint32_t CPT_int3 = 0;
uint32_t CPT_max = 0;
uint8_t detect = 0;



/* Private function prototypes -----------------------------------------------*/
static void LcSensorCountingUserHeader(void);
static void RTC_DeConfig(void);
static void DisplayLCSensorResults(uint32_t init_flag);
static void ErrorHandler(void);
static void COMP_Config(void);
static void RTC_Config(void);
static void RTCInitLCSensor(void);
static void DACInitLCSensor(void);
static void COMPInitLCSensor(void);
static void TIMInitLCSensor(void);
static void LPTIMInitLCSensor(void);
static void StopEntry(void);
static void SystemClock_ConfigMSI_2M(void);


/**
* @brief  setup the HW for the LP Uart application
* @param  None.
* @note   set the memeory + Hw initialisation.  
* @retval None.
*/
KMODULE_RETURN _LcSensorCountingConfig(void)
{
  
  return KMODULE_OK;
}


/**
* @brief  setup the HW for the LP Uart application
* @param  None.
* @note   set the memeory + Hw initialisation.
* @retval None.
*/
KMODULE_RETURN _LcSensorCountingUnConfig(void)
{
  LCSensorMode = CONFIG_LC_SENSOR_OFF;
  return KMODULE_OK;
}
/**
* @brief  Run the 8 uart application 
* @param  None.
* @note   run and display information about the uart transaction.  
* @retval None.
*/
KMODULE_RETURN _LcSensorCountingDemoExec(void)
{
  /* Prepare the main MMI */
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  kMenu_Execute(LcSensorCountingMenu);
  
  /* Execute the app 8uart */
  /* App initialization    */
  return KMODULE_OK;
}


void LcSensorCountingDemoLowPower(void)
{
  //  JOYState_TypeDef JoyState = JOY_NONE;
  
  LCSensorMode = CONFIG_LC_SENSOR_LOW_POWER;
  
  /* Display user information */
  LcSensorCountingUserHeader();
  /* Display user information */
  DisplayMsgWhenInPowerMode();

  /* Uncomment to enable MCO pin PA.8 for debug only */
  //HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_SYSCLK, RCC_MCODIV_1);

  /* Initializes WakeUp timer after STOP */
  RTCInitLCSensor();
  /* Initializes DAC as comparator threshold voltage for COMP2_INM */
  DACInitLCSensor();
  /* Initializes LPTIM as LC sensor pulses counter */
  LPTIMInitLCSensor();
  /* Initializes COMP2 as pulses detector */
  COMPInitLCSensor();
  /* Enter the application in STOP mode */
  StopEntry();
  
  while(1)
  {  
  }
}


void LcSensorCountingDemoStd(void)
{
  JOYState_TypeDef JoyState = JOY_NONE;
  LCSensorMode = CONFIG_LC_SENSOR_STD;
  
  uint32_t flag_display_result  = 0;
  
  /* Display user information */
  LcSensorCountingUserHeader();
  
  /* Save the GPIO context */
  SaveGpioContext();
  
  /* Enable GPIOD Clock */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  
  /* Configure PD.7 IO in output push-pull mode to drive VDD/2 external */
  GPIO_InitStruct.Pin = (GPIO_PIN_7);
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH  ;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
  
  /* Enable GPIOE Clock */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  
  /* Configure PE.4 IO in output push-pull mode to drive LD1 green */
  GPIO_InitStruct.Pin = (GPIO_PIN_4);
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH  ;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
  
  /* Enable MCO pin in debug mode only */
  /* System clock selected to output on MCO pin (PA.8)*/
  //HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_SYSCLK, RCC_MCODIV_1);

  /* TIM initialization to manage temporizations in code */
  TIMInitLCSensor();
  
  /* DAC */
  /* Configure the DAC peripheral */
  DacHandle.Instance = DACx;
  if(HAL_DAC_Init(&DacHandle) != HAL_OK)
  {
    /* Initiliazation Error */
    ErrorHandler();
  }
  
  /* Configure DAC channel1 */
  sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  
  if(HAL_DAC_ConfigChannel(&DacHandle, &sConfig, DAC_CHANNEL_1) != HAL_OK)
  {
    /* Channel configuration Error */
    ErrorHandler();
  }
  
  /* Set DAC Channel1 DHR register */
  // full scale = 0xFF, midrange = 0x80;
  if(HAL_DAC_SetValue(&DacHandle, DAC_CHANNEL_1, DAC_ALIGN_12B_R, DAC_VALUE) != HAL_OK)
  {
    /* Setting value Error */
    ErrorHandler();
  }
  
  /* Enable DAC Channel1 */
  if(HAL_DAC_Start(&DacHandle, DAC_CHANNEL_1) != HAL_OK)
  {
    /* Start Error */
    ErrorHandler();
  }
  
 
  /* Select the APB clock as LPTIM peripheral clock */
  RCC_PeriphCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LPTIM1;
  RCC_PeriphCLKInitStruct.LptimClockSelection = RCC_LPTIM1CLKSOURCE_HSI;
  HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitStruct);
  
  /* Initialize the LPTIM peripheral */
  /*
  *  Instance        = LPTIM1
  *  Clock Source    = LowPowerOSCillator
  *  Counter source  = External event.
  *  Clock prescaler = 1 (No division)
  *  Counter Trigger = Trigger source COMP2
  *  Output Polarity = High
  *  Update mode     = Immediate (Registers are immediately updated after any
  *                    write access)
  */
  
  LptimHandle.Instance = LPTIM1;
  
  LptimHandle.Init.Clock.Source    = LPTIM_CLOCKSOURCE_ULPTIM;
  LptimHandle.Init.CounterSource   = LPTIM_COUNTERSOURCE_EXTERNAL;
  LptimHandle.Init.Clock.Prescaler = LPTIM_PRESCALER_DIV1;
  LptimHandle.Init.Trigger.Source  = LPTIM_TRIGSOURCE_7;
  LptimHandle.Init.OutputPolarity  = LPTIM_OUTPUTPOLARITY_HIGH;
  LptimHandle.Init.UpdateMode      = LPTIM_UPDATE_IMMEDIATE;
  LptimHandle.Init.Trigger.ActiveEdge = LPTIM_ACTIVEEDGE_RISING;
  
  /* Initialize LPTIM peripheral according to the passed parameters */
  if (HAL_LPTIM_Init(&LptimHandle) != HAL_OK)
  {
    ErrorHandler();
  }
  
  /* Start counting */
  if (HAL_LPTIM_Counter_Start(&LptimHandle, 0xFFFF) != HAL_OK)
  {
    ErrorHandler();
  }
  /* LPTimer enable */
  LptimHandle.Instance->CR |= 0x00000001;
  
  /* LPTimer continuous mode */
  LptimHandle.Instance->CR |= 0x00000004;
  
  /* Enable internal COMP2 */
  COMP_Config();
  
  /* RTC INITIALIZATION */
  RTC_Config();
  
  /* Note RTC */
  /* Setting the Wake up time to 32Hz (1/32 = 0.03125s)
  RTC_WAKEUPCLOCK_RTCCLK_DIV = RTCCLK_DIV2= 2
  Wakeup Time Base = 2 /(~32.768KHz) = ~0,061035 ms
  Wakeup Time = 0.03125 = 0,061035ms  * WakeUpCounter
  ==> WakeUpCounter = 0.03125/0,061035ms = 512 = 0x200 */
  HAL_RTCEx_SetWakeUpTimer_IT(&RtcHandle, 0x200, RTC_WAKEUPCLOCK_RTCCLK_DIV2);  

  DisplayLCSensorResults(2); // Init Display
  flag_display_result = 1;   // Standard Display
  
  /* Wait for the user to exit the test through the Joystick
  *
  */
  BSP_JOY_Init(JOY_MODE_GPIO);
  while(JoyState != JOY_DOWN)
  {
    /* Run the test infinitively...*/
    DisplayLCSensorResults(flag_display_result);
    JoyState = BSP_JOY_GetState();
  }
  
  
  /* Exit from the application properly
  *
  */
  
  /* Deinitialize the RTC */
  RTC_DeConfig();
  
  /* Put back the GPIO configuration */
  RestoreGpioContext();
  
  /* Disable the different drivers in use...*/
  
  if(HAL_DAC_DeInit(&DacHandle) != HAL_OK)
  {
    /* Initiliazation Error */
    ErrorHandler();
  }
  
  hcomp2.Instance = COMP2;
  if(HAL_COMP_DeInit(&hcomp2) != HAL_OK)
  {
    /* Error */
    ErrorHandler();
  }
  
  if (HAL_LPTIM_DeInit(&LptimHandle) != HAL_OK)
  {
    ErrorHandler();
  }

  TimHandle.Instance = TIM22;
  if(HAL_TIM_Base_DeInit(&TimHandle) != HAL_OK)
  {
    /* Initialization Error */
    ErrorHandler();
  }

  /* Disable the different clocks */
  __HAL_RCC_TIM22_CLK_DISABLE();
  __HAL_RCC_LPTIM1_CLK_DISABLE();
  __HAL_RCC_DAC_CLK_DISABLE();

}


/**
* @brief  Input Capture callback in non blocking mode
* @param  htim : TIM IC handle
* @retval None
*/
void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc)
{
  /* Set DAC1 enable to feed COMP2 threshold */
  DAC->CR |= 0x01;
  /* Set power on VDD/2 generation */
  GPIOD->BSRR = GPIO_PIN_7;
  
 
  
  /* Re-configure all used pins in applicative conditions */
  /* Re-Enable PD7 VDD/2 reference power supply */
  GPIOD->MODER = 0xFFFF7FFF;
  
   /* Put stabilization time in HSI mode approx. 5µs */
  TIM22->CNT = 0x0000;
  while(TIM22->CNT <= 140);
  
  /* PB4 AF + PP no pull */
  GPIOB->MODER &= 0xFFFFFDFF;
  /* PA4-PA7 */
  GPIOA->MODER &= 0xFFFFBFFF;
   
  /* Measure current value in LPTIM counter before starting oscillations */
  CPT_int1= LptimHandle.Instance->CNT;
  
  /* Set PB4 and start oscillations on sensor */
  GPIOB->BSRR = GPIO_PIN_4;
  
  /* Set pulse width during HSI execution approx. 1.5µs */
  __DSB(); __DSB(); __DSB();
    
  /* back to Analog state */
  GPIOB->MODER |= 0x00000200;
  
  /* Define time capture */
  /* HSI corresponding time capture approx. 70µs */
  TIM22->CNT = 0x0000;
  while(TIM22->CNT <= 2000);  
  
  /* Reset PD7 VDD/2 power supply */
  GPIOD->BRR = GPIO_PIN_7;
  /* Set DAC1 disable */
  DAC->CR &= ~(0x01);
  
  /* Display detection on LD1 when not in LowPower conditions*/
  /* Measure counter after oscillation period */
  CPT_int2 = HAL_LPTIM_ReadCounter(&LptimHandle);
  
  /* Compute the number of total oscillations for this capture */
  CPT_int3 = CPT_int2 - CPT_int1;
  if((CPT_int3 > CPT_max) && (CPT_int3 < 100))
  {
    CPT_max = CPT_int3;
  }
  
  /* If oscillations are damped more than 80% then switch ON LD1 */
  if(CPT_int3 < (CPT_max * 80)/100)
  {
    HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_4);
    detect = 1;
  }
  else
  {
    /* switch OFF LD1 */
    GPIOE->BSRR = GPIO_PIN_4;
    detect = 0;
  }
  
}

/**
* @brief  Display uart transaction state 
* @param  None.
* @note   This display information about the uart transaction.  
* @retval None.
*/
static void LcSensorCountingUserHeader(void)
{
  /* Set the Back Color */
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
  /* Set the Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
}  

/**
* @brief  Get User action 
* @param  sel : User selection (JOY_SEL,...)
* @note
* @retval None
*/
void LcSensorCountingMenuUserAction(uint8_t sel)
{
  switch(sel)
  {
  case JOY_DOWN : 
    //user_stop = 1;
    break;
  default :
    break;
  }
}


/**
* @brief  Configures Comparator
* @param  None
* @retval None
*/
static void COMP_Config(void)
{
  /* Configure the COMP peripheral */
  hcomp2.Instance = COMP2;
  
  hcomp2.Init.InvertingInput    = COMP_INPUT_MINUS_DAC1_CH1;  
  hcomp2.Init.NonInvertingInput = COMP_INPUT_PLUS_IO2;
  hcomp2.Init.OutputPol         = COMP_OUTPUTPOL_NONINVERTED;
  hcomp2.Init.Mode              = COMP_POWERMODE_MEDIUMSPEED; 
  hcomp2.Init.WindowMode        = COMP_WINDOWMODE_DISABLE;
  hcomp2.Init.TriggerMode       = COMP_TRIGGERMODE_IT_RISING_FALLING;
  hcomp2.Init.LPTIMConnection   = COMP_LPTIMCONNECTION_IN2_ENABLED;
  
  if(HAL_COMP_Init(&hcomp2) != HAL_OK)
  {
    /* Error */
    ErrorHandler();
  }
  
  /* Start COMP2 */
  if(HAL_COMP_Start(&hcomp2) != HAL_OK)
  {
    /* Error */
    ErrorHandler();
  }
}


/**
* @brief  Configure the RTC peripheral by selecting the clock source.
* @param  None
* @retval None
*/
static void RTC_Config(void)
{
  /* Configure the RTC peripheral */
  /* Configure RTC prescaler and RTC data registers */
  /* RTC configured as follow:
  - Hour Format    = Format 24
  - Asynch Prediv  = Value according to source clock
  - Synch Prediv   = Value according to source clock
  - OutPut         = Output Disable
  - OutPutPolarity = High Polarity
  - OutPutType     = Open Drain */
  RtcHandle.Instance = RTC;
  RtcHandle.Init.HourFormat = RTC_HOURFORMAT_24;
  RtcHandle.Init.AsynchPrediv = RTC_ASYNCH_PREDIV; /* 0x7F */
  RtcHandle.Init.SynchPrediv = RTC_SYNCH_PREDIV; /* (32768Hz / 128) - 1 = 0x0FF */
  RtcHandle.Init.OutPut = RTC_OUTPUT_DISABLE;
  RtcHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  RtcHandle.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  
  if(HAL_RTC_Init(&RtcHandle) != HAL_OK)
  {
    /* Initialization Error */
    ErrorHandler();
  }

  /* Reset WUTF bit */
  RtcHandle.Instance->ISR &= 0xFFFFFBFF;
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


/** @defgroup HAL_MSP_Private_Functions
* @{
*/

/**
* @brief COMP MSP Initialization
*        This function configures the hardware resources used in this example:
*           - Peripheral's clock enable
* @param hcomp: COMP handle pointer
* @retval None
*/
void HAL_COMP_MspInit(COMP_HandleTypeDef* hcomp)
{
  GPIO_InitTypeDef       GPIO_InitStruct;
  
  /* Enable peripherals and GPIO Clocks */
  /* Enable GPIO clock */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  
  /* Configure peripheral GPIO */
  /* COMP2 Analog GPIO pin configuration */
  GPIO_InitStruct.Mode  = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Pin   = GPIO_PIN_4;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  
  GPIO_InitStruct.Alternate = GPIO_AF7_COMP2;
  GPIO_InitStruct.Mode  = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pin   = GPIO_PIN_7;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  
}


/**
* @brief DAC MSP Initialization
*        This function configures the hardware resources used in this example:
*           - Peripheral's clock enable
*           - Peripheral's GPIO Configuration
* @param hdac: DAC handle pointer
* @retval None
*/
void HAL_DAC_MspInit(DAC_HandleTypeDef* hdac)
{
  GPIO_InitTypeDef          GPIO_InitStruct;
  
  /* Enable peripherals and GPIO Clocks */
  /* Enable GPIO clock ****************************************/
  /* DAC Periph clock enable */
  __HAL_RCC_DAC_CLK_ENABLE();
  
  /* Configure peripheral GPIO */
  /* DAC Channel1 GPIO pin configuration */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}


/**
* @brief  LPTIM MSP Init
* @param  hlptim : LPTIM handle
* @retval None
*/
void HAL_LPTIM_MspInit(LPTIM_HandleTypeDef *hlptim)
{
  GPIO_InitTypeDef     GPIO_InitStruct;
  
  /* Enable LPTIM clock */
  __HAL_RCC_LPTIM1_CLK_ENABLE();
  
  /* Force & Release the LPTIM Periheral Clock Reset */
  /* Force the LPTIM Periheral Clock Reset */
  __HAL_RCC_LPTIM1_FORCE_RESET();
  
  /* Release the LPTIM Periheral Clock Reset */
  __HAL_RCC_LPTIM1_RELEASE_RESET();
  
  /* Enable & Configure LPTIM Ulra Low Power Input */
  /* Configure PC0 (LPTIM1_IN1) in alternate function (AF0), Low speed
  push-pull mode and pull-up enabled.
  Note: In order to reduce power consumption: GPIO Speed is configured in
  LowSpeed */
  
  /* Enable GPIO PORT C */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  
  /* Configure PC0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW     ;
  GPIO_InitStruct.Alternate = GPIO_AF0_LPTIM1;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  
  /* Enable and set LPTIM Interrupt to the highest priority */
  HAL_NVIC_SetPriority(LPTIM1_IRQn,IT_PRIO_LPTIM, 0);
  HAL_NVIC_EnableIRQ(LPTIM1_IRQn);
}


/**
* @brief  Configures the RTC.
* @param  None
* @retval None
*/
static void RTC_DeConfig(void)
{
  /* DeInit Rtc instance */
  if(HAL_RTC_DeInit(&RtcHandle) != HAL_OK)
  {
    /* Initialization Error */
    BSP_LCD_DisplayStringAtLine(3,(uint8_t *)"FATAL ERROR RTC_DECONFIG");
    while(1);
  }
}


/**
* @brief Display the results
* @param  None:
* @retval None
*/
static void DisplayLCSensorResults(uint32_t init_flag)
{
  float val_full_range;
  float val_actual_range;
  float range;
  uint32_t index;
  uint8_t LCDStr[30];
  static uint32_t index_previous = 100;
  uint32_t lcolor,lwhite;
  
  
#define C_POS_BARGRAPH_X      85
#define C_POS_BARGRAPH_Y      120
#define C_HEIGHT_BARGRAPH_Y   20
#define C_LENGHT_BARGRAPH_Y   200
#define C_BAR_OFFSET          10
  
  if (init_flag == 0)
    return;
  
  if (init_flag == 2)
  {
    index_previous = 100;
    BSP_LCD_SetFont(&Font16);
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_DisplayStringAt(10, 40,(uint8_t *)"After start, put a piece of", LEFT_MODE);
    BSP_LCD_DisplayStringAt(10, 55,(uint8_t *)"metal in front of the LC   ", LEFT_MODE);
    BSP_LCD_DisplayStringAt(10, 70,(uint8_t *)"sensor and check the result", LEFT_MODE);
    BSP_LCD_DisplayStringAt(1, 100,(uint8_t *)"Pulses 0                Max", LEFT_MODE);
    sprintf((char*)LCDStr, "Max. Pulses (air)     = ");
    BSP_LCD_DisplayStringAt(5, 170,LCDStr, LEFT_MODE);
    sprintf((char*)LCDStr, "Current Pulse reading = ");
    BSP_LCD_DisplayStringAt(5, 195,LCDStr, LEFT_MODE);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
    BSP_LCD_DisplayStringAt(1, 220,(uint8_t *)"Press JOYSTICK Down to Exit", LEFT_MODE);
    BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    
    BSP_LCD_SetFont(&Font24);
    
    /* Very first time... */
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_DrawRect((C_POS_BARGRAPH_X - 4),
                     (C_POS_BARGRAPH_Y -4),
                     (C_LENGHT_BARGRAPH_Y + 8),
                     (C_HEIGHT_BARGRAPH_Y + 8));
  }
  
  val_full_range = CPT_max;
  val_actual_range = CPT_int3;
  
  /* Compute the scale to be displayed */
  if (val_full_range != 0)
  {
    range = 1 - (val_actual_range/val_full_range);
    if (range < 0)
      range = 0;
  }
  else
    range = 0;
  
  
  lwhite =  (uint32_t) (range * C_LENGHT_BARGRAPH_Y);
  lcolor =  (uint32_t) ((1 - range) * C_LENGHT_BARGRAPH_Y);
  
  index = (uint32_t) ((1 - range) * 100);
  
  BSP_LCD_SetFont(&Font16);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  
  
  sprintf((char*)LCDStr, " %d ",(int)CPT_max);
  BSP_LCD_DisplayStringAt(270, 170,LCDStr, LEFT_MODE);
  
  if(detect == 1)
  {
    BSP_LCD_SetBackColor(LCD_COLOR_DARKGREEN);
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    
  }
  else
  {
    BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  }
  sprintf((char*)LCDStr, " %d ",(int)CPT_int3);
  BSP_LCD_DisplayStringAt(270, 195,LCDStr, LEFT_MODE);
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  
  BSP_LCD_SetFont(&Font24);
  
  /* Index varies from 0 to 10 */
  if (index < index_previous)
  {
    /* It is requested to clear some  part of the  color bar..
    *
    */
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_FillRect(C_POS_BARGRAPH_X + lcolor,
                     C_POS_BARGRAPH_Y,
                     lwhite,
                     C_HEIGHT_BARGRAPH_Y);
  }
  else // if (index > index_previous)
  {
    /* It is requested to update the  color bar..
    *
    */
    BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
    BSP_LCD_FillRect(C_POS_BARGRAPH_X,C_POS_BARGRAPH_Y,lcolor,C_HEIGHT_BARGRAPH_Y);
  }
  
  index_previous = index;
  
}




/**
* @brief RTC Init LC sensor initiates WakeTimer after STOP mode every 32Hz to sense the LC detection
* @param None
* @retval None
*/
static void RTCInitLCSensor(void)
{
  
  /* RTC INITIALIZATION */
  RTC_Config();
  
  /* Update the Calendar Configuration with the LSI exact value */
  RtcHandle.Init.HourFormat = RTC_HOURFORMAT_24;
  RtcHandle.Init.AsynchPrediv = 0x7F;
  RtcHandle.Init.SynchPrediv = 0x130;
  RtcHandle.Init.OutPut = RTC_OUTPUT_DISABLE;
  RtcHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  RtcHandle.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  
  if(HAL_RTC_Init(&RtcHandle) != HAL_OK)
  {
    /* Initialization Error */
    ErrorHandler();
  }
  /* Reset WUTF bit */
  RtcHandle.Instance->ISR &= 0xFFFFFBFF;
  
  /* Note RTC */
  /* Setting the Wake up time to 32Hz (1/32 = 0.03125s)
  RTC_WAKEUPCLOCK_RTCCLK_DIV = RTCCLK_DIV2= 2
  Wakeup Time Base = 2 /(~32.768KHz) = ~0,061035 ms
  Wakeup Time = 0.03125 = 0,061035ms  * WakeUpCounter
  ==> WakeUpCounter = 0.03125/0,061035ms = 512 = 0x200 */
  HAL_RTCEx_SetWakeUpTimer_IT(&RtcHandle, 0x200, RTC_WAKEUPCLOCK_RTCCLK_DIV2);  
}



/**
* @brief  Configures Comparator
* @param  None
* @retval None
*/
static void COMPInitLCSensor(void)
{
  /* Configure the COMP peripheral */
  hcomp2.Instance = COMP2;
  
  hcomp2.Init.InvertingInput    = COMP_INPUT_MINUS_DAC1_CH1;
  hcomp2.Init.NonInvertingInput = COMP_INPUT_PLUS_IO2;
  hcomp2.Init.OutputPol         = COMP_OUTPUTPOL_NONINVERTED;
  hcomp2.Init.Mode              = COMP_POWERMODE_MEDIUMSPEED;
  hcomp2.Init.WindowMode        = COMP_WINDOWMODE_DISABLE;
  hcomp2.Init.TriggerMode       = COMP_TRIGGERMODE_IT_RISING_FALLING;
  hcomp2.Init.LPTIMConnection   = COMP_LPTIMCONNECTION_IN2_ENABLED; 
  
  if(HAL_COMP_Init(&hcomp2) != HAL_OK)
  {
    /* Error */
    ErrorHandler();
  }
}



/**
* @brief LPTIM Init LC sensor initiates LPTIM settings to acquire pulses from LC sensor
* @param None
* @retval None
*/
static void LPTIMInitLCSensor(void)
{
  /* Select the APB clock as LPTIM peripheral clock */
  RCC_PeriphCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LPTIM1;
  RCC_PeriphCLKInitStruct.LptimClockSelection = RCC_LPTIM1CLKSOURCE_HSI;
  HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitStruct);
  
  /* Initialize the LPTIM peripheral */
  /*
  *  Instance        = LPTIM1
  *  Clock Source    = LowPowerOSCillator
  *  Counter source  = External event.
  *  Clock prescaler = 1 (No division)
  *  Counter Trigger = Trigger source COMP2
  *  Output Polarity = High
  *  Update mode     = Immediate (Registers are immediately updated after any
  *                    write access)
  */
  
  LptimHandle.Instance = LPTIM1;
  
  LptimHandle.Init.Clock.Source    = LPTIM_CLOCKSOURCE_ULPTIM;
  LptimHandle.Init.CounterSource   = LPTIM_COUNTERSOURCE_EXTERNAL;
  LptimHandle.Init.Clock.Prescaler = LPTIM_PRESCALER_DIV1;
  LptimHandle.Init.Trigger.Source  = LPTIM_TRIGSOURCE_7;
  LptimHandle.Init.OutputPolarity  = LPTIM_OUTPUTPOLARITY_HIGH;
  LptimHandle.Init.UpdateMode      = LPTIM_UPDATE_IMMEDIATE;
  LptimHandle.Init.Trigger.ActiveEdge = LPTIM_ACTIVEEDGE_RISING;
  
  /* Initialize LPTIM peripheral according to the passed parameters */
  if (HAL_LPTIM_Init(&LptimHandle) != HAL_OK)
  {
    ErrorHandler();
  }
  
  /* Start counting */
  if (HAL_LPTIM_Counter_Start(&LptimHandle, 0xFFFF) != HAL_OK)
  {
    ErrorHandler();
  }
  /* LPTimer enable */
  LptimHandle.Instance->CR |= 0x00000001;
  
  /* LPTimer continuous mode */
  LptimHandle.Instance->CR |= 0x00000004;
}


/**
* @brief DAC Init LC sensor initiates DAC settings to generate COMP2 threshold negative input
* @param None
* @retval None
*/
static void DACInitLCSensor(void)
{
  /* DAC */
  /* Configure the DAC peripheral */
  DacHandle.Instance = DACx;
  if(HAL_DAC_Init(&DacHandle) != HAL_OK)
  {
    /* Initiliazation Error */
    ErrorHandler();
  }
  
  /* Configure DAC channel1 */
  sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  
  if(HAL_DAC_ConfigChannel(&DacHandle, &sConfig, DAC_CHANNEL_1) != HAL_OK)
  {
    /* Channel configuration Error */
    ErrorHandler();
  }
  
  /* Set DAC Channel1 DHR register */
  // full scale = 0xFF, midrange = 0x80;
  if(HAL_DAC_SetValue(&DacHandle, DAC_CHANNEL_1, DAC_ALIGN_12B_R, DAC_VALUE) != HAL_OK)
  {
    /* Setting value Error */
    ErrorHandler();
  }
  
  /* Enable DAC Channel1 */
  if(HAL_DAC_Start(&DacHandle, DAC_CHANNEL_1) != HAL_OK)
  {
    /* Start Error */
    ErrorHandler();
  }  
}



/**
* @brief TIM Init LC sensor initiates TIM settings to generate temporizations
* @param None
* @retval None
*/
static void TIMInitLCSensor(void)
{
  /* TIMx Peripheral clock enable */
  __HAL_RCC_TIM22_CLK_ENABLE();
  
  /* Configure the TIM peripheral */ 
  /* Set TIMx instance */
  TimHandle.Instance = TIM22;
    
  /* Initialize TIMx peripheral as follow:
       + Period = 10000 - 1
       + Prescaler = SystemCoreClock/10000 Note that APB clock = TIMx clock if
                     APB prescaler = 1.
       + ClockDivision = 0
       + Counter direction = Up
  */
  TimHandle.Init.Period = 0xFFFF;
  TimHandle.Init.Prescaler = 0;
  TimHandle.Init.ClockDivision = 0;
  TimHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
  if(HAL_TIM_Base_Init(&TimHandle) != HAL_OK)
  {
    /* Initialization Error */
    ErrorHandler();
  }
  
  /* Start the TIM Base generation */
  /* Start Channel1 */
  if(HAL_TIM_Base_Start(&TimHandle) != HAL_OK)
  {
    /* Starting Error */
    ErrorHandler();
  }
}



/**
* @brief  This function is used to execute once the parameters needed to enter Low Power (I/Os, VREFINT, etc...)
* @param  None
* @retval None
*/
static void StopEntry(void)
{
  
  /* GPIO config */
  /* Enable GPIOs clock */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE(); 
  
  /* Set PD7 to supply VDD/2 reference voltage */
  GPIOD->BSRR = GPIO_PIN_7;
  
  /* Set PB4 to drive LC sensor */
  GPIOB->BSRR = GPIO_PIN_4;
  
  /* Change MODER to Analog state to decrease low power consumption */
  GPIOA->MODER = 0xFFFFFFFF;
  GPIOB->MODER = 0xFFFFFFFF;
  GPIOC->MODER = 0xFFFFFFFF;
  GPIOD->MODER = 0xFFFFFFFF;
  GPIOE->MODER = 0xFFFFFFFF; 
  GPIOH->MODER = 0xFFFFFFFF;  
  
  /* Disable GPIOs clock */
  __HAL_RCC_GPIOC_CLK_DISABLE();
  __HAL_RCC_GPIOE_CLK_DISABLE();
  __HAL_RCC_GPIOH_CLK_DISABLE(); 
  
  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();  
  __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
  
  /* Enter Stop Mode                                             */
  /* The UltraLow power should be put there !!! */
  /* VREFINT is inhibited in STOP mode and wake up is delayed until VREFINT is ready (no fast wake up mode) */
  HAL_PWREx_EnableUltraLowPower();
  HAL_PWREx_DisableFastWakeUp();
  HAL_PWR_EnableSleepOnExit();  
  
  /* Change system clock to 2MHz to be able to use voltage range 3 */
  SystemClock_ConfigMSI_2M();
  
  /* Disable the SysTick timer: must be done after clock config! */
  SysTick->CTRL &= (~SysTick_CTRL_ENABLE_Msk);
  
  /* The voltage scaling allows optimizing the power consumption when the device is 
  clocked below the maximum system frequency, to update the voltage scaling value 
  regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  
  while(!(PWR->CSR & ~PWR_CSR_VOSF));
  
  HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
  
}



/**
* @brief  System Clock Configuration
*         The system Clock is configured as follow :
*            System Clock source            = (MSI)
*            MSI Range                      = 2
*            SYSCLK(Hz)                     = 2MHz
*            HCLK(Hz)                       = 2MHz
*            AHB Prescaler                  = 2
*            APB1 Prescaler                 = 1
*            APB2 Prescaler                 = 1
*            Main regulator output voltage  = Scale2 mode
* @param  None
* @retval None
*/
void SystemClock_ConfigMSI_2M(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  
  /* Enable MSI Oscillator */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_5;
  RCC_OscInitStruct.MSICalibrationValue = 0x00;
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
}



/**
* @brief  This function save the Gpio configuration
* @param  None
* @retval None
*/
static void CopyGpioConfigLcSensor(uint32_t  index, GPIO_TypeDef * gpio)
{
  GpioConfigLcSensor[index].MODER = gpio-> MODER;
  GpioConfigLcSensor[index].OTYPER = gpio-> OTYPER;
  GpioConfigLcSensor[index].OSPEEDR = gpio-> OSPEEDR;
  GpioConfigLcSensor[index].PUPDR = gpio-> PUPDR;
  GpioConfigLcSensor[index].IDR = gpio-> IDR;
  GpioConfigLcSensor[index].ODR = gpio-> ODR;
  GpioConfigLcSensor[index].BSRR = gpio-> BSRR;
  GpioConfigLcSensor[index].LCKR = gpio-> LCKR;
  GpioConfigLcSensor[index].AFR[0] = gpio-> AFR[0];
  GpioConfigLcSensor[index].AFR[1] = gpio-> AFR[1];
  GpioConfigLcSensor[index].BRR = gpio-> BRR;
}

/**
* @brief  This function restore back the Gpio configuration
* @param  None
* @retval None
*/
static void RestoreGpioConfigLcSensor(uint32_t  index, GPIO_TypeDef * gpio)
{
  gpio-> MODER = GpioConfigLcSensor[index].MODER ;
  gpio-> OTYPER = GpioConfigLcSensor[index].OTYPER ;
  gpio-> OSPEEDR = GpioConfigLcSensor[index].OSPEEDR ;
  gpio-> PUPDR = GpioConfigLcSensor[index].PUPDR ;
  gpio-> IDR = GpioConfigLcSensor[index].IDR ;
  gpio-> ODR = GpioConfigLcSensor[index].ODR ;
  gpio-> BSRR = GpioConfigLcSensor[index].BSRR ;
  gpio-> LCKR = GpioConfigLcSensor[index].LCKR;
  gpio-> AFR[0] = GpioConfigLcSensor[index].AFR[0] ;
  gpio-> AFR[1] = GpioConfigLcSensor[index].AFR[1] ;
  gpio-> BRR = GpioConfigLcSensor[index].BRR ;
}

/**
* @brief  This function copy back the Gpio configuration
* @param  None
* @retval None
*/
static void SaveGpioContext(void)
{
  
  /* Store the GPIOs..*/
  CopyGpioConfigLcSensor(0,GPIOA);
  CopyGpioConfigLcSensor(1,GPIOB);
  CopyGpioConfigLcSensor(2,GPIOC);
  CopyGpioConfigLcSensor(3,GPIOD);
  CopyGpioConfigLcSensor(4,GPIOE);
  CopyGpioConfigLcSensor(5,GPIOH);
}

/**
* @brief Restore Demonstration context (GPIOs Configurations, peripherals,...).
* @param None
* @retval None
*/
static void RestoreGpioContext(void)
{
  /* Store the GPIOs..*/
  RestoreGpioConfigLcSensor(0,GPIOA);
  RestoreGpioConfigLcSensor(1,GPIOB);
  RestoreGpioConfigLcSensor(2,GPIOC);
  RestoreGpioConfigLcSensor(3,GPIOD);
  RestoreGpioConfigLcSensor(4,GPIOE);
  RestoreGpioConfigLcSensor(5,GPIOH);
  
}

/**
* @brief DisplayActionToExit.
* @param None
* @retval None
*/
static void DisplayMsgWhenInPowerMode(void)
{
    BSP_LCD_SetFont(&Font16);
    BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
    BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
    BSP_LCD_DisplayStringAtLine(3, (uint8_t *) "   LC sensor application");
    BSP_LCD_DisplayStringAtLine(4, (uint8_t *) "   running in low power");
    BSP_LCD_DisplayStringAtLine(5, (uint8_t *) "   mode.               ");
    BSP_LCD_DisplayStringAtLine(7, (uint8_t *) "Idd measurement not displayed");
    BSP_LCD_DisplayStringAtLine(8, (uint8_t *) "Current consumption possible");
    BSP_LCD_DisplayStringAtLine(9, (uint8_t *) "on JP10.");

    BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
    BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
    BSP_LCD_FillRect(0, BSP_LCD_GetYSize()-55, BSP_LCD_GetXSize(), 55);
    BSP_LCD_SetFont(&Font16);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_DisplayStringAtLine(12,(uint8_t *)" Press Reset to exit and");
    BSP_LCD_DisplayStringAtLine(13,(uint8_t *)" reboot...");
    BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_SetFont(&Font24);
}
/**
* @}
*/

/**
* @}
*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

