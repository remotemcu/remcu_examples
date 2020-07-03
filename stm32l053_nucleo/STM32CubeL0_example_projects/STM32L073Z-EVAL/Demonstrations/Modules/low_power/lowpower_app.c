
/**
  ******************************************************************************
  * @file    lowpower_app.c
  * @author  MCD Application Team   
  * @brief   low power application.
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
#define __LOWPOWER_APP_C

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include "k_config.h"
#include "k_module.h"
#include "k_menu.h"
#include "k_window.h"

/** @addtogroup MODULES
  * @{
  */

/** @defgroup LOWPOWER_MODULE
  * @brief low power routines 
  * @{
  */

  typedef enum {
      CONFIG_CLOCK_HSE_32M,
      CONFIG_CLOCK_MSI_64K,
      CONFIG_CLOCK_HSI_16M,
      CONFIG_CLOCK_DEFAULT,
  } clockConfigTypeEnum;

/* External function prototypes -----------------------------------------------*/

extern void SystemClock_Config(void);
/* Private typedef ----------------------------------------------------------*/    
/* Private constants ----------------------------------------------------------*/

/* Global function prototypes -----------------------------------------------*/
KMODULE_RETURN _LowPowerDemoExec(void);
KMODULE_RETURN _LowPowerDemoConfig(void);
KMODULE_RETURN _LowPowerDemoUnConfig(void);

void LowPowerDemo(void);
void LowPowerUserAction(uint8_t sel);
void DisplayStandbyIddMeasurement(void);

/* Private function prototypes -----------------------------------------------*/
static void StartIddMeasurement(void);

static void LowPowerStopEXTI(void);
static void LowPowerStopRTCAlarm(void);

static void LowPowerStandbyEXTI(void);
static void LowPowerStandbyRTCAlarm(void);

static void LowPowerSleepEXTI(void);
static void LowPowerSleepRTCAlarm(void);

static void LowPowerLowSleepEXTI(void);
static void LowPowerLowSleepRTCAlarm(void);

static void LowPowerRunEXTI(void);
static void LowPowerRunRTCAlarm(void);

static void RunEXTI(void);
static void RunRTCAlarm(void);

static void RTC_Init(void);
static void LowPowerHandleAlarm(void);
static void RTC_Config(void);
static void DesactivateAlarm(void);
static void RTC_DeConfig(void);

static void InitUserInteractionInExtiMode(char * messTitle,char * messActionToDo);
static void InitUserInteractionInRtcMode(char * messTitle,char * messActionToDo);
static void WaitForTamperButton(void);
static void ManageIddMeasurementMenu(char * messTitle,uint32_t flagModeIt);
static void IddInfoDisplay(uint32_t iddvalueInNanoAmps,uint32_t iddState,char * messTitle);
static void CopyGpioConfiguration(uint32_t index , GPIO_TypeDef * gpio);
static void RestoreGpioConfiguration(uint32_t index , GPIO_TypeDef * gpio);
static void SaveGpioContextAndDisable(uint32_t mode);
static void RestoreGpioContext(void);
static void DisplayNewClockConfig(clockConfigTypeEnum clockConfig);
static void ApplyNewClockConfig(clockConfigTypeEnum clockConfig);
static void ManageSleepLowPowerMode(uint32_t mode,clockConfigTypeEnum clockConfig);
static void ManageRunMode(uint32_t mode,clockConfigTypeEnum clockConfig);
static void ManageStopMode(uint32_t mode);
static void ManageSleepMode(uint32_t mode);
static void ManageStandbyMode(uint32_t mode);
static void DisplayActionExti(void);
static void DisplaySimpleAction(uint8_t * messAction);
static void DisplayDelayAction(void);

/* Defines used for the menu. */

/* Max size                               "------------------" */

#define LINE_EMPTY                        "                  "
#define MAIN_TITLE                        "Low power"

#define MAIN_MENU_STOP_MODE               "Stop mode"
#define MAIN_MENU_STANDBY_MODE            "Standby mode"
#define MAIN_MENU_SLEEP_MODE              "Sleep mode"
#define MAIN_MENU_LP_SLEEP_MODE           "LP sleep mode"
#define MAIN_MENU_RUN_MODE                "Low power run mode"
#define MAIN_MENU_RUN_STD_MODE            "Run mode"

#define IDD_MEASURE_TITLE_SLEEP           "Sleep measure     "
#define IDD_MEASURE_TITLE_STOP            "Stop measure      "
#define IDD_MEASURE_TITLE_STANDBY         "Standby measure   "
#define IDD_MEASURE_TITLE_RUN             "Run mode measure  "
#define IDD_MEASURE_TITLE_LP_RUN          "LP Run measure    "
#define IDD_MEASURE_TITLE_LP_SLEEP        "LP Sleep measure  "

#define SUBMENU_STOP_MODE_EXTI            "Exit Stop via EXTI"
#define SUBMENU_STOP_MODE_RTC             "Exit Stop via RTC"

#define SUBMENU_STANDBY_MODE_EXTI         "Exit Stby via EXTI"
#define SUBMENU_STANDBY_MODE_RTC          "Exit Stby via RTC"

#define SUBMENU_SLEEP_MODE_EXTI           "Exit Sleep : EXTI"
#define SUBMENU_SLEEP_MODE_RTC            "Exit Sleep : RTC"

#define SUBMENU_LPSLEEP_MODE_EXTI         "Exit LP Sleep:EXTI"
#define SUBMENU_LPSLEEP_MODE_RTC          "Exit LP Sleep:RTC"

#define SUBMENU_RUN_MODE_EXTI             "Exit Run via EXTI"
#define SUBMENU_RUN_MODE_RTC              "Exit Run via RTC"

#define SUBMENU_RUN_STD_MODE_EXTI         "Exit Run via EXTI"
#define SUBMENU_RUN_STD_MODE_RTC          "Exit Run via RTC"

#define TITLE_SUBMENU_STOP_EXTI           "Stop mode (EXTI)"
#define TITLE_SUBMENU_STOP_RTC            "Stop mode (EXTI)"

#define TITLE_SUBMENU_STANDBY_EXTI        "Standby mode EXTI"
#define TITLE_SUBMENU_STANDBY_RTC         "Standby mode (RTC)"

#define TITLE_SUBMENU_LOWSLEEP_EXTI       "LP sleep mode EXTI"
#define TITLE_SUBMENU_SLEEP_EXTI          "Sleep mode (EXTI)"

#define TITLE_SUBMENU_LOWSLEEP_RTC        "Low sleep mode RTC"
#define TITLE_SUBMENU_SLEEP_RTC           "Sleep mode (RTC)"

#define TITLE_SUBMENU_RUN_EXTI            "Low power run mode (EXTI)"
#define TITLE_SUBMENU_RUN_RTC             "Low power run mode (RTC)"

#define TITLE_SUBMENU_STD_RUN_EXTI        "Standard run mode (EXTI)"
#define TITLE_SUBMENU_STD_RUN_RTC         "Standard run mode (RTC)"

