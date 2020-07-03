/**
  ******************************************************************************
  * @file    histogram.c
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

/* Includes ------------------------------------------------------------------*/
#include "string.h"
#include "stdio.h"
#include "k_config.h"
#include "k_module.h"
#include "k_menu.h"
#include "k_storage.h"
#include "histogram.h"

/** @addtogroup HISTOGRAM_DISPLAY
  * @{
  */

/* Private typedef ----------------------------------------------------------*/
/* Private constants --------------------------------------------------------*/
#define FONT_TITLE       Font12
#define FONT_X           Font8
#define FONT_Y           Font8

/* Private typedef ----------------------------------------------------------*/
/* Private function ---------------------------------------------------------*/
static void DisplayDataColor(HistogramDisplay_Typedef * pHisto, uint32_t color);
static uint32_t GetMaxValue(HistogramDisplay_Typedef * pHisto);

/**
  * @brief  Init histo and display the background
  * @param  pHisto
  * @note   
  * @retval HISTO_RETURN
  */
HISTO_RETURN HistogramInit(HistogramDisplay_Typedef * pHisto)
{
   uint32_t length, y1, i;
  
  /* Clear internal context */
  pHisto->graph_x0 = pHisto->x0;
  pHisto->graph_y0 = pHisto->y0;
  pHisto->graph_xSize = pHisto->xSize;
  pHisto->graph_ySize = pHisto->ySize;

  /* Clear the whole window */
  length = pHisto->x0 + pHisto->xSize;
  
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  
  for(y1 = pHisto->y0; y1 < pHisto->ySize; y1++)
  {
    BSP_LCD_DrawHLine(pHisto->x0, y1 ,length);
  }

  /* Display Title */
  HistogramUpdateTitle(pHisto);

  /* Display Y description */
  HistogramUpdateYDescr(pHisto);

  /* Display X description */
  HistogramUpdateXDescr(pHisto);

  pHisto->graph_y0 += (FONT_TITLE.Height + FONT_Y.Height);
  pHisto->graph_ySize -= (FONT_TITLE.Height + FONT_Y.Height + FONT_X.Height);

  /* Display X and Y axis */
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_DrawHLine(pHisto->graph_x0, pHisto->graph_y0 + pHisto->graph_ySize, pHisto->graph_xSize);
  BSP_LCD_DrawVLine(pHisto->graph_x0, pHisto->graph_y0, pHisto->graph_ySize);

  /* Init curve variable  */
  if(pHisto->curve_Range == 0)
  {
    pHisto->curve_Range = CURVE_RANGE;
  }

  for (i = 0; i< SIZE_OF_LINE; i++) 
  {
    pHisto->curve_tab[i] = pHisto->graph_y0 + 1;
    pHisto->curve_tab2[i] = pHisto->graph_y0 + 1;
  }
  
  pHisto->curve_index = pHisto->graph_x0;
  
  return HISTO_OK;
}

/**
  * @brief  Update the title
  * @param  None.
  * @note   
  * @retval HISTO_RETURN
  */
HISTO_RETURN HistogramUpdateTitle(HistogramDisplay_Typedef * pHisto)
{
  /* Display Title */
  if(pHisto->title != NULL)
  {          
    /* Set the Text Color */
    BSP_LCD_SetFont(&FONT_TITLE);
    BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
    BSP_LCD_DisplayStringAt(pHisto->x0, pHisto->y0, (uint8_t *)pHisto->title, CENTER_MODE);
    return HISTO_OK;
  }
  return HISTO_FAIL;
}

/**
  * @brief  Update the title
  * @param  None.
  * @note   
  * @retval HISTO_RETURN
  */
HISTO_RETURN HistogramUpdateXDescr(HistogramDisplay_Typedef * pHisto)
{
  /* Display xDescr */
  if(pHisto->xDescr != NULL)
  {          
    /* Set the Text Color */
    BSP_LCD_SetFont(&FONT_X);
    BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
    BSP_LCD_DisplayStringAt(pHisto->x0, pHisto->y0 + pHisto->ySize - FONT_X.Height + 2, (uint8_t *)pHisto->xDescr, RIGHT_MODE);
    return HISTO_OK;
  }
  return HISTO_FAIL;
}

