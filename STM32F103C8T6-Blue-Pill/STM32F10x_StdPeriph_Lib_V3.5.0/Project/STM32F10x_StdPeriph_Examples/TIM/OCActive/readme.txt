/**
  @page TIM_OCActive TIM OC Active example
  
  @verbatim
  ******************** (C) COPYRIGHT 2011 STMicroelectronics *******************
  * @file    TIM/OCActive/readme.txt 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Description of the TIM OC Active example.
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

This example shows how to configure the TIM peripheral to generate four different 
signals with four different delays.

The TIM3CLK frequency is set to SystemCoreClock / 2 (Hz), and the objective is
to get TIM3 counter clock at 1 KHz so the Prescaler is computed as following:
   - Prescaler = (TIM3CLK / TIM3 counter clock) - 1
SystemCoreClock is set to 72 MHz for Low-density, Medium-density, High-density
and Connectivity line devices and to 24 MHz for Low-Density Value line,
Medium-Density Value line and High-Density Value line devices.

The TIM3 CCR1 register value is equal to 1000:
TIM3_CH1 delay = CCR1_Val/TIM3 counter clock  = 1000 ms
so the TIM3 Channel 1 generates a signal with a delay equal to 1000 ms.

The TIM3 CCR2 register value is equal to 500:
TIM3_CH2 delay = CCR2_Val/TIM3 counter clock = 500 ms
so the TIM3 Channel 2 generates a signal with a delay equal to 500 ms.

The TIM3 CCR3 register value is equal to 250:
TIM3_CH3 delay = CCR3_Val/TIM3 counter clock = 250 ms
so the TIM3 Channel 3 generates a signal with a delay equal to 250 ms.

The TIM3 CCR4 register value is equal to 125:
TIM3_CH4 delay = CCR4_Val/TIM3 counter clock = 125 ms
so the TIM3 Channel 4 generates a signal with a delay equal to 125 ms.

The delay correspond to the time difference between PC.06 and 
TIM3_CHx signal rising edges in case of STM32100E-EVAL, STM32100B-EVAL, STM3210E-EVAL 
and STM3210B-EVAL 

The delay correspond to the time difference between PD.07 and 
TIM3_CHx signal rising edges in case of STM3210C-EVAL

@par Directory contents 

  - TIM/OCActive/stm32f10x_conf.h    Library Configuration file
  - TIM/OCActive/stm32f10x_it.c      Interrupt handlers
  - TIM/OCActive/stm32f10x_it.h      Interrupt handlers header file
  - TIM/OCActive/main.c              Main program 
  - TIM/OCActive/system_stm32f10x.c  STM32F10x system source file
  
@par Hardware and Software environment 


  - This example runs on STM32F10x Connectivity line, High-Density, High-Density 
    Value line, Medium-Density, XL-Density, Medium-Density Value line, Low-Density 
    and Low-Density Value line Devices.
  
  - This example has been tested with STMicroelectronics STM32100E-EVAL (High-Density
    Value line), STM32100B-EVAL (Medium-Density Value line), STM3210C-EVAL (Connectivity line), 
    STM3210E-EVAL (High-Density and XL-Density) and STM3210B-EVAL (Medium-Density) 
    evaluation boards and can be easily tailored to any other supported device 
    and development board.
    

  - STM3210C-EVAL Set-up 
    - Connect the TIM3 pins(TIM3 full remapped pins) to an oscilloscope to
      monitor the different waveforms:
       - PD.07
       - PC.06 (TIM3_CH1)
       - PC.07 (TIM3_CH2)
       - PC.08 (TIM3_CH3)
       - PC.09 (TIM3_CH4)

  - STM32100E-EVAL, STM32100B-EVAL, STM3210E-EVAL and STM3210B-EVAL Set-up 
    - Connect the following pins to an oscilloscope to monitor the different 
      waveforms:
       - PC.06
       - PA.06 (TIM3_CH1)
       - PA.07 (TIM3_CH2)
       - PB.00 (TIM3_CH3)
       - PB.01 (TIM3_CH4)
  
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
