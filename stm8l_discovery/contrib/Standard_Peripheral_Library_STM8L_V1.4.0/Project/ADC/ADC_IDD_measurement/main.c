/**
  ******************************************************************************
  * @file    ADC/ADC_IDD_Measurement/main.c
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

/** @addtogroup ADC_IDD_Measurement
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#ifdef USE_STM8L1526_EVAL

/* ADC channel used for IDD measurement is channel 19 */
 #define ADC_IDD_MEASUREMENT_CHANNEL     ADC_Channel_19

/* Channel 19 is a slow channel */
 #define ADC_GROUP_SPEEDCHANNEL          ADC_Group_SlowChannels

 #define IDD_CNT_EN_PIN                  GPIO_Pin_7
 #define IDD_CNT_EN_PORT                 GPIOE
 #define IDD_WAKEUP_PIN                  GPIO_Pin_6
 #define IDD_WAKEUP_PORT                 GPIOE

#else /* USE_STM8L1528_EVAL */

/* ADC channel used for IDD measurement is channel 27 */
 #define ADC_IDD_MEASUREMENT_CHANNEL     ADC_Channel_27

/* Channel 27 is a fast channel */
 #define ADC_GROUP_SPEEDCHANNEL          ADC_Group_FastChannels

 #define IDD_CNT_EN_PIN                  GPIO_Pin_4
 #define IDD_CNT_EN_PORT                 GPIOH
 #define IDD_WAKEUP_PIN                  GPIO_Pin_5
 #define IDD_WAKEUP_PORT                 GPIOH

#endif /* USE_STM8L1526_EVAL */

#define EVAL_MAX9938_GAIN               50       /* Ampli-op gain = 50 */
#define ADC_CONVERT_RATIO               806      /* (3300mV / 0xFFF)* 1000 */
#define ADC_CONVERT_RATIO1000           805860   /* (3300mV / 0xFFF)* 1000000 */
#define EVAL_RESISTOR_R33               2         /* 2 ohm  */
#define EVAL_RESISTOR_R24               2000       /* 2 kohm  */

#define ASCII_NUM_0                     48

