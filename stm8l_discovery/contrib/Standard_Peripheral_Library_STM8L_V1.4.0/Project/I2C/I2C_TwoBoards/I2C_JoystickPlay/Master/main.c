/**
  ******************************************************************************
  * @file    I2C/I2C_TwoBoards/I2C_JoystickPlay/Master/main.c
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
#include "main.h"

/** @addtogroup STM8L15x_StdPeriph_Examples
  * @{
  */

/** @addtogroup I2C_TwoBoards
  * @{
  */

/** @addtogroup I2C_JoystickPlay
  * @{
  */

/* Private defines -----------------------------------------------------------*/
#define NO_BUTTON 0x06
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint8_t TxBuffer[BUFFERSIZE];
__IO uint8_t NumOfBytes = BUFFERSIZE;
__IO uint8_t ButtonPressed = 0;
__IO uint8_t PressedButton = 0x07;
uint8_t i = 0;

/* Private function prototypes -----------------------------------------------*/
void TimingDelay_Decrement(__IO uint32_t TimingDelay);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void main()
{
  /* TIM4 & I2C  clock Enable*/
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, ENABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_I2C1, ENABLE);

#ifdef FAST_I2C_MODE
  /* system_clock / 1 */
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);
#else
  /* system_clock / 2 */
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_2);
#endif

  /* Initialize LEDs mounted on STM8L1526-EVAL board */
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);

  /* Initialize push-buttons mounted on STM8L1526-EVAL board */
  STM_EVAL_PBInit(BUTTON_RIGHT, BUTTON_MODE_EXTI);
  STM_EVAL_PBInit(BUTTON_LEFT, BUTTON_MODE_EXTI);
  STM_EVAL_PBInit(BUTTON_UP, BUTTON_MODE_EXTI);
  STM_EVAL_PBInit(BUTTON_DOWN, BUTTON_MODE_EXTI);
  STM_EVAL_PBInit(BUTTON_SEL, BUTTON_MODE_EXTI);

  /* Initialize TIM4 peripheral to generate an interrupt each 1ms */
  TIM4_TimeBaseInit(TIM4_Prescaler_128, TIM4_PERIOD);
  /* Enable Update interrupt */
  TIM4_ITConfig(TIM4_IT_Update, ENABLE);

  /* Initialize I2C peripheral */
  I2C_Init(I2C1, I2C_SPEED, 0xA0,
           I2C_Mode_I2C, I2C_DutyCycle_2,
           I2C_Ack_Enable, I2C_AcknowledgedAddress_7bit);

  /* Set I2C IT software priority as highest */
#ifdef STM8L15X_MD
  ITC_SetSoftwarePriority(I2C1_IRQn, ITC_PriorityLevel_3);
#elif defined (STM8L15X_HD) || defined (STM8L15X_MDP)
  ITC_SetSoftwarePriority(I2C1_SPI2_IRQn, ITC_PriorityLevel_3);
#endif  /* STM8L15X_MD */

  enableInterrupts();

  /* Enable TIM4 */
  TIM4_Cmd(ENABLE);

  while (1)
  {
    switch (PressedButton)
    {
      case BUTTON_RIGHT:
        PressedButton = NO_BUTTON;
        NumOfBytes = 1;
        TxBuffer[NumOfBytes-1] = 0x01;
        ButtonPressed = 1;
        break;
      case BUTTON_LEFT:
        PressedButton = NO_BUTTON;
        NumOfBytes = 1;
        TxBuffer[NumOfBytes-1] = 0x02;
        ButtonPressed = 1;
        break;
      case BUTTON_UP:
        PressedButton = NO_BUTTON;
        NumOfBytes = 1;
        TxBuffer[NumOfBytes-1] = 0x03;
        ButtonPressed = 1;
        break;
      case BUTTON_DOWN:
        PressedButton = NO_BUTTON;
        NumOfBytes = 1;
        TxBuffer[NumOfBytes-1] = 0x04;
        ButtonPressed = 1;
        break;
      case BUTTON_SEL:
        PressedButton = NO_BUTTON;
        NumOfBytes = BUFFERSIZE;
        TxBuffer[0] = 0xAA;
        ButtonPressed = 1;
        for (i = 1; i < NumOfBytes; i++)
        {
          TxBuffer[i] = i;
        }
        break;
      default:
        break;
    }
    if (ButtonPressed == 1)
    {
      /* Enable Buffer and Event Interrupt*/
      I2C_ITConfig(I2C1, (I2C_IT_TypeDef)(I2C_IT_EVT | I2C_IT_BUF) , ENABLE);

      /* Generate the Start condition */
      I2C_GenerateSTART(I2C1, ENABLE);

      /*
        Data transfer is performed in the I2C interrupt routine
        
       */
      /* Wait until end of data transfer */
      while (NumOfBytes);
      while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));

      /* Make sure that the STOP bit is cleared by Hardware before CR2 write access */
      while ((I2C1->CR2 & I2C_CR2_STOP) == I2C_CR2_STOP);
      ButtonPressed = 0;
    }
  }
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(__IO uint32_t TimingDelay)
{
  if (TimingDelay != 0x00)
  {
    TimingDelay--;
  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
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

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/