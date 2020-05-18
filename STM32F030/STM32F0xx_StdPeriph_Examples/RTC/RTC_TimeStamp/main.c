/**
  ******************************************************************************
  * @file    RTC/RTC_TimeStamp/main.c 
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

/** @addtogroup RTC_TimeStamp
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t AsynchPrediv = 0, SynchPrediv = 0;
uint32_t Secondfraction = 0;
__IO uint8_t Button_State = 0;
/* Private function prototypes -----------------------------------------------*/
static void RTC_Time_display(uint8_t Line,__IO uint16_t Color_x, Table_TypeDef table);
static Table_TypeDef RTC_Get_Time(uint32_t Secondfraction , RTC_TimeTypeDef* RTC_TimeStructure);
static Table_TypeDef RTC_Get_Date(RTC_DateTypeDef* RTC_DateStructure );
static void RTC_Config(void);
static void RTC_TimeRegulate(void);
static void RTC_TimeShow(void);
static void RTC_DateShow(void);
static void RTC_TimeStampShow(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f4xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f4xx.c file
     */     
#if defined (RTC_CLOCK_SOURCE_LSI) 
  /* Enable the LSI OSC */ 
  RCC_LSICmd(ENABLE);
#endif /* RTC_CLOCK_SOURCE_LSI */
  
  /* Configure the external interrupt "WAKEUP" and "TAMPER" buttons */ 
  STM_EVAL_PBInit(BUTTON_TAMPER , BUTTON_MODE_EXTI);
  STM_EVAL_PBInit(BUTTON_SEL , BUTTON_MODE_EXTI);

  /* Configure LEDs */
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDOn(LED1);
  
  /* Initialize the LCD */
#ifdef USE_STM320518_EVAL
    STM320518_LCD_Init();
#else
    STM32072B_LCD_Init();
#endif /* USE_STM320518_EVAL */

  /* Clear the LCD */ 
  LCD_Clear(White);
  
  /* Enable The Display */
  LCD_DisplayOn(); 
  
  /* Set the LCD Back Color and Text Color*/
  LCD_SetBackColor(Blue);
  LCD_SetTextColor(White);

 
  LCD_DisplayStringLine(LCD_LINE_0,(uint8_t *) "  TimeStamp Example " );
  
  /* Set the LCD Text size */
  LCD_SetFont(&Font16x24);
  
  if (RTC_ReadBackupRegister(RTC_BKP_DR0) != 0x32F2)
  {
    /* RTC configuration  */
    RTC_Config();

    /* Configure the time&date register */
    RTC_TimeRegulate(); 
    
    /* Display the Date */
    RTC_DateShow();
    
    /* Display the Time */
    RTC_TimeShow();
    
  }
  else
  {
    /* Check if the Power On Reset flag is set */
    if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
    {
      /* Set the Back Color */
      LCD_SetBackColor(LCD_COLOR_CYAN);
      
      /* Set the Text Color */
      LCD_SetTextColor(LCD_COLOR_BLACK);
      LCD_SetFont(&Font12x12);
      LCD_DisplayStringLine(LCD_LINE_2,(uint8_t *) "  Power On Reset occurred        " );
    }
    /* Check if the Pin Reset flag is set */
    else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
    {
      /* Set the Back Color */
      LCD_SetBackColor(LCD_COLOR_CYAN);
      
      /* Set the Text Color */
      LCD_SetTextColor(LCD_COLOR_BLACK);
      LCD_SetFont(&Font12x12);
      LCD_DisplayStringLine(LCD_LINE_2,(uint8_t *) "  External Reset occurred       " );
    }

    /* Set the Back Color */
    LCD_SetBackColor(LCD_COLOR_CYAN);
    
    /* Set the Text Color */
    LCD_SetTextColor(LCD_COLOR_BLACK);
    LCD_SetFont(&Font12x12);
    LCD_DisplayStringLine(LCD_LINE_3,(uint8_t *) " No need to configure RTC     " );
    
    /* Enable the PWR clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

    /* Allow access to RTC */
    PWR_BackupAccessCmd(ENABLE);

    /* Wait for RTC APB registers synchronisation */
    RTC_WaitForSynchro();

    /* Clear the RTC Alarm Flag */
    RTC_ClearFlag(RTC_FLAG_ALRAF);

    /* Clear the EXTI Line 17 Pending bit (Connected internally to RTC Alarm) */
    EXTI_ClearITPendingBit(EXTI_Line17);

    /* Display the RTC Time/Date and TimeStamp Time/Date */ 
    RTC_DateShow();
    RTC_TimeShow();

  }
   
  while (1)
  {
    if (Button_State == TAMPER_ON)
    {
      /* Turn LED4 ON */
      STM_EVAL_LEDOn(LED2);
      LCD_SetFont(&Font12x12);
      LCD_DisplayStringLine(LCD_LINE_5,(uint8_t *) " TimeStamp Event Occurred      " );
      LCD_SetFont(&Font16x24);
      LCD_ClearLine(LCD_LINE_4);
      LCD_ClearLine(LCD_LINE_5);
      LCD_ClearLine(LCD_LINE_6);
      LCD_ClearLine(LCD_LINE_7);
      LCD_ClearLine(LCD_LINE_8);
      /* Display the TimeStamp */
      RTC_TimeStampShow();
      Button_State = 0;
    }
    else if (Button_State == SEL_ON)
    {
      /* Turn LED1 ON and LED2 OFF */
      STM_EVAL_LEDOn(LED1);
      STM_EVAL_LEDOff(LED2);
      
      /* Clear The TSF Flag (Clear TimeStamp Registers) */
      RTC_ClearFlag(RTC_FLAG_TSF);
      LCD_SetFont(&Font12x12);
      LCD_ClearLine(LCD_LINE_5);
      LCD_SetFont(&Font12x12);
      LCD_DisplayStringLine(LCD_LINE_5,(uint8_t *) " TimeStamp Event Cleared        " );
      Button_State = 0;
    }
  }
}

