/**
  ******************************************************************************
  * @file    RTC/RTC_StopWatch/main.c 
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

/** @addtogroup RTC_StopWatch
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define MESSAGE1   " **** STOPWATCH ****" 
#define MESSAGE2   " LEFT         |      RIGHT  " 
#define MESSAGE3   " START        |       GET   " 
#define MESSAGE4   "  Press and hold TAMPER   " 
#define MESSAGE5   " to reset Backup registers" 
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint32_t SecondFraction = 0;
uint32_t BackupIndex = 0;
__IO uint32_t SubSecFrac = 0;
__IO uint8_t StartEvent = 0; 
__IO uint8_t Button_State =0;
__IO uint8_t Button_RTC =0;
uint8_t i =0;
extern __IO uint32_t CurrentTimeSec;
/* Define the backup register */
uint32_t BKPDataReg[5] = { RTC_BKP_DR0, RTC_BKP_DR1, RTC_BKP_DR2, RTC_BKP_DR3, RTC_BKP_DR4
                          };
__IO uint32_t CurrentTimeSec = 0;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static void RTC_Config(void);
static void RTC_TamperConfig(void);
static void Display_Init(void);

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

  RTC_TimeTypeDef  RTC_TimeStruct;   
  RTC_TimeTypeDef  RTC_TimeStructureInit;
  RTC_TimeTypeDef  RTC_StampTimeStruct;
  
  /* Configure the external interrupt "RIGHT" and "LEFT" buttons */
  STM_EVAL_PBInit(BUTTON_RIGHT,BUTTON_MODE_EXTI);
  STM_EVAL_PBInit(BUTTON_LEFT,BUTTON_MODE_EXTI);
  
  /* Configure the RTC peripheral by selecting the clock source.*/
  RTC_Config();
  
  /* LCD Display init  */
  Display_Init();
  
  /* Configure the RTC tamper register : To Clear all the Backup data register */
  RTC_TamperConfig();

  /* Initialize time Stucture */
  RTC_TimeStructInit(&RTC_TimeStruct); 

  /* Infinite loop */
  while (1)
  {
    /* Set the LCD Back Color and text size */
    LCD_SetFont(&Font16x24);
    LCD_SetBackColor(White);
    
    /* Check on the event 'start' */
    if(StartEvent != 0x0)
    {  
      /* Get the RTC sub second fraction */
      SecondFraction = (((256 - (uint32_t)RTC_GetSubSecond()) * 1000) / 256);
      
      /* Get the Curent time */
      RTC_GetTime(RTC_Format_BIN, &RTC_TimeStruct);
      
      /* Refresh : Display the curent time and the sub second on the LCD  */
      RTC_Time_Display(37, Black , RTC_Get_Time(SecondFraction , &RTC_TimeStruct));
    }
    else
    {
      /* Re-initialize the Display time on the LCD */
      RTC_Time_InitDisplay();
    }  
    
    /* Left button is pressed */
      if (Button_State == LEFT_ON)
      {
        /* Enable Tamper interrupt */
      RTC_ITConfig(RTC_IT_TAMP, ENABLE);
      /* Enabale the tamper 1 */
      RTC_TamperCmd(RTC_Tamper_1 , ENABLE);
      
      /* Set the LCD Back Color */
      LCD_SetBackColor(White);
      
      /* Clear the LCD line 5 */
      LCD_ClearLine(Line5);
      
      /* Get the current time */
      RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructureInit);
      CurrentTimeSec = (RTC_TimeStructureInit.RTC_Hours * 3600) + (RTC_TimeStructureInit.RTC_Minutes * 60) +
        RTC_TimeStructureInit.RTC_Seconds;     
      
      /* start count */
      StartEvent = 0x1;
      /* Reinitialize Button_State variable */
      Button_State = 0; 
      }
    /* Right button is pressed */
      else if (Button_State == RIGHT_ON)
      {
        uint16_t Colorx;
      
      if (BackupIndex < 5)
      {
        if((uint8_t)(BackupIndex% 2) != 0x0)
        { 
          /* Set LCD backcolor*/
          LCD_SetBackColor(Blue2);
          Colorx = White;
        }
        else
        {
          /* Set LCD backcolor*/
          LCD_SetBackColor(Cyan);
          Colorx = Black;
        }
        SubSecFrac = 0;
        /* Get the Current sub second and time */
        SubSecFrac = (((256 - (uint32_t)RTC_GetSubSecond()) * 1000) / 256);
        
        RTC_GetTime(RTC_Format_BIN, &RTC_StampTimeStruct);
        
        LCD_SetFont(&Font16x24);
        /* Display result on the LCD */
        RTC_Time_Display( LINE(3 + BackupIndex), Colorx, RTC_Get_Time(SubSecFrac , &RTC_StampTimeStruct) ); 
        
        if (BackupIndex < 2)
        {
          /* Save time register  to Backup register ( the first 5 register is reserved for time) */
          RTC_WriteBackupRegister(BKPDataReg[BackupIndex],(uint32_t)RTC->TR);
        
          /* Save sub second time stamp register ( the latest 6 register is reserved for time) */
          RTC_WriteBackupRegister(BKPDataReg[BackupIndex + 2], SubSecFrac);
        }
        
      }
      else
      {
        /* the backup register is full with 10trials */
        /* Set the LCD Back Color */
        LCD_SetBackColor(White);
        LCD_SetFont(&Font12x12);
        /* Set the LCD Text Color */
        LCD_SetTextColor(Red); 
        LCD_DisplayStringLine(LINE(16), (uint8_t *)MESSAGE4);
        LCD_DisplayStringLine(LINE(17), (uint8_t *)MESSAGE5);
        
      }  
      BackupIndex++;
      /* Set the LCD Back Color */
      LCD_SetBackColor(White);  
      /* Reinitialize Button_State variable */
      Button_State = 0; 
      }
   
   /* TAMPER button is pressed */
   else if (Button_RTC == RTC_TAMP)
   {
     /* Set the LCD Back Color */
    LCD_SetBackColor(White);
    LCD_SetFont(&Font16x24);
    /* Clear LCD line 5 to 9 */
    for (i=0; i < 5; i++)
    {  
      /* Clear all the LCD lignes from 3 to 7 */
      LCD_ClearLine(LINE(3+i));
    }
    
    /* reset Counter */
    BackupIndex = 0 ;
    
    /* Enetr to idle */
    StartEvent =0x0;
    
    LCD_SetFont(&Font12x12);
    RTC_Time_InitDisplay();
    
    LCD_SetFont(&Font12x12);
    LCD_ClearLine(LINE(16));
    LCD_ClearLine(LINE(17));
    
    /* Enable Tamper interrupt */
    RTC_ITConfig(RTC_IT_TAMP, DISABLE);
    /* Enabale the tamper 1 */
    RTC_TamperCmd(RTC_Tamper_1 , DISABLE);
    /* Reinitialize Button_RTC variable */
    Button_RTC = 0; 
   }
  }
}

