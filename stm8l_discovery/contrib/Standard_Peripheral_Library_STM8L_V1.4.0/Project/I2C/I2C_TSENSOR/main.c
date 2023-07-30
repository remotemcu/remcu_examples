/**
  ******************************************************************************
  * @file    I2C/I2C_TSENSOR/main.c
  * @author  MCD Application Team
  * @version V1.5.2
  * @date    30-September-2014
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
#include "stm8l15x.h"
#include "stm8_eval_i2c_tsensor.h"
#include "stm8_eval_lcd.h"

/** @addtogroup STM8L15x_StdPeriph_Examples
  * @{
  */

/**
  * @addtogroup I2C_TSENSOR
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint8_t TempCelsiusDisplay[] = "   +ab.c C     ";
__IO uint32_t TempDecimal = 0;
__IO uint16_t TempCelsiusValue = 0;

/* Private function prototypes -----------------------------------------------*/
static void CLK_Config(void);
void Delay(uint32_t nCount);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void main(void)
{
  uint8_t i = 0;

   /* CLK configuration -------------------------------------------*/
  CLK_Config(); 
  
  /* Initialize dot matrix LCD */
  STM8_EVAL_LCD_Init();

  /* Clear LCD */
  LCD_Clear();

  /* Configure RTC */ 
  RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_16bits);
  RTC_ITConfig(RTC_IT_WUT, ENABLE);

  enableInterrupts();

  /* Initialize the Temperature Sensor */
  LM75_Init();

  while (LM75_GetStatus() != SUCCESS);

  while (1)
  {
    LCD_Clear(); /* Clear the LCD */
    LCD_SetCursorPos(LCD_LINE1, 0);
    LCD_Print("  EVAL Temp.   ");

    /* Get double of Temperature value */
    TempCelsiusValue = LM75_ReadTemp();

    if (TempCelsiusValue <= 256)
    {
      /* Positive temperature measured */
      TempCelsiusDisplay[3] = '+';
    }
    else
    {
      /* Negative temperature measured */
      TempCelsiusDisplay[3] = '-';
      /* Remove temperature value sign */
      TempCelsiusValue = 0x200 - TempCelsiusValue;
    }

    /* Calculate temperature digits in °C */
    if ((TempCelsiusValue & 0x01) == 0x01)
    {
      TempCelsiusDisplay[7] = 0x05 + 0x30;
    }
    else
    {
      TempCelsiusDisplay[7] = 0x00 + 0x30;
    }
    TempCelsiusValue >>= 1;

    TempCelsiusDisplay[4] = (uint8_t)(((TempCelsiusValue % 100) / 10) + 0x30);
    TempCelsiusDisplay[5] = (uint8_t)(((TempCelsiusValue % 100) % 10) + 0x30);

    LCD_SetCursorPos(LCD_LINE2, 0);
    LCD_Print((uint8_t*)TempCelsiusDisplay);

    for (i = 0;i < 250;i++)
    {
      Delay(0x7FFF);
    }

    LCD_SetCursorPos(LCD_LINE1, 0);
    LCD_Print(" CPU in Active");

    LCD_SetCursorPos(LCD_LINE2, 0);
    LCD_Print("   Halt mode");

    /* RTC will wake-up from halt every 10second */
    RTC_SetWakeUpCounter(10);
    RTC_WakeUpCmd(ENABLE);

    /*CPU in Active Halt mode */
    halt();

    RTC_WakeUpCmd(DISABLE);
  }
}

/**
  * @brief  Configure peripheral clock 
  * @param  None
  * @retval None
  */
static void CLK_Config(void)
{
  /* Select HSE as system clock source */
  CLK_SYSCLKSourceSwitchCmd(ENABLE);
  CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSE);
  /* High speed external clock prescaler: 1*/
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);
  while (CLK_GetSYSCLKSource() != CLK_SYSCLKSource_HSE)
  {}

 /* Enable I2C1 clock */
  CLK_PeripheralClockConfig(CLK_Peripheral_I2C1, ENABLE);

  /* Configures RTC clock */
  CLK_RTCClockConfig(CLK_RTCCLKSource_LSE, CLK_RTCCLKDiv_1);
  CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE);
}

/**
  * @brief  Delay.
  * @param  nCount
  * @retval None
  */
void Delay(uint32_t nCount)
{
  /* Decrement nCount value */
  while (nCount != 0)
  {
    nCount--;
  }
}
#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
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
  {}
}
#endif

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
