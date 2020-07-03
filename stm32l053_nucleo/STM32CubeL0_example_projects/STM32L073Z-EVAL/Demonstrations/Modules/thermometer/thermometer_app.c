/**
  ******************************************************************************
  * @file    thermometer_app.c
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

#define __THERMOMETER_APP_C

/* Set by default EXTI mode for Joystick control */
#define MODE_IT

/* Includes ------------------------------------------------------------------*/
#include "string.h"
#include "stdio.h"
#include "k_config.h"
#include "k_module.h"
#include "k_menu.h"
#include "k_storage.h"
#include "stm32l0xx_tsensor.h"



/** @addtogroup THERMOMETER_MODULE
  * @{
  */

/** @defgroup SYSTEM_INFO
  * @brief system info routines 
  * @{
  */

/* Private typedef ----------------------------------------------------------*/    
/* Private constants ----------------------------------------------------------*/
#define TSENSOR_ADDR            0x92    /* STLM75 Address   */  
#define TEMPERATURE_LOW         29      /* default 29°C             */
#define TEMPERATURE_HIGH        30      /* default 30°C             */

/* Private function prototypes -----------------------------------------------*/
KMODULE_RETURN _ThermometerDemoExec(void);
KMODULE_RETURN _ThermometerConfig(void);
KMODULE_RETURN _ThermometerUnConfig(void);

void ThermometerMenuUserAction(uint8_t sel);
void ThermometerUserHeader(void);

void ThermometerDemo_SMB(void);
static void TSENSOR_Display_Temperature(uint16_t Temperature,float *tempcelsiusdisplayround);
static void Error_Handler(void); 

/* Private Variable ----------------------------------------------------------*/

/* SmBus mode menu */
const tMenuItem SmBusModeMenuItems[] =
{
    {"Press down to exit",14, 30, TYPE_EXEC, MODULE_NONE, ThermometerDemo_SMB, ThermometerMenuUserAction, NULL, NULL },
};

const tMenu SmBusModeMenu = {
  "Thermostat mode", SmBusModeMenuItems, countof(SmBusModeMenuItems), TYPE_EXEC, 1, 1
};

/* Main menu */
const tMenuItem ThermometerMenuMenuItems[] =
{ 
    {"STLM75 with SMBUS",       14, 30,SEL_SUBMENU, MODULE_NONE, NULL, NULL, (const tMenu*)&SmBusModeMenu, NULL },   
    {"RETURN",            0,  0,SEL_EXIT, MODULE_NONE, NULL, NULL, NULL, NULL }
};

const tMenu ThermometerMenu = {
  "Temperature sensor", ThermometerMenuMenuItems, countof(ThermometerMenuMenuItems), TYPE_TEXT, 1, 1 };


/* used to exit application */

volatile static uint8_t user_stop = 0;


TSENSOR_SMB_InitTypeDef TSENSOR_InitStructure;
volatile uint8_t  ThermometerAlertOccus = 0;

  
/* Private typedef -----------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
const K_ModuleItem_Typedef ModuleTsensor =
{
  MODULE_TSENSOR,
  _ThermometerConfig,
  _ThermometerDemoExec,
  _ThermometerUnConfig,
  NULL
};  


/**
  * @brief  setup the HW for the 8 uart application 
  * @param  None.
  * @note   set the memeory + Hw initialisation.  
  * @retval None.
  */
KMODULE_RETURN _ThermometerConfig(void)
{

  /* start the Joystick interrupt mode*/
  BSP_JOY_Init(JOY_MODE_GPIO);

  /* Configure the Temperature Sensor for SMBUS*/
  TSENSOR_InitStructure.AlertMode             = TSENSOR_INTERRUPT_MODE;
  TSENSOR_InitStructure.ConversionMode        = TSENSOR_CONTINUOUS_MODE;
  
  TSENSOR_InitStructure.TemperatureLimitHigh  = TEMPERATURE_HIGH;
  TSENSOR_InitStructure.TemperatureLimitLow   = TEMPERATURE_LOW;

  if (TSENSOR_Init(TSENSOR_ADDR, &TSENSOR_InitStructure) != TSENSOR_SMB_OK)
  {
    /* Initialization Error */
    BSP_LCD_SetTextColor(LCD_COLOR_RED);    
    BSP_LCD_DisplayStringAt(0, 115, (uint8_t*)"Initialization problem", CENTER_MODE); 
    Error_Handler();
  }
  
  return KMODULE_OK;
}

