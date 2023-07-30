/**
******************************************************************************
* @file    discover_functions.c
* @author  Microcontroller Division
* @version V1.2.1
* @date    10/2010
* @brief   Discover demo functions
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

#include "discover_functions.h"
#include "discover_board.h"
#include "stm8l15x_flash.h"
#include "stm8l_discovery_lcd.h"

/* Bias current is saved in Data E²Prom 
This value is stored by FLASH_ProgramBias function*/

#ifdef _COSMIC_
#pragma section @near [dataeeprom] 
@near unsigned char  Bias_Current;
#pragma section []
#endif

#ifdef _RAISONANCE_
unsigned char eeprom Bias_Current;
#endif

#ifdef _IAR_
__no_init __eeprom unsigned char Bias_Current;
#endif

/* Used for indicate that the automatic test is ON (set in interrupt handler).*/
bool Auto_test;

/* Used for detect keypressed*/
extern bool KeyPressed;

/**
  * @brief  Store in E²Prom bias value
  * @caller Bias_measurement
  * @param Data: Bias current value to store in E²Prom
  * @retval None
  */
void FLASH_ProgramBias(uint8_t Data)
{
  FLASH_Unlock(FLASH_MemType_Data);
  Bias_Current = Data;
  FLASH_WaitForLastOperation(FLASH_MemType_Data);
  FLASH_Lock(FLASH_MemType_Data);
}	

/**
  * @brief  automatic test for VDD
  * @caller auto_test
  * @param None
  * @retval None
  */
void test_vdd(void)
{
  uint16_t vdd_test;
  
  LCD_GLASS_DisplayString("VDD");
  delay_ms(200);
  
  vdd_test = (int)Vref_measure();
  delay_ms(200);
  
  /* Test if value is correct */	
  if ((vdd_test>VCC_MAX) || (vdd_test<VCC_MIN))
  {
    while(1)
    {
      LCD_GLASS_ScrollSentence("VDD Not OK ",1,SCROLL_SPEED); //Press reset for exit
      KeyPressed = FALSE;
    }
  }
  
  LCD_GLASS_DisplayString("VDD OK");
  delay_ms(200);
}

/**
  * @brief  Automatic test current in Run Mode
  * @caller auto_test
  * @param None
  * @retval None
  */ 
void test_icc_Run(void)
{
  uint16_t icc_test;
  
  LCD_GLASS_DisplayString("RUN");
  delay_ms(200);
  
  icc_test = (int)Icc_measure_RUN();
  delay_ms(200);
  
  if ((icc_test>ICC_RUN_MAX) || (icc_test<ICC_RUN_MIN))
  {
    while (1)
    {
      LCD_GLASS_ScrollSentence("RUN Not OK ",1,SCROLL_SPEED); //Press reset for exit
      KeyPressed = FALSE;
    }
  }

  LCD_GLASS_DisplayString("RUN OK");
  delay_ms(200);
}

/**
  * @brief  Automatic test current in HALT Mode
  * @caller auto_test
  * @param None
  * @retval None
  */
void test_icc_HALT(void)
{
  uint16_t icc_test;
  
  LCD_GLASS_DisplayString("HALT");
  delay_ms(200);
  
  /* Current value measured in Halt mode*/	
  icc_test = (int)Icc_measure_HALT();
  delay_ms(200);
  
  /* Test if value is correct */
  if ((icc_test>ICC_HALT_MAX) || (icc_test<ICC_HALT_MIN))
  {
    delay_ms(400);	
    icc_test = (int)Icc_measure_HALT();
    delay_ms(200);		
    
  /* Test if value is correct */
    if ((icc_test>ICC_HALT_MAX) || (icc_test<ICC_HALT_MIN))
    {
      while (1)
      {
        LCD_GLASS_ScrollSentence("ICC HALT Not OK ",1,SCROLL_SPEED); //Press reset for exit
        KeyPressed = FALSE;
      }
    }
  }
  
  LCD_GLASS_DisplayString("HALTOK");
  delay_ms(200);
}

