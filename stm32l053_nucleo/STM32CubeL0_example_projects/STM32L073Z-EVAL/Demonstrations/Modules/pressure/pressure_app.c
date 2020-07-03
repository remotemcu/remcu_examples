/**
  ******************************************************************************
  * @file    pressure_app.c
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

#define __PRESSURE_APP_C

/* Includes ------------------------------------------------------------------*/
#include "string.h"
#include "stdio.h"
#include "k_config.h"
#include "k_module.h"
#include "k_menu.h"
#include "k_storage.h"
#include "histogram.h"

#include <math.h> 

/** @addtogroup PRESSURE_MODULE
  * @{
  */

/** @defgroup SYSTEM_INFO
  * @brief system info routines 
  * @{
  */

/* Private typedef ----------------------------------------------------------*/    

typedef enum {
  CURVE_OVS_AND_NO_OVS = 1,  /* First state */
  HISTO_NO_OVS = 2,          /* Second state */
  HISTO_OVS = 3,             /* Third state */
  NO_MORE_STEP = 4
} DEMO_STEP;

#define DEMO_FIRST_STEP  CURVE_OVS_AND_NO_OVS


/* Private constants ----------------------------------------------------------*/

/* With 12 bit ADC, 5 levels of quantification are enough to display all the measures */
#define HISTOGRAM_WIDTH_NOVS        5

/* With 16 bit ADC (oversampling) 16 time more levels of quantification to display the same range */
#define HISTOGRAM_WIDTH_OVS         (HISTOGRAM_WIDTH_NOVS * 16 - 1) /* minus one to optimize the display... */
/* Number of measure to centre the histogram before each bunch of measure */
#define HISTOGRAM_CENTER_AVERAGE    50
    
/* Number of measure during a bunch of measure */
#define HISTOGRAM_MAX               2000

/* Message displayed */
const char HistoNOVSTitle[] = "Statistical distribution without oversampling";
const char HistoOVSTitle[]  = " Statistical distribution with oversampling  ";
const char CurveTitle[]     = " Move up the board more than 1,5 meters      ";
const char HistoYdescr[] = "Hits    ";
const char CurveYdescr[] = "Pressure";
const char HistoXdescrOVS[]  = "      Code (16-bit) ";
const char HistoXdescrNOVS[] = "      Code (12-bit) ";
const char CurveXdescr[]     = "               Time  ";

/* Function prototypes ------------------------------------------------------*/
KMODULE_RETURN _PressureWakeupDemoExec(void);
KMODULE_RETURN _PressureWakeupConfig(void);
KMODULE_RETURN _PressureWakeupUnConfig(void);
void PressureWakeupDemo(void);

static void PressureWakeupUserHeader(void);
static void ADC_DeConfiguration(void);
static void ADC_Configuration(void);
static void ClearHistogram(uint32_t array[], uint32_t size);
static void CenterHistogram(uint32_t * pHisto, uint32_t size);
static void StatHistogram(uint32_t * pHisto, uint32_t size,  uint32_t valueFirstIndex, uint32_t StatHistogram);
static void Error_Handler(void);

/* Private Variable ----------------------------------------------------------*/
const tMenuItem PressureWakeupMenuMenuItems[] =
{
    {"", 14, 30, TYPE_EXEC, MODULE_NONE, PressureWakeupDemo, NULL, NULL, NULL },
};

const tMenu PressureWakeupMenu = {
  "Pressure Meas.", PressureWakeupMenuMenuItems, countof(PressureWakeupMenuMenuItems), TYPE_EXEC, 1, 1 };

/* used to exit application */

/* ADC handle declaration */
ADC_HandleTypeDef        AdcHandle;

/* ADC channel configuration structure declaration */
ADC_ChannelConfTypeDef   sConfig;

/* Private typedef -----------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
const K_ModuleItem_Typedef PressureModuleItem =
{
  MODULE_PRESSURE_MEASUREMENT,
  _PressureWakeupConfig,
  _PressureWakeupDemoExec,
  _PressureWakeupUnConfig,
  NULL
};  

/**
  * @brief  un-Configure the Pressure application
  * @param  None.
  * @note
  * @retval None.
  */
