/**
  ******************************************************************************
  * @file    I2C/IOExpander/main.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32F10x_StdPeriph_Examples
  * @{
  */

/** @addtogroup I2C_IOExpander
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#ifdef USE_STM3210C_EVAL 
  #define MESSAGE1   "   STM3210C-EVAL    " 
#elif defined (USE_STM32100E_EVAL)
  #define MESSAGE1   "   STM32100E-EVAL   " 
#endif

#define MESSAGE2   "  Example on how to " 
#define MESSAGE3   " use the IO Expander"   

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  /* Initialize LEDs and push-buttons mounted on STM3210X-EVAL board */
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);
  
   /* Initialize the LCD */
#ifdef USE_STM3210C_EVAL
  STM3210C_LCD_Init();
#elif defined (USE_STM32100E_EVAL)
  STM32100E_LCD_Init();  
#endif /* USE_STM3210C_EVAL */
 
  /* Clear the LCD */ 
  LCD_Clear(White);
  
  /* Set the LCD Back Color */
  LCD_SetBackColor(Blue);
  
  /* Set the LCD Text Color */
  LCD_SetTextColor(White);    
 
  /* Display messages on the LCD */
  LCD_DisplayStringLine(Line0, MESSAGE1);
  LCD_DisplayStringLine(Line1, MESSAGE2);
  LCD_DisplayStringLine(Line2, MESSAGE3);
  
  /* Configure the IO Expander */
  if (IOE_Config() == IOE_OK)
  {
    /* Display "IO Expander OK" on the LCD */
    LCD_DisplayStringLine(Line4, "   IO Expander OK   ");
  }
  else
  { 
    LCD_DisplayStringLine(Line4, "IO Expander FAILED ");
    LCD_DisplayStringLine(Line5, " Please Reset the  ");
    LCD_DisplayStringLine(Line6, "   board and start ");
    LCD_DisplayStringLine(Line7, "    again          ");
    while(1);
  }

  /* Draw a rectangle with the specifies parameters and Blue Color */
  LCD_SetTextColor(Blue);
  LCD_DrawRect(180, 310, 40, 60);
  
  /* Draw a rectangle with the specifies parameters and Red Color */
  LCD_SetTextColor(Red);
  LCD_DrawRect(180, 230, 40, 60);
  
  /* Draw a rectangle with the specifies parameters and Yellow Color */
  LCD_SetTextColor(Yellow);
  LCD_DrawRect(180, 150, 40, 60);
  
  /* Draw a rectangle with the specifies parameters and Black Color */
  LCD_SetTextColor(Black);
  LCD_DrawRect(180, 70, 40, 60);
  

#ifdef IOE_INTERRUPT_MODE

 #ifdef  USE_STM32100E_EVAL
  /* Enable the Touch Screen interrupts */
  IOE_ITConfig(IOE_ITSRC_TSC);
  
 #else
  /* Enable the Touch Screen and Joystick interrupts */
  IOE_ITConfig(IOE_ITSRC_JOYSTICK | IOE_ITSRC_TSC);
 #endif /* USE_STM32100E_EVAL */
 
#endif /* IOE_INTERRUPT_MODE */
  
  /* Loop infinitely */
  while(1)
  {
#ifdef IOE_POLLING_MODE
 static TS_STATE* TS_State;
    
 #ifdef  USE_STM3210C_EVAL
 
    static JOY_State_TypeDef JoyState = JOY_NONE;
    
    /* Get the Joytick State */
    JoyState = IOE_JoyStickGetState();
    
    switch (JoyState)
    {
	/* None Joystick has been selected */
    case JOY_NONE:
      LCD_DisplayStringLine(Line5, "JOY:     ----       ");
      break; 
    case JOY_UP:
      LCD_DisplayStringLine(Line5, "JOY:     UP         ");
      break;     
    case JOY_DOWN:
      LCD_DisplayStringLine(Line5, "JOY:    DOWN        ");
      break;          
    case JOY_LEFT:
      LCD_DisplayStringLine(Line5, "JOY:    LEFT        ");
      break;         
    case JOY_RIGHT:
      LCD_DisplayStringLine(Line5, "JOY:    RIGHT       ");
      break;                 
    case JOY_CENTER:
      LCD_DisplayStringLine(Line5, "JOY:    CENTER      ");
      break; 
    default:
      LCD_DisplayStringLine(Line5, "JOY:    ERROR       ");
      break;         
    }
 #endif /* USE_STM3210C_EVAL */
    
   
    /* Update the structure with the current position of the Touch screen */
    TS_State = IOE_TS_GetState();  
    
    if ((TS_State->TouchDetected) && (TS_State->Y < 220) && (TS_State->Y > 180))
    {
      if ((TS_State->X > 10) && (TS_State->X < 70))
      { 
      /* Display LD4 on the LCD and turn on LED4 */
        LCD_DisplayStringLine(Line6, " LD4                ");
        STM_EVAL_LEDOn(LED4);
      }
      else if ((TS_State->X > 90) && (TS_State->X < 150))
      {
      /* Display LD3 on the LCD and turn on LED3 */
        LCD_DisplayStringLine(Line6, "      LD3           ");
        STM_EVAL_LEDOn(LED3);
      }
      else if ((TS_State->X > 170) && (TS_State->X < 230))
      {
      /* Display LD2 on the LCD and turn on LED2 */
        LCD_DisplayStringLine(Line6, "           LD2      ");
        STM_EVAL_LEDOn(LED2);
      } 
      else if ((TS_State->X > 250) && (TS_State->X < 310))
      {
      /* Display LD1 on the LCD and turn on LED1 */
        LCD_DisplayStringLine(Line6, "                LD1 ");
        STM_EVAL_LEDOn(LED1);
      }
      
    }
    else
    {
    /* Turn off LED1..4 */
      STM_EVAL_LEDOff(LED1);
      STM_EVAL_LEDOff(LED2);
      STM_EVAL_LEDOff(LED3);
      STM_EVAL_LEDOff(LED4);
    }

#endif /* IOE_POLLING_MODE */  
  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

   LCD_DisplayStringLine(Line0, "assert_param error!!");

  /* Infinite loop */
  while (1)
  {
  }
}

#endif

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
