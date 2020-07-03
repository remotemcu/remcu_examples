/**
  ******************************************************************************
  * @file    BSP/Src/temperature_sensor.c 
  * @author  MCD Application Team
  * @brief   This example code shows how to manage an I2C Temperature Sensor
  *          =================================================================== 
  *          Notes:
  *           - This driver is intended for STM32L0xx families devices only. 
  *           - The I2C Temperature Sensor is available on the STM32L073Z-EVAL board.
  *          ===================================================================
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

/** @addtogroup BSP
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus; 
typedef enum {NOT_READY = 0, READY = !NOT_READY} Status; 
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void TSENSOR_SetHint(void);
static void TSENSOR_Display_Temperature(float temperature);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  TSENSOR Demo
  * @param  None
  * @retval None
  */
void TSENSOR_demo (void)
{ 
  float    TempValue = 0;
  uint8_t  TsensorReady = 0;
  uint8_t  RequestSample = 0;
  uint32_t Tick = 0;

  TSENSOR_SetHint();

  /* Initialize the I2C TSENSOR driver ----------------------------------------*/
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);  
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);    
  BSP_LCD_FillRect(12, 92, BSP_LCD_GetXSize() - 24, BSP_LCD_GetYSize()- 104);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);

  if (BSP_TSENSOR_Init() != TSENSOR_OK)
  {
    BSP_LCD_SetTextColor(LCD_COLOR_RED);    
    BSP_LCD_DisplayStringAt(0, 115, (uint8_t*)"Initialization problem", CENTER_MODE); 
    BSP_LCD_DisplayStringAt(0, 130, (uint8_t*)"Temperature Sensor not detected", CENTER_MODE); 
  }
  else
  {
    TsensorReady = READY;
    RequestSample = SET;
  }

  while (1)
  {
    if((TsensorReady == READY) && (RequestSample == SET))
    {
      TempValue = BSP_TSENSOR_ReadTemp();
      TSENSOR_Display_Temperature(TempValue);
      Tick = HAL_GetTick();
      RequestSample = RESET;
    }
    else
    {
      /* Request a Temperature sampling each 1s <-> 1000 ms */
      if(HAL_GetTick() >= Tick + 1000)
      {
        RequestSample = SET;
      }
    }
    
    
    if(CheckForUserInput() > 0)
    {
      return;
    }
    HAL_Delay(5);
  }
}

/**
  * @brief  Display TSENSOR Demo Hint
  * @param  None
  * @retval None
  */
static void TSENSOR_SetHint(void)
{
  /* Clear the LCD */ 
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  
  /* Set LCD Demo description */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 80);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE); 
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_DisplayStringAt(0, 0, (uint8_t*)"TEMPERATURE SENSOR", CENTER_MODE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 30, (uint8_t*)"This example shows how to", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 45, (uint8_t*)"read a Temperature", CENTER_MODE); 
  BSP_LCD_DisplayStringAt(0, 60, (uint8_t*)"Press key button to switch to next menu", CENTER_MODE); 
  
   /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);  
  BSP_LCD_DrawRect(10, 90, BSP_LCD_GetXSize() - 20, BSP_LCD_GetYSize()- 100);
  BSP_LCD_DrawRect(11, 91, BSP_LCD_GetXSize() - 22, BSP_LCD_GetYSize()- 102);
 }

  /**
  * @brief  Display temperature
  * @param  temperature : temperature value
  * @retval None
  */
static void TSENSOR_Display_Temperature(float temperature)
{
  uint8_t TempCelsiusDisplay[20] = {0};

  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 115, (uint8_t*)"Measured Temperature : ", CENTER_MODE);

  /* Change Font size to display Temperature Value */
  BSP_LCD_SetFont(&Font16);

  if ((uint16_t)temperature & 0x8000)
  {
    /* Display negative temperature */
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    if ((uint16_t)temperature & 0x0001)
    {
      sprintf ((char*)TempCelsiusDisplay, " -%d.5 C ", (((uint16_t)temperature & 0x7FFF)>>1));
    }
    else
    {
      sprintf ((char*)TempCelsiusDisplay, "  -%d C  ", (((uint16_t)temperature & 0x7FFF)>>1));
    }
  }
  else
  {
    /* Display positive temperature */
    BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
    if ((uint16_t)temperature & 0x0001)
    {
      sprintf ((char*)TempCelsiusDisplay, "  %d.5 C ", (((uint16_t)temperature & 0x7FFF)>>1));
    }
    else
    {
      sprintf ((char*)TempCelsiusDisplay, "  %d C   ", (((uint16_t)temperature & 0x7FFF)>>1));
    }
  }
  /* Display Temperature value on LCD */
  BSP_LCD_DisplayStringAt(0, 145, (uint8_t*)TempCelsiusDisplay, CENTER_MODE);

  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_SetFont(&Font12);
}

/**
  * @}
  */ 
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