#define TITLE_ENDMENU_STOP_EXTI           "\nStop mode started \n"
#define TITLE_ENDMENU_STOP_RTC            "\nStop mode started \n"
#define TITLE_ENDMENU_STANDBY_EXTI        "\nStandby mode started \n"
#define TITLE_ENDMENU_STANDBY_RTC         "\nStandby mode started \n"
#define TITLE_ENDMENU_LOWSLEEP_EXTI       "\nSleep mode started \n"
#define TITLE_ENDMENU_LOWSLEEP_RTC        "\nSleep mode started \n"
#define TITLE_ENDMENU_SLEEP_EXTI          "\nSleep mode started \n"
#define TITLE_ENDMENU_SLEEP_RTC           "\nSleep mode started \n"
#define TITLE_ENDMENU_RUN_EXTI            "\nLow power run mode started \n"
#define TITLE_ENDMENU_RUN_RTC             "\nLow power run mode started \n"
#define TITLE_ENDMENU_RUN_STD_EXTI        "\n Run mode started \n"
#define TITLE_ENDMENU_RUN_STD_RTC         "\nRun mode started \n"

#define TITLE_RETURN                      "RETURN"

#define C_STRING_PRESS_TAMPER     " Press Tamper to Exit"
#define C_STRING_WAIT_RTC         " Wait RTC to Exit"

#define   C_MODE_FULL_DISABLE   0
#define   C_MODE_EXTI           1
#define   C_MODE_RTC            2


/* Global variable  */
RTC_HandleTypeDef RtcHandle;

/* Private Variable ----------------------------------------------------------*/

static GPIO_TypeDef   GpioConfiguration[6];
static __IO uint8_t   FlagRTCAlarmAsExpired = 0;
static __IO uint8_t   FlagUserActionDetection=0;

/* Menu handlers  ----------------------------------------------------------*/

/* stop mode menu */
const tMenuItem StopModeMenuItems[] =
{
    {SUBMENU_STOP_MODE_EXTI, 14, 30,SEL_EXEC, MODULE_NONE,LowPowerStopEXTI,LowPowerUserAction, NULL, NULL },
    {SUBMENU_STOP_MODE_RTC, 14, 30,SEL_EXEC, MODULE_NONE, LowPowerStopRTCAlarm,LowPowerUserAction, NULL, NULL },
    {TITLE_RETURN,0,0,SEL_EXIT, MODULE_NONE, NULL, NULL, NULL, NULL }
};

const tMenu StopModeMenu = {
  MAIN_MENU_STOP_MODE, StopModeMenuItems, countof(StopModeMenuItems), TYPE_TEXT, 1, 1
};
/* SIZE MAX "                  " */
/* standby mode menu */
const tMenuItem StandbyModeMenuItems[] =
{   
    {SUBMENU_STANDBY_MODE_EXTI, 14,30,SEL_EXEC, MODULE_NONE, LowPowerStandbyEXTI,LowPowerUserAction, NULL, NULL },
    {SUBMENU_STANDBY_MODE_RTC,14,30,SEL_EXEC, MODULE_NONE, LowPowerStandbyRTCAlarm,LowPowerUserAction, NULL, NULL },
    {TITLE_RETURN, 0, 0,SEL_EXIT, MODULE_NONE, NULL, NULL, NULL, NULL }
};

const tMenu StandbyModeMenu = {
  MAIN_MENU_STANDBY_MODE, StandbyModeMenuItems, countof(StandbyModeMenuItems), TYPE_TEXT, 1, 1
};

/* sleep mode menu */
const tMenuItem SleepModeMenuItems[] =
{
    {SUBMENU_SLEEP_MODE_EXTI, 14,30,SEL_EXEC, MODULE_NONE,LowPowerSleepEXTI,LowPowerUserAction, NULL, NULL },
    {SUBMENU_SLEEP_MODE_RTC,14,30,SEL_EXEC, MODULE_NONE,LowPowerSleepRTCAlarm,LowPowerUserAction, NULL, NULL },
    {TITLE_RETURN, 0, 0,SEL_EXIT, MODULE_NONE, NULL, NULL, NULL, NULL }
};

const tMenu SleepModeMenu = {
  MAIN_MENU_SLEEP_MODE, SleepModeMenuItems, countof(SleepModeMenuItems), TYPE_TEXT, 1, 1
};

/* low power sleep mode menu */
const tMenuItem LowSleepModeMenuItems[] =
{
    {SUBMENU_LPSLEEP_MODE_EXTI, 14,30,SEL_EXEC, MODULE_NONE,LowPowerLowSleepEXTI,LowPowerUserAction, NULL, NULL },
    {SUBMENU_LPSLEEP_MODE_RTC,14,30,SEL_EXEC, MODULE_NONE,LowPowerLowSleepRTCAlarm,LowPowerUserAction, NULL, NULL },
    {TITLE_RETURN, 0, 0,SEL_EXIT, MODULE_NONE, NULL, NULL, NULL, NULL }
};

const tMenu LowSleepModeMenu = {
  MAIN_MENU_LP_SLEEP_MODE, LowSleepModeMenuItems, countof(LowSleepModeMenuItems), TYPE_TEXT, 1, 1
};

/* low power run mode menu */
const tMenuItem LowRunModeMenuItems[] =
{
    {SUBMENU_RUN_MODE_EXTI, 14,30,SEL_EXEC, MODULE_NONE,LowPowerRunEXTI,LowPowerUserAction, NULL, NULL },
    {SUBMENU_RUN_MODE_RTC,14,30,SEL_EXEC, MODULE_NONE,LowPowerRunRTCAlarm,LowPowerUserAction, NULL, NULL },
    {TITLE_RETURN, 0, 0,SEL_EXIT, MODULE_NONE, NULL, NULL, NULL, NULL }
};

const tMenu LowRunModeMenu = {
  MAIN_MENU_RUN_MODE, LowRunModeMenuItems, countof(LowRunModeMenuItems), TYPE_TEXT, 1, 1
};

/* low power run mode menu */
const tMenuItem RunModeMenuItems[] =
{
    {SUBMENU_RUN_STD_MODE_EXTI, 14,30,SEL_EXEC, MODULE_NONE,RunEXTI,LowPowerUserAction, NULL, NULL },
    {SUBMENU_RUN_STD_MODE_RTC,14,30,SEL_EXEC, MODULE_NONE,RunRTCAlarm,LowPowerUserAction, NULL, NULL },
    {TITLE_RETURN, 0, 0,SEL_EXIT, MODULE_NONE, NULL, NULL, NULL, NULL }
};

const tMenu RunModeMenu = {
  MAIN_MENU_RUN_STD_MODE, RunModeMenuItems, countof(RunModeMenuItems), TYPE_TEXT, 1, 1
};

/* Main menu */
const tMenuItem LowPowerMenuItems[] =
{
    {MAIN_MENU_STOP_MODE,       14, 30,SEL_SUBMENU, MODULE_NONE, NULL, NULL, (const tMenu*)&StopModeMenu, NULL },
    {MAIN_MENU_STANDBY_MODE,    14, 30,SEL_SUBMENU, MODULE_NONE, NULL, NULL, (const tMenu*)&StandbyModeMenu, NULL },
    {MAIN_MENU_SLEEP_MODE,      14, 30,SEL_SUBMENU, MODULE_NONE, NULL, NULL, (const tMenu*)&SleepModeMenu, NULL },
    {MAIN_MENU_LP_SLEEP_MODE,   14, 30,SEL_SUBMENU, MODULE_NONE, NULL, NULL, (const tMenu*)&LowSleepModeMenu, NULL },
    {MAIN_MENU_RUN_MODE,        14, 30,SEL_SUBMENU, MODULE_NONE, NULL, NULL, (const tMenu*)&LowRunModeMenu, NULL },
    {MAIN_MENU_RUN_STD_MODE,    14, 30,SEL_SUBMENU, MODULE_NONE, NULL, NULL, (const tMenu*)&RunModeMenu, NULL },
    {TITLE_RETURN,              0,  0,SEL_EXIT, MODULE_NONE, NULL, NULL, NULL, NULL }
};

