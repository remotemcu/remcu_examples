/**
  ******************************************************************************
  * @file    COMP/COMP_AnalogWatchdog/main.c 
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

/** @addtogroup COMP_AnalogWatchdog
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint32_t State = 0;

/* Private function prototypes -----------------------------------------------*/
static void COMP_Config(void);
static void STOPEntry(void);
static void RestoreConfiguration(void);

/* Private functions ---------------------------------------------------------*/

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

  /******* Initialize LEDs available on STM320518-EVAL board ******************/
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);
  
  /* PWR Peripheral clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

  /* configure COMP1 and COMP2 with interrupts enabled */
  COMP_Config();

  /* Check input voltage level: within the thresholds, above the upper threshold
     or under the lower threshold */
  CheckState();
  
  /* Infinite loop */
  while (1)
  {
    if (State == STATE_OVER_THRESHOLD)
    {
      /* Restore config: clock, GPIO... */
      RestoreConfiguration();
      
      /* Restore GPIO configuration */
      STM_EVAL_LEDInit(LED1);
      STM_EVAL_LEDInit(LED2);
      STM_EVAL_LEDInit(LED3);
      STM_EVAL_LEDInit(LED4);

      /* Turn on LD1 and LD3 and turn off LD2 and LD4 */
      STM_EVAL_LEDOn(LED1);
      STM_EVAL_LEDOff(LED2);
      STM_EVAL_LEDOn(LED3);
      STM_EVAL_LEDOff(LED4);
      
      while(State == STATE_OVER_THRESHOLD)
      {
         /* add your code here */
      }
    }
    else if (State == STATE_WITHIN_THRESHOLD)
    {
      /* Input voltage is within the thresholds: higher and lower thresholds */
      /* Enter STOP mode with regulator in low power */
      STOPEntry();
    }
    else /* (State == STATE_UNDER_THRESHOLD) */
    {
      /* Restore config: clock, GPIO... */
      RestoreConfiguration();

      /* Restore GPIO configuration */
      STM_EVAL_LEDInit(LED1);
      STM_EVAL_LEDInit(LED2);
      STM_EVAL_LEDInit(LED3);
      STM_EVAL_LEDInit(LED4);

      /* Turn on LD2 & LD4 and turn off LD1 & LD3 */
      STM_EVAL_LEDOff(LED1);
      STM_EVAL_LEDOn(LED2);
      STM_EVAL_LEDOff(LED3);
      STM_EVAL_LEDOn(LED4);

      while(State == STATE_UNDER_THRESHOLD)
      {
         /* add your code here */
      }
    }
  }
}

/**
  * @brief  Configure COMP1 and COMP2 with interrupt
  * @param  None
  * @retval None
  */