/**
  * @brief  Automatic test current in low power mode
  * @caller auto_test
  * @param None
  * @retval None
  */
void test_icc_LP(void)
{
  uint16_t icc_test;
  
  LCD_GLASS_DisplayString("LP");
  delay_ms(200);
  
  /* Current value measured in low power mode*/	
  icc_test = (int)Icc_measure_LPR();
  delay_ms(200);

  /* Test if value is correct */  
  if ((icc_test>ICC_LP_MAX) || (icc_test<ICC_LP_MIN))
  {
  /* check in twice measurements*/
    delay_ms(400);
    icc_test = (int)Icc_measure_LPR();
    delay_ms(200);			
      
	/* Test if value is correct */
    if ((icc_test>ICC_LP_MAX) || (icc_test<ICC_LP_MIN))
    {
      while(1)
      {
        LCD_GLASS_ScrollSentence("LP Not OK ",1,SCROLL_SPEED); //press reset for exit
        KeyPressed = FALSE;
      }
    }
  }
  
  LCD_GLASS_DisplayString("LP OK");
  delay_ms(200);
}

/**
  * @brief  Automatic test switch to LSE clock from HSI and return to HSI
  * @caller auto_test
  * @param None
  * @retval None
  */
void test_LSE(void)
{
	
/* Switch the clock to LSE */
	
  LCD_GLASS_DisplayString("LSE");
  
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);
  CLK_LSEConfig(CLK_LSE_ON);
  delay_ms(LSE_DELAY);	
  
  if((CLK->ECKCR & CLK_ECKCR_LSERDY) == RESET)
  {
    LCD_GLASS_DisplayString("LSE");
    delay_ms(LSE_DELAY);
    if((CLK->ECKCR & CLK_ECKCR_LSERDY) == RESET)
    {
      LCD_GLASS_DisplayString("LSE");
      delay_ms(LSE_DELAY);
      if((CLK->ECKCR & CLK_ECKCR_LSERDY) == RESET)
      {			
        /* Switch the clock to HSI*/
        CLK_LSEConfig(CLK_LSE_OFF);
        CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_8);
        CLK_HSICmd(ENABLE);
        while (((CLK->ICKCR)& 0x02)!=0x02);
        CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSI);
        CLK_SYSCLKSourceSwitchCmd(ENABLE);
        while (((CLK->SWCR)& 0x01)==0x01);
        
        while(1)
        {
          LCD_GLASS_ScrollSentence("LSE Not OK ",1,SCROLL_SPEED); //Press reset for exit
          KeyPressed = FALSE;
        }
      }
    }
  }

/* Wait flag LSE ready */
  while (!((CLK->ECKCR)& CLK_ECKCR_LSERDY));	

/* Switch in LSE clock */
  CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_LSE);
  CLK_SYSCLKSourceSwitchCmd(ENABLE);
  while (((CLK->SWCR)& 0x01)==0x01);
  
  LCD_GLASS_DisplayString("LSE OK");

/* Switch the clock to HSI */

  CLK_LSEConfig(CLK_LSE_OFF);
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_8);
  CLK_HSICmd(ENABLE);
  while (((CLK->ICKCR)& 0x02)!=0x02);
  CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSI);
  CLK_SYSCLKSourceSwitchCmd(ENABLE);
  while (((CLK->SWCR)& 0x01)==0x01);
  delay_ms(100);
}

/**
  * @brief  Run auto test
  * @caller main 
  * @param None
  * @retval None
  */ 
void auto_test(void)
{
  uint16_t tab[6]={0x20,0x20,0x20,0x20,0x20,0x20};
  
  Auto_test = TRUE;
	
	/* Switch off leds*/
  GPIO_LOW(LED_GREEN_PORT,LED_GREEN_PIN);	
  GPIO_LOW(LED_BLUE_PORT,LED_BLUE_PIN);		
  
  /* To display version */
  LCD_GLASS_DisplayString(" TEST ");
  delay_ms(150);
  STR_VERSION;
  LCD_GLASS_DisplayStrDeci(tab);
  delay_ms(200);
			
	/* And launch the tests*/
  test_LSE();
  test_vdd();
  test_icc_Run();
  test_icc_HALT();
  test_icc_LP();
  
  Auto_test = FALSE;

  /* Infinite loop: Press reset button at the end of test for exit*/
  while (1)
  {
    LCD_GLASS_ScrollSentence("TEST OK ",1,SCROLL_SPEED);
    KeyPressed = FALSE;
  }
}

