/**
  ******************************************************************************
  * @file    stm32l152c_discovery_lcd.c
  * @author  Microcontroller Division
  * @brief   This file includes driver for the glass LCD Module mounted on 
  *          STM32l152C discovery board MB963
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
#include "stm32l152c_discovery_lcd.h"


/** @addtogroup STM32L152C_DISCOVERY_LCD
  * @{
  */
  
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
LCD_HandleTypeDef LCDHandle;

/* LCD BAR status: To save the bar setting after writing in LCD RAM memory */
uint8_t LCDBar = BARLEVEL_OFF;

/*  =========================================================================
                                 LCD MAPPING
    =========================================================================
	    A
     _  ----------
COL |_| |\   |J  /|
       F| H  |  K |B
     _  |  \ | /  |
COL |_| --G-- --M--
        |   /| \  |
       E|  Q |  N |C
     _  | /  |P  \|   
DP  |_| -----------  
	    D         

 An LCD character coding is based on the following matrix:
      { E , D , P , N   }
      { M , C , COL , DP}
      { B , A , K , J   }
      { G , F , Q , H   }

 The character 'A' for example is:
  -------------------------------
LSB   { 1 , 0 , 0 , 0   }
      { 1 , 1 , 0 , 0   }
      { 1 , 1 , 0 , 0   }
MSB   { 1 , 1 , 0 , 0   }     
      -------------------
  'A' =  F    E   0   0 hexa

*/

/* Constant table for cap characters 'A' --> 'Z' */
const uint16_t CapLetterMap[26]=
{
  /* A       B       C       D       E       F       G       H       I  */
  0xFE00, 0x6714, 0x1D00, 0x4714, 0x9D00, 0x9C00, 0x3F00, 0xFA00, 0x0014,
  /* J       K       L       M       N       O       P       Q       R  */
  0x5300, 0x9841, 0x1900, 0x5A48, 0x5A09, 0x5F00, 0xFC00, 0x5F01, 0xFC01,
  /* S       T       U       V       W       X       Y       Z  */
  0xAF00, 0x0414, 0x5b00, 0x18C0, 0x5A81, 0x00C9, 0x0058, 0x05C0
};

/* Constant table for number '0' --> '9' */
const uint16_t NumberMap[10]=
{
  /* 0       1       2       3       4       5       6       7       8       9  */
  0x5F00, 0x4200, 0xF500, 0x6700, 0xEA00, 0xAF00, 0xBF00, 0x04600, 0xFF00, 0xEF00
};

/* Private function prototypes -----------------------------------------------*/
static void LCD_Convert_CharToSeg(uint8_t* c, uint8_t Point, uint8_t Column, uint8_t* digit);

/**
  * @brief Configures the LCD GLASS relative GPIO port IOs and LCD peripheral.
  * @param None
  * @retval None
  */
void BSP_LCD_GLASS_Init(void)
{
  LCDHandle.Instance = LCD;
  LCDHandle.Init.Prescaler = LCD_PRESCALER_2;
  LCDHandle.Init.Divider = LCD_DIVIDER_31;
  LCDHandle.Init.Duty = LCD_DUTY_1_4;
  LCDHandle.Init.Bias = LCD_BIAS_1_3;
  LCDHandle.Init.VoltageSource = LCD_VOLTAGESOURCE_INTERNAL;
  LCDHandle.Init.Contrast = LCD_CONTRASTLEVEL_7;
  LCDHandle.Init.DeadTime = LCD_DEADTIME_2;
  LCDHandle.Init.PulseOnDuration = LCD_PULSEONDURATION_7;
  LCDHandle.Init.HighDrive = DISABLE;
  LCDHandle.Init.BlinkMode = LCD_BLINKMODE_OFF;
  LCDHandle.Init.BlinkFrequency = LCD_BLINKFREQUENCY_DIV8;
  /* Initialize the LCD */
  HAL_LCD_Init(&LCDHandle);
}

/**
  * @brief This function writes a char in the LCD frame buffer.
  * @param ch: the character to display.
  * @param Point: a point to add in front of char
  *        This parameter can be: POINT_OFF or POINT_ON
  * @param Column: flag indicating if a column has to be add in front
  *        of displayed character.
  *        This parameter can be: COLUMN_OFF or COLUMN_ON.           
  * @param Position: position in the LCD of the caracter to write [0:7]
  * @retval None
  * @note  Required preconditions: The LCD should be cleared before to start the
  *        write operation.  
  */
