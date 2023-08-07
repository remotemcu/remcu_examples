/**
  ******************************************************************************
  * @file    DAC/DAC_SignalsGeneration/stm8l15x_it.h
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    02/12/2010
  * @brief   This file contains the headers of the interrupt handlers.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM8L15x_IT_H
#define __STM8L15x_IT_H

/* Includes ------------------------------------------------------------------*/
#include "stm8l15x.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
#ifdef _COSMIC_
 void _stext(void); /* RESET startup routine */
 @far @interrupt void NonHandledInterrupt(void);
 @far @interrupt void TRAP_IRQHandler(void); /* TRAP */
 @far @interrupt void FLASH_IRQHandler(void); /* FLASH EOP/PG_DIS */
 @far @interrupt void DMA1_CHANNEL0_1_IRQHandler(void); /* DMA1 Channel0/1*/
 @far @interrupt void DMA1_CHANNEL2_3_IRQHandler(void); /*DMA1 Channel2/3*/
 @far @interrupt void RTC_IRQHandler(void); /* RTC */
 @far @interrupt void EXTIE_F_PVD_IRQHandler(void); /*EXTI PORTE/EXTI PORTF/PVD*/
 @far @interrupt void EXTIB_IRQHandler(void); /* EXTI PORTB */
 @far @interrupt void EXTID_IRQHandler(void); /* EXTI PORTD */
 @far @interrupt void EXTI0_IRQHandler(void); /* EXTI PIN0 */
 @far @interrupt void EXTI1_IRQHandler(void); /* EXTI PIN1 */
 @far @interrupt void EXTI2_IRQHandler(void); /* EXTI PIN2 */
 @far @interrupt void EXTI3_IRQHandler(void); /* EXTI PIN3 */
 @far @interrupt void EXTI4_IRQHandler(void); /* EXTI PIN4 */
 @far @interrupt void EXTI5_IRQHandler(void); /* EXTI PIN5 */
 @far @interrupt void EXTI6_IRQHandler(void); /* EXTI PIN6 */
 @far @interrupt void EXTI7_IRQHandler(void); /* EXTI PIN7 */
 @far @interrupt void LCD_IRQHandler(void); /* LCD */
 @far @interrupt void SWITCH_CSS_BREAK_DAC_IRQHandler(void); /* Switch CLK/CSS/TIM1 Break/DAC */
 @far @interrupt void ADC1_COMP_IRQHandler(void); /*ADC1/COMP*/
 @far @interrupt void TIM2_UPD_OVF_TRG_BRK_IRQHandler(void); /* TIM2 UPD/OVF/TRG/BRK */
 @far @interrupt void TIM2_CAP_IRQHandler(void); /* TIM2 CAP*/
 @far @interrupt void TIM3_UPD_OVF_TRG_BRK_IRQHandler(void); /* TIM3 UPD/OVF/TRG/BRK */
 @far @interrupt void TIM3_CAP_IRQHandler(void); /* TIM3 CAP*/
 @far @interrupt void TIM1_UPD_OVF_TRG_COM_IRQHandler(void);/* TIM1 UPD/OVF/TRG/COM */
 @far @interrupt void TIM1_CAP_IRQHandler(void);/* TIM1 CAP*/
 @far @interrupt void TIM4_UPD_OVF_TRG_IRQHandler(void); /* TIM4 UPD/OVF/TRI */
 @far @interrupt void SPI1_IRQHandler(void); /* SPI1 */
 @far @interrupt void USART1_TX_IRQHandler(void); /* USART1 TX */
 @far @interrupt void USART1_RX_IRQHandler(void); /* USART1 RX */
 @far @interrupt void I2C1_IRQHandler(void); /* I2C1 */
#endif /* _COSMIC_ */

#endif /* __STM8L15x_IT_H */

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/