static void COMP_Config(void)
{
  COMP_InitTypeDef        COMP_InitStructure;
  EXTI_InitTypeDef        EXTI_InitStructure;
  NVIC_InitTypeDef        NVIC_InitStructure;
  GPIO_InitTypeDef        GPIO_InitStructure;
  
  /* GPIOA Peripheral clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

  /* Configure PA1: PA1 is used as COMP1 and COMP2 non inveting input */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* COMP Peripheral clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  
  /* COMP1 Init: the higher threshold is set to VREFINT ~ 1.22V
     but can be changed to other available possibilities */
  COMP_StructInit(&COMP_InitStructure);
  COMP_InitStructure.COMP_InvertingInput = COMP_InvertingInput_VREFINT;
  COMP_InitStructure.COMP_Output = COMP_Output_None;
  COMP_InitStructure.COMP_Mode = COMP_Mode_LowPower;
  COMP_InitStructure.COMP_Hysteresis = COMP_Hysteresis_High;
  COMP_Init(COMP_Selection_COMP1, &COMP_InitStructure);

  /* COMP2 Init: the lower threshold is set to VREFINT/4 ~ 1.22 / 4 ~ 0.305 V 
     but can be changed to other available possibilities */
  COMP_StructInit(&COMP_InitStructure);
  COMP_InitStructure.COMP_InvertingInput = COMP_InvertingInput_1_4VREFINT;
  COMP_InitStructure.COMP_Output = COMP_Output_None;
  COMP_InitStructure.COMP_Mode = COMP_Mode_LowPower;
  COMP_InitStructure.COMP_Hysteresis = COMP_Hysteresis_High;
  COMP_Init(COMP_Selection_COMP2, &COMP_InitStructure);

  /* Enable Window mode */
  COMP_WindowCmd(ENABLE);

  /* Enable COMP1: the higher threshold is set to VREFINT ~ 1.22 V */
  COMP_Cmd(COMP_Selection_COMP1, ENABLE);
  /* Enable COMP2: the lower threshold is set to VREFINT/4 ~ 0.305 V */
  COMP_Cmd(COMP_Selection_COMP2, ENABLE);

  /* Configure EXTI Line 21 in interrupt mode */
  EXTI_InitStructure.EXTI_Line = EXTI_Line21;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Configure EXTI Line 22 in interrupt mode */
  EXTI_InitStructure.EXTI_Line = EXTI_Line22;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Clear EXTI21 line */
  EXTI_ClearITPendingBit(EXTI_Line21);

  /* Clear EXTI22 line */
  EXTI_ClearITPendingBit(EXTI_Line22);

  /* Configure COMP IRQ */
  NVIC_InitStructure.NVIC_IRQChannel = ADC1_COMP_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  Prepare the system to enter STOP mode.
  * @param  None
  * @retval None
  */
static void STOPEntry(void)
{
  GPIO_InitTypeDef        GPIO_InitStructure;
  /* Enable GPIOs clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC |
                        RCC_AHBPeriph_GPIOD | RCC_AHBPeriph_GPIOF, ENABLE);

  /* Configure all GPIO port pins in Analog mode */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_Init(GPIOF, &GPIO_InitStructure);
  
  /* Request to enter STOP mode with regulator in low power */
  PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
}

/**
  * @brief  check input voltage level: within the thresholds, above the upper 
  *         threshold or under the lower threshold
  * @param  None
  * @retval None
  */
void CheckState(void)
{
  /* Check if COMP2 output level is high */
  if ((COMP_GetOutputLevel(COMP_Selection_COMP1) == COMP_OutputLevel_High) 
   && (COMP_GetOutputLevel(COMP_Selection_COMP2) == COMP_OutputLevel_High))
  {
    /* A rising edge is detected so the input voltage is higher than VREFINT */
    State = STATE_OVER_THRESHOLD;
  }
  else if ((COMP_GetOutputLevel(COMP_Selection_COMP1) == COMP_OutputLevel_Low)
       && (COMP_GetOutputLevel(COMP_Selection_COMP2) == COMP_OutputLevel_High))
  {
    /* A falling edge is detected so the input voltage is lower than VREFINT */
    State = STATE_WITHIN_THRESHOLD;
  }
  else if ((COMP_GetOutputLevel(COMP_Selection_COMP1) == COMP_OutputLevel_Low)
       && (COMP_GetOutputLevel(COMP_Selection_COMP2) == COMP_OutputLevel_Low))
  {
    State = STATE_UNDER_THRESHOLD;
  }
}

/**
  * @brief  Restore peripheral config before entering STOP mode.
  * @param  None
  * @retval None
  */
static void RestoreConfiguration(void)
{
  __IO uint32_t StartUpCounter = 0, HSEStatus = 0;
  
  /* SYSCLK, HCLK, PCLK configuration ----------------------------------------*/    
  /* Enable HSE */    
  RCC_HSEConfig(RCC_HSE_ON);
 
  /* Wait till HSE is ready and if Time out is reached exit */
  HSEStatus = RCC_WaitForHSEStartUp();

  if (HSEStatus == (uint32_t)0x01)
  {
    /* Enable Prefetch Buffer */
    FLASH_SetLatency(FLASH_Latency_1);
    
    /* HCLK = SYSCLK */
   RCC_HCLKConfig(RCC_SYSCLK_Div1); 
   
    /* PCLK = HCLK */
   RCC_PCLKConfig(RCC_HCLK_Div1);

    /*  PLL configuration:  = HSE *  6 = 48 MHz */
    RCC_PREDIV1Config(RCC_PREDIV1_Div1);
    RCC_PLLConfig(RCC_PLLSource_PREDIV1, RCC_PLLMul_6);
    
    /* Enable PLL */
    RCC_PLLCmd(ENABLE);
    
    /* PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
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