KMODULE_RETURN _PressureWakeupUnConfig(void)
{
  if (HAL_ADC_DeInit(&AdcHandle) != HAL_OK)
  {
    Error_Handler();
  }
  return KMODULE_OK;
}

/**
  * @brief  setup the pressure measurement application
  * @param  None.
  * @note   set the memeory + Hw initialisation.  
  * @retval None.
  */
KMODULE_RETURN _PressureWakeupConfig(void)
{

  return KMODULE_OK;
}

/**
  * @brief  Run the pressure measurement application 
  * @param  None.
  * @note   run and display information about the pressure measurement.  
  * @retval None.
  */
KMODULE_RETURN _PressureWakeupDemoExec(void)
{
  /* Prepare the main MMI */
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  kMenu_Execute(PressureWakeupMenu);
  
  /* Execute the app 8uart */
  /* App initialization    */
  return KMODULE_OK;
}

  __IO uint32_t  uwPressurePa;

void PressureWakeupDemo(void)
{
  /* Converted value declaration */
  uint32_t histogram_nonOVS[HISTOGRAM_WIDTH_NOVS], histogram_OVS[HISTOGRAM_WIDTH_OVS];
#ifdef DISPLAY_ALTITUDE
  uint32_t AltPressureInit = 0, AltPressureInit = 0, AltDeltaSum = 0;
#endif
  uint32_t uwConvertedValue = 0, uwConvertedValueOVS = 0, uwInputVoltage = 0, histogram_center = 0;

  int32_t i = 0, hist_avg_cnt = 0, histogram_cnt = 0;
  HistogramDisplay_Typedef histoDisplay;
  JOYState_TypeDef JoyState = JOY_NONE;
  DEMO_STEP demoStep;
  char pressure[20] = {0};
  
  /* Display user information */
  PressureWakeupUserHeader();

  /* Prepare histogram */
  histoDisplay.y0 = Font24.Height + 2; 
  histoDisplay.ySize = BSP_LCD_GetYSize() - (2 * Font24.Height) - 4;
  histoDisplay.x0 = 2;
  histoDisplay.xSize = BSP_LCD_GetXSize() - 4;
  histoDisplay.maxValue = 0;                     
  histoDisplay.xDescr = (char*) &CurveXdescr;
  histoDisplay.yDescr = (char*) &CurveYdescr;
  histoDisplay.title = (char*) &CurveTitle;
  histoDisplay.array = histogram_nonOVS;
  histoDisplay.width = HISTOGRAM_WIDTH_NOVS;
  histoDisplay.curve_Range = 0; /* use default initial range */

  /* Display user action messages */
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 2 * Font12.Height, (uint8_t *)"Long press Sel to continue", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - Font12.Height, (uint8_t *)"or long press Down to exit",   CENTER_MODE);
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
  
  /* Init histogram */
  if (HistogramInit(&histoDisplay) != HISTO_OK)
  {
    while(1);
  }

  /* Set the Text Color */
#ifdef DISPLAY_ALTITUDE
  BSP_LCD_DisplayStringAt(5, 190, (uint8_t *)"Pressure(Pa) :           Altitude(cm)  ", LEFT_MODE);
  sprintf((char *)pressure, "%5d", 0);
#else
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_SetTextColor(LCD_COLOR_RED);
  BSP_LCD_DisplayStringAt(5, 182, (uint8_t *)"Oversampling  ON (16-bit):       Pa ", LEFT_MODE);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_DisplayStringAt(5, 194, (uint8_t *)"Oversampling OFF (12-bit):       Pa ", LEFT_MODE);
