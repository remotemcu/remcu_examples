/**
  ******************************************************************************
  * @file    GPIO/GPIO_Toggle/stm8l15x_conf.h
  * @author  MCD Application Team
  * @version V1.3.0
  * @date    07/14/2010
  * @brief   Library configuration file.
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
#ifndef __STM8L15x_CONF_H
#define __STM8L15x_CONF_H

/* Includes ------------------------------------------------------------------*/
#include "stm8l15x.h"

/* Uncomment the line below to enable peripheral header file inclusion */
/* #include "stm8l15x_adc.h" */
/* #include "stm8l15x_beep.h" */
/* #include "stm8l15x_clk.h" */
/* #include "stm8l15x_comp.h" */
/* #include "stm8l15x_dac.h" */
/* #include "stm8l15x_dma.h" */
/* #include "stm8l15x_exti.h" */
/* #include "stm8l15x_flash.h" */
/* #include "stm8l15x_gpio.h" */
/* #include "stm8l15x_i2c.h" */
/* #include "stm8l15x_irtim.h" */
/* #include "stm8l15x_itc.h" */
/* #include "stm8l15x_iwdg.h" */
/* #include "stm8l15x_lcd.h" */
/* #include "stm8l15x_pwr.h" */
/* #include "stm8l15x_rst.h" */
/* #include "stm8l15x_rtc.h" */
/* #include "stm8l15x_spi.h"*/
/* #include "stm8l15x_syscfg.h" */
/* #include "stm8l15x_tim1.h" */
/* #include "stm8l15x_tim2.h" */
/* #include "stm8l15x_tim3.h" */
/* #include "stm8l15x_tim4.h" */
/* #include "stm8l15x_usart.h" */
/* #include "stm8l15x_wfe.h" */
/* #include "stm8l15x_wwdg.h" */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Uncomment the line below to expanse the "assert_param" macro in the
   Standard Peripheral Library drivers code */
/* #define USE_FULL_ASSERT    (1) */

/* Exported macro ------------------------------------------------------------*/
#ifdef  USE_FULL_ASSERT

/**
  * @brief  The assert_param macro is used for function's parameters check.
  * @param expr: If expr is false, it calls assert_failed function
  *   which reports the name of the source file and the source
  *   line number of the call that failed.
  *   If expr is true, it returns no value.
  * @retval : None
  */
#define assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, __LINE__))
/* Exported functions ------------------------------------------------------- */
void assert_failed(uint8_t* file, uint32_t line);
#else
#define assert_param(expr) ((void)0)
#endif /* USE_FULL_ASSERT */

#endif /* __STM8L15x_CONF_H */

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
