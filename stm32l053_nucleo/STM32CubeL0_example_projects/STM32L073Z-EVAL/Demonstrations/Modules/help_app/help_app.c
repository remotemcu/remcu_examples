
/**
******************************************************************************
* @file    help_app.c
* @author  MCD Application Team
* @brief   Help information.
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
#define _HELP_APP_C

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

/** @defgroup HELP_MODULE
* @brief low power routines 
* @{
*/



/* Private function prototypes -----------------------------------------------*/
KMODULE_RETURN _HelpDemoExec(void);


static void GenericHelpApp(void);
static void SystemHelpApp(void);
static void BoardHelpApp(void);
static void LpUartHelpApp(void);
static void LowPowerHelpApp(void);
static void ThermometerHelpApp(void);
static void LcSensorHelpApp(void);
static void PressureHelpApp(void);

static void displayTitleAndMsg(char *title, char *msg);
static void WaitJoyDown(void);

/* Defines used for the menu. */

/* Max size                             "------------------" */
#define STRING_EMPTY                    "                  "
/* Main menu */
#define TITLE_RETURN                    "RETURN"
#define TITLE_GENERIC_HELP              "GENERIC HELP"
#define TITLE_SYSTEM_HELP               "SYSTEM CONFIG"
#define TITLE_BOARD_HELP                "BOARD SETUP"
#define TITLE_LPUART_HELP               "LPUART README"
#define TITLE_LOW_POWER_HELP            "LOW POWER README"
#define TITLE_THERMOMETER_HELP          "THERMOMETER README"
#define TITLE_LC_SENSOR_HELP            "LC SENSOR README"
#define TITLE_PRESSURE_HELP             "PRESSURE README"


#define LINE_MAX 40

#define MSG_BOARD_HELP    \
"\n\n\
For the position of the \n\
jumpers on the Board, refer\n\
yourself to the readme.txt \n\n"


#define MSG_LPUART_HELP    \
"\n\
This application demonstrates\n\
the wake-up of the platform \n\
from a terminal connected to \n\
CN7 (RS232) in different \n\
modes. \n\
After wake-up, the current\n\
consumption measured by a\n\
companion chip (mfx) during\n\
Stop mode is displayed.\n"

#define MSG_LOW_POWER_HELP  \
"\n\
This application allows to \n\
enter in low power modes :\n\
RUN, LPRUN, SLEEP, LPSLEEP,\n\
STOP, STANDBY.\n\
The wakeup can be order by an \n\
EXTI or an alarm. \n\
After wake-up, the current \n\
consumption measured by a \n\
companion chip (mfx) is \n\
displayed.\n"

#define MSG_THERMOMETER_HELP \
"\
This application displays the \n\
temperature (sensor Stlm75) \n\
in radiator thermostat way.\n\
\n\
Thanks to SMBUS, a management\n\
of temperature setpoints\n\
tracking is done with  \n\
interruption events which \n\
display messages and Min/Max\n\
temperature values.\n"

#define MSG_LC_SENSOR_HELP \
"This application shows how \n\
is managed the LC Sensor.\n\n\
Mode STANDARD displays LC \n\
sensor sensitivity to metal \n\
graphically on LCD. Mode LOW \n\
POWER minimizes the power \n\
consumption that can be \n\
measured on JP10 jumper. \n\
Metal strip must be placed \n\
over the LC sensor. \n"

#define MSG_PRESSURE_HELP \
"The current air pressure is\n\
measured from the sensor U1.\n\n\
It demonstrates the\n\
capability of ADC 16 bits\n\
oversampling mode from the\n\
ADC 12 bits.\n"

#define DEMO_INFO1   "MCD Application Team"
#define DEMO_INFO3   "STMicroelectronics"

/* System help */
#define C_MFX_VERSION                             "MFX version"
#define C_HAL_VERSION                             "HAL version"
#define C_CLK_SRC                                 "Clock   "
#define C_SYS_CLOCK                               "SysClock"
#define C_SYS_HCLOCK                              "HClock  "
#define C_SYS_PCLOCK1                             "PClock1 "
#define C_SYS_PCLOCK2                             "PClock2 "