const tMenu LowpowerMenu = {
  MAIN_TITLE, LowPowerMenuItems, countof(LowPowerMenuItems), TYPE_TEXT, 1, 1};



/* Private typedef -----------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
const K_ModuleItem_Typedef LowPowerModuleItem =
{
  MODULE_LOW_POWER,
  _LowPowerDemoConfig,
  _LowPowerDemoExec,
  _LowPowerDemoUnConfig,
  NULL
};

/**
  * @brief  Configure the Lowpower application 
  * @param  None.
  * @note   run and display information about the lowpower feature.  
  * @retval None.
  */
KMODULE_RETURN _LowPowerDemoConfig(void)
{
  return KMODULE_OK;
}

/**
  * @brief  un-Configure the Lowpower application 
  * @param  None.
  * @note   run and display information about the lowpower feature.  
  * @retval None.
  */
KMODULE_RETURN _LowPowerDemoUnConfig(void)
{
  return KMODULE_OK;
}

/**
  * @brief  Run the Lowpower application 
  * @param  None.
  * @note   run and display information about the lowpower feature.  
  * @retval None.
  */
KMODULE_RETURN _LowPowerDemoExec(void)
{
  /* Prepare Execute the main MMI of lowpower application */
  kMenu_Execute(LowpowerMenu);
  return KMODULE_OK;
}

/**
  * @brief  Get User action 
  * @param  sel : User selection (JOY_SEL,...)
  * @note   This example is the only way to get user information.  
  * @retval None
  */
void LowPowerUserAction(uint8_t sel)
{
  if (FlagUserActionDetection == 0)
  {
    FlagUserActionDetection = 1;
  }
}

/**
  * @brief  Enter in standard run mode and exit by pressing the tamper button
  * @param  None
  * @note   This example enter in stop mode.
  * @retval None
  */

static void RunEXTI(void)
{

  /* Ensure that the RTC is off */
  RTC_Config();
  RTC_DeConfig();

  /* Init the demo to handle wake through EXTI */
  InitUserInteractionInExtiMode(TITLE_SUBMENU_STD_RUN_EXTI,
                                TITLE_ENDMENU_RUN_STD_EXTI);

  DisplayNewClockConfig(CONFIG_CLOCK_HSI_16M);
  ApplyNewClockConfig(CONFIG_CLOCK_HSI_16M);

  __HAL_RCC_PWR_CLK_ENABLE();

  /* Start IDD measurement */
  StartIddMeasurement();

  /* GPIO config */
  SaveGpioContextAndDisable(C_MODE_FULL_DISABLE);
  BSP_PB_Init(BUTTON_TAMPER,BUTTON_MODE_EXTI);

  /* Do nothing until the user press Tamper Button...*/
  WaitForTamperButton();

  /* GPIO config and clock */
  RestoreGpioContext();
  ApplyNewClockConfig(CONFIG_CLOCK_HSE_32M);

  /* Measure the power */
  ManageIddMeasurementMenu(IDD_MEASURE_TITLE_RUN,0);
}

/**
  * @brief  Enter in stop mode and exit by an alarm
  * @param  None
  * @note   This example enter in stop mode.
  * @retval None
  */
static void RunRTCAlarm(void)
{
  /* Init the demo to handle wake through EXTI */
  InitUserInteractionInRtcMode(SUBMENU_RUN_STD_MODE_RTC,
                               TITLE_ENDMENU_RUN_STD_RTC);

  DisplayNewClockConfig(CONFIG_CLOCK_HSI_16M);
  ApplyNewClockConfig(CONFIG_CLOCK_HSI_16M);

  /* Start the measurement */
  StartIddMeasurement();

  /* GPIO config */
  SaveGpioContextAndDisable(C_MODE_FULL_DISABLE);

  /* Wait until RTC alarm expire  */
  while(FlagRTCAlarmAsExpired == 0)
  {
  }

  /* GPIO config and clock */
  RestoreGpioContext();
  ApplyNewClockConfig(CONFIG_CLOCK_HSE_32M);

  /* Desactivate the alarm */
  DesactivateAlarm();

  /* Measure the power */
  ManageIddMeasurementMenu(IDD_MEASURE_TITLE_RUN,0);

}

/**
  * @brief  Enter in stop mode and exit by pressing the tamper button
  * @param  None
  * @note   This example enter in stop mode.  
  * @retval None
  */

static void LowPowerStopEXTI(void)
{
  /* Ensure that the RTC is off */
  RTC_Config();
  RTC_DeConfig();

  /* Init the demo to handle wake through EXTI */
  InitUserInteractionInExtiMode(TITLE_SUBMENU_STOP_EXTI,
                                TITLE_ENDMENU_STOP_EXTI);

  ManageStopMode(C_MODE_EXTI);
}

/**
  * @brief  Enter in stop mode and exit by an alarm
  * @param  None
  * @note   This example enter in stop mode.  
  * @retval None
  */
static void LowPowerStopRTCAlarm(void)
{
    /* Init the demo to handle wake through EXTI */
  InitUserInteractionInRtcMode(SUBMENU_STOP_MODE_RTC,
                               TITLE_ENDMENU_STOP_RTC);

  ManageStopMode(C_MODE_RTC);
}

/**
  * @brief  Enter in low power sleep mode and exit by pressing the tamper button
  * @param  None
  * @note   This example enter in stop mode.
  * @retval None
  */

static void LowPowerLowSleepEXTI(void)
{
  /* Ensure that the RTC is off */
  RTC_Config();
  RTC_DeConfig();

  /* Init the demo to handle wake through EXTI */
  InitUserInteractionInExtiMode(TITLE_SUBMENU_LOWSLEEP_EXTI,
                                TITLE_ENDMENU_SLEEP_EXTI);

  DisplayNewClockConfig(CONFIG_CLOCK_MSI_64K);
  ManageSleepLowPowerMode(C_MODE_EXTI,CONFIG_CLOCK_MSI_64K);
}

/**
  * @brief  Enter in low power sleep and exit by an alarm
  * @param  None
  * @note   This example enter in stop mode.
  * @retval None
  */
static void LowPowerLowSleepRTCAlarm(void)
{

  /* Init the demo to handle wake through RTC */
  InitUserInteractionInRtcMode(TITLE_SUBMENU_LOWSLEEP_RTC,
                               TITLE_ENDMENU_LOWSLEEP_RTC);

  DisplayNewClockConfig(CONFIG_CLOCK_MSI_64K);
  ManageSleepLowPowerMode(C_MODE_RTC,CONFIG_CLOCK_MSI_64K);
}

/**
  * @brief  Enter in low power sleep mode and exit by pressing the tamper button
  * @param  None
  * @note   This example enter in stop mode.
  * @retval None
  */

static void LowPowerSleepEXTI(void)
{
  /* Ensure that the RTC is off */
  RTC_Config();
  RTC_DeConfig();

  /* Init the demo to handle wake through EXTI */
  InitUserInteractionInExtiMode(TITLE_SUBMENU_SLEEP_EXTI,
                                TITLE_ENDMENU_LOWSLEEP_EXTI);

  ManageSleepMode(C_MODE_EXTI);

}

