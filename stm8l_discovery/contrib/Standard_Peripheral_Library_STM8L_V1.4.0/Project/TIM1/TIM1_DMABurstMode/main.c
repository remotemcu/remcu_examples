/**
  ******************************************************************************
  * @file    TIM1/TIM1_DMABurstMode/main.c
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


/** @addtogroup STM8L15x_StdPeriph_Examples
  * @{
  */
/** @addtogroup TIM1_DMABurstMode
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t PSCRHcurrentValue = 0;
uint8_t PSCRLcurrentValue = 0;
uint8_t ARRHcurrentValue  = 0;
uint8_t ARRLcurrentValue  = 0;
uint8_t RCRcurrentValue   = 0;
uint8_t CCR1HcurrentValue = 0;
uint8_t CCR1LcurrentValue = 0;

extern uint8_t RAMBuffer1[];
extern uint8_t RAMBuffer2[];

/* Private function prototypes -----------------------------------------------*/
static void CLK_Config(void);
static void GPIO_Config(void);
static void TIM1_Config(void);
void RAM_InitBuffers(void);

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

   /* GPIO configuration -------------------------------------------*/
  GPIO_Config(); 
    
  /* Configure key button in EXTI mode */
  STM_EVAL_PBInit(BUTTON_KEY, BUTTON_MODE_EXTI);

  /* Init RAM Buffers */
  RAM_InitBuffers();

   /* TIM1 configuration -------------------------------------------*/
  TIM1_Config();

  /* Enable global interrupts */
  enableInterrupts();

  while (1)
  {
    PSCRHcurrentValue = TIM1->PSCRH;
    PSCRLcurrentValue = TIM1->PSCRL;
    ARRHcurrentValue  = TIM1->ARRH;
    ARRLcurrentValue  = TIM1->ARRL;
    RCRcurrentValue   = TIM1->RCR;
    CCR1HcurrentValue = TIM1->CCR1H;
    CCR1LcurrentValue = TIM1->CCR1L;
  }
}

/**
  * @brief  Configure peripherals Clock   
  * @param  None
  * @retval None
  */
static void CLK_Config(void)
{
  /* Enable DMA1 clock */
  CLK_PeripheralClockConfig(CLK_Peripheral_DMA1, ENABLE);

  /* Enable TIM1 clock */
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM1, ENABLE);
}

/**
  * @brief  Configure TIM Channels GPIO 
  * @param  None
  * @retval None
  */
static void GPIO_Config(void)
{
  /* TIM1 Channel1 configuration: PD2 in output pushpull mode */
  GPIO_Init(GPIOD,  GPIO_Pin_2, GPIO_Mode_Out_PP_Low_Fast);
}

/**
  * @brief  Configure TIM1 peripheral 
  * @param  None
  * @retval None
  */
static void TIM1_Config(void)
{
  /* Configure TIM1 channel 1 to generate a PWM output */
  TIM1_OC1Init(TIM1_OCMode_PWM1, TIM1_OutputState_Enable,
               TIM1_OutputNState_Disable, 0/* TIM1_Pulse */,
               TIM1_OCPolarity_Low, TIM1_OCNPolarity_Low,
               TIM1_OCIdleState_Reset, TIM1_OCNIdleState_Reset);
  /* Enable TIM1 outputs */
  TIM1_CtrlPWMOutputs(ENABLE);

  /* Configure base address and buffer length for DMA burst mode */
  TIM1_DMAConfig(TIM1_DMABase_PSCH, TIM1_DMABurstLength_7Byte);
  /* Enable commutation event as DMA request source */
  TIM1_DMACmd(TIM1_DMASource_COM, ENABLE);

  /* Enable TIM1 counter */
  TIM1_Cmd(ENABLE);
}

/**
  * @brief  Init buffers RAMBuffer1 and RAMBuffer2.
  * @param  None
  * @retval None
  */
void RAM_InitBuffers(void)
{
  /* RAMBuffer1 Init*/
  RAMBuffer1[0] = 0xFF; /* PSCRH new value*/
  RAMBuffer1[1] = 0xEE; /* PSCRL new value*/
  RAMBuffer1[2] = 0xDD; /* ARRH new value*/
  RAMBuffer1[3] = 0xCC; /* ARRL new value*/
  RAMBuffer1[4] = 0xBB; /* RCR new value*/
  RAMBuffer1[5] = 0xAA; /* CCR1H new value*/
  RAMBuffer1[6] = 0x99; /* CCR1L new value*/

  /* RAMBuffer2 Init*/
  RAMBuffer2[0] = 0x77; /* PSCRH new value*/
  RAMBuffer2[1] = 0x66; /* PSCRL new value*/
  RAMBuffer2[2] = 0x55; /* ARRH new value*/
  RAMBuffer2[3] = 0x44; /* ARRL new value*/
  RAMBuffer2[4] = 0x33; /* RCR new value*/
  RAMBuffer2[5] = 0x22; /* CCR1H new value*/
  RAMBuffer2[6] = 0x11; /* CCR1L new value*/
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
while(1)
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
