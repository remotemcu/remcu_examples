/**
  ******************************************************************************
  * @file    stm8l_discovery_lcd.h
  * @author  Microcontroller Division
	* @version V1.2.0
	* @date    09/2010
  * @brief   This file contains all the functions prototypes for the glass LCD
  *          firmware driver.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __stm8l_discovery_lcd
#define __stm8l_discovery_lcd

/* Includes ------------------------------------------------------------------*/
#include "stm8l15x.h"
   
/* Define for scrolling sentences*/
#define SCROLL_SPEED  	40
#define SCROLL_SPEED_L  80
#define SCROLL_NUM    	1

/* Define for character '.' */
#define  POINT_OFF FALSE
#define  POINT_ON TRUE

/* Define for caracter ":" */
#define  COLUMN_OFF FALSE
#define  COLUMN_ON TRUE

#define DOT 0x8000 /* for add decimal point in string */
#define DOUBLE_DOT 0x4000 /* for add decimal point in string */

/* Macros used for set/reset bar LCD bar */
#define BAR0_ON  t_bar[0] |= 0x80
#define BAR0_OFF t_bar[0] &= ~0x80
#define BAR1_ON  t_bar[1] |= 0x08
#define BAR1_OFF t_bar[1] &= ~0x08
#define BAR2_ON  t_bar[0] |= 0x20
#define BAR2_OFF t_bar[0] &= ~0x20
#define BAR3_ON t_bar[1] |= 0x02
#define BAR3_OFF t_bar[1] &= ~0x02

/* code for 'µ' character */
#define C_UMAP 0x6081

/* code for 'm' character */
#define C_mMap 0xb210

/* code for 'n' character */
#define C_nMap 0x2210

/* constant code for '*' character */
#define star 0xA0D7

/* constant code for '-' character */
#define C_minus 0xA000

void LCD_bar(void);
void LCD_GLASS_Init(void);
void LCD_GLASS_WriteChar(uint8_t* ch, bool point, bool column,uint8_t position);
void LCD_GLASS_DisplayString(uint8_t* ptr);
void LCD_GLASS_DisplayStrDeci(uint16_t* ptr);
void LCD_GLASS_ClearChar(uint8_t position);
void LCD_GLASS_Clear(void);
void LCD_GLASS_ScrollSentence(uint8_t* ptr, uint16_t nScroll, uint16_t ScrollSpeed);
void LCD_GLASS_WriteTime(char a, uint8_t posi, bool column);

#endif /* __STM8L1526_EVAL_GLASS_LCD_H*/

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