#define Bit_RESET                       0
#define Bit_SET                         1
#define MODE_RUN                        0
#define MODE_HALT                       1


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint16_t ADCData = 0;
uint32_t VDD = 0;
uint32_t IDD = 0;
__IO uint8_t LCDString[16] = "  000.000 mA    ";
uint8_t ModeIndex = 0x00;
/* Private function prototypes -----------------------------------------------*/
void CurrentDisplay(uint32_t Current);
JOYState_TypeDef ReadJoystick(void);
void GPIO_LowPower_Config(void);
void Joystick_Config(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void main(void)
{
  JOYState_TypeDef Key = (JOYState_TypeDef)0x00;
  uint8_t i = 0x00;
  uint32_t result = 0x00;

  /* Select HSE as system clock source */
  CLK_SYSCLKSourceSwitchCmd(ENABLE);
  CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSE);
  /*High speed external clock prescaler: 1*/
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);

  while (CLK_GetSYSCLKSource() != CLK_SYSCLKSource_HSE)
  {}

  /*To generate 1 ms time base using TIM2 update interrupt*/
  TimingDelay_Init();

  /* Enable Interrupts*/
  enableInterrupts();

  Delay(200);

  /* Init the Eval board LCD */
  STM8_EVAL_LCD_Init();

  /* Clear LCD */
  LCD_Clear();

  while (1)
  {
    /* Print "IDD Run Mode" in the LCD first line */
    LCD_SetCursorPos(LCD_LINE1, 0);
    LCD_Print("IDD Measurement ");
    LCD_SetCursorPos(LCD_LINE2, 0);
    LCD_Print("   Run Mode     ");

    /* Read Key */
    while (Key != JOY_SEL)
    {
      Key = ReadJoystick();
      if (Key == JOY_RIGHT)
      {
        if (ModeIndex == MODE_RUN)
        {
          ModeIndex = MODE_HALT;
          LCD_SetCursorPos(LCD_LINE2, 0);
          LCD_Print("   Halt Mode    ");
        }
        else
        {
          ModeIndex = MODE_RUN;
          LCD_SetCursorPos(LCD_LINE2, 0);
          LCD_Print("   Run Mode     ");
        }
      }
      if (Key == JOY_LEFT)
      {
        if (ModeIndex == MODE_HALT)
        {
          LCD_SetCursorPos(LCD_LINE2, 0);
          LCD_Print("   Run Mode     ");
          ModeIndex = MODE_RUN;
        }
        else
        {
          ModeIndex = MODE_HALT;
          LCD_SetCursorPos(LCD_LINE2, 0);
          LCD_Print("   Halt Mode    ");
        }
      }
    }
    if (ModeIndex == MODE_RUN)
    {
      /* Enable ADC1 clock */
      CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, ENABLE);

      /* Initialize and configure ADC1 */
      ADC_Init(ADC1, ADC_ConversionMode_Continuous, ADC_Resolution_12Bit, ADC_Prescaler_2);

      /* ADC channel used for IDD measurement */
      ADC_SamplingTimeConfig(ADC1, ADC_GROUP_SPEEDCHANNEL, ADC_SamplingTime_384Cycles);

      /* Enable ADC1 */
      ADC_Cmd(ADC1, ENABLE);

      /* Disable SchmittTrigger for ADC_Channel, to save power */
      ADC_SchmittTriggerConfig(ADC1, ADC_IDD_MEASUREMENT_CHANNEL, DISABLE);

      /* Enable ADC1 Channel used for IDD measurement */
      ADC_ChannelCmd(ADC1, ADC_IDD_MEASUREMENT_CHANNEL, ENABLE);

      /* Start ADC1 Conversion using Software trigger*/
      ADC_SoftwareStartConv(ADC1);

      /* Waiting until press Joystick Up */
      while (Key != JOY_UP)
      {
        /* Wait until End-Of-Convertion */
        while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == 0)
        {}

        /* Get conversion value */
        ADCData = ADC_GetConversionValue(ADC1);

        /* Calculate voltage value in uV over capacitor  C67 for IDD measurement*/
        VDD = (uint32_t)((uint32_t)ADCData * (uint32_t)ADC_CONVERT_RATIO);

        /* Calculate the current consumption in uA */
        IDD = (uint32_t) ((uint32_t)((VDD / EVAL_MAX9938_GAIN)) / (uint32_t)(EVAL_RESISTOR_R33));

        /* Display Current  value on LCD */
        CurrentDisplay(IDD);

        /* Read Joystick */
        Key = ReadJoystick();

        /* Waiting Delay 200ms */
        Delay(200);
      }
      /* DeInitialize ADC1 */
      ADC_DeInit(ADC1);

      /* Disable ADC1 clock */
      CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, DISABLE);
    }
    else
    {
      PWR_UltraLowPowerCmd(ENABLE);

      /* Disable ADC1 clock */
      CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, ENABLE);

      /* Waiting Delay 2ms */
      Delay(2);

      /* Waiting until press Joystick Up */
      while (Key != JOY_UP)
      {
        /* Initialize the ADCData and the result variable */
        result = 0;
        ADCData = 0x00;

        /* Disable Interrupts*/
        disableInterrupts();

        /* Disable SPI clock */
        CLK_PeripheralClockConfig(LCD_SPI_CLK, DISABLE);
        /* Disable TIM2 clock */
        CLK_PeripheralClockConfig(CLK_Peripheral_TIM2, DISABLE);

        /* Deinitialize the ADC */
        ADC_DeInit(ADC1);

        /* Disable SchmittTrigger for ADC_Channel, to save power */
        ADC_SchmittTriggerConfig(ADC1, ADC_IDD_MEASUREMENT_CHANNEL, DISABLE);

        /* DeInitialize SPI */
        SPI_DeInit(LCD_SPI);

        /* DeInitialize TIM2 */
        TIM2_DeInit();

        /* Configure all GPIO in output push-pull mode */
        GPIO_LowPower_Config();

        /* Configure PH.4 (IDD_CNT_EN) as output push-pull -------------------------*/
        GPIO_Init(IDD_CNT_EN_PORT, IDD_CNT_EN_PIN, GPIO_Mode_Out_PP_High_Fast);

        /* Configure the EXTI Line 5 (IDD_WAKEUP) as input floating with IT */
        GPIO_Init(IDD_WAKEUP_PORT, IDD_WAKEUP_PIN, GPIO_Mode_In_FL_IT);

#ifdef USE_STM8L1526_EVAL
        EXTI_SetPinSensitivity(EXTI_Pin_6, EXTI_Trigger_Falling);
#else /* USE_STM8L1528_EVAL */
        EXTI_SetPortSensitivity(EXTI_Port_H, EXTI_Trigger_Falling);
        EXTI_SelectPort(EXTI_Port_H);
        EXTI_SetHalfPortSelection(EXTI_HalfPort_H_MSB, ENABLE);

        /* Disable the SPI2 pin remap on Port I*/
        SYSCFG_REMAPDeInit();
#endif /*USE_STM8L1526_EVAL*/

        /* Enable Interrupts*/
        enableInterrupts();

        /* Clear IDD_CNT_EN Pin */
        GPIO_ResetBits(IDD_CNT_EN_PORT, IDD_CNT_EN_PIN);

        halt();

        /* Reset the counter by programming IDD_CNT_EN High in less than 70ms after
         the wakeup to avoid 1Kohm to be connected later on VDD_MCU */
        GPIO_SetBits(IDD_CNT_EN_PORT, IDD_CNT_EN_PIN);

        /* Initialize and configure ADC1 */
        ADC_Init(ADC1, ADC_ConversionMode_Single, ADC_Resolution_12Bit, ADC_Prescaler_1);

        /* ADC channel used for IDD measurement */
        ADC_SamplingTimeConfig(ADC1, ADC_GROUP_SPEEDCHANNEL, ADC_SamplingTime_192Cycles);

        /* Enable ADC1 */
        ADC_Cmd(ADC1, ENABLE);

        /* Enable ADC1 Channel used for IDD measurement */
        ADC_ChannelCmd(ADC1, ADC_IDD_MEASUREMENT_CHANNEL, ENABLE);

        for (i = 4; i > 0; i--)
        {
          /* Start ADC1 Conversion using Software trigger*/
          ADC_SoftwareStartConv(ADC1);

          /* Wait until ADC Channel 15 end of conversion */
          while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET)
          {}
          /* read ADC convertion result */
          ADCData = ADC_GetConversionValue(ADC1);
          result += ADCData;
        }
        
        result = result>>2;
        
        /*Calculate voltage value in nV over capacitor  C67 for IDD measurement*/
        VDD = (uint32_t)((uint32_t)result * (uint32_t)ADC_CONVERT_RATIO1000);

        /* Calculate the current consumption in nA*/
        IDD = (uint32_t) ((uint32_t)((VDD / EVAL_MAX9938_GAIN)) / (uint32_t)(EVAL_RESISTOR_R33 + EVAL_RESISTOR_R24));

        /* Configue the Joystick push-button as input floating no IT */
        Joystick_Config();

        /* Init the Eval board LCD */
        STM8_EVAL_LCD_Init();

        /* Print "IDD halt Mode" in the LCD lines */
        LCD_SetCursorPos(LCD_LINE1, 0);

        LCD_Print("IDD Measurement ");

        /* Display Current  value on LCD */
        CurrentDisplay(IDD);

        /* Read Joystick */
        Key = ReadJoystick();
      }

      /* Disable ADC1 clock */
      CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, DISABLE);

      /*To generate 1 ms time base using TIM2 update interrupt*/
      TimingDelay_Init();

      ModeIndex = MODE_RUN;
    }
  }
}

