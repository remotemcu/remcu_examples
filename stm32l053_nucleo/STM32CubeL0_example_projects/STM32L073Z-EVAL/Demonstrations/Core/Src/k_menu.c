/**
  ******************************************************************************
  * @file    k_menu.c
  * @author  MCD Application Team   
  * @brief   This file provides the kernel menu functions 
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
#include "k_menu.h"
#include "k_storage.h"
#include "k_module.h"

/** @addtogroup CORE
  * @{
  */

/** @defgroup KERNEL_MENU
  * @brief Kernel menu routines
  * @{
  */

#define C_DELAY_JOYSTICK_MIN    200   // Before it was 300
#define C_DELAY_JOYSTICK_MAX    300   // Before it was 400
/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
enum {
  KMENU_HEADER,
  KMENU_ICON,
  KMENU_TEXT,
  KMENU_EXEC,
  KMENU_WAITEVENT,
  KMENU_EXIT
};
  
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Variable used to forward an user event to an application */
static tExecAction kMenuEventForward = NULL;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static void kMenu_HandleSelection(tMenu MainMenu, uint8_t *sel);
static void displayTextWhenNoBitMapsAvailable(uint32_t index);

/**
  * @brief  Function to initialize the module menu
  * @param  menu 
  * @retval None
  */
void kMenu_Init(void) {
  /* Initialise the JOystick in polling mode */ 
  BSP_JOY_Init(JOY_MODE_GPIO);
}

/**
  * @brief  Function in charge to handle the menu selection
  * @param  menu
  * @param  sel : slected item
  * @retval None
  */
void kMenu_HandleSelection(tMenu MainMenu, uint8_t *sel)
{
  uint8_t exit = 0;
  JOYState_TypeDef joyState = JOY_NONE;
  tMenu psCurrentMenu = MainMenu;
  uint8_t index;
  
  switch(psCurrentMenu.nType)
  {
  case TYPE_ICON :
    {
      /* display the default selection */
      BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
      BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
      BSP_LCD_DrawRect(psCurrentMenu.psItems[*sel].x, psCurrentMenu.psItems[*sel].y, 64,  64);
      
      do{
        while((joyState = BSP_JOY_GetState()) != JOY_NONE);
        while((joyState = BSP_JOY_GetState()) == JOY_NONE);

        /* Remove previous selection */
        BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
        BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
        BSP_LCD_DrawRect(psCurrentMenu.psItems[*sel].x, psCurrentMenu.psItems[*sel].y, 64,  64);
        
        switch(joyState)
        {
        case JOY_SEL :
          exit = 1;
          break;
        case JOY_DOWN :
          /* check if sel is on the latest line position */
          if(*sel >=  ((psCurrentMenu.line - 1)*psCurrentMenu.column))
          {
            *sel = ((*sel) % psCurrentMenu.column);
          }
          else
          {
            (*sel)+= psCurrentMenu.column;
          }
          break;
        case JOY_LEFT :
          if(*sel == 0 ) {
            *sel = psCurrentMenu.nItems -1;
          }
          else { (*sel)--; }
          break;    
        case JOY_RIGHT :
          (*sel)++;
          if ((*sel) >= (psCurrentMenu.nItems)) { (*sel) = 0; }
          break;
        case JOY_UP :
          /** check if sel is on the first line */
          if ((*sel) < psCurrentMenu.column) {
            (*sel)+= ((psCurrentMenu.line - 1)*psCurrentMenu.column);
          } else {
            (*sel)-= psCurrentMenu.column; }
          break;
        default:
              break;
        }
        
        /* display the new selection */
        BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
        BSP_LCD_DrawRect(psCurrentMenu.psItems[*sel].x, psCurrentMenu.psItems[*sel].y, 64,  64);
        
        /* display the new header file */
        BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
        BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
        BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), Font24.Height);
        BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
        BSP_LCD_DisplayStringAt(0, 0, (uint8_t *)psCurrentMenu.psItems[*sel].pszTitle, CENTER_MODE);
      } while(exit == 0);
    }
    break;
  case TYPE_TEXT :
    {
      do
      {
        index = 0;
        while (!(index >= psCurrentMenu.nItems))
        {
          if (index == (*sel))
          {
            /* Set the Back Color */
            BSP_LCD_SetBackColor(LCD_COLOR_GREEN);
            /* Set the Text Color */
            BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
          }
          else
          {
            /* Set the Back Color */
            BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
            /* Set the Text Color */
            BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
          }
          /* Get the current menu */
          BSP_LCD_DisplayStringAt(0, Font24.Height*(index+1), (uint8_t *)psCurrentMenu.psItems[index].pszTitle,LEFT_MODE);
          index++;
        }
        
        while((joyState = BSP_JOY_GetState()) != JOY_NONE);
        while((joyState = BSP_JOY_GetState()) == JOY_NONE);
        
        switch(joyState)
        {
        case JOY_SEL :
          exit = 1;
          break;
        case JOY_DOWN :
          (*sel)++;
          /* check if sel is on the latest line position */
          if(*sel >= psCurrentMenu.nItems)
          {
            *sel = psCurrentMenu.nItems - 1;
          }
          break;
        case JOY_UP :
          /** check if sel is on the first line */
          if ((*sel) != 0) { (*sel)--; }
          break;
        default:
          break;
        }
      } while(exit == 0);
    }
    break;
  }
  
  return;
}

