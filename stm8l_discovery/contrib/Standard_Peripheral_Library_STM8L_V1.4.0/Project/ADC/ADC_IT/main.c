/**
  ******************************************************************************
  * @file    ADC/ADC_IT/main.c
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
#include "timing_delay.h"

/** @addtogroup STM8L15x_StdPeriph_Examples
  * @{
  */

/** @addtogroup ADC_IT
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define ADC_RATIO              ((uint16_t) 806) /*ADC_RATIO = ( 3.3 * 1000 * 1000)/4095 */
#define ASCII_NUM_0            ((uint8_t)   48)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint16_t ADCdata = 0;
uint16_t PotVoltage = 0;
uint8_t LCDString[16] = "   0.00 Volt    ";

/* Private function prototypes -----------------------------------------------*/
static void ADC_Config(void);
static void CLK_Config(void);
void ShowVoltage(uint16_t Voltage);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void main(void)
{
   /* CLK configuration -------------------------------------------*/
  CLK_Config(); 

  /* Init TIM2 to generate 1 ms time base update interrupt */
  TimingDelay_Init();

  /* Enable Interrupts */
  enableInterrupts();

  /* 1s delay to ensure proper LCD Init*/
  Delay(1000);

  /* Init the Eval board LCD */
  STM8_EVAL_LCD_Init();

  /* Clear LCD */
  LCD_Clear();

  /* print "Pot ADC Voltage" on LCD line1*/
  LCD_SetCursorPos(LCD_LINE1, 0);
  LCD_Print("Pot ADC Voltage");

   /* ADC configuration -------------------------------------------*/
  ADC_Config();

  /* Infinite loop*/
  while (1)
  {
    /* Calculate voltage value*/
    PotVoltage = (uint16_t)((uint32_t)((uint32_t)ADCdata * (uint32_t)ADC_RATIO) / (uint32_t)1000);

    /* Display voltage value on LCD*/
    ShowVoltage((uint16_t)PotVoltage);
  }

}

/**
  * @brief  Configure System and peripheral Clock 
  * @param  None
  * @retval None
  */
static void CLK_Config(void)
{
  
  /* System Clock is HSI/1 */
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);

  /* Enable ADC1 clock */
  CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, ENABLE);
}

/**
  * @brief  Configure ADC peripheral
  * @param  None
  * @retval None
  */
static void ADC_Config(void)
{
  /* Initialise and configure ADC1 */
  ADC_Init(ADC1, ADC_ConversionMode_Continuous, ADC_Resolution_12Bit, ADC_Prescaler_2);
  ADC_SamplingTimeConfig(ADC1, ADC_Group_SlowChannels, ADC_SamplingTime_384Cycles);

  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);

  /* Enable ADC1 Channel 3 */
  ADC_ChannelCmd(ADC1, ADC_Channel_3, ENABLE);

  /* Enable End of conversion ADC1 Interrupt */
  ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);

  /* Start ADC1 Conversion using Software trigger*/
  ADC_SoftwareStartConv(ADC1);
}

/**
  * @brief  Display the current Potentiometer voltage on the LCD.
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

  /* Print the Potentiometer Voltage on the LCD line2 */
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