void BSP_LCD_GLASS_WriteChar(uint8_t* ch, uint8_t Point, uint8_t Column, uint8_t Position)
{
  uint8_t digit[4];     /* Digit frame buffer */
   
  /* To convert displayed character in segment in array digit */
  LCD_Convert_CharToSeg(ch, Point, Column, digit);
  
  switch(Position)
  {
    /* Position 1 on LCD (Digit1)*/
  case 1:
    HAL_LCD_Write(&LCDHandle, LCD_RAM_REGISTER0, 0xCFFFFFFC, ((digit[0]& 0x0C) << 26) | (digit[0]& 0x03));/* 1G 1B 1M 1E */
    HAL_LCD_Write(&LCDHandle, LCD_RAM_REGISTER2, 0xCFFFFFFC, ((digit[1]& 0x0C) << 26) | (digit[1]& 0x03));/* 1F 1A 1C 1D */
    HAL_LCD_Write(&LCDHandle, LCD_RAM_REGISTER4, 0xCFFFFFFC, ((digit[2]& 0x0C) << 26) | (digit[2]& 0x03));/* 1Q 1K 1Col 1P */
    HAL_LCD_Write(&LCDHandle, LCD_RAM_REGISTER6, 0xCFFFFFFC, ((digit[3]& 0x0C) << 26) | (digit[3]& 0x03));/* 1H 1J 1DP 1N */   
      break;
    
    /* Position 2 on LCD (Digit2)*/
  case 2:
    HAL_LCD_Write(&LCDHandle, LCD_RAM_REGISTER0, 0xF3FFFF03, ((digit[0]& 0x0C) << 24)|((digit[0]& 0x02) << 6)|((digit[0]& 0x01) << 2));/* 2G 2B 2M 2E */
    HAL_LCD_Write(&LCDHandle, LCD_RAM_REGISTER2, 0xF3FFFF03, ((digit[1]& 0x0C) << 24)|((digit[1]& 0x02) << 6)|((digit[1]& 0x01) << 2));/* 2F 2A 2C 2D */
    HAL_LCD_Write(&LCDHandle, LCD_RAM_REGISTER4, 0xF3FFFF03, ((digit[2]& 0x0C) << 24)|((digit[2]& 0x02) << 6)|((digit[2]& 0x01) << 2));/* 2Q 2K 2Col 2P */
    HAL_LCD_Write(&LCDHandle, LCD_RAM_REGISTER6, 0xF3FFFF03, ((digit[3]& 0x0C) << 24)|((digit[3]& 0x02) << 6)|((digit[3]& 0x01) << 2));/* 2H 2J 2DP 2N */    
    break;
    
    /* Position 3 on LCD (Digit3)*/
  case 3:
    HAL_LCD_Write(&LCDHandle, LCD_RAM_REGISTER0, 0xFCFFFCFF, ((digit[0]& 0x0C) << 22) | ((digit[0]& 0x03) << 8));/* 3G 3B 3M 3E */
    HAL_LCD_Write(&LCDHandle, LCD_RAM_REGISTER2, 0xFCFFFCFF, ((digit[1]& 0x0C) << 22) | ((digit[1]& 0x03) << 8));/* 3F 3A 3C 3D */
    HAL_LCD_Write(&LCDHandle, LCD_RAM_REGISTER4, 0xFCFFFCFF, ((digit[2]& 0x0C) << 22) | ((digit[2]& 0x03) << 8));/* 3Q 3K 3Col 3P */
    HAL_LCD_Write(&LCDHandle, LCD_RAM_REGISTER6, 0xFCFFFCFF, ((digit[3]& 0x0C) << 22) | ((digit[3]& 0x03) << 8));/* 3H 3J 3DP 3N */
    break;
    
    /* Position 4 on LCD (Digit4)*/
  case 4:
    HAL_LCD_Write(&LCDHandle, LCD_RAM_REGISTER0, 0xFFCFF3FF, ((digit[0]& 0x0C) << 18) | ((digit[0]& 0x03) << 10));/* 4G 4B 4M 4E */
    HAL_LCD_Write(&LCDHandle, LCD_RAM_REGISTER2, 0xFFCFF3FF, ((digit[1]& 0x0C) << 18) | ((digit[1]& 0x03) << 10));/* 4F 4A 4C 4D */
    HAL_LCD_Write(&LCDHandle, LCD_RAM_REGISTER4, 0xFFCFF3FF, ((digit[2]& 0x0C) << 18) | ((digit[2]& 0x03) << 10));/* 4Q 4K 4Col 4P */
    HAL_LCD_Write(&LCDHandle, LCD_RAM_REGISTER6, 0xFFCFF3FF, ((digit[3]& 0x0C) << 18) | ((digit[3]& 0x03) << 10));/* 4H 4J 4DP 4N */
    break;
    
    /* Position 5 on LCD (Digit5)*/
  case 5:
    HAL_LCD_Write(&LCDHandle, LCD_RAM_REGISTER0, 0xFFF3CFFF, ((digit[0]& 0x0C) << 16) | ((digit[0]& 0x03) << 12));/* 5G 5B 5M 5E */
    HAL_LCD_Write(&LCDHandle, LCD_RAM_REGISTER2, 0xFFF3CFFF, ((digit[1]& 0x0C) << 16) | ((digit[1]& 0x03) << 12));/* 5G 5B 5M 5E */
    HAL_LCD_Write(&LCDHandle, LCD_RAM_REGISTER4, 0xFFF3EFFF, ((digit[2]& 0x0C) << 16) | ((digit[2]& 0x01) << 12));/* 5Q 5K   5P */
    HAL_LCD_Write(&LCDHandle, LCD_RAM_REGISTER6, 0xFFF3EFFF, ((digit[3]& 0x0C) << 16) | ((digit[3]& 0x01) << 12));/* 5H 5J   5N */
    break;
    
    /* Position 6 on LCD (Digit6)*/
  case 6:
    HAL_LCD_Write(&LCDHandle, LCD_RAM_REGISTER0, 0xFFFC3FFF, ((digit[0]& 0x04) << 15) | ((digit[0]& 0x08) << 13) | ((digit[0]& 0x03) << 14));/* 6B 6G 6M 6E */
    HAL_LCD_Write(&LCDHandle, LCD_RAM_REGISTER2, 0xFFFC3FFF, ((digit[1]& 0x04) << 15) | ((digit[1]& 0x08) << 13) | ((digit[1]& 0x03) << 14));/* 6A 6F 6C 6D */
    HAL_LCD_Write(&LCDHandle, LCD_RAM_REGISTER4, 0xFFFC3FFF, ((digit[2]& 0x04) << 15) | ((digit[2]& 0x08) << 13) | ((digit[2]& 0x01) << 14));/* 6K 6Q    6P */
    HAL_LCD_Write(&LCDHandle, LCD_RAM_REGISTER6, 0xFFFC3FFF, ((digit[3]& 0x04) << 15) | ((digit[3]& 0x08) << 13) | ((digit[3]& 0x01) << 14));/* 6J 6H   6N */
    break;
    
  default:
    break;
  }

  /* Update the LCD display */
  HAL_LCD_UpdateDisplayRequest(&LCDHandle);
  
  /* Refresh LCD  bar */
  BSP_LCD_GLASS_BarLevelConfig(LCDBar);
}