/**
  * @brief Measures the BIAS current PJ1 Must be on OFF position
  * @caller main 
  * @param None
  * @retval None
  */  
void Bias_measurement(void)
{
  uint16_t V_Current;
  uint8_t B_Current;
  
  LCD_GLASS_ScrollSentence("      ** BIAS CURRENT ** JP1 OFF **",1,SCROLL_SPEED);	
  
  B_Current = ADC_Icc_Test(MCU_HALT);
  V_Current = (uint16_t)(B_Current * (Vdd_appli()/ADC_CONV)); 
  V_Current *= 10L;
  display_MuAmp(V_Current);

/* To store the value in E²Prom */
  FLASH_ProgramBias(B_Current);
	
  while (1)  /* Infinite loop for force to restart application */
  { 
    B_Current = ADC_Icc_Test(MCU_HALT);
    V_Current = (uint16_t)(B_Current * (Vdd_appli()/ADC_CONV)); 
    V_Current *= 10L;
    display_MuAmp(V_Current);
    delay_ms(300);
  }

}

/**
  * @brief converts a number into char
  * @caller several callers for display values
  * @param Number digit to displays
  *  p_tab values in array in ASCII   
  * @retval None
  */ 
void convert_into_char(uint16_t number, uint16_t *p_tab)
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
/**
  * @brief Function in RAM used for test low power and wait modes
  * @caller LPR_init
  * @param None   
  * @retval None
  */
/* Begin Section LPRUN */ 	
#ifdef _COSMIC_
#pragma section (LPRUN)
void	LPR_Ram(void)
#endif
#ifdef _RAISONANCE_
void	LPR_Ram(void) inram
#endif
#ifdef _IAR_
#pragma location="MY_RAM_FUNC"
void	LPR_Ram(void)
#endif
{ 
  uint8_t i = 0;

/* To reduce consumption to minimal 
  Swith off the Flash */
  FLASH->CR1 = 0x08;
  while(((CLK->REGCSR)&0x80)==0x80);
	
/* Swith off the Regulator*/
  CLK->REGCSR = 0x02;
  while(((CLK->REGCSR)&0x01)==0x01);

/* Set trigger on GPIOE pin6*/ 
  WFE->CR2 = 0x04;
  GPIOE->CR2 = 0x44;
  
  for (i=0; i<100; i++);

/* To start counter on falling edge*/
  GPIO_LOW(CTN_GPIO_PORT,CTN_CNTEN_GPIO_PIN);
  
/*Wait for end of counter */
  wfe();
  
  EXTI->SR1 |= 0x40;
  WFE->CR2 = 0x00;
  
  //Switch on the regulator
  CLK->REGCSR = 0x00;
  while(((CLK->REGCSR)&0x1) != 0x1);		
}
/* End Section LPRUN */
#ifdef _COSMIC_
#pragma section ()
#endif

/**
  * @brief Function to initialize the entry in low power and wait modes
  * @caller test low power mode
  * @param None   
  * @retval None
  */
void LPR_init(void)
{

/*Switch the clock to LSE and disable HSI*/
  #ifdef USE_LSE
    CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_LSE);	
    CLK_SYSCLKSourceSwitchCmd(ENABLE);
    while (((CLK->SWCR)& 0x01)==0x01);
    CLK_HSICmd(DISABLE);
  #else
    CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);
    CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_LSI);
    CLK_SYSCLKSourceSwitchCmd(ENABLE);
    while (((CLK->SWCR)& 0x01)==0x01);
    CLK_HSICmd(DISABLE);
  #endif

