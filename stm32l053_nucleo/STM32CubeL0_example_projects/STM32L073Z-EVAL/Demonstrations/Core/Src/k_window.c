/**
  ******************************************************************************
  * @file    k_window.c
  * @author  MCD Application Team   
  * @brief   This file contains the Hex dumps of the images available
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
#define  _K_WINDOW_C
/* Includes ------------------------------------------------------------------*/
#include <k_config.h>
#include <k_window.h>

static void kWindow_PopupGeneric(char *title, uint16_t title_tc, 
                                 uint16_t title_bc ,char *Msg, 
                                 uint16_t msg_tc, uint16_t msg_bc,
                                 Line_ModeTypdef mode) ;

/** @addtogroup CORE
  * @{
  */

/** @defgroup KERNEL_RES
  * @brief Kernel resources 
  * @{
  */


/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported functions ---------------------------------------------------------*/

/**
  * @brief  Handle a window display with message centered.
  * @param  title to display a message, \n is used for multiple line.
  * @param  title_tc : title text color.
  * @param  title_bc : title backgroun color.
  * @param  Msg to display a message, \n is used for multiple line.
  * @param  msg_tc : title text color.
  * @param  msg_bc : title backgroun color.
  * @retval None
  */
void kWindow_PopupCentered(char *title, uint16_t title_tc, uint16_t title_bc ,char *Msg, uint16_t msg_tc, uint16_t msg_bc )
{
   kWindow_PopupGeneric(title,title_tc,title_bc ,Msg,msg_tc,msg_bc,CENTER_MODE);
}

/**
  * @brief  Handle a window display with message left aligned.
  * @param  title to display a message, \n is used for multiple line.
  * @param  title_tc : title text color.
  * @param  title_bc : title backgroun color.
  * @param  Msg to display a message, \n is used for multiple line.
  * @param  msg_tc : title text color.
  * @param  msg_bc : title backgroun color.
  * @retval None
  */
void kWindow_PopupAligned(char *title, uint16_t title_tc, uint16_t title_bc ,char *Msg, uint16_t msg_tc, uint16_t msg_bc )
{
  kWindow_PopupGeneric(title,title_tc,title_bc ,Msg,msg_tc,msg_bc,LEFT_MODE);

}

/**
  * @brief  Handle a generic window display.
  * @param  title to display a message, \n is used for multiple line.
  * @param  title_tc : title text color.
  * @param  title_bc : title backgroun color.
  * @param  Msg to display a message, \n is used for multiple line.
  * @param  msg_tc : title text color.
  * @param  msg_bc : title backgroun color.
  * @retval None
  */
static void kWindow_PopupGeneric(char *title, uint16_t title_tc, uint16_t title_bc ,char *Msg, uint16_t msg_tc, uint16_t msg_bc,Line_ModeTypdef mode )
{
  uint8_t substring[25];
  uint8_t lineindex;
  uint16_t index,subindex;

  /* Clear the LCD Screen */
  BSP_LCD_Clear(title_bc);
  BSP_LCD_SetBackColor(title_bc);
  BSP_LCD_SetTextColor(title_tc);

  /* Set the Back Color */
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_SetTextColor(title_tc);
  BSP_LCD_DisplayStringAt(0, 0, (uint8_t *)title, mode);

  /* Prepare the msg area */
  BSP_LCD_SetBackColor(msg_bc);
  BSP_LCD_SetTextColor(msg_bc);
  BSP_LCD_FillRect(0, Font24.Height, BSP_LCD_GetXSize(), BSP_LCD_GetYSize() - Font24.Height);
  BSP_LCD_SetTextColor(msg_tc);

  lineindex = subindex = index = 0;
  do
  {
    substring[subindex]=Msg[index];
    if((Msg[index] == '\n') || (Msg[subindex] == '\0'))
    {
      substring[subindex] = '\0';
      BSP_LCD_DisplayStringAt(0, (2+lineindex) * Font24.Height, substring, mode);
      lineindex++;
      subindex = 0;
    }
    else
    {
      subindex++;
    }

    if(Msg[index] != '\0')
    {
      index++;
    }
  }
  while(Msg[index] != '\0');

}

/**
  * @brief  Handle a window display.
  * @param  Msg to display a message, \n is used for multiple line.
  * @retval None
  */
void kWindow_Error(char *msg)
{
  kWindow_PopupGeneric("Error popup",LCD_COLOR_BLACK,LCD_COLOR_RED,msg,LCD_COLOR_BLACK,LCD_COLOR_RED,CENTER_MODE);
}

/**
  * @}
  */

/**
  * @}
  */
  
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
