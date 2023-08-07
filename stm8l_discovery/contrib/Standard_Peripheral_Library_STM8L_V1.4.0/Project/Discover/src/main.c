/**
  ******************************************************************************
  * @file    main.c
  * @author  Microcontroller Division
  * @version V1.2.0
  * @date    09/2010
  * @brief   Main program body
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
#include "stm8l15x.h"
#include "stm8l_discovery_lcd.h"
#include "discover_board.h"
#include "icc_measure.h"
#include "discover_functions.h"

/* Machine status used by main for active function set by user button in interrupt handler */
uint8_t state_machine;

/* LCD bar graph: used for display active function */
extern uint8_t t_bar[2];

/* Auto_test activation flag: set by interrupt handler if user button is pressed few seconds */
extern bool Auto_test;

/* Set in interrupt handler for indicate that user button is pressed */ 
extern bool KeyPressed;

/**
  * @brief main entry point.
  * @par Parameters None
  * @retval void None
  * @par Required preconditions: None
  */
void main(void)
{ 

/* Init I/O ports */
 
/* USER button init: GPIO set in input interrupt active mode */
  GPIO_Init( BUTTON_GPIO_PORT, USER_GPIO_PIN, GPIO_Mode_In_FL_IT);
	
/* Green led init: GPIO set in output */
  GPIO_Init( LED_GREEN_PORT, LED_GREEN_PIN, GPIO_Mode_Out_PP_High_Fast);
	
/* Blue led init: GPIO set in output */
  GPIO_Init( LED_BLUE_PORT, LED_BLUE_PIN, GPIO_Mode_Out_PP_High_Fast);
	
/* Counter enable: GPIO set in output for enable the counter */
  GPIO_Init( CTN_GPIO_PORT, CTN_CNTEN_GPIO_PIN, GPIO_Mode_Out_OD_HiZ_Slow);
	
/* Wake up counter: for detect end of counter GPIO set in input interupt active mode */
  GPIO_Init( WAKEUP_GPIO_PORT, ICC_WAKEUP_GPIO_PIN, GPIO_Mode_In_FL_IT);

/* Enable Rising edge port PE6 for wake up conter */
  EXTI->CR2 = 0x10; 	

/* Initializes the LCD glass */
  LCD_GLASS_Init();
	
/* Enable ADC clock */
  CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, ENABLE);

/* Initializes ADC */
  ADC_Icc_Init();

/* Init the VREF value */
  Vref_measure();

/* No auto test at startup */
  Auto_test = FALSE;

/* Reset Keypressed used in interupt and Scrollsentence */
  KeyPressed = FALSE;
	 
/* Switch off the leds at start */
  GPIO_LOW(LED_GREEN_PORT,LED_GREEN_PIN);	
  GPIO_LOW(LED_BLUE_PORT,LED_BLUE_PIN);		
		
/* Check the User button at Power ON if pressed --> Bias current measurement and store in E²Prom */	
  while ((GPIOC->IDR & USER_GPIO_PIN) == 0x0)
  {
    Bias_measurement();
  }	
  
  enableInterrupts();	

/* Welcome display */
  LCD_GLASS_ScrollSentence("      ** STM8L-DISCOVERY **",1,SCROLL_SPEED);
  if (!KeyPressed)
  {
    LCD_BlinkConfig(LCD_BlinkMode_AllSEG_AllCOM,LCD_BlinkFrequency_Div512);
    LCD_GLASS_DisplayString("JP1 ON");
    delay_ms(500);
    LCD_BlinkConfig(LCD_BlinkMode_Off,LCD_BlinkFrequency_Div32);	
  }

  /* LED Green ON */
  GPIO_HIGH(LED_GREEN_PORT,LED_GREEN_PIN);	
  
  /* Init Bar on LCD all are OFF --> first function VDD displays */
  BAR0_OFF;
  BAR1_OFF;
  BAR2_OFF;
  BAR3_OFF;	
  
  /* At power on VDD diplays */
  state_machine = STATE_VREF;
  
  while (1)
  {
/* if auto test is set --> runs it once */
    if (Auto_test)
      auto_test();
    
    switch (state_machine)
    {
        /* Displays VDD */
        case STATE_VREF:
          GPIO_TOGGLE(LED_GREEN_PORT,LED_GREEN_PIN);	
          GPIO_TOGGLE(LED_BLUE_PORT,LED_BLUE_PIN);	
          Vref_measure();
        break;
        
        /* Displays current in Power Run with LCD */
        case STATE_ICC_RUN:
          Icc_measure_RUN();
        break;
        
        /* Displays current in Low Power with LCD */
        case STATE_LPR_LCD:
                Icc_measure_LPR_LCD();
        break;
        
        /* Displays current in Low Power without LCD */
        case STATE_LPR:
                Icc_measure_LPR();
        break;
        
        /* Displays current in Halt without LCD */
        case STATE_HALT:
                Icc_measure_HALT();	
        break;
        
        /* for safe: normaly never reaches */ 			
        default:
                LCD_GLASS_Clear();
                LCD_GLASS_DisplayString("Error");
        break;
      }
    
/* Tempo for display values*/
      delay_ms(100);
  
      KeyPressed = FALSE;		
    }
}		

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
