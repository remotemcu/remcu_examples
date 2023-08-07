/**
  ******************************************************************************
  * @file    delay.c
  * @author  Microcontroller Division
  * @version V1.1.0
  * @date    08/2010
  * @brief   delay functions
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

/* Includes ------------------------------------------------------------------*/

#include "stm8l15x_clk.h"
/**
  * @brief  delay for some time in ms unit
  * @caller auto_test
  * @param  n_ms is how many ms of time to delay
  * @retval None
  */
void delay_ms(u16 n_ms)
{
/* Init TIMER 4 */
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM2, ENABLE);

/* Init TIMER 4 prescaler: / (2^6) = /64 */
  TIM2->PSCR = 6;

/* HSI div by 1 --> Auto-Reload value: 16M / 64 = 1/4M, 1/4M / 1k = 250*/
  TIM2->ARRH = 0;
  TIM2->ARRL = 250;
  
/* Counter value: 2, to compensate the initialization of TIMER*/
    
  TIM2->CNTRH = 0;
  TIM2->CNTRL = 2;

/* clear update flag */
  TIM2->SR1 &= ~TIM_SR1_UIF;

/* Enable Counter */
  TIM2->CR1 |= TIM_CR1_CEN;

  while(n_ms--)
  {
    while((TIM2->SR1 & TIM_SR1_UIF) == 0) ;
    TIM2->SR1 &= ~TIM_SR1_UIF;
  }

/* Disable Counter */
  TIM2->CR1 &= ~TIM_CR1_CEN;
}

/**
  * @brief  delay for some time in 10us unit(partial accurate)
  * @caller auto_test
  * @param n_10us is how many 10us of time to delay
  * @retval None
  */
void delay_10us(u16 n_10us)
{
/* Init TIMER 4 */
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM2, ENABLE);

/* prescaler: / (2^0) = /1 */
  TIM2->PSCR = 0;

/* SYS_CLK_HSI_DIV1 Auto-Reload value: 16M / 1 = 16M, 16M / 100k = 160 */
  TIM2->ARRH = 0;
  TIM2->ARRL = 160;

/* Counter value: 10, to compensate the initialization of TIMER */
  TIM2->CNTRH = 0;
  TIM2->CNTRL = 10;

/* clear update flag */
  TIM2->SR1 &= ~TIM_SR1_UIF;

/* Enable Counter */
  TIM2->CR1 |= TIM_CR1_CEN;

  while(n_10us--)
  {
    while((TIM2->SR1 & TIM_SR1_UIF) == 0) ;
    TIM2->SR1 &= ~TIM_SR1_UIF;
  }

/* Disable Counter */
  TIM2->CR1 &= ~TIM_CR1_CEN;
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM2, DISABLE);

}

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