/*Configure event for WAKEUP and FUNCTION, disable the interrupts*/

  sim();
	
/* To copy function LPR_Ram in RAM section LPRUN*/
#ifdef _COSMIC_
  if (!(_fctcpy('L')))
    while(1);
#endif

  LPR_Ram(); // Call in RAM
  
  /*Switch the clock to HSI*/
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_8);
  CLK_HSICmd(ENABLE);
  while (((CLK->ICKCR)& 0x02)!=0x02);
  
  CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSI);
  CLK_SYSCLKSourceSwitchCmd(ENABLE);
  while (((CLK->SWCR)& 0x01)==0x01);

}

/**
  * @brief Function to return the VDD measurement
  * @caller All measurements: VDD display or Current
  * See STM8L152x4/6 and STM8L151x4/6 Errata sheet
  * Limitation: "Bandgap VREFINT_Factory_CONV byte value not programmed"
  *
  * 2 Methods for VDD measurement:
  * The first one offers a better accuracy
  *
  * 1st case: The VREFINT is stored in memory during factory tests
  * We use this value for better accuracy in this case
  *   Vdd_appli = ( VREF_Factory/Vref_measured ) * VDD_Factory 
  *   VDD_Factory = 3V+-10mV
  *   Vref_Factory +-5mV
  *
  * 2nd case: The VREFINT is not stored in memory.
  *   In this case:
  *   Vdd_appli = (Theorical_Vref/Vref mesure) * ADC_Converter
  *   Theorical_Vref = 1.224V
  *   ADC_Converter 4096
  *   ---> LSBIdeal = VREF/4096 or VDA/4096
  * @param None   
  * @retval VDD measurements
  */
float Vdd_appli(void)
{
  uint16_t MeasurINT,FullVREF_FACTORY  ;
  uint8_t *P_VREFINT_Factory ;
  float f_Vdd_appli ;
  
  P_VREFINT_Factory = VREFINT_Factory_CONV_ADDRESS;
  
  /*Read the BandGap value on ADC converter*/
  MeasurINT = ADC_Supply();	
  
  /* To check if VREFINT_Factory_CONV has been set
  the value is one byte we must add 0x600 to the factory byte */

/* For use VREFINT_Factory_CONV, we must to define VREFINT_FACTORY_CONV (file discover_functions.h */

#ifdef VREFINT_FACTORY_CONV
  if ((*P_VREFINT_Factory>VREFINT_Factory_CONV_MIN ) && (*P_VREFINT_Factory<VREFINT_Factory_CONV_MAX ))
  {
    /* If the value exists:
    Adds the hight byte to FullVREF_FACTORY */
    FullVREF_FACTORY = VREFINT_Factory_CONV_MSB;
    FullVREF_FACTORY += *P_VREFINT_Factory;
    f_Vdd_appli = (float)(FullVREF_FACTORY*VDD_FACTORY);
    f_Vdd_appli /= MeasurINT;
  } else {
    /* If the value doesn't exist (or not correct) in factory setting takes the theorical value 1.224 volt */
    f_Vdd_appli = (VREF/MeasurINT) * ADC_CONV;
  }
#else
    /* We use the theorcial value */
    f_Vdd_appli = (VREF/MeasurINT) * ADC_CONV;
#endif

/* Vdd_appli in mV */  
  f_Vdd_appli *= 1000L;
	
  return f_Vdd_appli;
}

/**
  * @brief Function to measure VDD
  * @caller main
  * @param None   
  * @retval Vdd value in mV
  */
uint16_t Vref_measure(void)
{
  uint16_t tab[6];	
  uint16_t Vdd_mV ;
  
  Vdd_mV = (uint16_t)Vdd_appli();

  convert_into_char (Vdd_mV, tab);
	
  /* To add unit and decimal point  */
  tab[5] = 'V';
  tab[4] = ' ';
  tab[1] |= DOT; /* To add decimal point for display in volt */
  tab[0] = ' ';
	
  LCD_GLASS_DisplayStrDeci(tab);

  return Vdd_mV;
}