/**
* @brief  RTC Tamper Configuration..
* @param  None
* @retval None
*/
static void RTC_TamperConfig(void)
{
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* EXTI configuration *********************************************************/
  EXTI_ClearITPendingBit(EXTI_Line19);
  EXTI_InitStructure.EXTI_Line = EXTI_Line19;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
  /* Enable RTC_IRQn */
  NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* determines the number of active pulse for the specific level */
  RTC_TamperFilterConfig(RTC_TamperFilter_2Sample);
  
  /* Determines the frequency at which each of the tamper inputs are sampled */
  RTC_TamperSamplingFreqConfig(RTC_TamperSamplingFreq_RTCCLK_Div32768);
  
  RTC_TamperPullUpCmd(DISABLE);
  
  /* Select the tamper 1 with High level */
  RTC_TamperTriggerConfig(RTC_Tamper_1, RTC_TamperTrigger_LowLevel );
  
  /* Clear tamper 1 flag */
  RTC_ClearFlag(RTC_FLAG_TAMP1F);
}

/**
* @brief  load the current time and sub second on a table form.
* @param   SecondFraction: the sub second fraction .
* @param  RTC_TimeStructure : pointer to a RTC_TimeTypeDef structure that 
*                             contains the current time values. 
* @retval table : return current time and sub second in a table form
*/
Table_TypeDef RTC_Get_Time(uint32_t SecondFrac , RTC_TimeTypeDef* RTC_TimeStructure )
{
  Table_TypeDef timetab;
  uint32_t currenttimesec = 0;
  
  /* Get the Current time in second */ 
  currenttimesec = ((RTC_TimeStructure->RTC_Hours * 3600) +(RTC_TimeStructure->RTC_Minutes * 60) +
                    RTC_TimeStructure->RTC_Seconds) - CurrentTimeSec; 
  
  /* Fill the table2 fields with the current Time*/
  timetab.tab[0]  = ((uint8_t)((currenttimesec/3600)/10) + 0x30);
  timetab.tab[1]  = ((uint8_t)((currenttimesec/3600) %10)+ 0x30); 
  timetab.tab[2]  = 0x3A;
  timetab.tab[3]  = (((uint8_t)(((currenttimesec % 3600)/60) /10)) + 0x30);
  timetab.tab[4]  = (((uint8_t)(((currenttimesec % 3600)/60) %10)) + 0x30);
  timetab.tab[5]  = 0x3A;
  timetab.tab[6]   = (((uint8_t)((currenttimesec% 60) /10))+ 0x30);
  timetab.tab[7]   = (((uint8_t)((currenttimesec% 60) %10)) + 0x30);
  timetab.tab[8]   = 0x2E;
  timetab.tab[9]   = (uint8_t)((SecondFrac / 100) + 0x30);
  timetab.tab[10]  = (uint8_t)(((SecondFrac % 100 ) / 10) +0x30);
  timetab.tab[11]  =  (uint8_t)((SecondFrac % 10) +0x30);
  
  /* return timetab */
  return timetab;
}

