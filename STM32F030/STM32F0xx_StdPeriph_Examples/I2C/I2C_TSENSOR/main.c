/**
  ******************************************************************************
  * @file    I2C/I2C_TSENSOR/main.c 
  * @author  MCD Application Team
  * @version V1.4.0
  * @date    24-July-2014
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32F0xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup I2C_TSENSOR
  * @{
  */
  
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define TEMPERATURE_THYS     31
#define TEMPERATURE_TOS      32

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint8_t TempCelsiusDisplay[]     = "       +abc.d C     ";
static uint8_t TempFahrenheitDisplay[]  = "       +abc.d F     ";
static int32_t TempValue = 0, TempValueCelsius = 0, TempValueFahrenheit = 0;
__IO uint8_t SMbusAlertOccurred = 0;

/* Private function prototypes -----------------------------------------------*/
static void NVIC_Config(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
  this is done through SystemInit() function which is called from startup
  file (startup_stm32f0xx.s) before to branch to application main.
  To reconfigure the default setting of SystemInit() function, refer to
  system_stm32f0xx.c file
  */
  
  uint32_t index = 0;
  
  /* NVIC Configuration */
  NVIC_Config();
  
  /* Initialize the LCD */
#ifdef USE_STM320518_EVAL
  STM320518_LCD_Init();
#else 
  STM32072B_LCD_Init();
#endif /* USE_STM320518_EVAL */
  
  /* Initialize the Temperature Sensor */
  LM75_Init();
  
  if (LM75_GetStatus() == SUCCESS)
  {
    /* Clear the LCD */
    LCD_Clear(LCD_COLOR_WHITE);
    
    /* Set the Back Color */
    LCD_SetBackColor(LCD_COLOR_BLUE);
    /* Set the Text Color */
    LCD_SetTextColor(LCD_COLOR_GREEN);
    
    LCD_DisplayStringLine(LCD_LINE_0, "     Temperature    ");
    
    /* Configure the Temperature sensor device STLM75:
    - Thermostat mode Interrupt
    - Fault tolerance: 00
    */
    LM75_WriteConfReg(0x02);
    
    /* Configure the THYS and TOS in order to use the SMbus alert interrupt */
    LM75_WriteReg(LM75_REG_THYS, TEMPERATURE_THYS << 8);  /*31°C*/
    LM75_WriteReg(LM75_REG_TOS, TEMPERATURE_TOS << 8);   /*32°C*/
    
    /* Enables the I2C SMBus Alert feature */
    I2C_SMBusAlertCmd(LM75_I2C, ENABLE);    
    I2C_ClearFlag(LM75_I2C, I2C_FLAG_ALERT);
    
    SMbusAlertOccurred = 0;
    
    /* Enable SMBus Alert interrupt */
    I2C_ITConfig(LM75_I2C, I2C_IT_ERRI, ENABLE);
    
    /* Infinite Loop */
    while (1)
    {
      /* Get double of Temperature value */
      TempValue = LM75_ReadTemp();
      
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
      
      /* Calculate temperature digits in °C */
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
          /* Convert temperature °C to Fahrenheit */
          TempValueFahrenheit = abs (32 - ((9 * TempValueCelsius) / 5));          
          /* Calculate temperature digits in °F */
          TempFahrenheitDisplay[8] = (TempValueFahrenheit / 100) + 0x30;
          TempFahrenheitDisplay[9] = ((TempValueFahrenheit % 100) / 10) + 0x30;
          TempFahrenheitDisplay[10] = ((TempValueFahrenheit % 100) % 10) + 0x30;          
          /* Positive temperature measured */
          TempFahrenheitDisplay[7] = '+';
        }
        else
        {
          /* Convert temperature °C to Fahrenheit */
          TempValueFahrenheit = abs(((9 * TempValueCelsius) / 5) - 32);
          /* Calculate temperature digits in °F */
          TempFahrenheitDisplay[8] = (TempValueFahrenheit / 100) + 0x30;
          TempFahrenheitDisplay[9] = ((TempValueFahrenheit % 100) / 10) + 0x30;
          TempFahrenheitDisplay[10] = ((TempValueFahrenheit % 100) % 10) + 0x30;          
          /* Negative temperature measured */
          TempFahrenheitDisplay[7] = '-';
        }
      }
      else
      {
        /* Convert temperature °C to Fahrenheit */
        TempValueFahrenheit = ((9 * TempValueCelsius) / 5) + 32;
        
        /* Calculate temperature digits in °F */
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
static void NVIC_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Reconfigure and enable I2C1 error interrupt to have the higher priority */
  NVIC_InitStructure.NVIC_IRQChannel = I2C1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