/**
  * @brief  Configure the RTC peripheral by selecting the clock source.
  * @param  None
  * @retval None
  */
static void RTC_Config(void)
{
  RTC_InitTypeDef RTC_InitStructure;
  
  /* Enable the PWR clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

  /* Allow access to RTC */
  PWR_BackupAccessCmd(ENABLE);
  
    RCC_BackupResetCmd(ENABLE);
  RCC_BackupResetCmd(DISABLE);
  
#if defined (RTC_CLOCK_SOURCE_LSE) /* LSE used as RTC source clock */
  /* Enable the LSE OSC */
  RCC_LSEConfig(RCC_LSE_ON);

  /* Wait till LSE is ready */  
  while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
  {
  }

  /* Select the RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
  /* ck_spre(1Hz) = RTCCLK(LSE) /(AsynchPrediv + 1)*(SynchPrediv + 1)*/
  SynchPrediv = 0xFF;
  AsynchPrediv = 0x7F;
  
#elif defined (RTC_CLOCK_SOURCE_LSI)  /* LSI used as RTC source clock*/
 /* The RTC Clock may varies due to LSI frequency dispersion. */
  /* Enable the LSI OSC */ 
  RCC_LSICmd(ENABLE);

  /* Wait till LSI is ready */  
  while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
  {
  }

  /* Select the RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
  
  /* ck_spre(1Hz) = RTCCLK(LSI) /(AsynchPrediv + 1)*(SynchPrediv + 1)*/
  SynchPrediv = 0xFF;
  AsynchPrediv = 0x7F;
  
#else
  #error Please select the RTC Clock source inside the main.c file
#endif /* RTC_CLOCK_SOURCE_LSE */

   /* Configure the RTC data register and RTC prescaler */
  RTC_InitStructure.RTC_AsynchPrediv = AsynchPrediv;
  RTC_InitStructure.RTC_SynchPrediv = SynchPrediv;
  RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
  
  /* Check on RTC init */
  if (RTC_Init(&RTC_InitStructure) == ERROR)
  {
    /* Set the Back Color */
    LCD_SetBackColor(LCD_COLOR_WHITE);
    
    /* Set the Text Color */
    LCD_SetTextColor(LCD_COLOR_RED);
    LCD_SetFont(&Font12x12);
    LCD_DisplayStringLine(LCD_LINE_3,(uint8_t *) " RTC Prescaler Conf failed       " );
    LCD_SetFont(&Font16x24);
  }
  
  /* Enable the RTC Clock */
  RCC_RTCCLKCmd(ENABLE);
  
  /* Wait for RTC APB registers synchronisation */
  RTC_WaitForSynchro();
  
  /* Enable The TimeStamp */
  RTC_TimeStampCmd(RTC_TimeStampEdge_Falling, ENABLE);    
}