#endif
  BSP_LCD_DisplayStringAt(BSP_LCD_GetXSize() - 40, 190, (uint8_t*)pressure, LEFT_MODE);
  
  /* Init ADC */
  ADC_Configuration();
  
  /* Display pressure until user event */
  demoStep = DEMO_FIRST_STEP;
  
  BSP_JOY_Init(JOY_MODE_GPIO);

  while(JoyState != JOY_DOWN)
  {
    JoyState = BSP_JOY_GetState();

    /* Change demo step and clear diplayed context */
    switch((int32_t)JoyState)
    {
      case JOY_SEL :
        demoStep++;
        
        if (demoStep == NO_MORE_STEP)
        {
          demoStep = DEMO_FIRST_STEP;
        }
        
        if (demoStep == HISTO_OVS)
        {
          /* Clear the displayed messages */
          BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
          BSP_LCD_FillRect(5, 60, 100 , 2 *Font12.Height);

          /* Update the title of the histogram */
          histoDisplay.title = (char*) &HistoOVSTitle;
          HistogramUpdateTitle(&histoDisplay);

          /* Update the X decription of the histogram */
          histoDisplay.xDescr = (char*) &HistoXdescrOVS;
          HistogramUpdateXDescr(&histoDisplay);
          
          /* Clear the displayed data */
          HistogramClearData(&histoDisplay);

          /* Change data to be displayed */
          histoDisplay.array = histogram_OVS;
          histoDisplay.width = HISTOGRAM_WIDTH_OVS;
        }
        
        if (demoStep == HISTO_NO_OVS)
        {
          /* Update the title of the histogram */
          histoDisplay.title = (char*) &HistoNOVSTitle;
          HistogramUpdateTitle(&histoDisplay);

          /* Update the X decription of the histogram */
          histoDisplay.xDescr = (char*) &HistoXdescrNOVS;
          HistogramUpdateXDescr(&histoDisplay);

          /* Update the Y decription of the histogram */
          histoDisplay.yDescr = (char*) &HistoYdescr;
          HistogramUpdateYDescr(&histoDisplay);
          
          /* Clear Curve */
          CurveClear(&histoDisplay);

          /* Clear the displayed messages */
          BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
          BSP_LCD_FillRect(5, 182, BSP_LCD_GetXSize()- 5 , Font12.Height);
          BSP_LCD_FillRect(5, 194, BSP_LCD_GetXSize()- 5 , Font12.Height - 1);
          
          /* Change data to be displayed */
          histoDisplay.array = histogram_nonOVS;
          histoDisplay.width = HISTOGRAM_WIDTH_NOVS;
        }
   
        if (demoStep == CURVE_OVS_AND_NO_OVS)
        {
          /* Clear the displayed messages */
          BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
          BSP_LCD_FillRect(5, 60, 100 , 2 *Font12.Height);

          /* Update the title of the histogram */
          histoDisplay.title = (char*) &CurveTitle;
          HistogramUpdateTitle(&histoDisplay);

          /* Update the X decription of the histogram */
          histoDisplay.xDescr = (char*) &CurveXdescr;
          HistogramUpdateXDescr(&histoDisplay);

          /* Update the Y decription of the histogram */
          histoDisplay.yDescr = (char*) &CurveYdescr;
          HistogramUpdateYDescr(&histoDisplay);
          
          /* Clear the displayed data */
          HistogramClearData(&histoDisplay);

          /* Set the Text Color */
#ifdef DISPLAY_ALTITUDE
          BSP_LCD_DisplayStringAt(5, 190, (uint8_t *)"Pressure(Pa) :           Altitude(cm)  ", LEFT_MODE);
          sprintf((char *)pressure, "%5d", 0);
          BSP_LCD_DisplayStringAt(BSP_LCD_GetXSize() - 40, 190, (uint8_t*)pressure, LEFT_MODE);
#else
          BSP_LCD_SetFont(&Font12);
          BSP_LCD_SetTextColor(LCD_COLOR_RED);
          BSP_LCD_DisplayStringAt(5, 182, (uint8_t *)"Oversampling  ON (16-bit):       Pa ", LEFT_MODE);
          BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
          BSP_LCD_DisplayStringAt(5, 194, (uint8_t *)"Oversampling OFF (12-bit):       Pa ", LEFT_MODE);
#endif
          
#ifdef DISPLAY_ALTITUDE
          /* Reset initiale altitude */
          AltPressureInit = 0;
          AltDeltaSum = 0;
          i = 0;
#endif
        }
        
        break;
    
      default :
      break;
    }

    while (BSP_JOY_GetState() != JOY_NONE);

    switch(demoStep)
    {
      case HISTO_NO_OVS :

        /* Initialize ADC peripheral w/o Oversampling */
        if (AdcHandle.Init.OversamplingMode != DISABLE)
        {
          AdcHandle.Init.OversamplingMode = DISABLE;
          if (HAL_ADC_Init(&AdcHandle) != HAL_OK)
          {
            Error_Handler();
          }
        }

        /* Start ADC Conversion */
        if (HAL_ADC_Start(&AdcHandle) != HAL_OK)
        {
          Error_Handler();
        }
        
        histogram_center = 0;
        
        for (hist_avg_cnt = 0; hist_avg_cnt < HISTOGRAM_CENTER_AVERAGE; hist_avg_cnt++)
        {
          if (HAL_ADC_PollForConversion(&AdcHandle, 10) != HAL_OK)
          {
            Error_Handler();
          }
          
          /* Read the converted value */
          histogram_center += HAL_ADC_GetValue(&AdcHandle);
        }
        
        histogram_center = histogram_center / HISTOGRAM_CENTER_AVERAGE;

        /* Clear the displayed data */
        HistogramClearData(&histoDisplay);
        
        /* Clear buffer */
        ClearHistogram(histogram_nonOVS, HISTOGRAM_WIDTH_NOVS);
        
        for (histogram_cnt = 0; histogram_cnt < HISTOGRAM_MAX; histogram_cnt++)
        {
          if (HAL_ADC_PollForConversion(&AdcHandle, 10) != HAL_OK)
          {
            Error_Handler();
          }
          
          /* Read the converted value */
          uwConvertedValue = HAL_ADC_GetValue(&AdcHandle);
          
          i = uwConvertedValue - histogram_center + HISTOGRAM_WIDTH_NOVS / 2;
          if (i >=0 && i < HISTOGRAM_WIDTH_NOVS)  // check if it's inside the histogram valid array indices
          {
            histogram_nonOVS[i]++; 
          }
        }

        /* Display stat */
        BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
        StatHistogram(histogram_nonOVS, HISTOGRAM_WIDTH_NOVS,  histogram_center - HISTOGRAM_WIDTH_NOVS / 2, 4096);

        /* Centre the graph with the real value */
        CenterHistogram(histogram_nonOVS, HISTOGRAM_WIDTH_NOVS);
        
        /* Stop ADC Conversion */
        if (HAL_ADC_Stop(&AdcHandle) != HAL_OK)
        {
          Error_Handler();
        }
        
        /* Display the histogram */
        HistogramDisplayData(&histoDisplay, LCD_COLOR_BLACK);
        HAL_Delay(500);
        break;
        
      case HISTO_OVS :
        /* *****************  Oversampling method used *********************/
        
        /* Initialize ADC peripheral with Oversampling */
        if (AdcHandle.Init.OversamplingMode == DISABLE)
        {
          AdcHandle.Init.OversamplingMode = ENABLE;
          if (HAL_ADC_Init(&AdcHandle) != HAL_OK)
          {
            Error_Handler();
          }
        }

        /* Start ADC Conversion */
        if (HAL_ADC_Start(&AdcHandle) != HAL_OK)
        {
          Error_Handler();
        }        

        histogram_center = 0;
        
        for (hist_avg_cnt = 0; hist_avg_cnt < HISTOGRAM_CENTER_AVERAGE; hist_avg_cnt++)
        {
          if (HAL_ADC_PollForConversion(&AdcHandle, 10) != HAL_OK)
          {
            Error_Handler();
          }
          
          /* Read the converted value */
          histogram_center += HAL_ADC_GetValue(&AdcHandle);
        }

        histogram_center = histogram_center / HISTOGRAM_CENTER_AVERAGE;
        
        /* Clear the displayed data */
        HistogramClearData(&histoDisplay);
        
        /* Clear buffer */
        ClearHistogram(histogram_OVS, HISTOGRAM_WIDTH_OVS);

        for (histogram_cnt = 0; histogram_cnt < HISTOGRAM_MAX; histogram_cnt++)
        {
          if (HAL_ADC_PollForConversion(&AdcHandle, 10) != HAL_OK)
          {
            Error_Handler();
          }
          
          /* Read the converted value */
          uwConvertedValue = HAL_ADC_GetValue(&AdcHandle);
          
          i = uwConvertedValue - histogram_center + HISTOGRAM_WIDTH_OVS/2;
          if (i >=0 && i < HISTOGRAM_WIDTH_OVS)  // check if it's inside the histogram valid array indices
          {
            histogram_OVS[i]++; 
          }
        }

        /* Stop ADC Conversion */
        if (HAL_ADC_Stop(&AdcHandle) != HAL_OK)
        {
          Error_Handler();
        }        
        
        /* Display stat */
        BSP_LCD_SetTextColor(LCD_COLOR_RED);
        StatHistogram(histogram_OVS, HISTOGRAM_WIDTH_OVS,  histogram_center - HISTOGRAM_WIDTH_OVS / 2, 65536);

        /* Centre the graph with the real value */
        CenterHistogram(histogram_OVS, HISTOGRAM_WIDTH_OVS);

        /* Display the histogram */
        HistogramDisplayData(&histoDisplay, LCD_COLOR_RED);
        HAL_Delay(500);
        break;
        
      case CURVE_OVS_AND_NO_OVS :
        
          /* Start ADC Conversion w/o oversampling */
          AdcHandle.Init.OversamplingMode = DISABLE;

          if (HAL_ADC_Init(&AdcHandle) != HAL_OK)
          {
            Error_Handler();
          }

          if (HAL_ADC_Start(&AdcHandle) != HAL_OK)
          {
            Error_Handler();
          }        
        
          if (HAL_ADC_PollForConversion(&AdcHandle, 10) != HAL_OK)
          {
            Error_Handler();
          }
          
          if (HAL_ADC_Stop(&AdcHandle) != HAL_OK)
          {
            Error_Handler();
          }        
          
          /* Read the converted value with oversampling */
          uwConvertedValue = HAL_ADC_GetValue(&AdcHandle);

          /* Display current pressure without oversampling */
          sprintf((char *)pressure, "%d", (int)uwPressurePa);
          uwInputVoltage = ((uint64_t)(uwConvertedValue) * 3300000) / 4096; 

          /* Convert mv to Pa (experiemental trendline formula)*/
          uwPressurePa = (uwInputVoltage * 147 / 1000 + 746000) / 10;
          
          sprintf((char *)pressure, "%d", (int)uwPressurePa);
          BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
          BSP_LCD_DisplayStringAt(5 + 183 , 194, (uint8_t*)pressure, LEFT_MODE);
          
          /* Start ADC Conversion with oversampling */
          AdcHandle.Init.OversamplingMode = ENABLE;

          if (HAL_ADC_Init(&AdcHandle) != HAL_OK)
          {
            Error_Handler();
          }

          if (HAL_ADC_Start(&AdcHandle) != HAL_OK)
          {
            Error_Handler();
          }        
        
          if (HAL_ADC_PollForConversion(&AdcHandle, 10) != HAL_OK)
          {
            Error_Handler();
          }
          
          if (HAL_ADC_Stop(&AdcHandle) != HAL_OK)
          {
            Error_Handler();
          }        
          
          /* Read the converted value with oversampling */
          uwConvertedValueOVS = HAL_ADC_GetValue(&AdcHandle);
          uwInputVoltage = ((uint64_t)(uwConvertedValueOVS) * 3300000) / 4096 / 16; 

          /* Convert mv to Pa (experiemental trendline formula)*/
          uwPressurePa = (uwInputVoltage * 147 / 1000 + 746000) / 10;
          
          /* Display current pressure oversampling */
          sprintf((char *)pressure, "%d", (int)uwPressurePa);
          BSP_LCD_SetTextColor(LCD_COLOR_RED);
          BSP_LCD_DisplayStringAt(5 + 183 , 182, (uint8_t*)pressure, LEFT_MODE);
          
          /* Add points to the curve */
          CurveAddPoint(&histoDisplay, uwConvertedValue * 16, uwConvertedValueOVS);

#ifdef DISPLAY_ALTITUDE
          /* Relative altitude compared to the first measure */
          if (AltPressureInit == 0)
          {
            /* First step, get the average pressure */
            if (i < 10)
            {
              AltDeltaSum += uwPressurePa;
              i++;
            }
            else
            {
              AltPressureInit = AltDeltaSum / 10;
              AltDeltaSum = 0;
              i = 0;
            }
          }
          else
          {
            /* Second step, compare delta the average pressure */
            if (i < 10)
            {
              AltDeltaSum += (AltPressureInit - uwPressurePa);
              i++;
            }
            else
            {
              i = 0;
              /* 1 mBar is approximately equivalent to 8 metres */
              sprintf((char *)pressure, "%5d", AltDeltaSum * 8 / (10 * 10));
              BSP_LCD_DisplayStringAt(BSP_LCD_GetXSize() - 40, 190, (uint8_t*)pressure, LEFT_MODE);
              AltDeltaSum = 0;
            }

            /* Reset the altitude delta */
            if (histoDisplay.curve_index == histoDisplay.graph_x0)
            {
              AltPressureInit = 0;
              i = 0;
            }            
          }
#endif /*#ifdef DISPLAY_ALTITUDE */         
          HAL_Delay(200);

        break;
        
      default :
        Error_Handler();
      break;
    }
  }
  
  /* DeInit ADC */
  ADC_DeConfiguration();
}