/* Board help */
#define C_BOARD_CONFIG1                           "- For Power "
#define C_BOARD_CONFIG2                           "  measurement"
#define C_BOARD_CONFIG3                           "  JP10 TO BE COMPLETED"

/* Private Variable ----------------------------------------------------------*/

/* Help menu */
const tMenuItem HelpMenuItem[] =
{
  {TITLE_GENERIC_HELP, 14,30,SEL_EXEC, MODULE_NONE,GenericHelpApp,NULL, NULL, NULL },
  {TITLE_SYSTEM_HELP,14,30,SEL_EXEC, MODULE_NONE,SystemHelpApp,NULL, NULL, NULL },
  {TITLE_BOARD_HELP,14,30,SEL_EXEC, MODULE_NONE,BoardHelpApp,NULL, NULL, NULL },
  {TITLE_LPUART_HELP,14,30,SEL_EXEC, MODULE_NONE,LpUartHelpApp,NULL, NULL, NULL },
  {TITLE_LOW_POWER_HELP,14,30,SEL_EXEC, MODULE_NONE,LowPowerHelpApp,NULL, NULL, NULL },
  {TITLE_THERMOMETER_HELP,14,30,SEL_EXEC, MODULE_NONE,ThermometerHelpApp,NULL, NULL, NULL },
  {TITLE_LC_SENSOR_HELP,14,30,SEL_EXEC, MODULE_NONE,LcSensorHelpApp,NULL, NULL, NULL },
  {TITLE_PRESSURE_HELP,14,30,SEL_EXEC, MODULE_NONE,PressureHelpApp,NULL, NULL, NULL },
  {TITLE_RETURN, 0, 0,SEL_EXIT, MODULE_NONE, NULL, NULL, NULL, NULL }
};

const tMenu HelpMenu = {
  " HELP ", HelpMenuItem, countof(HelpMenuItem), TYPE_TEXT, 1, 1};

/* used to exit application */
static __IO uint8_t user_action=0;

/* Private typedef -----------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
const K_ModuleItem_Typedef HelpModuleItem =
{
  MODULE_SYSTEM_INFO,
  NULL,
  _HelpDemoExec,
  NULL,
  NULL
};

/**
* @brief  Run the Lowpower application 
* @param  None.
* @note   run and display information about the lowpower feature.  
* @retval None.
*/
KMODULE_RETURN _HelpDemoExec(void)
{
  /* Prepare Execute the main MMI of Help application */
  kMenu_Execute(HelpMenu);
  return KMODULE_OK;
}

/**
* @brief  Generic informations for this demonstration.
* @param  None.
* @retval None.
*/
static void GenericHelpApp(void)
{
  uint8_t LCDStr[30];
  
  displayTitleAndMsg(TITLE_GENERIC_HELP , "");

  BSP_LCD_SetFont(&Font16);
  
  sprintf((char*)LCDStr, "%s", EVAL_BOARD);
  BSP_LCD_DisplayStringAt(3,60, (uint8_t*)LCDStr,CENTER_MODE);
  
  sprintf((char*)LCDStr, "%s", "Demonstration");
  BSP_LCD_DisplayStringAt(3,80, (uint8_t*)LCDStr,CENTER_MODE);
  
  sprintf((char*)LCDStr, "%s",DEMO_VERSION);
  BSP_LCD_DisplayStringAt(3,100, (uint8_t*)LCDStr,CENTER_MODE);
  
  sprintf((char*)LCDStr, "%s",DEMO_DATE);
  BSP_LCD_DisplayStringAt(3,120, (uint8_t*)LCDStr,CENTER_MODE);
  
  sprintf((char*)LCDStr, "%s",DEMO_INFO1);
  BSP_LCD_DisplayStringAt(3,140, (uint8_t*)LCDStr,CENTER_MODE);
  
  sprintf((char*)LCDStr, "%s", DEMO_INFO2);
  BSP_LCD_DisplayStringAt(3,160, (uint8_t*)LCDStr,CENTER_MODE);
  
  sprintf((char*)LCDStr, "%s", DEMO_INFO3);
  BSP_LCD_DisplayStringAt(3,180, (uint8_t*)LCDStr,CENTER_MODE);
  
  BSP_LCD_SetFont(&Font24);
  
  /* Wait user action */
  WaitJoyDown();  
}