/**
  * @brief  Unconfigure the SMBUS
  * @param  None.
  * @retval None.
  */
KMODULE_RETURN _ThermometerUnConfig(void)
{
 if (TSENSOR_DeInit() == TSENSOR_SMB_ERROR)
  {
    Error_Handler();
  }
  return KMODULE_OK;
}
/**
  * @brief  Run the 8 uart application 
  * @param  None.
  * @note   run and display information about the uart transaction.  
  * @retval None.
  */
KMODULE_RETURN _ThermometerDemoExec(void)
{
  /* Prepare the main MMI */
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  kMenu_Execute(ThermometerMenu);
  
  /* Execute the app 8uart */
  /* App initialization    */
  return KMODULE_OK;
}

void ThermometerDemo_SMB(void)
{
uint16_t tempvalue = 0;
uint16_t consignvalue_max = 0;
uint16_t consignvalue_min = 0;
uint16_t offsetvalue_max = 3;
uint16_t offsetvalue_min = 2;
uint8_t  temptargetStr[30];
uint8_t start = 0;
uint8_t step = 0;
float consignvalue;
float tempcelsiusdisplayround=0;

  BSP_JOY_Init(JOY_MODE_EXTI);
  user_stop = 0;

  /* Display user information */
  ThermometerUserHeader();
  
  /*Set Targeted temperature*/ 
  tempvalue = TSENSOR_ReadTemp(TSENSOR_ADDR); 
  consignvalue_max=(uint16_t)((tempvalue >> 4)+offsetvalue_max);
  consignvalue_min=(uint16_t)((tempvalue >> 4)+offsetvalue_min);
  consignvalue=(float)((consignvalue_min+consignvalue_max)/2.0);
  
  /*Display advice to heat thermometer*/
  BSP_LCD_SetFont(&Font16);
  start=150;
  step=16;
  
  BSP_LCD_SetTextColor(LCD_COLOR_DARKMAGENTA);
  BSP_LCD_DisplayStringAt(0, start, (uint8_t *)"Blow air to heat or cool", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, start+step, (uint8_t *)"STML75 Thermometer(U2)", CENTER_MODE); 
  BSP_LCD_DisplayStringAt(0, start+2*step, (uint8_t *)"sensor located on", CENTER_MODE); 
  BSP_LCD_DisplayStringAt(0, start+3*step, (uint8_t *)"board top left corner", CENTER_MODE); 

  /* Reconfigure the Temperature Sensor for SMBUS with new consigne*/
  TSENSOR_InitStructure.AlertMode             = TSENSOR_INTERRUPT_MODE;
  TSENSOR_InitStructure.ConversionMode        = TSENSOR_CONTINUOUS_MODE;
  TSENSOR_InitStructure.TemperatureLimitHigh  = consignvalue_max;
  TSENSOR_InitStructure.TemperatureLimitLow   = consignvalue_min;

  if (TSENSOR_Init(TSENSOR_ADDR, &TSENSOR_InitStructure) != TSENSOR_SMB_OK)
  {
       /* Initialization Error */
       BSP_LCD_SetTextColor(LCD_COLOR_RED);
       BSP_LCD_DisplayStringAt(0, 115, (uint8_t*)"Initialization problem", CENTER_MODE);
       Error_Handler();
   }

  while(user_stop == 0) 
  {
      /* Get temperature value */  
      tempvalue = TSENSOR_ReadTemp(TSENSOR_ADDR); 
        
      /*Display Targeted temperature*/
      BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
      BSP_LCD_SetFont(&Font16);
      BSP_LCD_DisplayStringAt(0, 50, (uint8_t*)"Targeted Temperature : ", CENTER_MODE);
      BSP_LCD_SetFont(&Font20);
      BSP_LCD_SetTextColor(LCD_COLOR_ORANGE);  
     
      sprintf((char*)temptargetStr, "       %5.1f C      ", (float)consignvalue);

      BSP_LCD_DisplayStringAt(0, 70, (uint8_t*)temptargetStr, CENTER_MODE);   
      
      /* Display current temperature value */
      tempcelsiusdisplayround=0;
      TSENSOR_Display_Temperature(tempvalue,&tempcelsiusdisplayround);
      	
      /* If integer temperature is inside threshold LOW and HIGH temperature, clear warning message */
      if ((tempcelsiusdisplayround > consignvalue_min) && (tempcelsiusdisplayround < consignvalue_max))
      {
        /* Clear previous warning message */
        BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
        BSP_LCD_FillRect(0, 150, BSP_LCD_GetXSize(), 60);
		
        /* Display warning message Temperature high limit exceeded            */
        BSP_LCD_SetTextColor(LCD_COLOR_DARKGREEN);
        BSP_LCD_DisplayStringAt(0, 150, (uint8_t *)" Temperature value ", CENTER_MODE);
        BSP_LCD_DisplayStringAt(0, 170, (uint8_t *)"  in targeted range  ", CENTER_MODE);
        BSP_LCD_DisplayStringAt(0, 190, (uint8_t *)"  *TEMP MATCH* ", CENTER_MODE);
		
      }
      
    /* Check if an alert occurs                                               */
    if (ThermometerAlertOccus == 1)
    {
      /* Clear first recommendation message */
      BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
      BSP_LCD_FillRect(0, 150, BSP_LCD_GetXSize(), 64);	

      /* Refresh current temperature value */
	    tempvalue = TSENSOR_ReadTemp(TSENSOR_ADDR);
      tempcelsiusdisplayround=0;
      TSENSOR_Display_Temperature(tempvalue,&tempcelsiusdisplayround);	  
			  
      /* Display warning message depends on Limit value                       */
      if (tempcelsiusdisplayround >= consignvalue)
      {
        BSP_LCD_SetTextColor(LCD_COLOR_RED);
        /* Display warning message Temperature high limit exceeded            */
        BSP_LCD_DisplayStringAt(0, 150, (uint8_t *)"Temperature Limit High", CENTER_MODE);
        BSP_LCD_DisplayStringAt(0, 170, (uint8_t *)"  has been exceeded  ", CENTER_MODE);
        BSP_LCD_DisplayStringAt(0, 190, (uint8_t *)"  *SWITCH OFF HEATER* ", CENTER_MODE);
      }
      else
      {
        BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
        /* Display warning message Temperature is at or blow low limit        */
        BSP_LCD_DisplayStringAt(0, 150, (uint8_t *)" Temperature is at or ", CENTER_MODE);
        BSP_LCD_DisplayStringAt(0, 170, (uint8_t *)"below the Low Limit", CENTER_MODE);
        BSP_LCD_DisplayStringAt(0, 190, (uint8_t *)"  *SWITCH ON HEATER* ", CENTER_MODE);
      }
      ThermometerAlertOccus = 0;
    }
        
    HAL_Delay(500);
  }

}

