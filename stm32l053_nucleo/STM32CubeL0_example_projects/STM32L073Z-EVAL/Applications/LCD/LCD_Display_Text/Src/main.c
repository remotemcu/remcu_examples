/**
  ******************************************************************************
  * @file    LCD/LCD_Display_Text/Src/main.c 
  * @author  MCD Application Team
  * @brief   This example describes how to configure and use GPIOs through 
  *          the STM32L0xx HAL API.
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

/** @addtogroup LCD_Blink_Frequency
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern LCD_HandleTypeDef LCDHandle;
BatteryLevel_TypeDef BatteryLevel_Tab;
uint16_t TextValue[7];
/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void Convert_Into_Char(uint32_t number, uint16_t *p_tab);
void Print_Smiley_LCD(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* This sample code shows how to use STM32L0xx LCD HAL API to configures the LCD Blink
     mode and Blink frequency.*/  

  /* STM32L0xx HAL library initialization:
       - Configure the Flash prefetch
       - Systick timer is configured by default as source of time base, but user 
         can eventually implement his proper time base source (a general purpose 
         timer for example or other time source), keeping in mind that Time base 
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
         handled in milliseconds basis.
       - Low Level Initialization
     */
  HAL_Init();

  /* Configure the system clock to 32 MHz */
  SystemClock_Config();

  /* Configure the Wkup/Tamper push-button */
  BSP_PB_Init(BUTTON_TAMPER, BUTTON_MODE_EXTI);
  
  /* Configure the Joystick */
  BSP_JOY_Init(JOY_MODE_EXTI);
 
  /* Initialize LED3 */
  BSP_LED_Init(LED3);  
  /* Initialize LED1 */
  BSP_LED_Init(LED1);
  /* Initialize LED2 */
  BSP_LED_Init(LED2);
  /* Initialize LED4 */
  BSP_LED_Init(LED4);

  /* LCD GLASS Initialization */
  BSP_LCD_GLASS_Init();

  /*----------------------------*/  
  BSP_LCD_GLASS_Clear();
  BSP_LCD_GLASS_BarLevelConfig(BATTERYLEVEL_OFF); 
  
  /* Display string on LCD glass*/
  BSP_LCD_GLASS_DisplayString((uint8_t*)"STM32L0\n\r");

  BSP_LED_On(LED1);

  HAL_Delay(1000);
  
  BSP_LCD_GLASS_Clear();

  BSP_LED_Off(LED1);
  /*----------------------------*/
  BSP_LCD_GLASS_Clear();
  BSP_LCD_GLASS_BarLevelConfig(BATTERYLEVEL_OFF); 

  /* Configure all LCD segments and Coms to blink at frequency LCDCLK/128 */
  __HAL_LCD_BLINK_CONFIG(&LCDHandle, LCD_BLINKMODE_ALLSEG_ALLCOM, LCD_BLINKFREQUENCY_DIV128);
  
  /* Display the "BK0128" message on the LCD GLASS */
  BSP_LCD_GLASS_DisplayString((uint8_t*)"BLK 128");
	
  BSP_LED_On(LED2);
  
  HAL_Delay(2000);
  
  __HAL_LCD_BLINK_CONFIG(&LCDHandle, LCD_BLINKMODE_OFF, LCD_BLINKFREQUENCY_DIV128); 
  
  BSP_LED_Off(LED2);
  
  /*----------------------------*/  
  BSP_LCD_GLASS_Clear();  

  /* Scroll sentence on LCD glass*/
  BSP_LCD_GLASS_ScrollSentence((uint8_t*)"SCROLLING SENTENCE ", 1, SCROLL_SPEED);
  HAL_Delay(2000);
  
  /*----------------------------*/

  BSP_LCD_GLASS_Clear();
	
  BSP_LED_On(LED2);

  HAL_Delay(1000);
  
  BSP_LCD_GLASS_BarLevelConfig(BATTERYLEVEL_OFF);  HAL_Delay(400);
  BSP_LCD_GLASS_BarLevelConfig(BATTERYLEVEL_1_4);  HAL_Delay(400);
  BSP_LCD_GLASS_BarLevelConfig(BATTERYLEVEL_1_2);  HAL_Delay(400);
  BSP_LCD_GLASS_BarLevelConfig(BATTERYLEVEL_3_4);  HAL_Delay(400);
  BSP_LCD_GLASS_BarLevelConfig(BATTERYLEVEL_FULL); HAL_Delay(400);

  BSP_LED_Off(LED2);
  /*----------------------------*/
  
  /*Display Voltage 3500 uA on LCD glass*/
  Convert_Into_Char(3500, TextValue);
 
  /* Test the significant digit for displays 3 or 4 digits*/
  if (TextValue[0] != '0')
  {
    TextValue[1] |= DOT; /* To add decimal point */
  }  
  else
  {
    /* To shift for suppress '0' before decimal */
    TextValue[0] = TextValue[1] ;	
    TextValue[0] |= DOUBLE_DOT;  /*DOT ;*/
    TextValue[1] = TextValue[2] ;
    TextValue[2] = TextValue[3] ;		
    TextValue[3] = TextValue[4] ;/*' ';*/
  }
  /*add units:uA*/
  TextValue[4] = ' ';
  TextValue[5] = 'U';  /*'µ';*/
  TextValue[6] = 'A';
		
  BSP_LCD_GLASS_DisplayStrDeci(TextValue);
  HAL_Delay(2000);
  
   BSP_LCD_GLASS_DisplayLogo(ENABLE); /*ST logo*/
  HAL_Delay(1000);
  
  BSP_LCD_GLASS_ArrowConfig(ARROWDIRECTION_OFF); HAL_Delay(200);
  BSP_LCD_GLASS_ArrowConfig(ARROWDIRECTION_DOWN); HAL_Delay(200);
  BSP_LCD_GLASS_ArrowConfig(ARROWDIRECTION_LEFT); HAL_Delay(200);
  BSP_LCD_GLASS_ArrowConfig(ARROWDIRECTION_UP); HAL_Delay(200);
  BSP_LCD_GLASS_ArrowConfig(ARROWDIRECTION_RIGHT); HAL_Delay(200);      
  HAL_Delay(1000);
  
  BSP_LCD_GLASS_WriteMatrixPixel(PIXELROW_5, PIXELCOLUMN_10);/*UL:10*19*/
  HAL_Delay(1000);
  
  BSP_LCD_GLASS_TemperatureConfig(TEMPERATURELEVEL_OFF); HAL_Delay(100); 
  BSP_LCD_GLASS_TemperatureConfig(TEMPERATURELEVEL_1); HAL_Delay(100);  
  BSP_LCD_GLASS_TemperatureConfig(TEMPERATURELEVEL_2); HAL_Delay(100); 
  BSP_LCD_GLASS_TemperatureConfig(TEMPERATURELEVEL_3); HAL_Delay(100); 
  BSP_LCD_GLASS_TemperatureConfig(TEMPERATURELEVEL_4); HAL_Delay(100); 
  BSP_LCD_GLASS_TemperatureConfig(TEMPERATURELEVEL_5); HAL_Delay(100);  
  BSP_LCD_GLASS_TemperatureConfig(TEMPERATURELEVEL_6); HAL_Delay(100);   
  HAL_Delay(1000);
  
  BSP_LCD_GLASS_ValueUnitConfig(VALUEUNIT_OFF);  HAL_Delay(100);  
  BSP_LCD_GLASS_ValueUnitConfig(VALUEUNIT_MILLIAMPERE); HAL_Delay(100); 
  BSP_LCD_GLASS_ValueUnitConfig(VALUEUNIT_MICROAMPERE); HAL_Delay(100); 
  BSP_LCD_GLASS_ValueUnitConfig(VALUEUNIT_NANOAMPERE); HAL_Delay(100); 
  HAL_Delay(1000);
 
  BSP_LCD_GLASS_SignCmd(SIGN_NEGATIVE, ENABLE); HAL_Delay(100); 
  BSP_LCD_GLASS_SignCmd(SIGN_NEGATIVE, DISABLE); HAL_Delay(100); 
  BSP_LCD_GLASS_SignCmd(SIGN_NEGATIVE, ENABLE); HAL_Delay(100); 
  BSP_LCD_GLASS_SignCmd(SIGN_POSITIVE, ENABLE); HAL_Delay(100); 
  HAL_Delay(1000);
  
  BSP_LCD_GLASS_ClearMatrixPixel(PIXELROW_5, PIXELCOLUMN_10);/*UL:10*19*/
  HAL_Delay(1000);
  
  Print_Smiley_LCD();
  
  
  BSP_LED_On(LED1);
  
  HAL_Delay(1000);

  BSP_LED_Off(LED1);

  BSP_LCD_GLASS_Clear();
  HAL_Delay(100);
  BSP_LCD_GLASS_BarLevelConfig(BATTERYLEVEL_OFF); 
  /*----------------------------*/
  HAL_Delay(100);

 /*Display " END " on LCD glass*/
  BSP_LCD_GLASS_DisplayString((uint8_t*)"THE END\n\r");
  /*----------------------------*/
	
  /* Infinite loop */
  while (1)
  {
    HAL_Delay(100);
    BSP_LED_Toggle(LED4);
  }
}