/**
* @brief  Help regarding the system configuration. It gives informations
*         regarding the FMX version, the HAL version, the System clock
*         configuration and some other informations.
* @param  None.
* @retval None.
*/
static void SystemHelpApp(void)
{
  RCC_ClkInitTypeDef clkdef;
  RCC_OscInitTypeDef  rcc_oscInitStruct;
  uint32_t latency;
  uint8_t LCDStr[22];
  uint16_t version;
  
  displayTitleAndMsg(TITLE_SYSTEM_HELP , "");
  
  BSP_LCD_SetFont(&Font16);

  BSP_LCD_DisplayStringAtLine(2,(uint8_t *)"-----------Version-------------");
  /* Get the MFX version */
  /***********************/
  version = mfxstm32l152_ReadFwVersion(IDD_I2C_ADDRESS);
  sprintf((char*)LCDStr, "%s = %x", C_MFX_VERSION,version);
  BSP_LCD_DisplayStringAtLine(3, (uint8_t*)LCDStr);
  
  /* Get the HAL version */
  /***********************/
  sprintf((char*)LCDStr, "%s = %x",C_HAL_VERSION,(int)HAL_GetHalVersion());
  BSP_LCD_DisplayStringAtLine(4, (uint8_t*)LCDStr);
  
  BSP_LCD_DisplayStringAtLine(6,(uint8_t *)"--------Clock setup------------");
  /* System System clock  */
  /************************/
  HAL_RCC_GetClockConfig(&clkdef, &latency);
  
  switch(clkdef.SYSCLKSource)
  {
  case RCC_SYSCLKSOURCE_MSI :    sprintf((char*)LCDStr, "%s = %s",C_CLK_SRC,"MSI");
  break;
  case RCC_SYSCLKSOURCE_HSI :    sprintf((char*)LCDStr, "%s = %s",C_CLK_SRC,"HSI");
  break;
  case RCC_SYSCLKSOURCE_HSE :    sprintf((char*)LCDStr, "%s = %s",C_CLK_SRC,"HSE");
  break;
  case RCC_SYSCLKSOURCE_PLLCLK :
    HAL_RCC_GetOscConfig(&rcc_oscInitStruct);
    switch(rcc_oscInitStruct.PLL.PLLSource)
    {
    case RCC_PLLSOURCE_HSI  :sprintf((char*)LCDStr,"%s = %s",C_CLK_SRC,"PLL-HSI");
    break;
    case RCC_PLLSOURCE_HSE  : sprintf((char*)LCDStr,"%s = %s",C_CLK_SRC,"PLL-HSE");
    break;
    default                 : sprintf((char*)LCDStr, "%s = %s",C_CLK_SRC,"?");
    break;
    }
    break;
  default :                      break;
  
  }
  BSP_LCD_DisplayStringAtLine(7, (uint8_t*)LCDStr);
  
  /* Frequency           */
  /************************/
  
  sprintf((char*)LCDStr, "%s = %d MHz",C_SYS_CLOCK, (int)(HAL_RCC_GetSysClockFreq()/1000000));
  BSP_LCD_DisplayStringAtLine(8, (uint8_t*)LCDStr);
  
  sprintf((char*)LCDStr, "%s = %d MHz",C_SYS_HCLOCK, (int)(HAL_RCC_GetHCLKFreq()/1000000));
  BSP_LCD_DisplayStringAtLine(9, (uint8_t*)LCDStr);
  
  sprintf((char*)LCDStr, "%s = %d MHz",C_SYS_PCLOCK1, (int)(HAL_RCC_GetPCLK1Freq()/1000000));
  BSP_LCD_DisplayStringAtLine(10, (uint8_t*)LCDStr);
  
  sprintf((char*)LCDStr, "%s = %d MHz",C_SYS_PCLOCK2, (int)(HAL_RCC_GetPCLK2Freq()/1000000));
  BSP_LCD_DisplayStringAtLine(11, (uint8_t*)LCDStr);
  
  /* Wait user action */
  WaitJoyDown();
}