/**
  * @brief  Enter in sleep and exit by an alarm
  * @param  None
  * @note   This example enter in stop mode.
  * @retval None
  */
static void LowPowerSleepRTCAlarm(void)
{

  /* Init the demo to handle wake through RTC */
  InitUserInteractionInRtcMode(TITLE_SUBMENU_SLEEP_RTC,
                               TITLE_ENDMENU_LOWSLEEP_RTC);

   ManageSleepMode(C_MODE_EXTI);
}

/**
  * @brief  Enter in Standby mode and exit by pressing the tamper button
  * @param  None
  * @note   This example enter in stop mode.
  * @retval None
  */

static void LowPowerStandbyEXTI(void)
{

  /* Ensure that the RTC is off */
  RTC_Config();
  RTC_DeConfig();

  /* Disable all used wakeup sources: PWR_WAKEUP_PIN2 */
  HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN2);

  /* Init the demo to handle wake through EXTI */
  InitUserInteractionInExtiMode(TITLE_SUBMENU_STANDBY_EXTI,
                                TITLE_ENDMENU_STANDBY_EXTI);

  ManageStandbyMode(C_MODE_EXTI);

}

/**
  * @brief  Enter in Standby and exit by an alarm
  * @param  None
  * @note   This example enter in stop mode.
  * @retval None
  */
static void LowPowerStandbyRTCAlarm(void)
{

  HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN2);

   /* Init the demo to handle wake through RTC */
  InitUserInteractionInRtcMode(TITLE_SUBMENU_STANDBY_RTC,
                               TITLE_ENDMENU_STANDBY_RTC);

  ManageStandbyMode(C_MODE_RTC);
}

/**
  * @brief  Enter in low power run mode mode and exit by pressing the tamper button
  * @param  None
  * @note   This example enter in stop mode.
  * @retval None
  */

static void LowPowerRunEXTI(void)
{
  /* Ensure that the RTC is off */
  RTC_Config();
  RTC_DeConfig();

  /* Init the demo to handle wake through EXTI */
  InitUserInteractionInExtiMode(TITLE_SUBMENU_RUN_EXTI,
                                TITLE_ENDMENU_RUN_EXTI);

  DisplayNewClockConfig(CONFIG_CLOCK_MSI_64K);
  ManageRunMode(C_MODE_EXTI,CONFIG_CLOCK_MSI_64K);
}

/**
  * @brief  Enter in low power run mode and exit by an alarm
  * @param  None
  * @note   This example enter in stop mode.
  * @retval None
  */
static void LowPowerRunRTCAlarm(void)
{

  /* Init the demo to handle wake through RTC */
  InitUserInteractionInRtcMode(TITLE_SUBMENU_RUN_RTC,
                               TITLE_ENDMENU_RUN_RTC);

  DisplayNewClockConfig(CONFIG_CLOCK_MSI_64K);
  ManageRunMode(C_MODE_RTC,CONFIG_CLOCK_MSI_64K);
}

/**
  * @brief  Configures the RTC.
  * @param  None
  * @retval None
  */
static void RTC_Config(void)
{
  /* Enable Power Clock*/
  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* Allow Access to RTC Backup domaine */
  HAL_PWR_EnableBkUpAccess();
  
  /* Initialize the RTC */
  RTC_Init();
  
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
  * @brief  DesactivateAlarm
  * @param  None
  * @retval None
  */
static void DesactivateAlarm(void)
{
  /* Desactivate the alarm */
  HAL_RTC_DeactivateAlarm(&RtcHandle, RTC_ALARM_A);
  /* Clear the Alarm interrupt pending bit */
  __HAL_RTC_ALARM_CLEAR_FLAG(&RtcHandle,RTC_FLAG_ALRAF);
  /* Clear the EXTI's line Flag for RTC Alarm */
  __HAL_RTC_ALARM_EXTI_CLEAR_FLAG();

  /*Deinitialise the RTC */
  RTC_DeConfig();
}

/**
  * @brief  Initializarion of RTC
  * @param  RTC handle
  * @note   This function wait user info to setup the alarm.
  * @retval None
  */
static void RTC_Init(void)
{
    RtcHandle.Instance = RTC;
    RtcHandle.Init.HourFormat = RTC_HOURFORMAT_24;
    RtcHandle.Init.AsynchPrediv = RTC_ASYNCH_PREDIV;
    RtcHandle.Init.SynchPrediv = RTC_SYNCH_PREDIV;
    RtcHandle.Init.OutPut = RTC_OUTPUT_DISABLE;
    RtcHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
    RtcHandle.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
    if(HAL_RTC_Init(&RtcHandle) != HAL_OK)
    {
      /* Initialization Error */
      BSP_LCD_DisplayStringAtLine(3,(uint8_t *)"RTC_Init");
      while(1);
    }

      /*#### Disable all used wakeup sources ####*/
     HAL_RTC_DeactivateAlarm(&RtcHandle, RTC_ALARM_A);
     /* Disable all previous wake up interrupt */
     HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN1);

     /*#### Clear all related wakeup flags ####*/
     /* Clear the Alarm interrupt pending bit */
     __HAL_RTC_ALARM_CLEAR_FLAG(&RtcHandle,RTC_FLAG_ALRAF);
     /* Clear PWR wake up Flag */
     __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
}

/**
  * @brief  Get user info to setup an alarm
  * @param  RTC handle
  * @note   This function wait user info to setup the alarm.  
  * @retval None
  */