/**
  * @brief  Set Matrix Pixel on.
  * @param  PixelRow: Specify Matrix Row.
  * @param  PixelColumn: Specify Matrix Column.
  * @retval None
  */
void Print_Smiley_LCD(void)
{
    /*clear all->loop on 10*19*/
    BSP_LCD_GLASS_ClearMatrixPixel(PIXELROW_5, PIXELCOLUMN_10);
	
    /*draw smiley on LCD*/
    BSP_LCD_GLASS_WriteMatrixPixel(PIXELROW_2, PIXELCOLUMN_9);
    BSP_LCD_GLASS_WriteMatrixPixel(PIXELROW_2, PIXELCOLUMN_10);
    BSP_LCD_GLASS_WriteMatrixPixel(PIXELROW_2, PIXELCOLUMN_11);
    
    BSP_LCD_GLASS_WriteMatrixPixel(PIXELROW_3, PIXELCOLUMN_8);
    BSP_LCD_GLASS_WriteMatrixPixel(PIXELROW_3, PIXELCOLUMN_12);
    
    BSP_LCD_GLASS_WriteMatrixPixel(PIXELROW_4, PIXELCOLUMN_7);
    BSP_LCD_GLASS_WriteMatrixPixel(PIXELROW_5, PIXELCOLUMN_7);
    BSP_LCD_GLASS_WriteMatrixPixel(PIXELROW_6, PIXELCOLUMN_7);    
    BSP_LCD_GLASS_WriteMatrixPixel(PIXELROW_7, PIXELCOLUMN_7);
    
    BSP_LCD_GLASS_WriteMatrixPixel(PIXELROW_4, PIXELCOLUMN_13);
    BSP_LCD_GLASS_WriteMatrixPixel(PIXELROW_5, PIXELCOLUMN_13);
    BSP_LCD_GLASS_WriteMatrixPixel(PIXELROW_6, PIXELCOLUMN_13);    
    BSP_LCD_GLASS_WriteMatrixPixel(PIXELROW_7, PIXELCOLUMN_13);
    
    BSP_LCD_GLASS_WriteMatrixPixel(PIXELROW_8, PIXELCOLUMN_8);
    BSP_LCD_GLASS_WriteMatrixPixel(PIXELROW_8, PIXELCOLUMN_12);
    
    BSP_LCD_GLASS_WriteMatrixPixel(PIXELROW_9, PIXELCOLUMN_9);
    BSP_LCD_GLASS_WriteMatrixPixel(PIXELROW_9, PIXELCOLUMN_10);
    BSP_LCD_GLASS_WriteMatrixPixel(PIXELROW_9, PIXELCOLUMN_11);
    
    BSP_LCD_GLASS_WriteMatrixPixel(PIXELROW_4, PIXELCOLUMN_9);
    BSP_LCD_GLASS_WriteMatrixPixel(PIXELROW_4, PIXELCOLUMN_11);

    BSP_LCD_GLASS_WriteMatrixPixel(PIXELROW_6, PIXELCOLUMN_9);
    BSP_LCD_GLASS_WriteMatrixPixel(PIXELROW_6, PIXELCOLUMN_11);  
    BSP_LCD_GLASS_WriteMatrixPixel(PIXELROW_7, PIXELCOLUMN_9);
    BSP_LCD_GLASS_WriteMatrixPixel(PIXELROW_7, PIXELCOLUMN_10); 
    BSP_LCD_GLASS_WriteMatrixPixel(PIXELROW_7, PIXELCOLUMN_11);

}