/**
  * @brief This function writes a char in the LCD RAM.
  * @param ptr: Pointer to string to display on the LCD Glass.
  * @retval None
  */
void BSP_LCD_GLASS_DisplayString(uint8_t* ptr)
{
  uint8_t i = 1;

  /* Send the string character by character on lCD */
  while ((*ptr != 0) & (i < 8))
  {
    /* Display one character on LCD */
    BSP_LCD_GLASS_WriteChar(ptr, 0, 0, i);

    /* Point on the next character */
    ptr++;

    /* Increment the character counter */
    i++;
  }
}

/**
  * @brief This function writes a char in the LCD RAM.
  * @param ptr: Pointer to string to display on the LCD Glass.
  * @retval None
  * @note Required preconditions: Char is ASCCI value "Ored" with decimal point or Column flag
  */
void BSP_LCD_GLASS_DisplayStrDeci(uint16_t* ptr)
{
  uint8_t index = 1;
  uint8_t tmpchar;

  /* Send the string character by character on lCD */
  while((*ptr != 0) & (index < 8))
  {			
    tmpchar = (*ptr) & 0x00FF;
    
    switch((*ptr) & 0xF000)
    {
    case DOT:
      /* Display one character on LCD with decimal point */
      BSP_LCD_GLASS_WriteChar(&tmpchar, POINT_ON, COLUMN_OFF, index);
      break;
    case DOUBLE_DOT:
      /* Display one character on LCD with decimal point */
      BSP_LCD_GLASS_WriteChar(&tmpchar, POINT_OFF, COLUMN_ON, index);
      break;
    default:
      BSP_LCD_GLASS_WriteChar(&tmpchar, POINT_OFF, COLUMN_OFF, index);		
      break;
    }/* Point on the next character */
    ptr++;
    
    /* Increment the character counter */
    index++;
  }
}

