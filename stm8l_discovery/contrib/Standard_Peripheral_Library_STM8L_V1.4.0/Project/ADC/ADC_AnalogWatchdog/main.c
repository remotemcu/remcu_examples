/**
  ******************************************************************************
  * @file    ADC/ADC_AnalogWatchdog/main.c
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
#include "stm8_eval.h"
#include "stm8_eval_lcd.h"

/** @addtogroup STM8L15x_StdPeriph_Examples
  * @{
  */

/** @addtogroup ADC_AnalogWatchdog
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define HIGH_THRESHOLD_VOLTAGE ((uint16_t)2500) /* 2.500 V */
#define LOW_THRESHOLD_VOLTAGE  ((uint16_t)1500) /* 1.500 V */
#define ADC_RATIO              ((uint16_t) 806) /*ADC_RATIO = ( 3.3 * 1000 * 1000)/4095 */
#define ASCII_NUM_0            ((uint8_t)   48)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint16_t ADCData = 0;
uint16_t ADCSavedData = 0;
uint16_t HighThresholdData = 0;
uint16_t LowThresholdData = 0;
uint8_t  LCDString[16] = "   0.00 Volt    ";

/* Private function prototypes -----------------------------------------------*/
static void ADC_Config(void);
void ShowVoltage(uint16_t Voltage);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void main(void)
{
  uint16_t voltage = 0;

  /* Init the Eval board LCD */
  STM8_EVAL_LCD_Init();

  /* Clear LCD */
  LCD_Clear();

  /* Print "RV ADC Voltage" on LCD line1*/
  LCD_SetCursorPos(LCD_LINE1, 0);
  LCD_Print("RV ADC Voltage");

   /* ADC configuration -------------------------------------------*/
  ADC_Config(); 

  /* Init Leds */
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);

  /* Infinite loop*/
  while (1)
  {
    ADCSavedData = ADCData;
    while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
    ADCData = ADC_GetConversionValue(ADC1);

    if (ADCSavedData != ADCData)
    {
      /* Calculate voltage value*/
      voltage = (uint16_t)(((uint32_t)ADCData * (uint32_t)ADC_RATIO) / (uint32_t)1000);

      /* Display voltage value on LCD*/
      ShowVoltage(voltage);
      STM_EVAL_LEDOff(LED1);

      /* LED4 is On only if ADC converted data is higher
         than High Analog watchdog Threshold */
      if (ADCData >= HighThresholdData)
      {
        STM_EVAL_LEDOn(LED4);
        LCDString[14] = '<';
      }
      else
      {
        STM_EVAL_LEDOff(LED4);
        LCDString[14] = ' ';
      }

      /* LED3 is On only if ADC converted data is lower
        than Low Analog watchdog Threshold */
      if (ADCData <= LowThresholdData)
      {
        STM_EVAL_LEDOn(LED3);
        LCDString[0] = '>';
      }
      else
      {
        STM_EVAL_LEDOff(LED3);
        LCDString[0] = ' ';
      }
    }
  }

}

/**
  * @brief  Configure ADC and Analog watchdog
  * @param  None
  * @retval None
  */
static void ADC_Config(void)
{
  /* Enable ADC1 clock */
  CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, ENABLE);
  
  /* Initialise and configure ADC1 */
  ADC_Init(ADC1, ADC_ConversionMode_Continuous, ADC_Resolution_12Bit, ADC_Prescaler_2);
  ADC_SamplingTimeConfig(ADC1, ADC_Group_SlowChannels, ADC_SamplingTime_384Cycles);

  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);

  /* Enable ADC1 Channel 3 */
  ADC_ChannelCmd(ADC1, ADC_Channel_3, ENABLE);

  /* Calculate Threshold data value*/
  HighThresholdData = (uint16_t)(((uint32_t)HIGH_THRESHOLD_VOLTAGE * 1000) / (uint32_t)ADC_RATIO) ;
  LowThresholdData  = (uint16_t)(((uint32_t)LOW_THRESHOLD_VOLTAGE * 1000) / (uint32_t)ADC_RATIO) ;

  /* Configure Analog Watchdog selected channel and Thresholds */
  ADC_AnalogWatchdogConfig(ADC1, ADC_AnalogWatchdogSelection_Channel3,
                           HighThresholdData,
                           LowThresholdData);

  /* Enable Analog watchdog ADC1 Interrupt */
  ADC_ITConfig(ADC1, ADC_IT_AWD, ENABLE);

  /* Enable Interrupts */
  enableInterrupts();

  /* Start ADC1 Conversion using Software trigger*/
  ADC_SoftwareStartConv(ADC1);
}

/**
  * @brief  Display the current RV voltage on the LCD.
  * @param  voltage: specifies the voltage to display.
  * @retval None
  */
void ShowVoltage(uint16_t Voltage)
{
  uint8_t voltage1000 = 0;
  uint8_t voltage100 = 0;
  uint8_t voltage10 = 0;


  /* Thousands voltage value*/
  voltage1000 = (uint8_t)(Voltage / 1000);
  /* Hundreds voltage value */
  voltage100 = (uint8_t)((Voltage % 1000) / 100);
  /* Tens voltage value */
  voltage10 = (uint8_t)((Voltage % 100 ) / 10);


  /* Fill the LCDString fields with the current Voltage */
  LCDString[3] = (uint8_t)((uint8_t)(voltage1000) + ASCII_NUM_0);
  LCDString[5] = (uint8_t)((uint8_t)(voltage100) + ASCII_NUM_0);
  LCDString[6] = (uint8_t)((uint8_t)(voltage10) + ASCII_NUM_0);

  /* Print the RV Voltage  on the LCD line2 */
  /* Print "RV ADC Voltage" on LCD line1*/
  LCD_SetCursorPos(LCD_LINE2, 0);
  LCD_Print(LCDString);

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
  {
    LCD_SetCursorPos(LCD_LINE1, 0);
    LCD_Print("      ERR       ");
    LCD_SetCursorPos(LCD_LINE2, 0);
    LCD_Print("     ASSERT     ");

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