/**
  * @brief funtion to display the current in µA
  * @caller several funcions
  * @param Current value.
  * @retval none
  */ 
void display_MuAmp (uint16_t Current)
{
  uint16_t tab[6];
          
  convert_into_char((int)Current, tab);
  tab[5] = 'A';
  tab[4] = 'µ';
		
/* Test the significant digit for displays 3 or 4 digits*/
  if ( tab[0] != '0')
  {
    tab[1] |= DOT; /* To add decimal point */
  }  else  {
    /* To shift for suppress '0' before decimal */
    tab[0] = tab[1] | DOT ;	
    tab[1] = tab[2] ;
    tab[2] = tab[3] ;		
    tab[3] = ' ';
  }
	
  LCD_GLASS_DisplayStrDeci(tab);
}

/**
  * @brief funtion Current measurement in RUN mode
  * @caller main and test_icc_RUN
  * @param none
  * @retval Current (mA)
  */ 
float Icc_measure_RUN(void)
{
  float Run_Conso;
  uint16_t MeasurINT;
  uint16_t tab[6];	
	
  MeasurINT = ADC_Icc_Test(MCU_RUN);
  
  Run_Conso = MeasurINT * Vdd_appli()/ADC_CONV;
  Run_Conso *= 10L; 
  
  convert_into_char((int)(Run_Conso*10), tab);

  /* To add unit and decimal point  */
  tab[5] = 'A';
  tab[4] = 'm';
  tab[3] = ' ';
  tab[0] |= DOT; /* To add decimal point for display in volt */

  LCD_GLASS_DisplayStrDeci(tab);
	
  return (Run_Conso);
}

/**
  * @brief funtion Current measurement in HALT mode
  * @caller main and test_icc_HALT
  * @param none
  * @retval Current (µA)
  */ 
float Icc_measure_HALT(void)
{
  float Current;
  uint16_t MeasurINT;
  
  /* To init the mode and measurement*/
  MeasurINT = ADC_Icc_Test(MCU_HALT);
  
  /* Substract bias curent*/
  MeasurINT -=	Bias_Current;
  Current = MeasurINT * Vdd_appli()/ADC_CONV;  

  Current *= 10L;
  
  if ((int) Current<MAX_CURRENT)
  {	
    display_MuAmp((int)Current);
  } else{
    LCD_GLASS_Clear();
    LCD_GLASS_DisplayString("Error");
  }
  
  return(Current);
}

/**
  * @brief funtion Current measurement in LOW POWER mode with LCD off
  * @caller main and test_icc_LCD
  * @param none
  * @retval Current (µA)
  */
float Icc_measure_LPR(void)
{
  float Current;
  uint16_t MeasurINT;
  
  /* To init the mode and measurement*/	
  MeasurINT = ADC_Icc_Test(MCU_LPR);
  
  /* Substract bias curent*/
  MeasurINT -= Bias_Current;
  
  Current = MeasurINT * Vdd_appli()/ADC_CONV; 
  Current *= 10L;
  
  /* To test if value is "normal"   */
  if ((int) Current<MAX_CURRENT)
  {	
    display_MuAmp((int)Current);
  }
  else
  {
    if (!Auto_test)
    {
      LCD_GLASS_Clear();
      LCD_GLASS_DisplayString("Error");
    }
  }

  return (Current);
}

/**
  * @brief funtion Current measurement in LOW POWER mode with LCD ON
  * @caller main
  * @param none
  * @retval none
  */
void Icc_measure_LPR_LCD(void)
{
  float Current;
  uint16_t MeasurINT;
	
  /* To init the mode and measurement*/		
  MeasurINT = ADC_Icc_Test(MCU_LPR_LCD);
  
  /* Substract bias curent*/
  MeasurINT -= Bias_Current;

  Current = MeasurINT * Vdd_appli()/ADC_CONV; 
  Current *= 10L;
  
  display_MuAmp((int)Current);

}	

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
