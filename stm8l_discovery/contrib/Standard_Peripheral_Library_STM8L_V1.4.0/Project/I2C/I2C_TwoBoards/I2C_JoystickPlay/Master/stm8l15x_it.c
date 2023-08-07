/**
  ******************************************************************************
  * @file    I2C/I2C_TwoBoards/I2C_JoystickPlay/Master/stm8l15x_it.c
  * @author  MCD Application Team
  * @version V1.5.2
  * @date    30-September-2014
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all peripherals interrupt service routine.
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
#include "stm8l15x_it.h"
#include "main.h"
#include "stm8_eval.h"

/** @addtogroup STM8L15x_StdPeriph_Examples
  * @{
  */

/** @addtogroup I2C_TwoBoards
  * @{
  */

/** @addtogroup I2C_JoystickPlay
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
uint8_t HEADER_ADDRESS_Write = (((SLAVE_ADDRESS & 0xFF00) >> 7) | 0xF0);
/* Private variables ---------------------------------------------------------*/
extern __IO uint8_t TxBuffer[BUFFERSIZE];
extern __IO uint8_t PressedButton;
extern __IO uint8_t NumOfBytes;
__IO uint8_t Tx_Idx = 0;

/* Private function prototypes -----------------------------------------------*/
extern void TimingDelay_Decrement(__IO uint32_t TimingDelay);
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

#ifdef _COSMIC_
/**
  * @brief  Dummy interrupt routine
  * @param  None
  * @retval None
*/
INTERRUPT_HANDLER(NonHandledInterrupt, 0)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
#endif

/**
  * @brief  TRAP interrupt routine
  * @param  None
  * @retval None
*/
INTERRUPT_HANDLER_TRAP(TRAP_IRQHandler)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
/**
  * @brief  FLASH Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(FLASH_IRQHandler, 1)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
/**
  * @brief  DMA1 channel0 and channel1 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(DMA1_CHANNEL0_1_IRQHandler, 2)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
/**
  * @brief  DMA1 channel2 and channel3 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(DMA1_CHANNEL2_3_IRQHandler, 3)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief RTC / CSS_LSE Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(RTC_CSSLSE_IRQHandler, 4)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
/**
  * @brief  External IT PORTE/F and PVD Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTIE_F_PVD_IRQHandler, 5)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief External IT PORTB / PORTG Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTIB_G_IRQHandler, 6)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief External IT PORTD /PORTH Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTID_H_IRQHandler, 7)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief  External IT PIN0 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI0_IRQHandler, 8)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
#ifdef USE_STM8L1528_EVAL
  /* Joystick UP button is pressed */
  PressedButton = BUTTON_UP;
  /* Cleat Interrupt pending bit */
  EXTI_ClearITPendingBit(EXTI_IT_Pin0);
#endif /* USE_STM8L1528_EVAL */
}

/**
  * @brief  External IT PIN1 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI1_IRQHandler, 9)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
#ifdef USE_STM8L1526_EVAL
  /* Check if Joystick left button is pressed */
  if (STM_EVAL_PBGetState(BUTTON_LEFT) == RESET)
  {
    PressedButton = BUTTON_LEFT;
  }

  /* Check if Key button is pressed */
  if (STM_EVAL_PBGetState(BUTTON_KEY) == RESET)
  {
    PressedButton = BUTTON_KEY;
  }

  /* Cleat Interrupt pending bit */
  EXTI_ClearITPendingBit(EXTI_IT_Pin1);

#elif defined USE_STM8L1528_EVAL

  /* Check if Joystick down button is pressed */
  if (STM_EVAL_PBGetState(BUTTON_DOWN) == RESET)
  {
    PressedButton = BUTTON_DOWN;
  }

  /* Cleat Interrupt pending bit */
  EXTI_ClearITPendingBit(EXTI_IT_Pin1);

#endif /* USE_STM8L152X_EVAL */
}

/**
  * @brief  External IT PIN2 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI2_IRQHandler, 10)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
#ifdef USE_STM8L1526_EVAL

  /* Joystick SEL button is pressed */
  PressedButton = BUTTON_SEL;
  /* Cleat Interrupt pending bit */
  EXTI_ClearITPendingBit(EXTI_IT_Pin2);

#elif defined USE_STM8L1528_EVAL

  /* Joystick LEFT button is pressed */
  PressedButton = BUTTON_LEFT;
  /* Cleat Interrupt pending bit */
  EXTI_ClearITPendingBit(EXTI_IT_Pin2);

#endif /* USE_STM8L152X_EVAL */
}

/**
  * @brief  External IT PIN3 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI3_IRQHandler, 11)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
#ifdef USE_STM8L1528_EVAL
  /* Joystick RIGHT button is pressed */
  PressedButton = BUTTON_RIGHT;
  /* Cleat Interrupt pending bit */
  EXTI_ClearITPendingBit(EXTI_IT_Pin3);
#endif /* USE_STM8L1528_EVAL */
}

/**
  * @brief  External IT PIN4 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI4_IRQHandler, 12)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
#ifdef USE_STM8L1526_EVAL
  /* Joystick DOWN button is pressed */
  PressedButton = BUTTON_DOWN;
  /* Cleat Interrupt pending bit */
  EXTI_ClearITPendingBit(EXTI_IT_Pin4);

#elif defined USE_STM8L1528_EVAL

  /* Joystick SEL button is pressed */
  PressedButton = BUTTON_SEL;
  /* Cleat Interrupt pending bit */
  EXTI_ClearITPendingBit(EXTI_IT_Pin4);

#endif /* USE_STM8L152X_EVAL */
}