static void LowPowerHandleAlarm(void)
{
  enum {
    HOURS,
    MINUTES,
    SECONDS,
    END
  };
  
  RTC_DateTypeDef currentdate;
  RTC_TimeTypeDef time, currenttime;
  RTC_AlarmTypeDef Alarm;
  uint8_t temp[16];
  uint8_t exit = 0;
  uint8_t index = SECONDS;
  uint8_t position = 0;
  JOYState_TypeDef JoyState = JOY_NONE;

  /****************************/
  /* Init                     */
  /****************************/

  memset(&Alarm, 0x00, sizeof(RTC_AlarmTypeDef));
  memset(&time,0x00,sizeof(RTC_TimeTypeDef));
  memset(&currenttime,0x00,sizeof(RTC_TimeTypeDef));
  

  /**************************************/
  /* Get the time from the user         */
  /**************************************/

  /* Set the JOYSTICK in GPIO mode */
  BSP_JOY_Init(JOY_MODE_GPIO);

  /* Set the initial time to 5 sec by default*/
  time.Seconds = 5;
  /* Get the alarm time from user */
  do
  {
    BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
    sprintf((char *)temp, "    %.2d:%.2d:%.2d", time.Hours, time.Minutes, time.Seconds);
    BSP_LCD_DisplayStringAt(0, 5 * Font24.Height, temp, NO_MODE);
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    
    switch(index)
    {
    case HOURS :
      position = 4;
      sprintf((char *)temp, "%.2d", time.Hours);
      break;
    case MINUTES:
      position = 7;
      sprintf((char *)temp, "%.2d", time.Minutes);
      break;
    case SECONDS :
      position = 10;
      sprintf((char *)temp, "%.2d", time.Seconds);
      break;
    }
    BSP_LCD_DisplayStringAt(position* Font24.Width, 5 * Font24.Height, temp, NO_MODE);
    
    /* Retrieve the joystik state */
    HAL_Delay(100);
    JoyState = BSP_JOY_GetState();
    
    switch(JoyState)
    {
    case JOY_UP :
      if(index == HOURS)
      {
        if( time.Hours == 23 ) time.Hours = 0;
        else
          time.Hours++;
      }
      
      if(index == MINUTES)
      {
        if(time.Minutes == 59 ) time.Minutes= 0;
        else
          time.Minutes++;
      }
      if(index == SECONDS)
      {
        if(time.Seconds == 59 ) time.Seconds =0;
        else
          time.Seconds++;
      }
      break;
    case JOY_DOWN :
      if(index == HOURS)
      {
        if (time.Hours == 0 ) time.Hours = 23;
        else time.Hours--;
      }
      if(index == MINUTES)
      {
        if(time.Minutes == 0) time.Minutes=59;
        else 
          time.Minutes--;
      }
      if(index == SECONDS)
      {
        if(time.Seconds == 0) time.Seconds = 59;
        else
          time.Seconds--;
      }
      break;
    case JOY_RIGHT :
      if(index != SECONDS ) index++; 
      break;
    case JOY_LEFT :
      if(index != HOURS ) index--;
      break;
    case JOY_SEL :
      exit = 1;
      break;
    case JOY_NONE :
       break;
    }
    
    /* 4 seconds are needed to proceed to the measurement */
    if ((time.Seconds <= 4) && (time.Minutes == 0) && (time.Hours == 0))
      time.Seconds = 4;
    
  } while(exit == 0);

  /**************************************/
  /* Manage the RTC timer               */
  /**************************************/

  /* Get the curent time */
  HAL_RTC_GetTime(&RtcHandle, &currenttime,  RTC_FORMAT_BIN);
  HAL_RTC_GetDate(&RtcHandle, &currentdate, RTC_FORMAT_BIN);

  time.SubSeconds = currenttime.SubSeconds;
  if((time.Seconds + currenttime.Seconds) > 60 )  time.Minutes++;
  time.Seconds = ((time.Seconds + currenttime.Seconds) % 60);
  
  if((time.Minutes + currenttime.Minutes) > 60 )  time.Hours++;
  time.Minutes = ((time.Minutes + currenttime.Minutes) % 60);
  
  time.Hours = ((time.Hours + currenttime.Hours) % 24);
  
  /* Set the alarm */
  Alarm.Alarm = RTC_ALARM_A;
  Alarm.AlarmDateWeekDay = RTC_WEEKDAY_MONDAY;
  Alarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
  Alarm.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY;
  Alarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
  Alarm.AlarmTime.TimeFormat = RTC_HOURFORMAT_24;
  Alarm.AlarmTime.Hours = time.Hours;
  Alarm.AlarmTime.Minutes = time.Minutes;
  Alarm.AlarmTime.Seconds = time.Seconds;
  Alarm.AlarmTime.SubSeconds = time.SubSeconds;

   /* Set the timer in order to generate an alarm after the tmo set by the user */
  FlagRTCAlarmAsExpired = 0;
  if(HAL_RTC_SetAlarm_IT(&RtcHandle, &Alarm, RTC_FORMAT_BIN) != HAL_OK)
  {
    BSP_LCD_DisplayStringAtLine(3,(uint8_t *)"LowPowerHandleAlarm2");
    while(1);
  }
}

/**
  * @brief  RTC Alarm callback
  * @param  hrtc RTC handler
  * @retval None
  */

void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
  /* Clear Wake Up Flag */
  __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
  /* Set the RTC expire flag to 1 */
  FlagRTCAlarmAsExpired = 1;
}

/**
  * @brief  Initialize the demo to handle wake through EXTI
  * @param  Title displayed at the top of the display
  * @param  messActionToDo Message showing the action to do
  * @retval None
  */

static void InitUserInteractionInExtiMode(char * messTitle,
                                   char * messActionToDo)
{

  kWindow_PopupAligned(messTitle, LCD_COLOR_WHITE, LCD_COLOR_BLUE,        \
                       messActionToDo, \
                       LCD_COLOR_GREEN, LCD_COLOR_WHITE );

  DisplayActionExti();



  /* User push-button (EXTI_Line0) will be used to wakeup the system  */

 __HAL_GPIO_EXTI_CLEAR_IT(TAMPER_BUTTON_PIN);
  BSP_PB_Init(BUTTON_TAMPER, BUTTON_MODE_EXTI);

}

/**
  * @brief  Display and apply new clock configuration
  * @param  Title displayed at the top of the display
  * @param  messActionToDo Message showing the action to do
  * @retval None
  */
static void DisplayNewClockConfig(clockConfigTypeEnum clockConfig)
{
  BSP_LCD_SetFont(&Font16);

  /* WARNING : It is not possible to compute the clock values because it takes too much time to be
   *           displayed once the clock is slowed down !!
   */
  switch(clockConfig)
  {
      case CONFIG_CLOCK_HSE_32M :BSP_LCD_DisplayStringAtLine(7, (uint8_t *)"  System Clock : PLL-HSE");
                                 BSP_LCD_DisplayStringAtLine(8, (uint8_t *)"  SYSCLK       : 32 MHz");
                                 BSP_LCD_DisplayStringAtLine(9, (uint8_t *)"  HCLK         : 32 MHz");
                                 break;
      case CONFIG_CLOCK_MSI_64K :BSP_LCD_DisplayStringAtLine(7, (uint8_t *)"  System Clock : MSI");
                                 BSP_LCD_DisplayStringAtLine(8, (uint8_t *)"  SYSCLK       : 64 kHz");
                                 BSP_LCD_DisplayStringAtLine(9, (uint8_t *)"  HCLK         : 32 kHz");
                                 break;
      case CONFIG_CLOCK_HSI_16M :BSP_LCD_DisplayStringAtLine(7, (uint8_t *)"  System Clock : HSI");
                                 BSP_LCD_DisplayStringAtLine(8, (uint8_t *)"  SYSCLK       : 16 MHz");
                                 BSP_LCD_DisplayStringAtLine(9, (uint8_t *)"  HCLK         : 16 MHz");
                                 break;
      case CONFIG_CLOCK_DEFAULT :BSP_LCD_DisplayStringAtLine(7, (uint8_t *)"");
                                 break;
      default                :   break;
  }
   BSP_LCD_SetFont(&Font24);
}

static void ApplyNewClockConfig(clockConfigTypeEnum clockConfig)
{
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  switch(clockConfig)
  {
      case CONFIG_CLOCK_HSE_32M :SystemClock_Config();
                         /*         The system Clock is configured as follow :
                          *            System Clock source            = PLL (HSE)
                          *            SYSCLK(Hz)                     = 32000000
                          *            HCLK(Hz)                       = 32000000
                          */
                                 break;
      case CONFIG_CLOCK_MSI_64K :SystemClock_ConfigMSI();
                         /*            System Clock source            = (MSI)
                          *            SYSCLK(Hz)                     = 64000
                          *            HCLK(Hz)                       = 32000
                          */
                                 break;
      case CONFIG_CLOCK_HSI_16M :SystemClock_ConfigHSI_noPLL_16M();
                         /*            System Clock source            = HSI
                          *            SYSCLK(Hz)                     = 16000000
                          *            HCLK(Hz)                       = 16000000
                          */
                                 break;
      case CONFIG_CLOCK_DEFAULT :
                                 break;
      default                :   break;

  }
}