/**
  * @brief  Function in charge to execture a menu 
  * @param  menu 
  * @retval None
  */
void kMenu_Execute(tMenu psCurrentMenu) 
{
  uint32_t index = 0, exit = 1;
  uint32_t k_MenuState = KMENU_HEADER;
  uint8_t sel = 0;
  STORAGE_RETURN result = KSTORAGE_NOERROR;
  
  do 
  {  
    switch(k_MenuState)
    {
    case KMENU_HEADER :
      {
        /****************************** Display Header *************************/  
        /* Clear the LCD Screen */
        BSP_LCD_Clear(LCD_COLOR_WHITE);

        if(psCurrentMenu.pszTitle != NULL)
        {          
          /* Set the Back Color */
          BSP_LCD_SetFont(&Font24);
          BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
          BSP_LCD_SetTextColor(LCD_COLOR_BLUE);  
          BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), Font24.Height);
          
          /* Set the Text Color */
          BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
          BSP_LCD_DisplayStringAt(0, 0, (uint8_t *)psCurrentMenu.pszTitle, CENTER_MODE);
        }
        
        switch(psCurrentMenu.nType)
        {
        case TYPE_ICON :
          k_MenuState = KMENU_ICON;
          break;
        case TYPE_TEXT :
          k_MenuState = KMENU_TEXT;
          break;
        case TYPE_EXEC :
          k_MenuState = KMENU_EXEC;
          break;
        default : 
          k_MenuState = KMENU_EXIT;
          break;
        }
      }
      break;
    case KMENU_ICON :
      {
        BSP_LCD_SetTextColor(LCD_COLOR_BLUE);  
        BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), Font24.Height);
        BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
        BSP_LCD_DisplayStringAt(0, 0, (uint8_t *)psCurrentMenu.psItems[sel].pszTitle, CENTER_MODE);
        for(index = 0; index < psCurrentMenu.nItems; index++)
        {
          if(psCurrentMenu.psItems[index].pIconPath != NULL)
          {
             result = kStorage_OpenFileDrawPixel(psCurrentMenu.psItems[index].y, psCurrentMenu.psItems[index].x, (uint8_t *)psCurrentMenu.psItems[index].pIconPath);
             if (result != KSTORAGE_NOERROR)
             {
               displayTextWhenNoBitMapsAvailable(index);
             }
          }
        }
        k_MenuState = KMENU_WAITEVENT;
      }
      break;
    case KMENU_TEXT :
      {
        /* Set the Back Color */
        BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
        /* Set the Text Color */
        BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
        k_MenuState = KMENU_WAITEVENT;
      }
      break;
    case KMENU_EXEC :
      {
        while(BSP_JOY_GetState() != JOY_NONE);
        /* if the function need user feedback enable JOY interrupt and set callback function */
        if(psCurrentMenu.psItems[0].pfActionFunc != NULL)
        {
          /* start the Joystick interrupt */
          BSP_JOY_Init(JOY_MODE_EXTI);
          HAL_Delay(C_DELAY_JOYSTICK_MIN);
          /* set the function to report joystick event */
          kMenuEventForward = psCurrentMenu.psItems[0].pfActionFunc;
        }
        
        kMenu_Header(psCurrentMenu.psItems[0].pszTitle);
        /* Execute the test */
        psCurrentMenu.psItems[0].pfExecFunc();
        
        /* rest user feedback, in polling mode */
        if(psCurrentMenu.psItems[0].pfActionFunc != NULL)
        {
          /* stop the Joystick interrupt */
          BSP_JOY_Init(JOY_MODE_GPIO);
          while(BSP_JOY_GetState() != JOY_NONE);
          /* set the function to report to NULL */
          kMenuEventForward = NULL;
        }
        k_MenuState = KMENU_EXIT;
      }
      break;
    case KMENU_WAITEVENT:
      {
        kMenu_HandleSelection(psCurrentMenu,&sel);
        /* The user has selected an execution menu */
        switch(psCurrentMenu.psItems[sel].SelType)
        {
        case SEL_MODULE:
          /* start the module execution */
          kModule_Execute(psCurrentMenu.psItems[sel].ModuleId);
          k_MenuState = KMENU_HEADER;
          break;
        case SEL_EXEC :
          
          while(BSP_JOY_GetState() != JOY_NONE);
          /* if the function need user feedback enable JOY interrupt and set callback function */
          if(psCurrentMenu.psItems[sel].pfActionFunc != NULL)
          {
            /* start the Joystick interrupt */
            BSP_JOY_Init(JOY_MODE_GPIO);
            while(BSP_JOY_GetState() != JOY_NONE);
            /* set the function to report joystick event */
            kMenuEventForward = psCurrentMenu.psItems[sel].pfActionFunc;
          }
          /* start the function execution */
          psCurrentMenu.psItems[sel].pfExecFunc();
          
          /* rest user feedback, in polling mode */
          if(psCurrentMenu.psItems[sel].pfActionFunc != NULL)
          {
            /* stop the Joystick interrupt */
            BSP_JOY_Init(JOY_MODE_GPIO);
            while(BSP_JOY_GetState() != JOY_NONE);
            /* set the function to report to NULL */
            kMenuEventForward = NULL;
          }
          k_MenuState = KMENU_HEADER;
          break;
        case SEL_SUBMENU :
          /* Select submenu or return on the main module menu */
          kMenu_Execute(*(psCurrentMenu.psItems[sel].psSubMenu));
          k_MenuState = KMENU_HEADER;
          break;
        case SEL_EXIT :
          k_MenuState = KMENU_EXIT;
          break;
        }
      }
      break;
    case KMENU_EXIT :
      exit = 0;
      break;
      
    }
  }while(exit);
}