/**
  * @brief  Display transaction state
  * @param  None.
  * @note   This display information about the uart transaction.  
  * @retval None.
  */
static void PressureWakeupUserHeader(void)
{
  /* Set the Back Color */
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
  /* Set the Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
}  

/**
* @brief  ADC MSP Init
* @param  hadc : ADC handle
* @retval None
*/
void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc)
{
  GPIO_InitTypeDef          GPIO_InitStruct;
  
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO clock ****************************************/
  __HAL_RCC_GPIOA_CLK_ENABLE();
  /* ADC1 Periph clock enable */
  __HAL_RCC_ADC1_CLK_ENABLE();
  
  /*##-2- Configure peripheral GPIO ##########################################*/ 
  /* ADC1 Channel0 GPIO pin configuration */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct); 
}

/**
* @brief  ADC MSP Deinit
* @param  hadc : ADC handle
* @retval None
*/
void HAL_ADC_MspDeInit(ADC_HandleTypeDef *hadc)
{
  /* ADC1 Periph clock disable */
  __HAL_RCC_ADC1_CLK_DISABLE();

  __HAL_RCC_ADC1_FORCE_RESET();
  __HAL_RCC_ADC1_RELEASE_RESET();

  /* GPIO clock disable ****************************************/
  __HAL_RCC_GPIOA_CLK_DISABLE();
  
}

