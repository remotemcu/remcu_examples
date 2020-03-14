/**
  ******************************************************************************
  * @file    I2C/I2C_TSENSOR/main.c 
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
#include "stm32f10x.h"
#include <stdlib.h>
#include "stm32_eval_i2c_tsensor.h"
#include "stm32_eval.h"

#ifdef USE_STM32100E_EVAL
  #include "stm32100e_eval_lcd.h"
#elif defined USE_STM3210E_EVAL
  #include "stm3210e_eval_lcd.h"
#elif defined USE_STM32100B_EVAL
  #include "stm32100b_eval_lcd.h"
#elif defined USE_STM3210B_EVAL
  #include "stm3210b_eval_lcd.h"
#endif

/** @addtogroup STM32F10x_StdPeriph_Examples
  * @{
  */

/** @addtogroup I2C_TSENSOR
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/* Private define ------------------------------------------------------------*/
#define TEMPERATURE_THYS    31
#define TEMPERATURE_TOS     32

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint8_t TempCelsiusDisplay[]     = "       +abc.d C     ";
static uint8_t TempFahrenheitDisplay[]  = "       +abc.d F     ";
static int32_t TempValue = 0, TempValueCelsius = 0, TempValueFahrenheit = 0;
__IO uint8_t SMbusAlertOccurred = 0; 
uint32_t index = 0;
      
/* Private functions ---------------------------------------------------------*/
void NVIC_Configuration(void);

/**
  * @brief   Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f10x_xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f10x.c file
     */
 
  /* NVIC Configuration */
  NVIC_Configuration();

  /* Initialize the LCD */
#ifdef USE_STM32100E_EVAL
  STM32100E_LCD_Init();
#elif defined USE_STM3210E_EVAL
  STM3210E_LCD_Init();
#elif defined USE_STM32100B_EVAL
  STM32100B_LCD_Init();
#elif defined USE_STM3210B_EVAL
  STM3210B_LCD_Init();
#endif

  
#ifdef USE_STM3210E_EVAL
  /* Disable FSMC only for STM32 High-density and XL-density devices */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, DISABLE);
#endif /* USE_STM3210E_EVAL */
  
  /* Initialize the Temperature Sensor */
  LM75_Init();

  if (LM75_GetStatus() == SUCCESS)
  {    
#ifdef USE_STM3210E_EVAL
    /* Enable FSMC */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
#endif /* USE_STM3210E_EVAL */
    
    /* Clear the LCD */
    LCD_Clear(LCD_COLOR_WHITE);
    
    /* Set the Back Color */
    LCD_SetBackColor(LCD_COLOR_BLUE);
    
    /* Set the Text Color */
    LCD_SetTextColor(LCD_COLOR_GREEN);
    
    LCD_DisplayStringLine(LCD_LINE_0, "     Temperature    ");

#ifdef USE_STM3210E_EVAL        
    /* Disable FSMC */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, DISABLE);

    /* Initialize the Temperature Sensor */
    LM75_Init();
    
#endif /* USE_STM3210E_EVAL */
    
    /* Configure the Temperature sensor device STLM75:
    - Thermostat mode Interrupt
    - Fault tolerance: 00
    */
    LM75_WriteConfReg(0x02);
    
    /* Configure the THYS and TOS in order to use the SMbus alert interrupt */
    LM75_WriteReg(LM75_REG_THYS, TEMPERATURE_THYS << 8);  /*31ÝC*/
    LM75_WriteReg(LM75_REG_TOS, TEMPERATURE_TOS << 8);   /*32ÝC*/
    
    I2C_ClearITPendingBit(LM75_I2C, I2C_IT_SMBALERT);
    
    SMbusAlertOccurred = 0;
    
    /* Infinite Loop */
    while (1)
    {
#ifdef USE_STM3210E_EVAL        
    /* Disable FSMC */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, DISABLE);

    /* Initialize the Temperature Sensor */
    LM75_Init();
#endif /* USE_STM3210E_EVAL */
      
      /* Get double of Temperature value */
      TempValue = LM75_ReadTemp();

#ifdef USE_STM3210E_EVAL          
      /* Enable FSMC */
      RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