/**
  * @brief  Display the current on the LCD.
  * @param  Current: specifies the current in mA.
  * @retval None
  */
void CurrentDisplay(uint32_t Current)
{
  uint8_t i = 0;
  uint8_t current1000 = 0;
  uint8_t current100 = 0;
  uint8_t current10 = 0;
  uint8_t current1 = 0;
  uint8_t current01 = 0;
  uint8_t current001 = 0;

  for (i = 0x00; i < 16; i++)
  {
    LCDString[i] = ' ';
  }

  if (ModeIndex == MODE_RUN)
  {
    /* x  current value*/
    current1000 = (uint8_t)(Current / 100000);
    /* Hundreds current value */
    current100 = (uint8_t)((Current % 100000) / 10000);
    /* Tens current value */
    current10 = (uint8_t)((Current % 10000) / 1000);
    /* 1mA current value */
    current1 = (uint8_t)((Current % 1000) / 100);
    /* 0.1 mA current value */
    current01 = (uint8_t)((Current % 100 ) / 10);
    /* 0.01 mA current value */
    current001 =  (uint8_t)(Current % 10);

    /* Fill the LCDString fields with the current Voltage */
    if (current1000 != 0)
    {
      LCDString[2] = (uint8_t)((uint8_t)(current1000) + ASCII_NUM_0);
    }
    if (current100 != 0 || current1000 != 0)
    {
      LCDString[3] = (uint8_t)((uint8_t)(current100) + ASCII_NUM_0);
    }

    LCDString[4] = (uint8_t)((uint8_t)(current10) + ASCII_NUM_0);
    LCDString[5] = '.';
    LCDString[6] = (uint8_t)((uint8_t)(current1) + ASCII_NUM_0);
    LCDString[7] = (uint8_t)((uint8_t)(current01) + ASCII_NUM_0);
    LCDString[8] = (uint8_t)((uint8_t)(current001) + ASCII_NUM_0);

    LCDString[10] = 'm'; /* mA */
    LCDString[11] = 'A';
  }

  else
  {
    /* x  current value*/
    current1000 = (uint8_t)(Current / 100000);
    /* Hundreds current value */
    current100 = (uint8_t)((Current % 100000) / 10000);
    /* Tens current value */
    current10 = (uint8_t)((Current % 10000) / 1000);
    /* 1mA current value */
    current1 = (uint8_t)((Current % 1000) / 100);
    /* 0.1 mA current value */
    current01 = (uint8_t)((Current % 100 ) / 10);
    /* 0.01 mA current value */
    current001 =  (uint8_t)(Current % 10);

    /* Fill the LCDString fields with the current Voltage */
    if (current1000 != 0)
    {
      LCDString[2] = (uint8_t)((uint8_t)(current1000) + ASCII_NUM_0);
    }
    if (current100 != 0 || current1000 != 0)
    {
      LCDString[3] = (uint8_t)((uint8_t)(current100) + ASCII_NUM_0);
    }

    LCDString[4] = (uint8_t)((uint8_t)(current10) + ASCII_NUM_0);
    LCDString[5] = '.';
    LCDString[6] = (uint8_t)((uint8_t)(current1) + ASCII_NUM_0);
    LCDString[7] = (uint8_t)((uint8_t)(current01) + ASCII_NUM_0);
    LCDString[8] = (uint8_t)((uint8_t)(current001) + ASCII_NUM_0);

    LCDString[10] = 'u'; /* uA */
    LCDString[11] = 'A';

  }
  /* Print the Voltage on the LCD*/
  LCD_SetCursorPos(LCD_LINE2, 0);
  LCD_Print((uint8_t*)LCDString);
}

