/**
  ******************************************************************************
  * @file    CRC/CRC_TwoBoards/main.h 
  * @author  MCD Application Team
  * @version V1.4.0
  * @date    24-July-2014
  * @brief   Header for main.c module
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
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"
#ifdef USE_STM32072B_EVAL
  #include "stm32072b_eval.h" 
#endif /* USE_STM32072B_EVAL */
/* Exported typedef ----------------------------------------------------------*/
#define countof(a)   (sizeof(a) / sizeof(*(a)))

/* Exported define -----------------------------------------------------------*/

/* Uncomment the line below if you will use the TRANSMITTER mode */
#define MODE_TRANSMITTER  
/* Uncomment the line below if you will use the RECIVER mode */
//#define MODE_RECEIVER

/* USART Communication boards Interface */
#define USARTx                           USART2
#define USARTx_CLK                       RCC_APB1Periph_USART2
#define USARTx_APBPERIPHCLOCK            RCC_APB1PeriphClockCmd
#define USARTx_IRQn                      USART2_IRQn
#define USARTx_IRQHandler                USART2_IRQHandler

#define USARTx_TX_PIN                    GPIO_Pin_5                
#define USARTx_TX_GPIO_PORT              GPIOD                       
#define USARTx_TX_GPIO_CLK               RCC_AHBPeriph_GPIOD
#define USARTx_TX_SOURCE                 GPIO_PinSource5
#define USARTx_TX_AF                     GPIO_AF_0

#define USARTx_RX_PIN                    GPIO_Pin_6               
#define USARTx_RX_GPIO_PORT              GPIOD              
#define USARTx_RX_GPIO_CLK               RCC_AHBPeriph_GPIOD
#define USARTx_RX_SOURCE                 GPIO_PinSource6
#define USARTx_RX_AF                     GPIO_AF_0

#define TXBUFFERSIZE                     (countof(TxBuffer) - 1)
#define RXBUFFERSIZE                     TXBUFFERSIZE + 1

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
