/**
  ******************************************************************************
  * @file    I2C/I2C_TwoBoards/I2C_JoystickPlay/Slave/main.c
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

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

__IO uint8_t Slave_Buffer_Rx[255];

__IO uint8_t CommunicationEnd = 0x00;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void main(void)
{
  uint8_t i = 0;
  /* I2C  clock Enable*/
  CLK_PeripheralClockConfig(CLK_Peripheral_I2C1, ENABLE);

  /* system_clock / 1 */
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);

  /* Initialize LEDs mounted on STM8L1526-EVAL board */
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);
  STM_EVAL_LEDInit(LED5);

  I2C_DeInit(I2C1);
  /* Initialize I2C peripheral */

#ifdef I2C_slave_7Bits_Address
  I2C_Init(I2C1, 100000, SLAVE_ADDRESS,
           I2C_Mode_I2C, I2C_DutyCycle_2,
           I2C_Ack_Enable, I2C_AcknowledgedAddress_7bit);
#else
  I2C_Init(I2C1, 100000, SLAVE_ADDRESS,
           I2C_Mode_I2C, I2C_DutyCycle_2,
           I2C_Ack_Enable, I2C_AcknowledgedAddress_10bit);
#endif

  /* Enable Error Interrupt*/
  I2C_ITConfig(I2C1, (I2C_IT_TypeDef)(I2C_IT_ERR | I2C_IT_EVT | I2C_IT_BUF), ENABLE);

  /* Enable general interrupts */
  enableInterrupts();

  /*Main Loop */

  while (1)
  {
    while (CommunicationEnd != 0x01);
    CommunicationEnd = 0x00;
    switch (Slave_Buffer_Rx[0])
    {
      case 0x01 :
        for (i = 100; i > 0 ; i--)
          /* LD1 is on */
          STM_EVAL_LEDOn(LED1);
        /* all other leds are off */
        STM_EVAL_LEDOff(LED2);
        STM_EVAL_LEDOff(LED3);
        STM_EVAL_LEDOff(LED4);
        break;
      case 0x02:
        /* LD2 is on */
        STM_EVAL_LEDOn(LED2);
        /* all other leds are off */
        STM_EVAL_LEDOff(LED1);
        STM_EVAL_LEDOff(LED3);
        STM_EVAL_LEDOff(LED4);
        break;
      case 0x03:
        /* LED3 is on */
        STM_EVAL_LEDOn(LED3);
        /* all other leds are off */
        STM_EVAL_LEDOff(LED2);
        STM_EVAL_LEDOff(LED1);
        STM_EVAL_LEDOff(LED4);
        break;
      case 0x04:
        /* LED4 is on */
        STM_EVAL_LEDOn(LED4);
        /* all other leds are off */
        STM_EVAL_LEDOff(LED2);
        STM_EVAL_LEDOff(LED3);
        STM_EVAL_LEDOff(LED1);
        break;
      case 0xAA:
        /* All LEDs are on */
        STM_EVAL_LEDOn(LED1);
        STM_EVAL_LEDOn(LED2);
        STM_EVAL_LEDOn(LED3);
        STM_EVAL_LEDOn(LED4);
        break;
      default:
        break;
    }
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
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