/**
  * @brief  Configure all GPIO in output push-pull mode .
  * @param  None.
  * @retval None.
  */
void GPIO_LowPower_Config(void)
{
  /* Port A in output push-pull 0 */
  GPIO_Init(GPIOA, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7, GPIO_Mode_Out_PP_Low_Slow);
  /* Port B in output push-pull 0 */
  GPIO_Init(GPIOB, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7, GPIO_Mode_Out_PP_Low_Slow);
  /* Port C in output push-pull 0 */
  GPIO_Init(GPIOC, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7, GPIO_Mode_Out_PP_Low_Slow);
  /* Port D in output push-pull 0 */
  GPIO_Init(GPIOD, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7, GPIO_Mode_Out_PP_Low_Slow);
  /* Port E in output push-pull 0 */
  GPIO_Init(GPIOE, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7, GPIO_Mode_Out_PP_Low_Slow);
  /* Port F Pin_0 in output push-pull 0 */
  GPIO_Init(GPIOF, GPIO_Pin_0, GPIO_Mode_Out_PP_Low_Slow);

#ifdef USE_STM8L1528_EVAL
  /* Port F in output push-pull 0 */
  GPIO_Init(GPIOF, GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7, GPIO_Mode_Out_PP_Low_Slow);
  /* Port G in output push-pull 0 */
  GPIO_Init(GPIOG, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7, GPIO_Mode_Out_PP_Low_Slow);
  /* Port H in output push-pull 0 */
  GPIO_Init(GPIOH, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7, GPIO_Mode_Out_PP_Low_Slow);
  /* Port I in output push-pull 0 */
  GPIO_Init(GPIOI, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3, GPIO_Mode_Out_PP_Low_Slow);
#endif /* USE_STM8L1528_EVAL */
}
/**
  * @brief  Reads joystick position from evalboard.
  * @param  None.
  * @retval Joystick position.
  *         This parameter can be a value of @ref JOYState_TypeDef enumeration.
  */