/**
* @brief  Configure the RTC peripheral by selecting the clock source.
* @param  None
* @retval None
*/
static void RTC_Config(void)
{
  RTC_InitTypeDef RTC_InitStructure;
  RTC_TimeTypeDef  RTC_TimeStruct;
    
  /* Enable the PWR clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
  
  /* Allow access to RTC */
  PWR_BackupAccessCmd(ENABLE);
  
  /* Enable the LSE OSC */
  RCC_LSEConfig(RCC_LSE_ON);
  
  /* Wait till LSE is ready */  
  while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
  {
  }
  
  /* Select the RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
  
  RTC_DeInit();
  
  /* Configure the RTC data register and RTC prescaler */
  RTC_InitStructure.RTC_AsynchPrediv = 0x7F;
  RTC_InitStructure.RTC_SynchPrediv  = 0xFF;
  RTC_InitStructure.RTC_HourFormat   = RTC_HourFormat_24;
  RTC_Init(&RTC_InitStructure);
  
  /* Set the time to 00h 00mn 00s AM */
  RTC_TimeStruct.RTC_H12     = RTC_H12_AM;
  RTC_TimeStruct.RTC_Hours   = 0x00;
  RTC_TimeStruct.RTC_Minutes = 0x00;
  RTC_TimeStruct.RTC_Seconds = 0x00;  
  RTC_SetTime(RTC_Format_BCD, &RTC_TimeStruct);
  
  
  /* Enable the RTC Clock */
  RCC_RTCCLKCmd(ENABLE);
  
  /* Wait for RTC APB registers synchronisation */
  RTC_WaitForSynchro();
}

/**
  * @brief  Display Init (LCD)
  * @param  None
  * @retval None
  */
static void Display_Init(void)
{
  /* Initialize the LCD */
#ifdef USE_STM320518_EVAL
    STM320518_LCD_Init();
#else
    STM32072B_LCD_Init();
#endif /* USE_STM320518_EVAL */

  /* Clear the LCD */
  LCD_Clear(LCD_COLOR_WHITE);
  
  /* Set the LCD Back Color */
  LCD_SetBackColor(Blue);
  
  /* Set the LCD Text Color */
  LCD_SetTextColor(White);
  
  /* Displays MESSAGE1 on line 1 */
  LCD_DisplayStringLine(LINE(0), (uint8_t *)MESSAGE1);
  
  /* Set the LCD Text Color */
  LCD_SetTextColor(Red);
  
  /* Set the LCD Back Color */
  LCD_SetBackColor(Red);
  LCD_DrawFullRect(31, 292,264,34);
  
  /* Set the LCD Back Color */
  LCD_SetBackColor(White);
  LCD_DrawFullRect(33, 290 ,260,30);
  
  
  /* Set the LCD Text, Back Colors and Text size */
  LCD_SetTextColor(Black); 
  LCD_SetBackColor(Cyan);
  LCD_SetFont(&Font12x12);
  
  LCD_DisplayStringLine(LINE(18), (uint8_t *)MESSAGE2);
  
  /* Set the LCD Back Color */
  LCD_SetBackColor(Blue);
  LCD_SetTextColor(White);
  
  LCD_DisplayStringLine(LINE(19), (uint8_t *)MESSAGE3);
  
  /* Set text size */
  LCD_SetFont(&Font16x24);
  
  /* Set the LCD Text Color */
  LCD_SetTextColor(Blue);
  
}

/**
* @brief  Initialize the time displays  on the LCD.
* @param  None.
* @retval None
*/
void RTC_Time_InitDisplay(void)
{
  uint8_t i = 0;
  for (i=0;i<12;i++)
  {
    if ((i==2) || (i==5))
    {
      /* Display char on the LCD */
      LCD_DisplayChar(37, (280 - (20 *i)), ':');
    }
    else if ((i==8) )
    {
      /* Display char on the LCD */
      LCD_DisplayChar(37, (280 - (20 *i)), '.');
    }
    else
    {
      /* Display char on the LCD */
      LCD_DisplayChar(37, (280 - (20 *i)), '0');
    }
  }
}

/**
* @brief  Displays the current Time on the LCD.
* @param   Line:  the Line where to display the Current time .
*   This parameter can be one of the following values:
*     @arg Linex: where x can be 0..9
* @param  Color_x : specifies the Background Color.
* @param  table   : the Current time and sub second.
* @retval None
*/
void RTC_Time_Display(uint8_t Line, __IO uint16_t Colorx, Table_TypeDef timetable)
{   
  uint8_t i = 0;
  
  /* Initialize table */
  LCD_SetTextColor(Colorx);
  
  for (i=0;i<12;i++)
  {
    /* Display char on the LCD */
    LCD_DisplayChar(Line, (280 - (20 *i)), timetable.tab[i]);
    
  }  
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