/**
  * @brief  Function to display header information 
  * @param  menu 
  * @retval None
  */
void kMenu_Header(char *string)
{
  if(string == NULL) return;
  
  BSP_LCD_FillRect(0, 30, BSP_LCD_GetXSize(), Font24.Height);
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);  
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE);  
  BSP_LCD_FillRect(0, BSP_LCD_GetYSize() - Font24.Height, BSP_LCD_GetXSize(), Font24.Height);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_DisplayStringAt(0,  BSP_LCD_GetYSize() - Font24.Height, (uint8_t *)string , CENTER_MODE);
}    


/**
  * @brief  Function in charge to hanlde user event and forward them to the module
  * @param  GPIO_Pin
  * @retval None
  */
void kMenu_EventHandler(uint16_t GPIO_Pin) {
  
  if(kMenuEventForward != NULL)
  {
    switch(GPIO_Pin)
    {
    case JOY_DOWN_PIN :
      (kMenuEventForward)(JOY_DOWN);
      break;
    case JOY_UP_PIN :
      (kMenuEventForward)(JOY_UP);
    case JOY_SEL_PIN :
      (kMenuEventForward)(JOY_SEL);
    case JOY_RIGHT_PIN :
      (kMenuEventForward)(JOY_RIGHT);
    case JOY_LEFT_PIN :
      (kMenuEventForward)(JOY_LEFT);
      break;
    }
  }
}
/**
  * @brief  Function in charge of displaying incons when no SDC card available
  * @param  GPIO_Pin
  * @retval None
  */