/**
  * @brief  Initialize the demo to handle wake through RTC
  * @param  messTitle Title displayed at the top of the display
  * @param  messActionToDo Message showing the action to do
  * @retval None
  */

static void InitUserInteractionInRtcMode(char * messTitle,
                                         char * messActionToDo)
{
  kWindow_PopupAligned(messTitle, LCD_COLOR_WHITE, LCD_COLOR_BLUE,\
                "\nSet delay time\n",                          \
                LCD_COLOR_BLUE, LCD_COLOR_WHITE );

  DisplayDelayAction();

  /* Configure RTC */
  RTC_Config();

   /*Disable all used wakeup sources */
  HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN1);

  /*Clear all related wakeup flags */
  /* Clear the Alarm interrupt pending bit */
  __HAL_RTC_ALARM_CLEAR_FLAG(&RtcHandle,RTC_FLAG_ALRAF);
  /* Clear PWR wake up Flag */
  __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);

  /* Set the alarm */
  LowPowerHandleAlarm();

  /* Display the message */
  kWindow_PopupAligned(messTitle, LCD_COLOR_WHITE, LCD_COLOR_BLUE, \
                       messActionToDo,\
                       LCD_COLOR_GREEN, LCD_COLOR_WHITE );
/* Max size                      "------------------" */

    DisplaySimpleAction((uint8_t *)C_STRING_WAIT_RTC);
}

/**
  * @brief  Wait for the user to press the Tamper button
  * @param  None
  * @retval None
  */

static void WaitForTamperButton(void)
{
  FlagItTamperButton = RESET;
  __HAL_GPIO_EXTI_CLEAR_IT(TAMPER_BUTTON_PIN);
  BSP_PB_Init(BUTTON_TAMPER, BUTTON_MODE_EXTI);

  while(FlagItTamperButton == RESET)
  {
  };
  FlagItTamperButton = RESET;
}

/**
  * @brief  Display the IDD power measurement
  * @param  Idd Value in tenth of nano amps
  * @param  IddState State of the Idd measurement
  * @param  MessTitle Title message detected
  * @retval None
  */
static void IddInfoDisplay(uint32_t IddvalueInTenNanoAmps,
                           uint32_t IddState,
                           char * MessTitle)
{
  float TempIddDisplay = 0;
  uint8_t LCDStr[48];

  kWindow_PopupAligned(MessTitle, LCD_COLOR_WHITE, LCD_COLOR_BLUE,        \
                      "",LCD_COLOR_BLUE, LCD_COLOR_WHITE );


  /* Display the measurement */
  if (IddState == IDD_MEASURE_COMPLETED)
  {
    TempIddDisplay = (float) IddvalueInTenNanoAmps * 10;  //Value in nano amps

    if (TempIddDisplay < 1000)
    {
      sprintf((char*)LCDStr, "Idd = %.0f nA", TempIddDisplay);
      BSP_LCD_DisplayStringAtLine(4, (uint8_t*)LCDStr);
    }
    else
    {
      /* Value in uA */
      TempIddDisplay = TempIddDisplay/1000;
      if (TempIddDisplay < 1000)
      {
        sprintf((char*)LCDStr, "Idd = %.3f uA", TempIddDisplay);
        BSP_LCD_DisplayStringAtLine(4, (uint8_t*)LCDStr);
      }
      else
      { /* Value in mA */
        TempIddDisplay = TempIddDisplay/1000;
        sprintf((char*)LCDStr, "Idd = %.3f mA", TempIddDisplay);
        BSP_LCD_DisplayStringAtLine(4, (uint8_t*)LCDStr);
      }
    }
  }
  else
  {
    BSP_LCD_DisplayStringAtLine(3,(uint8_t *)"Idd measurement  ");
    BSP_LCD_DisplayStringAtLine(4,(uint8_t *)"not available.   ");
  }

  /* JP10 Warning if zero is read */
  if (TempIddDisplay == 0)
  {
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_SetFont(&Font12);
    sprintf((char*)LCDStr, "Please check that the jumper JP10 is switched");
    BSP_LCD_DisplayStringAtLine(12, (uint8_t*)LCDStr);
    sprintf((char*)LCDStr, "to IDD.");
    BSP_LCD_DisplayStringAtLine(13, (uint8_t*)LCDStr);
    BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
    BSP_LCD_SetFont(&Font16);
  }
}
/**
  * @brief  This function is used to manage the display
  *         associated to the Idd measurement
  *
  *         IDD ERROR codes:
  *
  *   0x00  No error.
  *   0x01  IRQ pending while Main MCU requests an IDD measurement.
  *   0x02  IDD_CTRL register updated by Main MCU while IDD measurement on going.
  *   0x03  Number of shunt equal to 0 when Main MCU requests an IDD measurement.
  *   0x04  Amplifier gain equal to 0 when Main MCU requests an IDD measurement.
  *   0x05  Shunt_4 equal to 0 when Main MCU requests an IDD measurement.
  *   0x06  Shunt value equal to 0 when Main MCU requests an IDD measurement.
  *   0x0F  Difference between IDD_on_shunt[N] and IDD_on_shunt[N+1] > 50%. => IDD not stable.
  *   0x10  ADC saturation (ADC_val=0xFFF).
  *   0x11  IDD calculation variable overflow.
  *   0x12  VDD_MCU undershoot detected during measurement procedure.
  *   0xF0  Unreachable state during IDD measurement process.
  *
  * @param  None
  * @retval None
*/

static void ManageIddMeasurementMenu(char * messTitle,uint32_t flagModeIt)
{
  uint32_t IddReadValue = 0;
  uint32_t JoyState = JOY_NONE_PIN;
  uint32_t IddState = IDD_STATE_UNCHANGED;

 /* Check the status */
  if (flagModeIt == 1)
  {
    DecodeMFXInterrupt(&JoyState,&IddState);
  }
  else
  {
    IddState = IDD_MEASURE_COMPLETED;
  }

  /* Get the values from MFX regarding the IDD measurement */
  BSP_IDD_GetValue(&IddReadValue);

  /* Display the results */
  IddInfoDisplay(IddReadValue,IddState,messTitle);

  /* Display message to exit.. */
  DisplaySimpleAction((uint8_t *)C_STRING_PRESS_TAMPER);

  /* wait for button to exit  */
  WaitForTamperButton();
}

/**
  * @brief  This function is used to start the Idd measurement
  *         and it initialize the IID_BSP
  * @param  None
  * @retval None
  */
static void StartIddMeasurement()
{
  BSP_IDD_Init();
  BSP_IDD_ClearIT();
  BSP_IDD_ErrorClearIT();
  BSP_IDD_DisableIT();
  BSP_IDD_ErrorDisableIT();
  BSP_IDD_StartMeasure();
}

/**
  * @brief  This function save the Gpio configuration
  * @param  None
  * @retval None
  */
