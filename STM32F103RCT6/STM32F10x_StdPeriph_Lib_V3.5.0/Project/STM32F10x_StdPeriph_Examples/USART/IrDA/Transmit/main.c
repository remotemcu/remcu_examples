/**
  ******************************************************************************
  * @file    USART/IrDA/Transmit/main.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "platform_config.h"

/** @addtogroup STM32F10x_StdPeriph_Examples
  * @{
  */

/** @addtogroup USART_IrDA_Transmit
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
USART_InitTypeDef USART_InitStructure;
JOYState_TypeDef MyKey = JOY_NONE;

/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);
void GPIO_Configuration(void);
JOYState_TypeDef ReadKey(void);

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
       file (startup_stm32f10x_xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f10x.c file
     */     
       
  /* System Clocks Configuration */
  RCC_Configuration();

  /* Configure the GPIO ports */
  GPIO_Configuration();

#ifndef USE_STM3210C_EVAL
  /* Initialize JoyStick Button mounted on STM3210X-EVAL board */       
  STM_EVAL_PBInit(BUTTON_UP, BUTTON_MODE_GPIO);
  STM_EVAL_PBInit(BUTTON_DOWN, BUTTON_MODE_GPIO);
  STM_EVAL_PBInit(BUTTON_LEFT, BUTTON_MODE_GPIO);
  STM_EVAL_PBInit(BUTTON_RIGHT, BUTTON_MODE_GPIO);
  STM_EVAL_PBInit(BUTTON_SEL, BUTTON_MODE_GPIO);
#else
  /* Configure the IO Expander */
  if (IOE_Config())
  {
    /* IO Expander config error */
    while(1);
  }

#endif  
     
/* USARTy configuration ------------------------------------------------------*/
  /* USARTy configured as follow:
        - BaudRate = 115200 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No ;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  /* Configure the USARTy */
  USART_Init(USARTy, &USART_InitStructure);
  /* Enable the USARTy */
  USART_Cmd(USARTy, ENABLE);
  /* Set the USARTy prescaler */
  USART_SetPrescaler(USARTy, 0x1);
  /* Configure the USARTy IrDA mode */
  USART_IrDAConfig(USARTy, USART_IrDAMode_Normal);

  /* Enable the USARTy IrDA mode */
  USART_IrDACmd(USARTy, ENABLE);


  while (1)
  {
    /* Read Key */
    MyKey = ReadKey();

    switch(MyKey)
    {
      case JOY_UP:
        USART_SendData(USARTy, JOY_UP);
        while(USART_GetFlagStatus(USARTy, USART_FLAG_TXE) == RESET)
        {
        }
        break;
      case JOY_DOWN:
        USART_SendData(USARTy, JOY_DOWN);
        while(USART_GetFlagStatus(USARTy, USART_FLAG_TXE) == RESET)
        {
        }
        break;
      case JOY_LEFT:
        USART_SendData(USARTy, JOY_LEFT);
        while(USART_GetFlagStatus(USARTy, USART_FLAG_TXE) == RESET)
        {
        }
        break;
      case JOY_RIGHT:
        USART_SendData(USARTy, JOY_RIGHT);
        while(USART_GetFlagStatus(USARTy, USART_FLAG_TXE) == RESET)
        {
        }
        break;
      case JOY_SEL:
        USART_SendData(USARTy, JOY_SEL);
        while(USART_GetFlagStatus(USARTy, USART_FLAG_TXE) == RESET)
        {
        }
        break;
      case JOY_NONE:
        USART_SendData(USARTy, JOY_NONE);
        while(USART_GetFlagStatus(USARTy, USART_FLAG_TXE) == RESET)
        {
        } 
        break;
      default:
        break;
    }
  }
}

/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
void RCC_Configuration(void)
{  
  /* Enable GPIO clock */
  RCC_APB2PeriphClockCmd(USARTy_GPIO_CLK | RCC_APB2Periph_AFIO, ENABLE);
  
  /* Enable USARTy clocks */
  RCC_APB1PeriphClockCmd(USARTy_CLK, ENABLE);
}

/**
  * @brief  Configures the different GPIO ports.
  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

#ifndef USE_STM3210C_EVAL
  /* Enable the USART3 Pins Partial Software Remapping */
  GPIO_PinRemapConfig(GPIO_PartialRemap_USART3, ENABLE);
#else
  /* Enable the USART2 Pins Software Remapping */
  GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);
#endif  

  /* Configure USARTy Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = USARTy_TxPin;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(USARTy_GPIO, &GPIO_InitStructure);
  
  /* Configure USARTy Rx as input floating */
  GPIO_InitStructure.GPIO_Pin = USARTy_RxPin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(USARTy_GPIO, &GPIO_InitStructure);
}

/**
  * @brief  Reads key from board.
  * @param  None
  * @retval Return JOY_RIGHT, JOY_LEFT, JOY_SEL, JOY_UP, JOY_DOWN or JOY_NONE
  */
JOYState_TypeDef ReadKey(void)
{
#ifndef USE_STM3210C_EVAL
  /* "right" key is pressed */
  if(!STM_EVAL_PBGetState(BUTTON_RIGHT))
  {
    while(STM_EVAL_PBGetState(BUTTON_RIGHT) == Bit_RESET);
    return JOY_RIGHT; 
  }
  /* "left" key is pressed */
  if(!STM_EVAL_PBGetState(BUTTON_LEFT))
  {
    while(STM_EVAL_PBGetState(BUTTON_LEFT) == Bit_RESET);
    return JOY_LEFT; 
  }
  /* "up" key is pressed */
  if(!STM_EVAL_PBGetState(BUTTON_UP))
  {
    while(STM_EVAL_PBGetState(BUTTON_UP) == Bit_RESET);
    return JOY_UP; 
  }
  /* "down" key is pressed */
  if(!STM_EVAL_PBGetState(BUTTON_DOWN))
  {
    while(STM_EVAL_PBGetState(BUTTON_DOWN) == Bit_RESET);
    return JOY_DOWN; 
  }
  /* "sel" key is pressed */
  if(!STM_EVAL_PBGetState(BUTTON_SEL))
  {
    while(STM_EVAL_PBGetState(BUTTON_SEL) == Bit_RESET);
    return JOY_SEL; 
  }
  /* No key is pressed */
  else 
  {
    return JOY_NONE;
  }
#else
  return IOE_JoyStickGetState();
#endif  
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

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