/**
  * @brief  Update the title
  * @param  None.
  * @note   
  * @retval HISTO_RETURN
  */
HISTO_RETURN HistogramUpdateYDescr(HistogramDisplay_Typedef * pHisto)
{
  /* Display yDescr */
  if(pHisto->yDescr != NULL)
  {          
    /* Set the Text Color */
    BSP_LCD_SetFont(&FONT_Y);
    BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
    BSP_LCD_DisplayStringAt(pHisto->graph_x0, pHisto->y0 + FONT_TITLE.Height, (uint8_t *)pHisto->yDescr, LEFT_MODE);
    return HISTO_OK;
  }
  return HISTO_FAIL;
}

/**
  * @brief  Clear the histogram
  * @param  None.
  * @note   
  * @retval HISTO_RETURN
  */
HISTO_RETURN HistogramClearData(HistogramDisplay_Typedef * pHisto)
{
  DisplayDataColor(pHisto, LCD_COLOR_WHITE);
  return HISTO_OK;
}

/**
  * @brief  Display the histo
  * @param  None.
  * @note   run and display information about the pressure measurement.  
  * @retval None.
  */
HISTO_RETURN HistogramDisplayData(HistogramDisplay_Typedef * pHisto,  uint32_t color)
{
  DisplayDataColor(pHisto, color);
  return HISTO_OK;
}

/**
  * @brief  Curve add point
  * @param  None.
  * @note   run and display information about the pressure measurement.  
  * @retval None.
  */
HISTO_RETURN CurveAddPoint(HistogramDisplay_Typedef * pHisto, uint32_t value, uint32_t value2)
{
  uint32_t delta, halfRange, PointYcenter,histoValueOneStep;
  uint8_t PointY;
  
  PointYcenter = pHisto->graph_y0 + (pHisto->graph_ySize / 2);
  
  halfRange = pHisto->curve_Range / 2;
  
  if (pHisto->curve_index == pHisto->graph_x0)
  {
    pHisto->curve_FirstPointValue = value;
    pHisto->curve_FirstPointValue2 = value2;

    /* Clear all previous measures */  
    CurveClear(pHisto);
  }
  else
  {
    histoValueOneStep = (pHisto->curve_Range / pHisto->graph_ySize);
      
    /* Clean Last point value 1 */
    if (value <= pHisto->curve_FirstPointValue)
    {
      delta = pHisto->curve_FirstPointValue - value;
      if (delta > halfRange)
      {
        /* Under range */
        PointY = pHisto->graph_y0 + pHisto->graph_ySize;
      }
      else
      {
        PointY = PointYcenter + delta / histoValueOneStep; 
      }
    }
    else
    {
      delta =  value - pHisto->curve_FirstPointValue;
      if (delta > halfRange)
      {
        /* Over range */
        PointY = pHisto->graph_y0;
      }
      else
      {
        PointY = PointYcenter - delta / histoValueOneStep; 
      }
    }
    
    if (PointY == (pHisto->graph_y0 + pHisto->graph_ySize))
    {
      PointY --; /* make it visible */
    }
                               
    /* Display new point */
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_DrawPixel(pHisto->curve_index, PointY);

    pHisto->curve_tab[pHisto->curve_index] = PointY;


    /* Clean Last point value 2 */
    if (value2 <= pHisto->curve_FirstPointValue2)
    {
      delta = pHisto->curve_FirstPointValue2 - value2;
      if (delta > halfRange)
      {
        /* Under range */
        PointY = pHisto->graph_y0 + pHisto->graph_ySize;
      }
      else
      {
        PointY = PointYcenter + delta / histoValueOneStep; 
      }
    }
    else
    {
      delta =  value2 - pHisto->curve_FirstPointValue2;
      if (delta > halfRange)
      {
        /* Over range */
        PointY = pHisto->graph_y0;
      }
      else
      {
        PointY = PointYcenter - delta / histoValueOneStep; 
      }
    }
    
    if (PointY == (pHisto->graph_y0 + pHisto->graph_ySize))
    {
      PointY --; /* make it visible */
    }
                               
    /* Display new point */
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_DrawPixel(pHisto->curve_index, PointY);
    
    /* Sav position */
    pHisto->curve_tab2[pHisto->curve_index] = PointY;
  }

  pHisto->curve_index++;
  if (pHisto->curve_index > (pHisto->graph_x0 + pHisto->graph_xSize))
  {
    pHisto->curve_index = pHisto->graph_x0;
  }
  return HISTO_OK;
}

