/**
  ******************************************************************************
  * @file    main_app.c
  * @author  MCD Application Team   
  * @brief   System information functions
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
#include "k_config.h"
#include "k_module.h"
#include "k_menu.h"
#include "k_storage.h"

/* Private function prototypes -----------------------------------------------*/
KMODULE_RETURN AppMainExec(void);
KMODULE_RETURN AppMainExecCheckRessource(void);


/* Defines  ----------------------------------------------------------*/
#define C_PATH_LOGO_UART            "STFILES/Uart.bmp"
#define C_PATH_LOGO_POWER           "STFILES/Power.bmp"
#define C_PATH_LOGO_THERMOMETER     "STFILES/Temp.bmp"
#define C_PATH_LOGO_LC_SENSOR       "STFILES/LCsensor.bmp"
#define C_PATH_LOGO_PRESSURE        "STFILES/Pressure.bmp"
#define C_PATH_LOGO_HELP            "STFILES/Help.bmp"

/* Private Variable ----------------------------------------------------------*/
__IO uint8_t exit_func;


const tMenuItem MainMenuItems[] =
{
    {"LPUART Wakeup"        ,32,60,  SEL_MODULE,MODULE_LPUART_WAKEUP, NULL, NULL, NULL,C_PATH_LOGO_UART },
    {"Low Power"            ,128,60, SEL_MODULE,MODULE_LOW_POWER, NULL, NULL, NULL,C_PATH_LOGO_POWER },
    {"Thermometer"          ,224,60, SEL_MODULE,MODULE_TSENSOR, NULL, NULL, NULL,C_PATH_LOGO_THERMOMETER },
    {"LC sensor metering"   ,32,144, SEL_MODULE,MODULE_LC_SENSOR_METERING, NULL, NULL, NULL,C_PATH_LOGO_LC_SENSOR },
    {"Pressure"             ,128,144,SEL_MODULE,MODULE_PRESSURE_MEASUREMENT, NULL, NULL, NULL,C_PATH_LOGO_PRESSURE },
    {"Help"                ,224,144,SEL_MODULE,MODULE_SYSTEM_INFO, NULL, NULL, NULL,C_PATH_LOGO_HELP}
};

const tMenu MainMenu = {
  "Main menu", MainMenuItems, countof(MainMenuItems), TYPE_ICON, 2, 3
  };


    

/* Private typedef -----------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
const K_ModuleItem_Typedef ModuleAppMain =
{
  MODULE_MAIN_APP,
  NULL,
  AppMainExec,
  NULL,
  AppMainExecCheckRessource
};  

/**
  * @brief  Run the main application 
  * @param  None.
  * @note   run and display main menu.  
  * @retval None.
  */
KMODULE_RETURN AppMainExec(void)
{
  kMenu_Execute(MainMenu);
  return KMODULE_OK;
}

/**
  * @brief  check the main application ressources 
  * @param  None.
  * @note   None.  
  * @retval None.
  */
KMODULE_RETURN AppMainExecCheckRessource(void)
{
 uint8_t index; 
 
  /* check icon menu */
  for(index = 0; index <  countof(MainMenuItems); index++)
  {
    if(kStorage_FileExist((uint8_t *)MainMenuItems[index].pIconPath) != KSTORAGE_NOERROR)
    {
      return KMODULE_ERROR_ICON;
    }
  }
  return KMODULE_OK;
}

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

