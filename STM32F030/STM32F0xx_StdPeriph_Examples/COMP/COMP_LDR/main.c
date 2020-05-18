/**
  ******************************************************************************
  * @file    COMP/COMP_LDR/main.c 
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

/** @addtogroup COMP_LDR
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define MESSAGE1   "        LDR         "

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t ldrlevel = 11, ldrlevelp = 0, daclevel = 0;
uint16_t tmp = 0;
  
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static void COMP_Config(void);
static void DAC_Config(void);
void Delay(__IO uint32_t nTime);

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


  /* Initialize the TFT-LCD */
#ifdef USE_STM320518_EVAL
  STM320518_LCD_Init();
#else 
  STM32072B_LCD_Init();
#endif /* USE_STM320518_EVAL */
  
  /* Clear the LCD */ 
  LCD_Clear(LCD_COLOR_BLACK);
  
  /* Set the LCD Back Color */
  LCD_SetBackColor(LCD_COLOR_BLACK);
  
  /* Set the LCD Text Color */
  LCD_SetTextColor(LCD_COLOR_WHITE);
  
  /* Displays Light dependent resistor (LDR) message on line 1 */
  LCD_DisplayStringLine(LINE(0), (uint8_t *)MESSAGE1);
  
  /* COMP Configuration */
  COMP_Config();
 
  /* AC channel 2 Configuration */
  DAC_Config();
  
  
  /* Infinite loop */
  while (1)
  {
    for(daclevel = 0; daclevel < 11; daclevel++)
    {
      /* Set DAC Channel2 DHR register: DAC_OUT2 = (3.3 * 868) / 4095 ~ 0.7 V */
      DAC_SetChannel1Data(DAC_Align_12b_R, (uint16_t)(daclevel * 150));

      Delay((10000));
      /* Check on the Comp output level*/
      if (COMP_GetOutputLevel(COMP_Selection_COMP1) == COMP_OutputLevel_High)
      {
        ldrlevel--;
      }
    }

    switch(ldrlevel)
    {
    case 1:
      /* Displays MESSAGE on line 7 */
      LCD_DisplayStringLine(LINE(7), "       Level 0      ");
      break;
      
    case 2:
      /* Displays MESSAGE on line 7 */
      LCD_DisplayStringLine(LINE(7), "       Level 1      ");
      break;
      
    case 3:
      /* Displays MESSAGE on line 7  */
      LCD_DisplayStringLine(LINE(7), "       Level 2      ");
      break;
      
    case 4:
      /* Displays MESSAGE on line 7  */
      LCD_DisplayStringLine(LINE(7), "       Level 3      ");
      break;
      
    case 5:
      /* Displays MESSAGE on line 7  */
      LCD_DisplayStringLine(LINE(7), "       Level 4      ");
      break;
      
    case 6:
      /* Displays MESSAGE on line 7  */
      LCD_DisplayStringLine(LINE(7), "       Level 5      ");
      break;
      
    case 7:
      /* Displays MESSAGE on line 7  */
      LCD_DisplayStringLine(LINE(7), "       Level 6      ");
      break;
      
    case 8:
      /* Displays MESSAGE on line 7  */
      LCD_DisplayStringLine(LINE(7), "       Level 7      ");
      break;
      
    case 9:
      /* Displays MESSAGE on line 7 */
      LCD_DisplayStringLine(LINE(7), "       Level 8      ");
      break;
      
    case 10:
      /* Displays MESSAGE on line 7  */
      LCD_DisplayStringLine(LINE(7), "       Level 9      ");
      break;
      
    case 11:
      /* Displays MESSAGE on line 7  */
      LCD_DisplayStringLine(LINE(7), "       Level 10     ");
      break;
      
    default :
      /* Displays MESSAGE on line 7  */
      LCD_DisplayStringLine(LINE(7), "       Level 0      ");
      ldrlevel = 1;
      break;
    }
    
    if(ldrlevelp != ldrlevel)
    {
      /* Set the LCD Text Color */
      LCD_SetTextColor(LCD_COLOR_WHITE);
      
      /* Displays a rectangle on the LCD */
      LCD_DrawRect(199, 311, 22, 302 );
      
      /* Set the LCD Back Color */
      LCD_SetBackColor(LCD_COLOR_BLACK);
      LCD_SetTextColor(LCD_COLOR_BLACK);
      LCD_DrawFullRect(200, 310,  300, 20);
      
      /* Set the LCD Text Color */
      LCD_SetTextColor(LCD_COLOR_YELLOW);
      
      /* Set the LCD Back Color */
      LCD_SetBackColor(LCD_COLOR_YELLOW);
      
      /* Displays a full rectangle */
      tmp = 30 * (ldrlevel-1);
      if (tmp ==0) tmp = 5;
      LCD_DrawFullRect(200, 310, tmp , 20);
    }
    
    ldrlevelp = ldrlevel;
    ldrlevel = 11;
    
    /* Set the LCD Back Color */
    LCD_SetBackColor(LCD_COLOR_BLACK);
  }

}

/**
  * @brief COMP Configuration.
  * @param None
  * @retval None
  */
static void COMP_Config(void)
{
  COMP_InitTypeDef COMP_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  /* GPIOB Peripheral clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
 
  /* Configure PA1 in analog mode: PA1 is connected to COMP1 non inverting input */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /* Enable SYSCFG clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  
  /* COMP1 Init: COMP1 is enabled as soon as inverting input is selected */
  /* use DAC1 output as a reference voltage: DAC1 output is connected to COMP1
  inverting input */
  COMP_InitStructure.COMP_InvertingInput = COMP_InvertingInput_DAC1;
  COMP_InitStructure.COMP_Output = COMP_Output_None;
  COMP_InitStructure.COMP_OutputPol = COMP_OutputPol_NonInverted;
  COMP_InitStructure.COMP_Hysteresis = COMP_Hysteresis_High;
  COMP_InitStructure.COMP_Mode = COMP_Mode_UltraLowPower;
  COMP_Init(COMP_Selection_COMP1, &COMP_InitStructure);
  
  /* Enable the COMP peripheral */
  COMP_Cmd(COMP_Selection_COMP1, ENABLE);
}

/**
  * @brief  Configures the DAC channel 2 with output buffer enabled.
  * @param  None
  * @retval None
  */
static void DAC_Config(void)
{
  DAC_InitTypeDef  DAC_InitStructure;
  
  /* DAC clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
  
  /* Deinitialize DAC */
  DAC_DeInit();

  DAC_StructInit(&DAC_InitStructure);
   
  /* Fill DAC InitStructure */ 
  /* DAC Channel1: 12bit right */
  /* DAC Channel1 Init */
  DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;
  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;
  DAC_Init(DAC_Channel_1, &DAC_InitStructure);

  /* Enable DAC Channel1 */
  DAC_Cmd(DAC_Channel_1, ENABLE);
}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length.
  * @retval None
  */
void Delay(__IO uint32_t nTime)
{ 
  while(nTime != 0)
  {
  nTime--;
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