static void ClearHistogram(uint32_t array[], uint32_t size)
{
  uint32_t index;
  

  for (index = 0; index < size; index++)
  {
    array[index] = 0;
  }
}

static void CenterHistogram(uint32_t * pHisto, uint32_t size)
{
  uint32_t index = 0, indexMax = 0, maxValue = 0;
  int32_t indexDelta = 0;
  
  /* Get the index of the max value */
  for (index = 0; index < size; index++)
  {
    if (pHisto[index] > maxValue)
    {
      maxValue = pHisto[index];
      indexMax = index;
    }
  }
  
  /* Get the delta with the expected max */
  indexDelta = indexMax - (size / 2);
  
  if (indexDelta < 0)
  {
    indexDelta = - indexDelta;
    /* Center the histo on indexMax */
    for (index = (size - indexDelta); index > 0; index--)
    {
       pHisto[index + indexDelta] = pHisto[index];
    }
  }
  else  if (indexDelta > 0)
  {
    /* Center the histo on indexMax */
    for (index = 0; index < (size - indexDelta); index++)
    {
      pHisto[index] = pHisto[index + indexDelta];
    }
  }
}

static void StatHistogram(uint32_t * pHisto, uint32_t size,  uint32_t valueFirstIndex, uint32_t resolution)
{
  float average = 0, std_deviation = 0, hitSum = 0, min, max;
  uint32_t index, i;
  char temp[20] = {0};  
  
  /* Calculate Average */
  for (index = 0; index < size; index++)
  {
    average += pHisto[index] * (index + 1);
    hitSum += pHisto[index];
  }
  
  average = average / hitSum;
    
  /* Calculate standart deviation */
  for (i=0 ; i < size ; i++)
  {
    std_deviation += pHisto[i] * pow(((i+1) - average), 2);
  }
  std_deviation = sqrt(std_deviation / hitSum);  

  /* min max value with standart deviation */ 
  min = (average) + valueFirstIndex - (std_deviation);
  max = min + 2  * (std_deviation) ;
  
  /* Convert to voltage */ 
  min = ((min) * 3300000) / resolution; 
  max = ((max) * 3300000) / resolution; 

  /* Convert mv to Pa (experiemental trendline formula)*/
  min = (min * 147 / 1000 + 746000) / 10;
  max = (max * 147 / 1000 + 746000) / 10;
 
  sprintf((char *)temp, "%.0f", min);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(5, 60, (uint8_t *)"Min", LEFT_MODE);
  BSP_LCD_DisplayStringAt(5 + 30 , 60, (uint8_t*)temp, LEFT_MODE);
  BSP_LCD_DisplayStringAt(5 + 30 + 45, 60,(uint8_t *)"Pa", LEFT_MODE);
  
  sprintf((char *)temp, "%.0f", max);
  BSP_LCD_DisplayStringAt(5, 72, (uint8_t *)"Max", LEFT_MODE);
  BSP_LCD_DisplayStringAt(5 + 30 , 72, (uint8_t*)temp, LEFT_MODE);
  BSP_LCD_DisplayStringAt(5 + 30 + 45, 72,(uint8_t *)"Pa", LEFT_MODE);
}