/**
  * @brief  Curve add point
  * @param  None.
  * @note   run and display information about the pressure measurement.  
  * @retval None.
  */
HISTO_RETURN CurveClear(HistogramDisplay_Typedef * pHisto)
{
  uint32_t i;
  
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);

  for (i = pHisto->graph_x0; i< (pHisto->graph_x0 + pHisto->graph_xSize); i++)
  {
    BSP_LCD_DrawPixel(i, pHisto->curve_tab[i]);
    BSP_LCD_DrawPixel(i, pHisto->curve_tab2[i]);
  }
  
  pHisto->curve_index = pHisto->graph_x0;
  
  return HISTO_OK;
}


/**
  * @brief  Curve add point
  * @param  None.
  * @note   run and display information about the pressure measurement.  
  * @retval None.
  */
HISTO_RETURN CurveZoomOut(HistogramDisplay_Typedef * pHisto)
{
  return HISTO_OK;
}

/**
  * @brief  Curve add point
  * @param  None.
  * @note   run and display information about the pressure measurement.  
  * @retval None.
  */
HISTO_RETURN CurveZoomIn(HistogramDisplay_Typedef * pHisto)
{
  return HISTO_OK;
}

/**
  * @brief  Display the histo
  * @param  histo.
  * @note   run and display information about the pressure measurement.  
  * @retval None.
  */
static void DisplayDataColor(HistogramDisplay_Typedef * pHisto, uint32_t color)
{
  uint32_t index, histoValueOneStep, histoValueWidth, histoYmax, maxValue;
  uint32_t X0, Y0, Ylength;
  
  
  maxValue = (pHisto->maxValue != 0) ? pHisto->maxValue : GetMaxValue(pHisto);
  
  histoValueWidth = pHisto->graph_xSize / pHisto->width;
  histoValueOneStep = (maxValue / pHisto->graph_ySize) + 1;
  histoYmax = pHisto->graph_y0 + pHisto->graph_ySize;
  
  BSP_LCD_SetTextColor(color);

  for (index = 0; index < pHisto->width; index++)
  {
    X0 = pHisto->graph_x0 + (index * histoValueWidth);
    Ylength = (pHisto->array[index] / histoValueOneStep);
    Y0 = histoYmax - Ylength;

    if (Y0 != histoYmax) /* do not modify the X axis */
    {
      if (index) /* do not modify the Y axis */
      {
        BSP_LCD_DrawVLine(X0 + histoValueWidth, Y0, Ylength); 
      }

      if (X0 != pHisto->graph_x0)
      {
        BSP_LCD_DrawVLine(X0, Y0, Ylength); 
      }
      else
      {
        X0++; /* do not modify the Y axis */
      }
      BSP_LCD_DrawHLine(X0, Y0, histoValueWidth); 
    }
  }
}

/**
  * @brief  Get the max value of the array
  * @param  histo.
  * @note   run and display information about the pressure measurement.  
  * @retval None.
  */
static uint32_t GetMaxValue(HistogramDisplay_Typedef * pHisto)
{
  uint32_t index, maxValueIndex = 0;
  
  for (index = 0; index < pHisto->width; index++)
  {
    if (pHisto->array[index] > maxValueIndex)
    {
      maxValueIndex = pHisto->array[index];
    }
  }
  return maxValueIndex;
}



/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