static void CopyGpioConfiguration(uint32_t  index, GPIO_TypeDef * gpio)
{
    GpioConfiguration[index].MODER = gpio-> MODER;
    GpioConfiguration[index].OTYPER = gpio-> OTYPER;
    GpioConfiguration[index].OSPEEDR = gpio-> OSPEEDR;
    GpioConfiguration[index].PUPDR = gpio-> PUPDR;
    GpioConfiguration[index].IDR = gpio-> IDR;
    GpioConfiguration[index].ODR = gpio-> ODR;
    GpioConfiguration[index].BSRR = gpio-> BSRR;
    GpioConfiguration[index].LCKR = gpio-> LCKR;
    GpioConfiguration[index].AFR[0] = gpio-> AFR[0];
    GpioConfiguration[index].AFR[1] = gpio-> AFR[1];
    GpioConfiguration[index].BRR = gpio-> BRR;
}

/**
  * @brief  This function restore back the Gpio configuration
  * @param  None
  * @retval None
  */
static void RestoreGpioConfiguration(uint32_t  index, GPIO_TypeDef * gpio)
{
    gpio-> MODER = GpioConfiguration[index].MODER ;
    gpio-> OTYPER = GpioConfiguration[index].OTYPER ;
    gpio-> OSPEEDR = GpioConfiguration[index].OSPEEDR ;
    gpio-> PUPDR = GpioConfiguration[index].PUPDR ;
    gpio-> IDR = GpioConfiguration[index].IDR ;
    gpio-> ODR = GpioConfiguration[index].ODR ;
    gpio-> BSRR = GpioConfiguration[index].BSRR ;
    gpio-> LCKR = GpioConfiguration[index].LCKR;
    gpio-> AFR[0] = GpioConfiguration[index].AFR[0] ;
    gpio-> AFR[1] = GpioConfiguration[index].AFR[1] ;
    gpio-> BRR = GpioConfiguration[index].BRR ;
}

/**
  * @brief  This function copy back the Gpio configuration
  * @param  None
  * @retval None
  */
static void SaveGpioContextAndDisable(uint32_t mode)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable GPIOs clock */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();

  /* Store the GPIOs..*/
  CopyGpioConfiguration(0,GPIOA);
  CopyGpioConfiguration(1,GPIOB);
  CopyGpioConfiguration(2,GPIOC);
  CopyGpioConfiguration(3,GPIOD);
  CopyGpioConfiguration(4,GPIOE);
  CopyGpioConfiguration(5,GPIOH);

  /* Configure all GPIO port pins in Analog Input mode (floating input trigger OFF) */
  GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStructure.Pull = GPIO_NOPULL;

 /* GPIO_A */
  GPIO_InitStructure.Pin = GPIOA_PIN_AVAILABLE;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
  /* GPIO_B */
  /* Warning PIO_PB6 and GPIO_PB7 are used by MFX */
  GPIO_InitStructure.Pin = GPIOB_PIN_AVAILABLE;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
  /* GPIO_C */
  /* Warning PC_13 is used for the Tamper button */
  if (mode == C_MODE_EXTI)
  {
    /* It is mandatory to keep C active ... */
    GPIO_InitStructure.Pin = GPIOC_PIN_AVAILABLE & ~GPIO_PIN_13;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
  }
  else
  {
    GPIO_InitStructure.Pin = GPIOC_PIN_AVAILABLE;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
  }
  /* GPIO_D */
  GPIO_InitStructure.Pin = GPIOD_PIN_AVAILABLE;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);

   /* GPIO_E */
  GPIO_InitStructure.Pin = GPIOE_PIN_AVAILABLE;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStructure);

  /* GPIO_H */
  GPIO_InitStructure.Pin = GPIOH_PIN_AVAILABLE;
  HAL_GPIO_Init(GPIOH, &GPIO_InitStructure);

  /* Disable GPIOs clock */
  __HAL_RCC_GPIOA_CLK_DISABLE();
  __HAL_RCC_GPIOB_CLK_DISABLE();
  if (mode != C_MODE_EXTI)
      __HAL_RCC_GPIOC_CLK_DISABLE();
  __HAL_RCC_GPIOD_CLK_DISABLE();
  __HAL_RCC_GPIOE_CLK_DISABLE();
  __HAL_RCC_GPIOH_CLK_DISABLE();

}

/**
* @brief Restore Demonstration context (GPIOs Configurations, peripherals,...).
* @param None
* @retval None
*/
static void RestoreGpioContext(void)
{
    /* Enable GPIOs clock */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();

  /* Store the GPIOs..*/
  RestoreGpioConfiguration(0,GPIOA);
  RestoreGpioConfiguration(1,GPIOB);
  RestoreGpioConfiguration(2,GPIOC);
  RestoreGpioConfiguration(3,GPIOD);
  RestoreGpioConfiguration(4,GPIOE);
  RestoreGpioConfiguration(5,GPIOH);
}

/**
  * @brief  This function is used to manage Stop mode
  * @param  None
  * @retval None
  */
void ManageStopMode(uint32_t mode)
 {

  /* Start the measurement */
  StartIddMeasurement();

  /* GPIO config */
  SaveGpioContextAndDisable(C_MODE_FULL_DISABLE);
  if (mode == C_MODE_EXTI)
    BSP_PB_Init(BUTTON_TAMPER,BUTTON_MODE_EXTI);

  /* Enable Power Control clock and clear wakeup flag*/
  __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);

  /* Enter Stop Mode                                             */
  /* The UltraLow power and fast wake up should be put there !!! */
  HAL_PWREx_EnableUltraLowPower();
  HAL_PWREx_EnableFastWakeUp();
  
  HAL_SuspendTick();
    
  HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);

  HAL_ResumeTick();
   
  /* GPIO config */
  RestoreGpioContext();

  ApplyNewClockConfig(CONFIG_CLOCK_HSE_32M);

  if (mode == C_MODE_RTC)
      DesactivateAlarm();

    /* Measure the power */
  ManageIddMeasurementMenu(IDD_MEASURE_TITLE_STOP,0);
 }

/**
  * @brief  This function is used to manage Standby mode
  * @param  None
  * @retval None
  */
static void ManageStandbyMode(uint32_t mode)
 {
  /* Start the measurement */
  StartIddMeasurement();

  /* Clear all related wakeup flags*/
  __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);

  /* Clear GPIO ! */
  SaveGpioContextAndDisable(C_MODE_FULL_DISABLE);

  /* Enable WakeUp Pin PWR_WAKEUP_PIN2 connected to PC.13 */
  if (mode == C_MODE_EXTI)
      HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN2);

  /* Enter the Standby mode */
  HAL_PWREx_EnableUltraLowPower();
  HAL_PWREx_EnableFastWakeUp();

  HAL_SuspendTick();

  HAL_PWR_EnterSTANDBYMode();

  /* This code will never be reached! */
  HAL_ResumeTick();
  while (1)
  {
  }
 }


/**
  * @brief  This function is used to manage LowPowerSleep
  *         used by the application
  * @param  None
  * @retval None
  */
