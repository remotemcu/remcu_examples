/**
  ******************************************************************************
  * @file    ADC/ADC_DMA/main.c
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

/** @addtogroup ADC_DMA
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define ADC1_DR_ADDRESS        ((uint16_t)0x5344)
#define BUFFER_SIZE            ((uint8_t) 0x02)
#define BUFFER_ADDRESS         ((uint16_t)(&Buffer))
#define ASCII_NUM_0            ((uint8_t)   48)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint16_t Buffer[BUFFER_SIZE]  = {0, 0};
uint32_t PotVoltage = 0;
uint32_t BNCVoltage = 0;
uint8_t LCDString[16] = " 0.00V   0.00V ";
FunctionalState DisplayStatus = DISABLE;

/* Private function prototypes -----------------------------------------------*/
static void CLK_Config(void);
static void ADC_Config(void);
static void DMA_Config(void);
void ShowVoltages(uint32_t PotVoltage, uint32_t BNCVoltage );

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

  /* Init the Eval board LCD */
  STM8_EVAL_LCD_Init();

  /* Clear  LCD */
  LCD_Clear();

  /* Print the Voltage on the LCD*/
  LCD_SetCursorPos(LCD_LINE1, 0);
  LCD_Print("  POT      BNC  ");

   /* ADC configuration -------------------------------------------*/
  ADC_Config();

   /* DMA configuration -------------------------------------------*/
  DMA_Config();
    
  /* Enable ADC1 DMA requests*/
  ADC_DMACmd(ADC1, ENABLE);

  /* Start ADC1 Conversion using TIM1 TRGO*/
  ADC_ExternalTrigConfig(ADC1, ADC_ExtEventSelection_Trigger2,
                         ADC_ExtTRGSensitivity_Rising);

  /* Master Mode selection: Update event */
  TIM1_SelectOutputTrigger(TIM1_TRGOSource_Update);

  /* Enable TIM1 */
  TIM1_Cmd(ENABLE);

  /* Enable Interrupts */
  enableInterrupts();

  /* Infinite loop*/
  while (1)
  {
    if (DisplayStatus != DISABLE)
    {
      /* Display Potentiometer RV and BNC voltage values on LCD*/
      ShowVoltages(PotVoltage, BNCVoltage);
      /* Disable displaying voltages on LCD until next DMA Channel0 Transfer complete occurs */
      DisplayStatus = DISABLE;
    }
  }
}

/**
  * @brief  Configure peripherals and system clock
  * @param  None
  * @retval None
  */
static void CLK_Config(void)
{
  /* Select HSE as system clock source */
  CLK_SYSCLKSourceSwitchCmd(ENABLE);
  CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSE);
  /*High speed external clock prescaler: 1*/
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);

  while (CLK_GetSYSCLKSource() != CLK_SYSCLKSource_HSE)
  {}

  /* Enable ADC1 clock */
  CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, ENABLE);

  /* Enable DMA1 clock */
  CLK_PeripheralClockConfig(CLK_Peripheral_DMA1, ENABLE);

  /* Enable TIM1 clock */
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM1, ENABLE);
}

/**
  * @brief  Configure ADC peripheral 
  * @param  None
  * @retval None
  */
static void ADC_Config(void)
{
  /* Initialize and configure ADC1 */
  ADC_Init(ADC1, ADC_ConversionMode_Single, ADC_Resolution_12Bit, ADC_Prescaler_2);
  ADC_SamplingTimeConfig(ADC1, ADC_Group_SlowChannels, ADC_SamplingTime_384Cycles);
  ADC_SamplingTimeConfig(ADC1, ADC_Group_FastChannels, ADC_SamplingTime_384Cycles);

  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);

  /* Enable ADC1 Channels 3 */
  ADC_ChannelCmd(ADC1, ADC_Channel_3, ENABLE); /* connected to Potentiometer RV */
  /* Enable ADC1 Channels 24 */
  ADC_ChannelCmd(ADC1, ADC_Channel_24, ENABLE); /* connected to BNC */
}

/**
  * @brief  Configure DMA peripheral 
  * @param  None
  * @retval None
  */
static void DMA_Config(void)
{
  /* Connect ADC to DMA channel 0 */
  SYSCFG_REMAPDMAChannelConfig(REMAP_DMA1Channel_ADC1ToChannel0);

  DMA_Init(DMA1_Channel0, BUFFER_ADDRESS,
           ADC1_DR_ADDRESS,
           BUFFER_SIZE,
           DMA_DIR_PeripheralToMemory,
           DMA_Mode_Circular,
           DMA_MemoryIncMode_Inc,
           DMA_Priority_High,
           DMA_MemoryDataSize_HalfWord);

  /* DMA Channel0 enable */
  DMA_Cmd(DMA1_Channel0, ENABLE);

  /* Enable DMA1 channel0 Transfer complete interrupt */
  DMA_ITConfig(DMA1_Channel0, DMA_ITx_TC, ENABLE);

  /* DMA enable */
  DMA_GlobalCmd(ENABLE);
}

/**
  * @brief  Display the current voltage on the LCD.
  * @param  voltage: specifies the voltage to display.
  * @retval None
  */
void ShowVoltages(uint32_t PotVoltage, uint32_t BNCVoltage )
{
  uint8_t voltage1000 = 0;
  uint8_t voltage100 = 0;
  uint8_t voltage10 = 0;

  /* Thousands voltage value*/
  voltage1000 = (uint8_t)(PotVoltage / 1000);
  /* Hundreds voltage value */
  voltage100 = (uint8_t)((PotVoltage % 1000) / 100);
  /* Tens voltage value */
  voltage10 = (uint8_t)((PotVoltage % 100 ) / 10);


  /* Fill the LCDString fields with the current Voltage */
  LCDString[1]  = (uint8_t)((uint8_t)(voltage1000) + ASCII_NUM_0);
  LCDString[3] = (uint8_t)((uint8_t)(voltage100) + ASCII_NUM_0);
  LCDString[4] = (uint8_t)((uint8_t)(voltage10) + ASCII_NUM_0);

  /* Thousands voltage value*/
  voltage1000 = (uint8_t)(BNCVoltage / 1000);
  /* Hundreds voltage value */
  voltage100 = (uint8_t)((BNCVoltage % 1000) / 100);
  /* Tens voltage value */
  voltage10 = (uint8_t)((BNCVoltage % 100 ) / 10);

  LCDString[9]  = (uint8_t)((uint8_t)(voltage1000) + ASCII_NUM_0);
  LCDString[11] = (uint8_t)((uint8_t)(voltage100) + ASCII_NUM_0);
  LCDString[12] = (uint8_t)((uint8_t)(voltage10) + ASCII_NUM_0);

  /* Print the Voltage on the LCD*/
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
    LCD_PrintString(LCD_LINE1, DISABLE, DISABLE, "      ERR       ");
    LCD_PrintString(LCD_LINE2, DISABLE, DISABLE, "     ASSERT     ");
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
