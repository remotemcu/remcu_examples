/**
  @page TIM_Parallel_Synchro TIM Parallel Synchro example
  
  @verbatim
  ******************** (C) COPYRIGHT 2011 STMicroelectronics *******************
  * @file    TIM/Parallel_Synchro/readme.txt 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Description of the TIM Parallel Synchro example.
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

This example shows how to synchronize TIM peripherals in parallel mode.
In this example three timers are used:

Timers synchronisation in parallel mode:

1/TIM2 is configured as Master Timer:
 - PWM Mode is used
 - The TIM2 Update event is used as Trigger Output 
 
2/TIM3 and TIM4 are slaves for TIM2,
 - PWM Mode is used
 - The ITR1(TIM2) is used as input trigger for both slaves 
 - Gated mode is used, so starts and stops of slaves counters are controlled 
   by the Master trigger output signal(update event).

o For Low-density, Medium-density, High-density and Connectivity line devices:
  The TIMxCLK is fixed to 72 MHz, the TIM2 counter clock is 72 MHz.
  The Master Timer TIM2 is running at TIM2 frequency:
  TIM2 frequency = TIM2 counter clock/ (TIM2 period + 1) = 281.250 KHz 
  and the duty cycle is equal to TIM2_CCR1/(TIM2_ARR + 1) = 25%.

  The TIM3 is running at:
  (TIM2 frequency)/ (TIM3 period + 1) = 28.1250 KHz and a duty cycle equal to 
  TIM3_CCR1/(TIM3_ARR + 1) = 30%

  The TIM4 is running at:
  (TIM2 frequency)/ (TIM4 period + 1) = 56.250 KHz and a duty cycle equal to 
  TIM4_CCR1/(TIM4_ARR + 1) = 60%

o For Low-Density Value line, Medium-Density Value line and High-Density Value line devices:
  The TIMxCLK is fixed to 24 MHz, the TIM2 counter clock is 24 MHz.
  TIM2 frequency = 93.75 KHz 
  TIM3 frequency = 9.375 KHz 
  TIM4 frequency = 18.75 KHz 
  
@par Directory contents 

  - TIM/Parallel_Synchro/stm32f10x_conf.h    Library Configuration file
  - TIM/Parallel_Synchro/stm32f10x_it.c      Interrupt handlers
  - TIM/Parallel_Synchro/stm32f10x_it.h      Interrupt handlers header file
  - TIM/Parallel_Synchro/main.c              Main program
  - TIM/Parallel_Synchro/system_stm32f10x.c  STM32F10x system source file
   
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
    - Connect the pins to an oscilloscope to monitor the different waveforms:
      - TIM2 CH1 (PA.00)
      - TIM3 CH1 (PC.06) Remapped pin
      - TIM4 CH1 (PB.06) 

  - STM32100B-EVAL, STM3210E-EVAL, STM32100E-EVAL and STM3210B-EVAL Set-up 
    - Connect the pins to an oscilloscope to monitor the different waveforms:
      - TIM2 CH1 (PA.00)
      - TIM3 CH1 (PA.06) 
      - TIM4 CH1 (PB.06) 
  
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