/**
  * @brief  Returns the time entered by user, using Hyperterminal.
  * @param  None
  * @retval None
  */
static void RTC_TimeRegulate(void)
{
  RTC_TimeTypeDef RTC_TimeStructure;
  RTC_DateTypeDef RTC_DateStructure;
  
  /* Set Time hh:mm:ss */
  RTC_TimeStructure.RTC_H12     = RTC_H12_AM;
  RTC_TimeStructure.RTC_Hours   = 0x08;  
  RTC_TimeStructure.RTC_Minutes = 0x10;
  RTC_TimeStructure.RTC_Seconds = 0x00;
  RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure);

  /* Set Date Week/Date/Month/Year */
  RTC_DateStructure.RTC_WeekDay = 01;
  RTC_DateStructure.RTC_Date = 0x31;
  RTC_DateStructure.RTC_Month = 0x12;
  RTC_DateStructure.RTC_Year = 0x12;
  RTC_SetDate(RTC_Format_BCD, &RTC_DateStructure);
  
  /* Write BkUp DR0 */
  RTC_WriteBackupRegister(RTC_BKP_DR0, 0x32F2);
}

/**
  * @brief  Display the current time on the Hyperterminal.
  * @param  None
  * @retval None
  */
static void RTC_TimeShow(void)
{
  RTC_TimeTypeDef RTC_TimeStructure;
  /* Get the current Time and Date */
  RTC_GetTime(RTC_Format_BCD, &RTC_TimeStructure);
   
  /* Set the Back Color */
  LCD_SetBackColor(LCD_COLOR_WHITE);
  
  /* Set the Text Color */
  LCD_SetTextColor(LCD_COLOR_BLUE);
  LCD_SetFont(&Font16x24);
  LCD_DisplayStringLine(LCD_LINE_4, (uint8_t *)"Current Time Display");
  /* Display the curent time and the sub second on the LCD */
  RTC_Time_display(LCD_LINE_5, Black , RTC_Get_Time(Secondfraction , &RTC_TimeStructure));
}

/**
  * @brief  Display the current date on the Hyperterminal.
  * @param  None
  * @retval None
  */
static void RTC_DateShow(void)
{
  RTC_DateTypeDef RTC_DateStructure;
  /* Get the current Date */
  RTC_GetDate(RTC_Format_BCD, &RTC_DateStructure);
  /* Set the Back Color */
  LCD_SetBackColor(LCD_COLOR_WHITE);
  
  /* Set the Text Color */
  LCD_SetTextColor(LCD_COLOR_BLUE);
  LCD_SetFont(&Font16x24);
  LCD_DisplayStringLine(LCD_LINE_6, (uint8_t *)"Current Date Display");
  /* Set the Text Color */
  LCD_SetTextColor(LCD_COLOR_BLACK);
  RTC_Time_display( LCD_LINE_7, Black,  RTC_Get_Date( &RTC_DateStructure));
}

/**
  * @brief  Display the current TimeStamp (time and date) on the Hyperterminal.
  * @param  None
  * @retval None
  */
static void RTC_TimeStampShow(void)
{
  RTC_TimeTypeDef  RTC_TimeStampStructure;
  RTC_DateTypeDef  RTC_TimeStampDateStructure;
  /* Get the current TimeStamp */
  RTC_GetTimeStamp(RTC_Format_BCD, &RTC_TimeStampStructure, &RTC_TimeStampDateStructure);

  /* Set the Back Color */
  LCD_SetBackColor(LCD_COLOR_WHITE);
  
  /* Set the Text Color */
  LCD_SetTextColor(LCD_COLOR_BLUE);
  LCD_SetFont(&Font16x24);
  LCD_DisplayStringLine(LCD_LINE_4, (uint8_t *)"TimeStamp Display");
  /* Display the curent time and the sub second on the LCD */
  LCD_DisplayStringLine(LCD_LINE_5, (uint8_t *) "Time");
  RTC_Time_display(LCD_LINE_5, Black , RTC_Get_Time(Secondfraction , &RTC_TimeStampStructure));
  /* Set the Text Color */
  LCD_SetTextColor(LCD_COLOR_BLUE);
  LCD_DisplayStringLine(LCD_LINE_6, (uint8_t *) "Date");
  RTC_Time_display(LCD_LINE_6, Black,  RTC_Get_Date( &RTC_TimeStampDateStructure));
}