/**
  * @brief  Display uart transaction state 
  * @param  None.
  * @note   This display information about the uart transaction.  
  * @retval None.
  */
void ThermometerUserHeader(void)
{
  /* Set the Back Color */
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
  /* Set the Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
}  

/**
  * @brief  Get User action 
  * @param  sel : User selection (JOY_SEL,...)
  * @note   This example is the only way to get user information.  
  * @retval None
  */
void ThermometerMenuUserAction(uint8_t sel)
{
  switch(sel)
  {
  case JOY_DOWN : 
       user_stop = 1;
    break;
  default :
    break;
  }

}

  /**
  * @brief  Display temperature
  * @param  temperature : temperature value
  * @retval None
  */
static void TSENSOR_Display_Temperature(uint16_t Temperature,float *tempcelsiusdisplayround)
{
  uint8_t tempcelsiusdisplay[] = "+abc.dddd C";
  uint8_t tempcelsiusdisplaytrk[]="+bc.d C";;
  uint16_t temperaturevalue = Temperature;
  uint16_t temperaturevaluecelsius = 0;
  uint32_t tempcelsius = 0;
  float tempcelsiusdisplayroundin=0;
  
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_SetFont(&Font16);
  BSP_LCD_DisplayStringAt(0, 100, (uint8_t*)"Measured Temperature : ", CENTER_MODE);
    
  /* Change Font size to display Temperature Value */
  BSP_LCD_SetFont(&Font20);
  
  /* Verify the sign of the temperature */
  if (temperaturevalue <= 2048)
  {
    /* Positive temperature measured */
    tempcelsiusdisplay[0] = '+';
    /* Initialize the temperature sensor value*/
    temperaturevaluecelsius = temperaturevalue;
    /* Set Text color to Green */
    BSP_LCD_SetTextColor(LCD_COLOR_GREEN);    
  }
  else
  {
    /* Negative temperature measured */
    tempcelsiusdisplay[0] = '-';
    /* Remove temperature value sign */
    temperaturevaluecelsius = 0x1000 - temperaturevalue;
    /* Set Text color to Red */
    BSP_LCD_SetTextColor(LCD_COLOR_BLUE);  
  }

  tempcelsius = 0;
  /* Calculate temperature digits in °C */
  if (temperaturevaluecelsius & 0x01)
  {
    tempcelsius += 625;     
  }
  if (temperaturevaluecelsius & 0x02)
  {
    tempcelsius += 1250;
  }
  if (temperaturevaluecelsius & 0x04)
  {
    tempcelsius += 2500;
  }
  if (temperaturevaluecelsius & 0x08)
  {
    tempcelsius += 5000;
  }
  
  tempcelsiusdisplay[5] = (tempcelsius / 1000) + 0x30;
  tempcelsiusdisplay[6] = ((tempcelsius % 1000) / 100) + 0x30;
  tempcelsiusdisplay[7] = (((tempcelsius % 1000) % 100) / 10)+ 0x30;
  tempcelsiusdisplay[8] = (((tempcelsius % 1000) % 100) % 10) + 0x30;
    
  temperaturevaluecelsius >>= 4;

  tempcelsiusdisplay[1] = (temperaturevaluecelsius / 100) + 0x30;
  tempcelsiusdisplay[2] = ((temperaturevaluecelsius % 100) / 10) + 0x30;
  tempcelsiusdisplay[3] = ((temperaturevaluecelsius % 100) % 10) + 0x30;

  /*Reformat: trunked temperature value*/
  tempcelsiusdisplaytrk[0]=tempcelsiusdisplay[0];
  tempcelsiusdisplaytrk[1]=tempcelsiusdisplay[2];
  tempcelsiusdisplaytrk[2]=tempcelsiusdisplay[3];
  tempcelsiusdisplaytrk[3]=tempcelsiusdisplay[4];
  tempcelsiusdisplaytrk[4]=tempcelsiusdisplay[5];
  tempcelsiusdisplaytrk[6]=tempcelsiusdisplay[10]; 

  /* Display Temperature value on LCD */
  BSP_LCD_DisplayStringAt(0, 120, tempcelsiusdisplaytrk, CENTER_MODE);

  /*return equivalent floating rounded value*/
  tempcelsiusdisplayroundin=(float)(temperaturevaluecelsius+(float)tempcelsius/10000.0);
  if (tempcelsiusdisplaytrk[0]=='-')tempcelsiusdisplayroundin=-tempcelsiusdisplayroundin;
  *tempcelsiusdisplayround=tempcelsiusdisplayroundin;
  
}

/**
  * @brief  TSENSOR error callbacks.
  * @param  Error : Temperature Sensor Error status.
  * @retval None
  */
void TSENSOR_SMB_ERRORCallback(uint16_t Error)
{
  if (Error == TSENSOR_SMB_ALERT)
  {
    ThermometerAlertOccus = 1;
  }
  else
  {
    Error_Handler();
  }
  
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)  
{
    /* Clear Previous message on the LCD */
    BSP_LCD_SetBackColor(LCD_COLOR_WHITE);  
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);    
    BSP_LCD_FillRect(12, 92, BSP_LCD_GetXSize() - 24, BSP_LCD_GetYSize()- 104);

    /* Display Communication error message */
    BSP_LCD_SetTextColor(LCD_COLOR_RED);    
    BSP_LCD_DisplayStringAt(0, 115, (uint8_t*)"An error occurs during", CENTER_MODE); 
    BSP_LCD_DisplayStringAt(0, 130, (uint8_t*)"communication with", CENTER_MODE);
    BSP_LCD_DisplayStringAt(0, 145, (uint8_t*)"the temperature sensor", CENTER_MODE);

    /* Turn LED_RED on */
    BSP_LED_On(LED_RED);
    while(1)
    {
    }
}

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