static void ManageSleepLowPowerMode(uint32_t mode,clockConfigTypeEnum clockConfig)
 {

 /* Disable Prefetch Buffer */
  __HAL_FLASH_PREFETCH_BUFFER_DISABLE();

  /* Enable Ultra low power mode */
   HAL_PWREx_EnableUltraLowPower();

  /* Enable the fast wake up from Ultra low power mode */
   HAL_PWREx_EnableFastWakeUp();

  /* Enable the power down mode during Sleep mode */
  __HAL_FLASH_SLEEP_POWERDOWN_ENABLE();

  /* Start the measurement */
  StartIddMeasurement();

  /* GPIO config */
  SaveGpioContextAndDisable(C_MODE_FULL_DISABLE);

  if (mode == C_MODE_EXTI)
    BSP_PB_Init(BUTTON_TAMPER,BUTTON_MODE_EXTI);

  ApplyNewClockConfig(clockConfig);

  /* Suspend Tick increment to prevent wakeup by Systick interrupt.         */
  /* Otherwise the Systick interrupt will wake up the device within 1ms     */
  /* (HAL time base).                                                       */
  HAL_SuspendTick();

  /* Enter Sleep Mode , wake up is done once Wkup/Tamper push-button is pressed */
  HAL_PWR_EnterSLEEPMode(PWR_LOWPOWERREGULATOR_ON, PWR_SLEEPENTRY_WFI);

  /* Resume Tick interrupt if disabled prior to sleep mode entry */
  HAL_ResumeTick();

  /* GPIO config */
  RestoreGpioContext();

  ApplyNewClockConfig(CONFIG_CLOCK_HSE_32M);

  /* Force the disabling of the MSI */
  SystemClock_DisableMSI();

  if (mode == C_MODE_RTC)
      DesactivateAlarm();

  /* Measure the power */
  ManageIddMeasurementMenu(IDD_MEASURE_TITLE_LP_SLEEP,0);
 }

/**
  * @brief  This function is used to manage Sleep mode
  *         used by the application
  * @param  None
  * @retval None
  */
static void ManageSleepMode(uint32_t mode)
 {

  /* Enable the power down mode during Sleep mode */
  __HAL_FLASH_SLEEP_POWERDOWN_ENABLE();

  __HAL_RCC_PWR_CLK_ENABLE();

  /* Start the measurement */
  StartIddMeasurement();

  /* GPIO config */
  SaveGpioContextAndDisable(C_MODE_FULL_DISABLE);
  if (mode == C_MODE_EXTI)
    BSP_PB_Init(BUTTON_TAMPER,BUTTON_MODE_EXTI);

  DisplayNewClockConfig(CONFIG_CLOCK_HSI_16M);
  ApplyNewClockConfig(CONFIG_CLOCK_HSI_16M);

  /* Suspend Tick increment to prevent wakeup by Systick interrupt.         */
  /* Otherwise the Systick interrupt will wake up the device within 1ms     */
  /* (HAL time base).                                                       */
  HAL_SuspendTick();

  /* Enter Sleep Mode , wake up is done once Wkup/Tamper push-button is pressed */
    /* Enable Ultra low power mode */
  HAL_PWREx_EnableUltraLowPower();
  HAL_PWREx_EnableFastWakeUp();
  HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);

  /* Resume Tick interrupt if disabled prior to sleep mode entry */
  HAL_ResumeTick();

  /* GPIO config */
  RestoreGpioContext();

  ApplyNewClockConfig(CONFIG_CLOCK_HSE_32M);

  if (mode == C_MODE_RTC)
      DesactivateAlarm();

  /* Measure the power */
  ManageIddMeasurementMenu(IDD_MEASURE_TITLE_SLEEP,0);
 }

/**
  * @brief  This function is used to manage LowPowerRun
  *         used by the application
  * @param  None
  * @retval None
  */
static void ManageRunMode(uint32_t mode,clockConfigTypeEnum clockConfig)
 {

 /* Disable Prefetch Buffer */
  __HAL_FLASH_PREFETCH_BUFFER_DISABLE();

  /* Enable Ultra low power mode */
   HAL_PWREx_EnableUltraLowPower();

  /* Enable the fast wake up from Ultra low power mode */
   HAL_PWREx_EnableFastWakeUp();

  /* Start the measurement */
  StartIddMeasurement();

  /* GPIO config */
  SaveGpioContextAndDisable(C_MODE_FULL_DISABLE);
  if (mode == C_MODE_EXTI)
    BSP_PB_Init(BUTTON_TAMPER,BUTTON_MODE_EXTI);

  ApplyNewClockConfig(clockConfig);

  /* Enter LP RUN mode */
 /*********************/
 HAL_PWREx_EnableLowPowerRunMode();

 /* Wait until the system enters LP RUN and the Regulator is in LP mode */
 while(__HAL_PWR_GET_FLAG(PWR_FLAG_REGLP) == RESET)
 {
 }

 if (mode == C_MODE_EXTI)
 {
     /* Wait Until Wkup/Tamper push-button pressed */
     while(BSP_PB_GetState(BUTTON_TAMPER) != GPIO_PIN_SET)
     {
     }
 }

 if (mode == C_MODE_RTC)
 {
     while(FlagRTCAlarmAsExpired == 0);
 }

  /* Exit LP RUN mode */
  /********************/
  HAL_PWREx_DisableLowPowerRunMode();

  /* Wait until the system exits LP RUN and the Regulator is in main mode */
  while(__HAL_PWR_GET_FLAG(PWR_FLAG_REGLP) != RESET)
  {
  }

  /* GPIO config */
  RestoreGpioContext();

  /* Apply standard configuration */
  ApplyNewClockConfig(CONFIG_CLOCK_HSE_32M);

  /* Disable MSI clock */
  SystemClock_DisableMSI();

  if (mode == C_MODE_RTC)
      DesactivateAlarm();

  /* Measure the power */
  ManageIddMeasurementMenu(IDD_MEASURE_TITLE_LP_RUN,0);

 }

/**
  * @brief  This function is used by the main.c
  *         to display standby idd measurement
  * @param  None
  * @retval None
  */
void DisplayStandbyIddMeasurement(void)
{
  ManageIddMeasurementMenu(IDD_MEASURE_TITLE_STANDBY,0);
}

/**
  * @brief  This function is used by the main.c
  *         to display standby idd measurement
  * @param  None
  * @retval None
  */

static void DisplayActionExti(void)
{

     BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
     BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
     BSP_LCD_FillRect(0, BSP_LCD_GetYSize()-55, BSP_LCD_GetXSize(), 55);
     BSP_LCD_SetFont(&Font16);
     BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
     BSP_LCD_DisplayStringAtLine(12,(uint8_t *)" Please wait 4 seconds and");
     BSP_LCD_DisplayStringAtLine(13,(uint8_t *)" press Tamper to Exit");
     BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
     BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
     BSP_LCD_SetFont(&Font24);
}

/**
  * @brief  Display specific function
  * @param  message to display
  * @retval None
  */
static void DisplaySimpleAction(uint8_t * message)
{

     BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
     BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
     BSP_LCD_FillRect(0, BSP_LCD_GetYSize()-45, BSP_LCD_GetXSize(), 45);
     BSP_LCD_SetFont(&Font16);
     BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
     BSP_LCD_DisplayStringAtLine(13,message);
     BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
     BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
     BSP_LCD_SetFont(&Font24);
}
/**
  * @brief  Display specific action
  * @param  message to display
  * @retval None
  */
static void DisplayDelayAction(void)
{
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_SetFont(&Font16);
  BSP_LCD_FillRect(0, BSP_LCD_GetYSize()-90, BSP_LCD_GetXSize(), 90);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_DisplayStringAtLine(10, (uint8_t *)"-Use JOYSTICK up/down/left/");
  BSP_LCD_DisplayStringAtLine(11, (uint8_t *)" right to set delay");
  BSP_LCD_DisplayStringAtLine(13, (uint8_t *)"-Use JOYSTICK select to start");
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