/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 32000000
  *            HCLK(Hz)                       = 32000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            Flash Latency(WS)              = 1
  *            Main regulator output voltage  = Scale1 mode
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct ={0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  
  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  
  /* Enable HSE Oscillator */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLSource   = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLState    = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLMUL      = RCC_PLL_MUL8;
  RCC_OscInitStruct.PLL.PLLDIV      = RCC_PLL_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct)!= HAL_OK)
  {
    /* Initialization Error */
    while(1); 
  }
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1)!= HAL_OK)
  {
    /* Initialization Error */
    while(1); 
  }
}


/**
  * @brief EXTI line detection callbacks.
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
 void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
 {
   if(GPIO_Pin == TAMPER_BUTTON_PIN)
   {
	 while (BSP_PB_GetState(BUTTON_TAMPER) == GPIO_PIN_SET)
	 { 
		__HAL_GPIO_EXTI_CLEAR_IT(TAMPER_BUTTON_PIN);
	 } 
	} 
 
 }


 /**
  * @brief converts a 32bit unsigned int into ASCII 
  * @caller several callers for display values
  * @param Number digit to displays
  *  p_tab values in array in ASCII   
  * @retval None
  */ 
static void Convert_Into_Char(uint32_t number, uint16_t *p_tab)
{
  uint16_t units=0, tens=0, hundreds=0, thousands=0, misc=0;
  
  units = (((number%10000)%1000)%100)%10;
  tens = ((((number-units)/10)%1000)%100)%10;
  hundreds = (((number-tens-units)/100))%100%10;
  thousands = ((number-hundreds-tens-units)/1000)%10;
  misc = ((number-thousands-hundreds-tens-units)/10000);
  
  *(p_tab+4) = units + 0x30;
  *(p_tab+3) = tens + 0x30;
  *(p_tab+2) = hundreds + 0x30;
  *(p_tab+1) = thousands + 0x30;
  *(p_tab) = misc + 0x30;

}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
