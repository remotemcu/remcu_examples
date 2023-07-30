/**
  ******************************************************************************
  * @file    icc_measure.c
  * @author  Microcontroller Division
  * @version V1.2.0
  * @date    09/2010
  * @brief   Current measurements
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
 
/* Includes ------------------------------------------------------------------*/
#include "icc_measure.h"
#include "discover_functions.h"
#include "discover_board.h"
#include "stm8l_discovery_lcd.h"

/**
  * @brief  configure GPIOs before entering low power
	* @caller Halt_Init and ADC_Icc_Test
  * @param None
  * @retval None
  */  
void GPIO_LowPower_Config(void)
{

/* Port A in output push-pull 0 */
  GPIO_Init(GPIOA,GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7 ,GPIO_Mode_Out_PP_Low_Slow);
	
/* Port B in output push-pull 0 */
  GPIO_Init(GPIOB, GPIO_Pin_All, GPIO_Mode_Out_PP_Low_Slow);

/* Port C in output push-pull 0 except Button pins */
  GPIO_Init(GPIOC, GPIO_Pin_0|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_5|GPIO_Pin_6, GPIO_Mode_Out_PP_Low_Slow);

/* Port D in output push-pull 0 */
  GPIO_Init(GPIOD, GPIO_Pin_All, GPIO_Mode_Out_PP_Low_Slow);

/* Port E in output push-pull 0 */
  GPIO_Init(GPIOE, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_5, GPIO_Mode_Out_PP_Low_Slow);

/* Port F in output push-pull 0 */
/* Not PF0 because Input for ICC measurement */
  GPIO_Init(GPIOF,GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7 ,GPIO_Mode_Out_PP_Low_Slow);

  GPIO_Init(GPIOC, GPIO_Pin_1, GPIO_Mode_Out_PP_High_Slow);
}

/**
  * @brief  Read ADC1
	* @caller several functions
  * @param None
  * @retval ADC value
  */ 
u16 ADC_Supply(void)
{
	uint8_t i;
	uint16_t res;

/* Enable ADC clock */
  CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, ENABLE);

/* de-initialize ADC */
  ADC_DeInit(ADC1);

/*ADC configuration
  ADC configured as follow:
  - Channel VREF
  - Mode = Single ConversionMode(ContinuousConvMode disabled)
  - Resolution = 12Bit
  - Prescaler = /1
  - sampling time 9 */
  
  ADC_VrefintCmd(ENABLE);
  delay_10us(3);
  
  
  ADC_Cmd(ADC1, ENABLE);	 
  ADC_Init(ADC1, ADC_ConversionMode_Single,
  ADC_Resolution_12Bit, ADC_Prescaler_1);
  
  ADC_SamplingTimeConfig(ADC1, ADC_Group_FastChannels, ADC_SamplingTime_9Cycles);
  ADC_ChannelCmd(ADC1, ADC_Channel_Vrefint, ENABLE);
  delay_10us(3);

/* initialize result */
  res = 0;
  for(i=8; i>0; i--)
  {
/* start ADC convertion by software */
    ADC_SoftwareStartConv(ADC1);
/* wait until end-of-covertion */
    while( ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == 0 );
/* read ADC convertion result */
    res += ADC_GetConversionValue(ADC1);
  }
	
/* de-initialize ADC */
  ADC_VrefintCmd(DISABLE);

  ADC_DeInit(ADC1);
 
 /* disable SchmittTrigger for ADC_Channel_24, to save power */
  ADC_SchmittTriggerConfig(ADC1, ADC_Channel_24, DISABLE);
	
  CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, DISABLE);
  ADC_ChannelCmd(ADC1, ADC_Channel_Vrefint, DISABLE);
	
  return (res>>3);
}

/**
  * @brief ADC_Icc(ADC_24 of ADC_0~27) initialization
	* @caller main and ADC_Icc_Test
  * @param None
  * @retval None
  */ 
void ADC_Icc_Init(void)
{

/* Enable ADC clock */
  CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, ENABLE);

/* de-initialize ADC */
  ADC_DeInit(ADC1);
	
/*  ADC configured as follow:
  - NbrOfChannel = 1 - ADC_Channel_24
  - Mode = Single ConversionMode(ContinuousConvMode disabled)
  - Resolution = 12Bit
  - Prescaler = /1
  - sampling time 159 */
	
/* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);
  ADC_Init(ADC1, ADC_ConversionMode_Single,ADC_Resolution_12Bit, ADC_Prescaler_1);

  ADC_SamplingTimeConfig(ADC1, ADC_Group_FastChannels, ADC_SamplingTime_9Cycles);
  ADC_ChannelCmd(ADC1, ADC_Channel_24, ENABLE);

/* disable SchmittTrigger for ADC_Channel_24, to save power */
  ADC_SchmittTriggerConfig(ADC1, ADC_Channel_24, DISABLE);

/* a short time of delay is required after enable ADC */
  delay_10us(3);
	
}
/**
  * @brief This function initializes in Ultra Low Power mode, 
	*				disable the LCD, LSE and configures the unused IOs 
	*				in output push-pull
	* @caller main and ADC_Icc_Test
  * @param None
  * @retval None
  */ 
void Halt_Init(void)
{

/* Set STM8 in low power */
  PWR->CSR2 = 0x2;
  
  LCD_Cmd(DISABLE);

  /* To wait LCD disable */
  while ((LCD->CR3 & 0x40) != 0x00);
  
/* Set GPIO in low power*/	
  GPIO_LowPower_Config();
  
/* Stop RTC Source clock */
  CLK_RTCClockConfig(CLK_RTCCLKSource_Off, CLK_RTCCLKDiv_1);
  
  #ifdef USE_LSE
    CLK_LSEConfig(CLK_LSE_OFF);
    while ((CLK->ECKCR & 0x04) != 0x00);
  #else
    CLK_LSICmd(DISABLE);
    while ((CLK->ICKCR & 0x04) != 0x00);
  #endif
  
  /* Stop clock RTC and LCD */ 	
  CLK_PeripheralClockConfig(CLK_Peripheral_RTC, DISABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_LCD, DISABLE);
}

/**
  * @brief Current measurement in different MCU modes:
		RUN/HALT/LowPower withouto LCD/LowPower with LCD
	* @caller main and ADC_Icc_Test
  * @param MCU state
  * @retval ADC value.
  */
u16 ADC_Icc_Test(u8 Mcu_State)
{
  uint16_t res;
	uint8_t i;
	
/* Test MCU state for configuration */
  switch (Mcu_State)
  {
    /* test Run mode nothing to do */	
    case MCU_RUN:
      break;

    /* Low power mode */		
    case MCU_LPR:
      Halt_Init();
      sim();
      /* To prepare to start counter */
      GPIO_HIGH(CTN_GPIO_PORT,CTN_CNTEN_GPIO_PIN);
  
      /* To configure Low Power */			
      LPR_init();
      break;

    /* Halt mode */
    case MCU_HALT:
      /* Init for Halt mode  */
      Halt_Init();
      sim();	
      
      /* To prepare to start counter */
      GPIO_HIGH(CTN_GPIO_PORT,CTN_CNTEN_GPIO_PIN);
  
      /* Falling edge for start counter */		
      GPIO_LOW(CTN_GPIO_PORT,CTN_CNTEN_GPIO_PIN);
      delay_10us(1);
      
      /* MCU in halt during measurement */	
      /* Wake up by Interrupt done counter Input Port E pin 6 */
      halt();
      break;
    
    case MCU_LPR_LCD:
      PWR->CSR2 = 0x2;
      sim();
      /* To configure GPIO for reduce current. */
      GPIO_LowPower_Config();

      /* To prepare to start counter */
      GPIO_HIGH(CTN_GPIO_PORT,CTN_CNTEN_GPIO_PIN);

      /* To configure Low Power */
      LPR_init();
      break;		
  }
	
  sim();

/* re-start ADC chanel 24 for Current measurement */
  ADC_Icc_Init();	
	
/* Read ADC for current measurmeent */
/* initialize result */
  res = 0;

  for(i=8; i>0; i--)
  {
    /* start ADC convertion by software */
    ADC_SoftwareStartConv(ADC1);
    
    /* wait until end-of-covertion */
    while( ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == 0 );
    
    /* read ADC convertion result */
    res += ADC_GetConversionValue(ADC1);
  }
	
  /* ICC_CNT_EN invalid */
  GPIO_HIGH(CTN_GPIO_PORT,CTN_CNTEN_GPIO_PIN);
 
  GPIO_Init(BUTTON_GPIO_PORT, USER_GPIO_PIN,GPIO_Mode_In_FL_IT);

  rim();
	
/* Disable ADC 1 for reduce current */
  ADC_Cmd(ADC1, DISABLE);

  CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, DISABLE);

  if (Mcu_State !=MCU_LPR_LCD)
  {
    CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE);
    LCD_GLASS_Init();
  }

  return (res>>3);
}
	

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