/**
  * @brief  External IT PIN5 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI5_IRQHandler, 13)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */

#ifdef USE_STM8L1526_EVAL
  /* Joystick RIGHT button is pressed */
  PressedButton = BUTTON_RIGHT;
  /* Cleat Interrupt pending bit */
  EXTI_ClearITPendingBit(EXTI_IT_Pin5);
#endif /* USE_STM8L1526_EVAL */
}

/**
  * @brief  External IT PIN6 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI6_IRQHandler, 14)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
#ifdef USE_STM8L1528_EVAL
  /* Key button is pressed */
  PressedButton = BUTTON_KEY;
  /* Cleat Interrupt pending bit */
  EXTI_ClearITPendingBit(EXTI_IT_Pin6);
#endif /* USE_STM8L1526_EVAL */
}

/**
  * @brief  External IT PIN7 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI7_IRQHandler, 15)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
  /* Joystick UP button is pressed */
  PressedButton = BUTTON_UP;

  /* Cleat Interrupt pending bit */
  EXTI_ClearITPendingBit(EXTI_IT_Pin7);
}

/**
  * @brief LCD /AES Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(LCD_AES_IRQHandler, 16)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
/**
  * @brief  CLK switch/CSS/TIM1 break Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(SWITCH_CSS_BREAK_DAC_IRQHandler, 17)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief  ADC1/Comparator Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(ADC1_COMP_IRQHandler, 18)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief TIM2 Update/Overflow/Trigger/Break /USART2 TX Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM2_UPD_OVF_TRG_BRK_USART2_TX_IRQHandler, 19)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief Timer2 Capture/Compare / USART2 RX Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM2_CC_USART2_RX_IRQHandler, 20)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}


/**
  * @brief Timer3 Update/Overflow/Trigger/Break Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM3_UPD_OVF_TRG_BRK_USART3_TX_IRQHandler, 21)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief Timer3 Capture/Compare /USART3 RX Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM3_CC_USART3_RX_IRQHandler, 22)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
/**
  * @brief  TIM1 Update/Overflow/Trigger/Commutation Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM1_UPD_OVF_TRG_COM_IRQHandler, 23)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief TIM1 Capture/Compare Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM1_CC_IRQHandler, 24)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief  TIM4 Update/Overflow/Trigger Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM4_UPD_OVF_TRG_IRQHandler, 25)
{
  /* In order to detect unexpected events during development,
      it is recommended to set a breakpoint on the following instruction.
   */

  /* This Delay is implemented to simulate load */
  TimingDelay_Decrement(TIM_DELAY);

  /* Cleat Interrupt Pending bit */
  TIM4_ClearITPendingBit(TIM4_IT_Update);
}
/**
  * @brief  SPI1 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(SPI1_IRQHandler, 26)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief USART1 TX / TIM5 Update/Overflow/Trigger/Break Interrupt  routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(USART1_TX_TIM5_UPD_OVF_TRG_BRK_IRQHandler, 27)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */

}

/**
  * @brief USART1 RX / Timer5 Capture/Compare Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(USART1_RX_TIM5_CC_IRQHandler, 28)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief I2C1 / SPI2 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(I2C1_SPI2_IRQHandler, 29)
{
  /* Read SR2 register to get I2C error */
  if (I2C_ReadRegister(I2C1, I2C_Register_SR2))
  {
    /* Clears SR2 register */
    I2C1->SR2 = 0;

    /* Set LED2 */
    STM_EVAL_LEDOn(LED2);
  }

  switch (I2C_GetLastEvent(I2C1))
  {
      /* EV5 */
    case I2C_EVENT_MASTER_MODE_SELECT :
#ifdef TEN_BITS_ADDRESS
      /* Send Header to Slave for write */
      I2C_SendData(I2C1, HEADER_ADDRESS_Write);
      break;

      /* EV9 */
    case I2C_EVENT_MASTER_MODE_ADDRESS10:
      /* Send slave Address for write */
      I2C_Send7bitAddress(I2C1, SLAVE_ADDRESS, I2C_Direction_Transmitter);
      break;
#else
      /* Send slave Address for write */
      I2C_Send7bitAddress(I2C1, SLAVE_ADDRESS, I2C_Direction_Transmitter);
      break;
#endif

      /* EV6 */
    case I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED:
      if (NumOfBytes != 0)
      {
        /* Send the first Data */
        I2C_SendData(I2C1, TxBuffer[Tx_Idx++]);
        /* Decrement number of bytes */
        NumOfBytes--;
      }
      if (NumOfBytes == 0)
      {
        I2C_ITConfig(I2C1, I2C_IT_BUF, DISABLE);
      }
      break;

      /* EV8 */
    case I2C_EVENT_MASTER_BYTE_TRANSMITTING:
      /* Transmit Data */
      I2C_SendData(I2C1, TxBuffer[Tx_Idx]);
      Tx_Idx++;
      /* Decrement number of bytes */
      NumOfBytes--;
      if (NumOfBytes == 0)
      {
        I2C_ITConfig(I2C1, I2C_IT_BUF, DISABLE);
      }
      break;

      /* EV8_2 */
    case I2C_EVENT_MASTER_BYTE_TRANSMITTED:
      /* Read DR register to clear BTF Flag */
      I2C1->DR;
      if (NumOfBytes == 0)
      {
        /* Send STOP condition */
        I2C_GenerateSTOP(I2C1, ENABLE);
        Tx_Idx = 0;
        I2C_ITConfig(I2C1, I2C_IT_EVT, DISABLE);
      }
      break;

    default:
      break;
  }

}

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