static void ADC_DeConfiguration(void)
{
  /* DeInitialize ADC peripheral according to the passed parameters */
  if (HAL_ADC_DeInit(&AdcHandle) != HAL_OK)
  {
    Error_Handler();
  }
}

static void ADC_Configuration(void)
{
  /* ### - Initialize ADC peripheral #################################### */
  /*
  *  Instance                  = ADC1.
  *  OversamplingMode          = DISABLE
  *  .Oversample.Ratio         = x16
  *  .Oversample.RightBitShift = no bit shift
  *  .Oversample.TriggeredMode = single trigger (each trigger, all the oversampling series are performed).
  *  ClockPrescaler            = PCLK clock with no division.
  *  LowPowerAutoOff           = Disabled 
  *  LowPowerFrequencyMode     = Disabled (To be enabled only if ADC clock is lower than 2.8MHz) 
  *  LowPowerAutoWait          = Disabled (New conversion starts only when the previous conversion is completed)       
  *  Resolution                = 12 bit (increased to 16 bit with oversampler)
  *  SamplingTime              = 28.5 cycles od ADC clock.
  *  ScanDirection             = Upward 
  *  DataAlign                 = Right
  *  ContinuousConvMode        = Enabled
  *  DiscontinuousConvMode     = Enabled
  *  ExternalTrigConvEdge      = None (Software start)
  *  EOCSelection              = End Of Conversion event
  *  DMAContinuousRequests     = Disabled
  */
  
  AdcHandle.Instance = ADC1;
  
  AdcHandle.Init.OversamplingMode         = DISABLE;
  AdcHandle.Init.Oversample.Ratio         = ADC_OVERSAMPLING_RATIO_256;
  AdcHandle.Init.Oversample.RightBitShift = ADC_RIGHTBITSHIFT_4;
  AdcHandle.Init.Oversample.TriggeredMode = ADC_TRIGGEREDMODE_SINGLE_TRIGGER;
  
  AdcHandle.Init.ClockPrescaler        = ADC_CLOCK_SYNC_PCLK_DIV2;
  AdcHandle.Init.LowPowerAutoPowerOff  = DISABLE;
  AdcHandle.Init.LowPowerFrequencyMode = DISABLE;
  AdcHandle.Init.LowPowerAutoWait      = DISABLE;
  
  AdcHandle.Init.Resolution            = ADC_RESOLUTION_12B;
  AdcHandle.Init.SamplingTime          = ADC_SAMPLETIME_39CYCLES_5;
  AdcHandle.Init.ScanConvMode          = ADC_SCAN_DIRECTION_FORWARD;
  AdcHandle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
  AdcHandle.Init.ContinuousConvMode    = ENABLE;
  AdcHandle.Init.DiscontinuousConvMode = DISABLE;
  AdcHandle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;
  AdcHandle.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;
  AdcHandle.Init.DMAContinuousRequests = DISABLE;
  
  /* Initialize ADC peripheral according to the passed parameters */
  if (HAL_ADC_Init(&AdcHandle) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* ### - 2 - Channel configuration ######################################## */
  /* Select Channel 0 to be converted */
  sConfig.Channel = ADC_CHANNEL_0;    
  sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;    

  if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /* ### - 3 - Start calibration ############################################ */
  if (HAL_ADCEx_Calibration_Start(&AdcHandle, ADC_SINGLE_ENDED) != HAL_OK)
  {
    Error_Handler();
  }
}

void Error_Handler(void)
{
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