/**
* @brief  It gives informations about the Board setup needed when running the application.
* @param  None.
* @retval None.
*/
static void BoardHelpApp(void)
{

  displayTitleAndMsg(TITLE_BOARD_HELP , MSG_BOARD_HELP);
  WaitJoyDown();
}

/**
* @brief  It gives informations about the LPuart application launched by this demo.
* @param  None.
* @retval None.
*/
static void LpUartHelpApp(void)
{
  displayTitleAndMsg(TITLE_LPUART_HELP , MSG_LPUART_HELP);
  WaitJoyDown();
}

/**
* @brief  It gives informations about the Low power application launched by
*         this demo.
* @param  None.
* @retval None.
*/
static void LowPowerHelpApp(void)
{
  displayTitleAndMsg(TITLE_LOW_POWER_HELP , MSG_LOW_POWER_HELP);
  WaitJoyDown();
  
}

/**
* @brief  It gives informations about the Thermometer application
*         launched by this demo.
* @param  None.
* @retval None.
*/
static void ThermometerHelpApp(void)
{
  displayTitleAndMsg(TITLE_THERMOMETER_HELP , MSG_THERMOMETER_HELP);
  WaitJoyDown();

}

/**
* @brief  It gives informations about the Lc sensor application launched by this demo.
* @param  None.
* @retval None.
*/
static void LcSensorHelpApp(void)
{
  displayTitleAndMsg(TITLE_LC_SENSOR_HELP , MSG_LC_SENSOR_HELP);
  WaitJoyDown();
}

/**
* @brief  It gives informations about the Pressure application launched by this demo.
* @param  None.
* @retval None.
*/
static void PressureHelpApp(void)
{
  displayTitleAndMsg(TITLE_PRESSURE_HELP , MSG_PRESSURE_HELP);
  WaitJoyDown();
}

/**
* @brief  Wait user action.
* @retval None
*/
static void WaitJoyDown(void)
{
  JOYState_TypeDef joyState = JOY_NONE;

  /* Wait down to exit */
  while(joyState != JOY_DOWN)
  {
    joyState = BSP_JOY_GetState();
  }
  while(BSP_JOY_GetState() != JOY_NONE);
}
/**
* @brief  Handle a generic window display.
* @param  title to display a message, \n is used for multiple line.
* @param  msg to display a message, \n is used for multiple line.
* @retval None
*/
static void displayTitleAndMsg(char *title, char *msg)
{
  uint8_t substring[LINE_MAX];
  uint8_t lineindex;
  uint16_t index,subindex;
  lineindex = subindex = index = 0;
  
  /* Title */
  kWindow_PopupCentered(title, LCD_COLOR_WHITE, LCD_COLOR_BLUE, "", LCD_COLOR_BLUE, LCD_COLOR_WHITE );
  
  /* Msg */
  BSP_LCD_SetFont(&Font16);
  do
  {
    substring[subindex]=msg[index];
    if((msg[index] == '\n') || (msg[subindex] == '\0') || (subindex == LINE_MAX))
    {
      substring[subindex] = '\0';
      BSP_LCD_DisplayStringAt(0, (2+lineindex) * Font16.Height, substring, LEFT_MODE);
      lineindex++;
      subindex = 0;
    }
    else
    {
      subindex++;
    }
    
    if(msg[index] != '\0')
    {
      index++;
    }
  }
  while(msg[index] != '\0');
  
  /* Footer part*/
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, BSP_LCD_GetYSize()-30, BSP_LCD_GetXSize(), 30);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()-Font24.Height , (uint8_t*)"Press down to exit", CENTER_MODE);
  
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
}


/**
* @}
*/

/**
* @}
*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