/**
  * @brief This function Clear the whole LCD RAM.
  * @param None
  * @retval None
  */
void BSP_LCD_GLASS_Clear(void)
{
  HAL_LCD_Clear(&LCDHandle); 
}

/**
  * @brief  Display a string in scrolling mode
  * @param  ptr: Pointer to string to display on the LCD Glass.
  * @param  nScroll: Specifies how many time the message will be scrolled
  * @param  ScrollSpeed : Speciifes the speed of the scroll, low value gives
  *         higher speed 
  * @retval None
  * @note    Required preconditions: The LCD should be cleared before to start the
  *         write operation.
  */
void BSP_LCD_GLASS_ScrollSentence(uint8_t* ptr, uint16_t nScroll, uint16_t ScrollSpeed)
{
  uint8_t Repetition;
  uint8_t nbrchar;
  uint8_t* ptr1;
  uint8_t str[7] = "";
  uint8_t sizestr;
  
  if(ptr == 0)
  {
    return;
  }
  /* To calculate end of string */
  for(ptr1 = ptr, sizestr = 0; *ptr1 != 0; sizestr++, ptr1++);
  
  ptr1 = ptr;
  
  BSP_LCD_GLASS_DisplayString(ptr);
  HAL_Delay(ScrollSpeed);
  
  /* To shift the string for scrolling display*/
  for(Repetition = 0; Repetition < nScroll; Repetition++)
  {
    for(nbrchar = 0; nbrchar < sizestr; nbrchar++)
    {
      *(str) =* (ptr1+((nbrchar+1)%sizestr));
      *(str+1) =* (ptr1+((nbrchar+2)%sizestr));
      *(str+2) =* (ptr1+((nbrchar+3)%sizestr));
      *(str+3) =* (ptr1+((nbrchar+4)%sizestr));
      *(str+4) =* (ptr1+((nbrchar+5)%sizestr));
      *(str+5) =* (ptr1+((nbrchar+6)%sizestr));
      BSP_LCD_GLASS_Clear();
      BSP_LCD_GLASS_DisplayString(str);
      
      HAL_Delay(ScrollSpeed);
    }	
  }  
}

/**
  * @brief Setting bar on LCD, writes bar value in LCD frame buffer 
  * @param BarLevel: specifies the LCD GLASS Batery Level.
  *     This parameter can be one of the following values:
  *     @arg BARLEVEL_OFF: LCD GLASS Batery Empty
  *     @arg BATTERYLEVEL_1_4: LCD GLASS Batery 1/4 Full
  *     @arg BATTERYLEVEL_1_2: LCD GLASS Batery 1/2 Full
  *     @arg BATTERYLEVEL_3_4: LCD GLASS Batery 3/4 Full
  *     @arg BATTERYLEVEL_FULL: LCD GLASS Batery Full
  * @note To update the display on the LCD Glass the HAL_LCD_UpdateDisplayRequest() API
  *       must be called after calling this API (BSP_LCD_GLASS_BarLevelConfig()). 
  * @retval None
  */
