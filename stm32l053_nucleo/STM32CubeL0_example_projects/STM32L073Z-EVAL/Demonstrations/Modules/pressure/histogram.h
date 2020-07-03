/**
  ******************************************************************************
  * @file    histogram.h
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

/** @devgroup HISTOGRAM_DISPLAY
  * @{
  */

#define SIZE_OF_LINE 320

#define CURVE_RANGE  160


typedef struct
{
/*-- Public interface --*/
  /* Display window */
  uint32_t x0;    /* Top left corner */
  uint32_t y0;    /* Top left corner */
  uint32_t xSize; /* window width */
  uint32_t ySize; /* window height */

  /* Description */
  char * xDescr;
  char * yDescr;
  char * title;
  
  /* Histogram array, size and maximum value
     The values can be directly changed by the user at anytime*/
  uint32_t * array;
  uint32_t width;
  uint32_t maxValue;    /* If zero, the Y scaling is calculated to otpimize the display.
                           If not zero this value is used to calculate the Y scaling */

  uint32_t curve_Range;   /* Y range for the curve display */
                        /* The first display point is automatically centered */
                             
                             
/*-- Private context --*/    
  uint32_t graph_x0;    /* Graph Top left corner */
  uint32_t graph_y0;    /* Graph Top left corner */
  uint32_t graph_xSize; /* Graph width */
  uint32_t graph_ySize; /* Graph height */

  uint8_t  curve_tab[SIZE_OF_LINE]; /* Save curvez point to use the cleaning, 
                             use bigger value for larger screen */
  uint8_t  curve_tab2[SIZE_OF_LINE]; /* Save curvez point to use the cleaning, 
                             use bigger value for larger screen */
  uint32_t curve_index; 
  uint32_t curve_FirstPointValue; 
  uint32_t curve_FirstPointValue2; 
  
} HistogramDisplay_Typedef;


/* typedef -----------------------------------------------------------*/

typedef enum {
  HISTO_OK, 
  HISTO_FAIL,
  HISTO_BAD_PARAM,
} HISTO_RETURN;

/* macros ------------------------------------------------------------*/
/* variables ---------------------------------------------------------*/
/* function prototypes -----------------------------------------------*/

HISTO_RETURN HistogramInit(HistogramDisplay_Typedef * pHisto);

HISTO_RETURN HistogramClearData(HistogramDisplay_Typedef * pHisto);
HISTO_RETURN HistogramDisplayData(HistogramDisplay_Typedef * pHisto,  uint32_t color);

HISTO_RETURN HistogramUpdateTitle(HistogramDisplay_Typedef * pHisto);
HISTO_RETURN HistogramUpdateXDescr(HistogramDisplay_Typedef * pHisto);
HISTO_RETURN HistogramUpdateYDescr(HistogramDisplay_Typedef * pHisto);

HISTO_RETURN CurveAddPoint(HistogramDisplay_Typedef * pHisto, uint32_t value, uint32_t value2);
HISTO_RETURN CurveClear(HistogramDisplay_Typedef * pHisto);
HISTO_RETURN CurveZoomIn(HistogramDisplay_Typedef * pHisto);
HISTO_RETURN CurveZoomOut(HistogramDisplay_Typedef * pHisto);
/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

