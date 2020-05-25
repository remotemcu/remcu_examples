/**
  @page TIM10_PWMOutput TIM10 PWM Output example
  
  @verbatim
  ******************** (C) COPYRIGHT 2011 STMicroelectronics *******************
  * @file    TIM/TIM10_PWMOutput/readme.txt 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Description of the  TIM10 PWM Output example.
  ******************************************************************************
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  ******************************************************************************
   @endverbatim

@par Example Description

This example shows how to configure the TIM10 peripheral in PWM (Pulse Width Modulation) 
mode.

The TIM10CLK frequency is set to SystemCoreClock (72 MHz), to get TIM10 counter
clock at 24 MHz the Prescaler is computed as following:
   - Prescaler = (TIM10CLK / TIM10 counter clock) - 1

The TIM10 is running at 36 KHz: TIM10 Frequency = TIM10 counter clock/(ARR + 1)
                                                = 24 MHz / 666 = 36 KHz
The TIM10 CCR1 register value is equal to 500, so the TIM10 Channel 1 generates a 
PWM signal with a frequency equal to 36 KHz and a duty cycle equal to 37.5%:
TIM10 Channel1 duty cycle = (TIM10_CCR1/ TIM10_ARR + 1)* 100 = 37.5%

The PWM waveform can be displayed using an oscilloscope.


@par Directory contents 

  - TIM/TIM10_PWMOutput/stm32f10x_conf.h    Library Configuration file
  - TIM/TIM10_PWMOutput/stm32f10x_it.c      Interrupt handlers
  - TIM/TIM10_PWMOutput/stm32f10x_it.h      Interrupt handlers header file
  - TIM/TIM10_PWMOutput/main.c              Main program 
  - TIM/TIM10_PWMOutput/system_stm32f10x.c  STM32F10x system source file

@par Hardware and Software environment 

  - This example runs only on STM32F10x XL-Density Devices.
  
  - This example has been tested with STMicroelectronics STM3210E-EVAL (XL-Density) 
    evaluation board and can be easily tailored to any development board.    

  - STM3210E-EVAL Set-up 
    - Connect PF.06 (TIM10_CH1 remapped pin) to an oscilloscope to monitor the PWM waveform

  
@par How to use it ? 

In order to make the program work, you must do the following :
 - Copy all source files from this example folder to the template folder under
   Project\STM32F10x_StdPeriph_Template
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example 

@note
 - Low-density Value line devices are STM32F100xx microcontrollers where the 
   Flash memory density ranges between 16 and 32 Kbytes.
 - Low-density devices are STM32F101xx, STM32F102xx and STM32F103xx 
   microcontrollers where the Flash memory density ranges between 16 and 32 Kbytes.
 - Medium-density Value line devices are STM32F100xx microcontrollers where
   the Flash memory density ranges between 64 and 128 Kbytes.  
 - Medium-density devices are STM32F101xx, STM32F102xx and STM32F103xx 
   microcontrollers where the Flash memory density ranges between 64 and 128 Kbytes.
 - High-density Value line devices are STM32F100xx microcontrollers where
   the Flash memory density ranges between 256 and 512 Kbytes. 
 - High-density devices are STM32F101xx and STM32F103xx microcontrollers where
   the Flash memory density ranges between 256 and 512 Kbytes.
 - XL-density devices are STM32F101xx and STM32F103xx microcontrollers where
   the Flash memory density ranges between 512 and 1024 Kbytes.
 - Connectivity line devices are STM32F105xx and STM32F107xx microcontrollers.
   
 * <h3><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h3>
 */