void BSP_LCD_GLASS_BarLevelConfig(uint8_t BarLevel)
{
  switch (BarLevel)
  {
  case BARLEVEL_OFF:
    /* Write the new values for BAR0 and BAR2 */
    HAL_LCD_Write(&LCDHandle, LCD_RAM_REGISTER6, 0xFFFF5FFF, 0);    
    /* Write the new values for BAR1 and BAR3 */
    HAL_LCD_Write(&LCDHandle, LCD_RAM_REGISTER4, 0xFFFF5FFF, 0);
    LCDBar = BARLEVEL_OFF;
    break;
    
    /* BARLEVEL 1/4 */
  case BARLEVEL_1_4:
    /* Write the new values for BAR0 and BAR2 */
    HAL_LCD_Write(&LCDHandle, LCD_RAM_REGISTER6, 0xFFFF5FFF, (uint32_t)(1 << 15));    
    /* Write the new values for BAR1 and BAR3 */
    HAL_LCD_Write(&LCDHandle, LCD_RAM_REGISTER4, 0xFFFF5FFF, 0);
    LCDBar = BARLEVEL_1_4;
    break;
    
    /* BARLEVEL 1/2 */
  case BARLEVEL_1_2:
    /* Write the new values for BAR0 and BAR2 */
    HAL_LCD_Write(&LCDHandle, LCD_RAM_REGISTER6, 0xFFFF5FFF, (uint32_t)(1 << 15));    
    /* Write the new values for BAR1 and BAR3 */
    HAL_LCD_Write(&LCDHandle, LCD_RAM_REGISTER4, 0xFFFF5FFF, (uint32_t)(1 << 15));
    LCDBar = BARLEVEL_1_2;
    break;
    
    /* Battery Level 3/4 */
  case BARLEVEL_3_4:
    /* Write the new values for BAR0 and BAR2 */
    HAL_LCD_Write(&LCDHandle, LCD_RAM_REGISTER6, 0xFFFF5FFF, (uint32_t)((1 << 13) | (1 << 15)));    
    /* Write the new values for BAR1 and BAR3 */
    HAL_LCD_Write(&LCDHandle, LCD_RAM_REGISTER4, 0xFFFF5FFF, (uint32_t)(1 << 15));
    LCDBar = BARLEVEL_3_4;
    break;
    
    /* BARLEVEL_FULL */
  case BARLEVEL_FULL:
    /* Write the new values for BAR0 and BAR2 */
    HAL_LCD_Write(&LCDHandle, LCD_RAM_REGISTER6, 0xFFFF5FFF, (uint32_t)((1 << 13) | (1 << 15)));    
    /* Write the new values for BAR1 and BAR3 */
    HAL_LCD_Write(&LCDHandle, LCD_RAM_REGISTER4, 0xFFFF5FFF, (uint32_t)((1 << 13) | (1 << 15)));
    LCDBar = BARLEVEL_FULL;
    break;
    
  default:
    break;
  }
  
  /* Update the LCD display */
  HAL_LCD_UpdateDisplayRequest(&LCDHandle);
}

/**
  * @brief  Converts an ascii char to the a LCD digit.
  * @param  c: a char to display.
  * @param  Point: a point to add in front of char
  *         This parameter can be: POINT_OFF or POINT_ON
  * @param  Column : flag indicating if a column has to be add in front
  *         of displayed character.
  *         This parameter can be: COLUMN_OFF or COLUMN_ON.
  * @param  digit: array with segment 
  * @retval None
  */
static void LCD_Convert_CharToSeg(uint8_t* c, uint8_t Point, uint8_t Column, uint8_t* digit)
{
  uint16_t ch = 0 ;
  uint8_t i, j;
  
  switch (*c)
  {
  case ' ' : 
    ch = 0x00;
    break;
    
  case '*':
    ch = STAR;
    break;
    
  case 'µ' :
    ch = C_UMAP;
    break;
    
  case 'm' :
    ch = C_MMAP;
    break;
    
  case 'n' :
    ch = C_NMAP;
    break;					
    
  case '-' :
    ch = C_MINUS;
    break;
    
  case '+' :
    ch = C_PLUS;
    break;
    
  case '/' :
    ch = C_SLATCH;
    break;  
    
  case '°' :
    ch = C_PERCENT_1;
    break;
    
  case '%' :
    ch = C_PERCENT_2; 
    break;
    
  case 255 :
    ch = C_FULL;
    break ;
    
  case '0':
  case '1':
  case '2':
  case '3':
  case '4':
  case '5':
  case '6':
  case '7':
  case '8':
  case '9':			
    ch = NumberMap[*c-0x30];		
    break;
    
  default:
    /* The character c is one letter in upper case*/
    if((*c < 0x5b) && (*c > 0x40))
    {
      ch = CapLetterMap[*c-'A'];
    }
    /* The character c is one letter in lower case*/
    if((*c <0x7b) && (*c> 0x60))
    {
      ch = CapLetterMap[*c-'a'];
    }
    break;
  }
  
  /* Set the digital point can be displayed if the point is on */
  if(Point)
  {
    ch |= 0x0002;
  }
  
  /* Set the "COL" segment in the character that can be displayed if the column is on */
  if(Column)
  {
    ch |= 0x0020;
  }		
  
  for(i = 12, j = 0; j < 4; i-=4, j++)
  {
    digit[j] = (ch >> i) & 0x0F; //To isolate the less signifiant digit
  }
}

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