#endif /* USE_STM3210E_EVAL */
      
      if (TempValue <= 256)
      {
        /* Positive temperature measured */
        TempCelsiusDisplay[7] = '+';
        
        /* Initialize the temperature sensor value*/
        TempValueCelsius = TempValue;
      }
      else
      {
        /* Negative temperature measured */
        TempCelsiusDisplay[7] = '-';
        /* Remove temperature value sign */
        TempValueCelsius = 0x200 - TempValue;
      }
      
      /* Calculate temperature digits in ÝC */
      if ((TempValueCelsius & 0x01) == 0x01)
      {
        TempCelsiusDisplay[12] = 0x05 + 0x30;
        TempFahrenheitDisplay[12] = 0x05 + 0x30;
      }
      else
      {
        TempCelsiusDisplay[12] = 0x00 + 0x30;
        TempFahrenheitDisplay[12] = 0x00 + 0x30;
      }
      
      TempValueCelsius >>= 1;
      
      TempCelsiusDisplay[8] = (TempValueCelsius / 100) + 0x30;
      TempCelsiusDisplay[9] = ((TempValueCelsius % 100) / 10) + 0x30;
      TempCelsiusDisplay[10] = ((TempValueCelsius % 100) % 10) + 0x30;
      
      if (TempValue > 256)
      {
        if (((9 * TempValueCelsius) / 5) <= 32)
        {
          /* Convert temperature ÝC to Fahrenheit */
          TempValueFahrenheit = abs (32 - ((9 * TempValueCelsius) / 5));
          
          /* Calculate temperature digits in ÝF */
          TempFahrenheitDisplay[8] = (TempValueFahrenheit / 100) + 0x30;
          TempFahrenheitDisplay[9] = ((TempValueFahrenheit % 100) / 10) + 0x30;
          TempFahrenheitDisplay[10] = ((TempValueFahrenheit % 100) % 10) + 0x30;
          /* Positive temperature measured */
          TempFahrenheitDisplay[7] = '+';
        }
        else
        {
          /* Convert temperature ÝC to Fahrenheit */
          TempValueFahrenheit = abs(((9 * TempValueCelsius) / 5) - 32);
          /* Calculate temperature digits in ÝF */
          TempFahrenheitDisplay[8] = (TempValueFahrenheit / 100) + 0x30;
          TempFahrenheitDisplay[9] = ((TempValueFahrenheit % 100) / 10) + 0x30;
          TempFahrenheitDisplay[10] = ((TempValueFahrenheit % 100) % 10) + 0x30;
          
          /* Negative temperature measured */
          TempFahrenheitDisplay[7] = '-';
        }
      }
      else
      {
        /* Convert temperature ÝC to Fahrenheit */
        TempValueFahrenheit = ((9 * TempValueCelsius) / 5) + 32;
        
        /* Calculate temperature digits in ÝF */
        TempFahrenheitDisplay[8] = (TempValueFahrenheit / 100) + 0x30;
        TempFahrenheitDisplay[9] = ((TempValueFahrenheit % 100) / 10) + 0x30;
        TempFahrenheitDisplay[10] = ((TempValueFahrenheit % 100) % 10) + 0x30;
        
        /* Positive temperature measured */
        TempFahrenheitDisplay[7] = '+';
      }
      
      /* Display Fahrenheit value on LCD */
      for (index = 0; index < 20; index++)
      {
        LCD_DisplayChar(LCD_LINE_6, (319 - (16 * index)), TempCelsiusDisplay[index]);
        
        LCD_DisplayChar(LCD_LINE_7, (319 - (16 * index)), TempFahrenheitDisplay[index]);
      }
      
      if (SMbusAlertOccurred == 1)
      {
        /* Set the Back Color */
        LCD_SetBackColor(LCD_COLOR_BLUE);
        /* Set the Text Color */
        LCD_SetTextColor(LCD_COLOR_RED);
        LCD_DisplayStringLine(LCD_LINE_2, "Warning: Temp exceed");
        LCD_DisplayStringLine(LCD_LINE_3, "        32 C        ");
      }
      if (SMbusAlertOccurred == 2)
      {
        /* Set the Back Color */
        LCD_SetBackColor(LCD_COLOR_WHITE);
        /* Set the Text Color */
        LCD_SetTextColor(LCD_COLOR_WHITE);
        LCD_ClearLine(LCD_LINE_2);
        LCD_ClearLine(LCD_LINE_3);
        SMbusAlertOccurred = 0;
        /* Set the Back Color */
        LCD_SetBackColor(LCD_COLOR_BLUE);
        /* Set the Text Color */
        LCD_SetTextColor(LCD_COLOR_GREEN);
      }
    }
  }
  else
  {
    LCD_Clear(LCD_COLOR_WHITE);
    LCD_DisplayStringLine(LCD_LINE_2, " LM75 not correctly ");
    LCD_DisplayStringLine(LCD_LINE_3, " initialized...     ");
    LCD_DisplayStringLine(LCD_LINE_4, " Please restart the ");
    LCD_DisplayStringLine(LCD_LINE_5, " example.           ");   
    /* Infinite Loop */
    while(1)
    {
    }
  }
}

/**
  * @brief  Configures the different interrupt.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Re-configure and enable I2C2 error interrupt to have the higher priority */
#ifdef USE_STM32100E_EVAL
  NVIC_InitStructure.NVIC_IRQChannel = I2C2_ER_IRQn;
#else
  NVIC_InitStructure.NVIC_IRQChannel = I2C1_ER_IRQn;
#endif
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
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

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
