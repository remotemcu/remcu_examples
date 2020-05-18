/**
  ******************************************************************************
  * @file    COMP/COMP_PulseWidthMeasurement/main.c 
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

/** @addtogroup COMP_PulseWidthMeasurement
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint32_t Capture = 0;
__IO uint32_t MeasuredPulse = 0;
__IO uint32_t DisplayActive = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static void DAC_Config(void);
static void COMP_Config(void);
static void TIM_Config(void);
static void DisplayOnLCD(uint32_t data);

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
    
    /* Clear the TFT-LCD */
    LCD_Clear(LCD_COLOR_WHITE);
      
    /* DAC Channel1 configuration */
    DAC_Config();
    
    /* COMP1 Configuration */
    COMP_Config();
    
    /* TIM2 Configuration in input capture mode */
    TIM_Config();
   

  /* Infinite loop */
  while (1)
  {
    if (DisplayActive != 0)
    {
      /* Compute the pulse width in us */
      MeasuredPulse = (uint32_t)(((uint64_t) Capture * 1000000) / ((uint32_t)SystemCoreClock));
      
      /* Display measured pulse width on Glass LCD and color LCD */
      DisplayOnLCD(MeasuredPulse);  
      DisplayActive = 0;
    }
  }
}

/**
  * @brief  Configures the DAC channel 1 with output buffer enabled.
  * @param  None
  * @retval None
  */
static void DAC_Config(void)
{

  /* Init Structure definition */
  DAC_InitTypeDef  DAC_InitStructure;
  
  /* DAC clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

  /* Fill DAC InitStructure */
  DAC_StructInit(&DAC_InitStructure);
  DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;
  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;

  /* DAC Channel1 Init */
  DAC_Init(DAC_Channel_1, &DAC_InitStructure);
  
  /* Enable DAC Channel1 */
  DAC_Cmd(DAC_Channel_1, ENABLE);
  
  /* Set DAC Channel1 DHR register: DAC_OUT1 = (3.3 * 2000) / 4095 ~ 1.61 V */
  DAC_SetChannel1Data(DAC_Align_12b_R, 2000);
}

/**
  * @brief  Configures COMP1: DAC channel 1 to COMP1 inverting input
  *                           and COMP1 output to TIM2 IC4.
  * @param  None
  * @retval None
  */
static void COMP_Config(void)
{

  /* Init Structure definition */
  COMP_InitTypeDef COMP_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  /* GPIOA Peripheral clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

  /* Configure PA1: PA1 is used as COMP1 non inveting input */
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* COMP Peripheral clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

  /* COMP1 Init: DAC1 output is used COMP1 inverting input */
  COMP_StructInit(&COMP_InitStructure);
  COMP_InitStructure.COMP_InvertingInput = COMP_InvertingInput_DAC1;
  /* Redirect COMP1 output to TIM2 Input capture 4 */
  COMP_InitStructure.COMP_Output = COMP_Output_TIM2IC4;
  COMP_InitStructure.COMP_Mode = COMP_Mode_HighSpeed;
  COMP_InitStructure.COMP_Hysteresis = COMP_Hysteresis_No;
  COMP_Init(COMP_Selection_COMP1, &COMP_InitStructure);

  /* Enable COMP1 */
  COMP_Cmd(COMP_Selection_COMP1, ENABLE);
}

/**
  * @brief  Configures TIM2 channel 4 in input capture mode
  * @param  None
  * @retval None
  */
static void TIM_Config(void)
{

  /* Init Structure definition */
  TIM_ICInitTypeDef TIM_ICInitStructure;
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  /* TIM2 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  
  /* TIM2 Time base configuration */
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period = 65535;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
  TIM_ClearFlag(TIM2, TIM_FLAG_Update);
  
  /* TIM2 Channel4 Input capture Mode configuration */
  TIM_ICStructInit(&TIM_ICInitStructure);
  TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;
  /* TIM2 counter is captured at each transition detection: rising or falling edges (both edges) */
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_BothEdge;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0;
  TIM_ICInit(TIM2, &TIM_ICInitStructure);
  
  /* TIM2 IRQChannel enable */  
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* Enable capture interrupt */
  TIM_ITConfig(TIM2, TIM_IT_CC4, ENABLE);
  
  /* Enable the TIM2 counter */
  TIM_Cmd(TIM2, ENABLE);
  
  /* Reset the flags */
  TIM2->SR = 0;
}

/**
  * @brief  Display measured pulse width on color LCD
  * @param  None
  * @retval None
  */
static void DisplayOnLCD(uint32_t value)
{
  uint8_t text[50];
  sprintf((char*)text,"PulseWidth = %d us  ",value);
  LCD_DisplayStringLine(LINE(5),text);
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
