/**
  ******************************************************************************
  * @file    USART/USART_IrDA/main.c
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

/**
  * @addtogroup USART_IrDA
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define Bit_RESET  0
#define Bit_SET    1

/* Uncomment the line below according to the target USART Transmit/Receive used
   in the IrDA example.*/
#define  USART_IrDA_TRANSMIT  /* USART in IrDA transmit mode. */
//#define  USART_IrDA_RECEIVE   /* USART in IrDA receive mode. */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void USART_Config(void);
JOYState_TypeDef ReadJoystick(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void main(void)
{
#ifdef USART_IrDA_TRANSMIT
  JOYState_TypeDef Key = (JOYState_TypeDef)0;
#else /* USART_IrDA_RECEIVE */
  uint8_t ReceivedData = 0;
#endif /*USART_IrDA_TRANSMIT*/

   /* USART configuration -------------------------------------------*/
  USART_Config();

#ifdef USART_IrDA_TRANSMIT
  while (1)
  {
    /* Read Key */
    while (Key == JOY_NONE)
    {
      Key = ReadJoystick();
    }

    switch (Key)
    {
      case JOY_UP:
        USART_SendData8(EVAL_COM1, JOY_UP);
        while (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TC) == RESET)
        {}
        Key = JOY_NONE;
        break;
      case JOY_DOWN:
        USART_SendData8(EVAL_COM1, JOY_DOWN);
        while (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TC) == RESET)
        {}
        Key = JOY_NONE;
        break;
      case JOY_LEFT:
        USART_SendData8(EVAL_COM1, JOY_LEFT);
        while (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TC) == RESET)
        {}
        Key = JOY_NONE;
        break;
      case JOY_RIGHT:
        USART_SendData8(EVAL_COM1, JOY_RIGHT);
        while (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TC) == RESET)
        {}
        Key = JOY_NONE;
        break;
      case JOY_SEL:
        USART_SendData8(EVAL_COM1, JOY_SEL);
        while (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TC) == RESET)
        {}
        Key = JOY_NONE;
        break;
      default:
        break;
    }
  }
#else /* USART_IrDA_RECEIVE */
  /* Initialize I/Os in Output Mode for LEDs */
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);

  /* Turn on LEDs */
  STM_EVAL_LEDOn(LED1);
  STM_EVAL_LEDOn(LED2);
  STM_EVAL_LEDOn(LED3);
  STM_EVAL_LEDOn(LED4);

  while (1)
  {
    /* Wait until a byte is received */
    while (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_RXNE) == RESET)
    {}
    /* Read the received byte */
    ReceivedData = USART_ReceiveData8(EVAL_COM1);

    switch (ReceivedData)
    {
        /* LED4 toggle */
      case JOY_UP:
        STM_EVAL_LEDToggle(LED4);
        break;
        /* LED3 toggle */
      case JOY_DOWN:
        STM_EVAL_LEDToggle(LED3);
        break;
        /* LED2 toggle */
      case JOY_LEFT:
        STM_EVAL_LEDToggle(LED2);
        break;
        /* LED1 toggle */
      case JOY_RIGHT:
        STM_EVAL_LEDToggle(LED1);
        break;

      case JOY_SEL:
        STM_EVAL_LEDToggle(LED1);
        STM_EVAL_LEDToggle(LED2);
        STM_EVAL_LEDToggle(LED3);
        STM_EVAL_LEDToggle(LED4);
        break;
      default:
        break;
    }
  }
#endif /*USART_IrDA_TRANSMIT*/
}

/**
  * @brief  Configure USART peripheral  
  * @param  None
  * @retval None
  */
static void USART_Config(void)
{
  /* High speed internal clock prescaler: 1*/
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);
  
  /* EVAL COM (USARTx) configuration -----------------------------------------*/
  /* USART configured as follow:
        - BaudRate = 9600 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Receive and transmit enabled
  */
  STM_EVAL_COMInit(COM1, (uint32_t)9600, USART_WordLength_8b, USART_StopBits_1,
                   USART_Parity_No, (USART_Mode_TypeDef)(USART_Mode_Tx | USART_Mode_Rx));

  /* Set Prescaler*/
  USART_SetPrescaler(EVAL_COM1, 0x1);

  USART_IrDAConfig(EVAL_COM1, USART_IrDAMode_Normal);

  USART_IrDACmd(EVAL_COM1, ENABLE);
}

/**
  * @brief  Reads joystick position from evalboard.
  * @param  None.
  * @retval Joystick position.
  *  This parameter can be a value of @ref JOYState_TypeDef enumeration.
  * @par Required preconditions:
  * None
  */
JOYState_TypeDef ReadJoystick(void)
{
  /* "right" key is pressed */
  if (!STM_EVAL_PBGetState(BUTTON_RIGHT))
  {
    while (STM_EVAL_PBGetState(BUTTON_RIGHT) == Bit_RESET);
    return JOY_RIGHT;
  }
  /* "left" key is pressed */
  if (!STM_EVAL_PBGetState(BUTTON_LEFT))
  {
    while (STM_EVAL_PBGetState(BUTTON_LEFT) == Bit_RESET);
    return JOY_LEFT;
  }
  /* "up" key is pressed */
  if (!STM_EVAL_PBGetState(BUTTON_UP))
  {
    while (STM_EVAL_PBGetState(BUTTON_UP) == Bit_RESET);
    return JOY_UP;
  }
  /* "down" key is pressed */
  if (!STM_EVAL_PBGetState(BUTTON_DOWN))
  {
    while (STM_EVAL_PBGetState(BUTTON_DOWN) == Bit_RESET);
    return JOY_DOWN;
  }
  /* "sel" key is pressed */
  if (!STM_EVAL_PBGetState(BUTTON_SEL))
  {
    while (STM_EVAL_PBGetState(BUTTON_SEL) == Bit_RESET);
    return JOY_SEL;
  }
  /* No key is pressed */
  else
  {
    return JOY_NONE;
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