static void displayTextWhenNoBitMapsAvailable(uint32_t index)
{

  switch(index)
  {
  case 0 :
     BSP_LCD_SetFont(&Font16);
     BSP_LCD_SetTextColor(LCD_COLOR_LIGHTBLUE);
     BSP_LCD_SetBackColor(LCD_COLOR_LIGHTBLUE);
     BSP_LCD_FillRect(C_MENU_XA + 2,
                      C_MENU_Y_LINE_1 + 2,
                      C_SQUARE,
                      C_SQUARE);
     BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
     BSP_LCD_DisplayStringAt(C_MENU_XA + 5, C_MENU_Y_LINE_1 + 10,(uint8_t *)C_ICON_INTERNAL_TITLE_UART, LEFT_MODE);
     break;
  case 1:
     BSP_LCD_SetFont(&Font16);
     BSP_LCD_SetTextColor(LCD_COLOR_LIGHTGREEN);
     BSP_LCD_SetBackColor(LCD_COLOR_LIGHTGREEN);
     BSP_LCD_FillRect(C_MENU_XB + 2,
                      C_MENU_Y_LINE_1 + 2,
                      C_SQUARE,
                      C_SQUARE);
     BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
     BSP_LCD_DisplayStringAt(C_MENU_XB + 5, C_MENU_Y_LINE_1 + 10,(uint8_t *)C_ICON_INTERNAL_TITLE_LOW_POWER, LEFT_MODE);
     break;

  case 2:
     BSP_LCD_SetFont(&Font16);
     BSP_LCD_SetTextColor(LCD_COLOR_LIGHTRED);
     BSP_LCD_SetBackColor(LCD_COLOR_LIGHTRED);
     BSP_LCD_FillRect(C_MENU_XC + 2,
                      C_MENU_Y_LINE_1 + 2,
                      C_SQUARE,
                      C_SQUARE);
     BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
     BSP_LCD_DisplayStringAt(C_MENU_XC + 5, C_MENU_Y_LINE_1 + 10,(uint8_t *)C_ICON_INTERNAL_TITLE_THERMOMETER, LEFT_MODE);
     break;
  case 3 :
     BSP_LCD_SetFont(&Font16);
     BSP_LCD_SetTextColor(LCD_COLOR_LIGHTCYAN);
     BSP_LCD_SetBackColor(LCD_COLOR_LIGHTCYAN);
     BSP_LCD_FillRect(C_MENU_XA + 2,
                      C_MENU_Y_LINE_2 + 2,
                      C_SQUARE,
                      C_SQUARE);
     BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
     BSP_LCD_DisplayStringAt(C_MENU_XA + 5, C_MENU_Y_LINE_2 + 10,(uint8_t *)C_ICON_INTERNAL_TITLE_LC, LEFT_MODE);
     break;
  case 4:
     BSP_LCD_SetFont(&Font16);
     BSP_LCD_SetTextColor(LCD_COLOR_LIGHTMAGENTA);
     BSP_LCD_SetBackColor(LCD_COLOR_LIGHTMAGENTA);
     BSP_LCD_FillRect(C_MENU_XB + 2,
                      C_MENU_Y_LINE_2 + 2,
                      C_SQUARE,
                      C_SQUARE);
     BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
     BSP_LCD_DisplayStringAt(C_MENU_XB + 5, C_MENU_Y_LINE_2 + 10,(uint8_t *)C_ICON_INTERNAL_TITLE_PRESSURE, LEFT_MODE);
     break;

  case 5:
     BSP_LCD_SetFont(&Font16);
     BSP_LCD_SetTextColor(LCD_COLOR_LIGHTYELLOW);
     BSP_LCD_SetBackColor(LCD_COLOR_LIGHTYELLOW);
     BSP_LCD_FillRect(C_MENU_XC + 2,
                      C_MENU_Y_LINE_2 + 2,
                      C_SQUARE,
                      C_SQUARE);
     BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
     BSP_LCD_DisplayStringAt(C_MENU_XC + 5, C_MENU_Y_LINE_2 + 10,(uint8_t *)C_ICON_INTERNAL_TITLE_ABOUT, LEFT_MODE);
     break;

default :
  break;
 }
     BSP_LCD_SetFont(&Font24);

}


/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