/**
  * @brief  Displays the current Time on the LCD.
  * @param  Line:  the Line where to display the Current time .
  *           This parameter can be one of the following values:
  *             @arg Linex: where x can be 0..9
  * @param  Color_x: specifies the Background Color.
  * @param  table: the Current time and sub second.
  * @retval None
  */
static void RTC_Time_display(uint8_t Line,__IO uint16_t Color_x, Table_TypeDef table )
{   
  uint8_t i = 0;

  /* Initialize table */
  LCD_SetTextColor(Color_x);
  
  for (i=0;i<8;i++)
  {
    /* Display char on the LCD */
    LCD_DisplayChar(Line, (250 - (20 *i)), table.tab[i]);
  }  
}

/**
  * @brief  Returns the current time and sub second.
  * @param  Secondfraction: the sub second fraction.
  * @param  RTC_TimeStructure : pointer to a RTC_TimeTypeDef structure that 
  *         contains the current time values. 
  * @retval table : return current time and sub second in a table form
  */
static Table_TypeDef RTC_Get_Time(uint32_t Secondfraction , RTC_TimeTypeDef* RTC_TimeStructure )
{
  Table_TypeDef table2;

  /* Fill the table2 fields with the current Time*/
  table2.tab[0] = (((uint8_t)(RTC_TimeStructure->RTC_Hours & 0xF0) >> 0x04) + 0x30);
  table2.tab[1]  = (((uint8_t)(RTC_TimeStructure->RTC_Hours & 0x0F))+ 0x30);
  table2.tab[2]  = 0x3A;
  
  table2.tab[3]  = (((uint8_t)(RTC_TimeStructure->RTC_Minutes & 0xF0) >> 0x04) + 0x30);
  table2.tab[4]  =(((uint8_t)(RTC_TimeStructure->RTC_Minutes & 0x0F))+ (uint8_t)0x30);
  table2.tab[5]  = 0x3A;

  table2.tab[6]   = (((uint8_t)(RTC_TimeStructure->RTC_Seconds & 0xF0) >> 0x04)+ 0x30);
  table2.tab[7]   = (((uint8_t)(RTC_TimeStructure->RTC_Seconds & 0x0F)) + 0x30);
  table2.tab[8]   = 0xA0;
  
  /* return table2 */
  return table2;
}

/**
  * @brief  Returns the current time and sub second.
  * @param  Secondfraction: the sub second fraction.
  * @param  RTC_TimeStructure : pointer to a RTC_TimeTypeDef structure that 
  *         contains the current time values. 
  * @retval table : return current time and sub second in a table form
  */
static Table_TypeDef RTC_Get_Date(RTC_DateTypeDef* RTC_DateStructure )
{
  Table_TypeDef table2;

  /* Fill the table2 fields with the current Time*/
  table2.tab[0] = (((uint8_t)(RTC_DateStructure->RTC_Date & 0xF0) >> 0x04) + 0x30);
  table2.tab[1]  = (((uint8_t)(RTC_DateStructure->RTC_Date & 0x0F))+ 0x30);
  table2.tab[2]  = 0x2F;
  
  table2.tab[3]  = (((uint8_t)(RTC_DateStructure->RTC_Month & 0xF0) >> 0x04) + 0x30);
  table2.tab[4]  =(((uint8_t)(RTC_DateStructure->RTC_Month & 0x0F))+ (uint8_t)0x30);
  table2.tab[5]  = 0x2F;

  table2.tab[6]   = (((uint8_t)(RTC_DateStructure->RTC_Year & 0xF0) >> 0x04)+ 0x30);
  table2.tab[7]   = (((uint8_t)(RTC_DateStructure->RTC_Year & 0x0F)) + 0x30);
  table2.tab[8]   = 0xA0;
 
  /* return table2 */
  return table2;
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