JOYState_TypeDef ReadJoystick(void)
{
  /* "right" key is pressed */
  if (!STM_EVAL_PBGetState(BUTTON_RIGHT))
  {
    while (STM_EVAL_PBGetState(BUTTON_RIGHT) == Bit_RESET);
    return JOY_RIGHT;
  }
  /* "left" key is pressed */
  if (!STM_EVAL_PBGetState(BUTTON_LEFT))
  {
    while (STM_EVAL_PBGetState(BUTTON_LEFT) == Bit_RESET);
    return JOY_LEFT;
  }
  /* "up" key is pressed */
  if (!STM_EVAL_PBGetState(BUTTON_UP))
  {
    while (STM_EVAL_PBGetState(BUTTON_UP) == Bit_RESET);
    return JOY_UP;
  }
  /* "down" key is pressed */
  if (!STM_EVAL_PBGetState(BUTTON_DOWN))
  {
    while (STM_EVAL_PBGetState(BUTTON_DOWN) == Bit_RESET);
    return JOY_DOWN;
  }
  /* "sel" key is pressed */
  if (!STM_EVAL_PBGetState(BUTTON_SEL))
  {
    while (STM_EVAL_PBGetState(BUTTON_SEL) == Bit_RESET);
    return JOY_SEL;
  }
  /* No key is pressed */
  else
  {
    return JOY_NONE;
  }
}
/**
  * @brief  configure the joystick button from evalboard.
  * @param  None.
  * @retval None.
  */
void Joystick_Config(void)
{
  STM_EVAL_PBInit(BUTTON_RIGHT, BUTTON_MODE_GPIO);
  STM_EVAL_PBInit(BUTTON_LEFT, BUTTON_MODE_GPIO);
  STM_EVAL_PBInit(BUTTON_UP, BUTTON_MODE_GPIO);
  STM_EVAL_PBInit(BUTTON_DOWN, BUTTON_MODE_GPIO);
  STM_EVAL_PBInit(BUTTON_SEL, BUTTON_MODE_GPIO);
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
