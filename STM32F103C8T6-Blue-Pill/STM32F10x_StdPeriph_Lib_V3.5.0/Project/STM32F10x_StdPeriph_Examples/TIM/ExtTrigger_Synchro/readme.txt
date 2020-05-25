/**
  @page TIM_ExtTrigger_Synchro TIM External Trigger Synchro example
  
  @verbatim
  ******************** (C) COPYRIGHT 2011 STMicroelectronics *******************
  * @file    TIM/ExtTrigger_Synchro/readme.txt 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Description of the TIM External Trigger Synchro example.
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

This example shows how to synchronize TIM peripherals in cascade mode with an
external trigger.
In this example three timers are used:

1/TIM1 is configured as Master Timer:
  - Toggle Mode is used
  - The TIM1 Enable event is used as Trigger Output 

2/TIM1 is configured as Slave Timer for an external Trigger connected to TIM1 
  TI2 pin (TIM1 CH2 configured as input pin):
  - The TIM1 TI2FP2 is used as Trigger Input
  - Rising edge is used to start and stop the TIM1: Gated Mode.

3/TIM3 is slave for TIM1 and Master for TIM4,
  - Toggle Mode is used
  - The ITR1(TIM1) is used as input trigger 
  - Gated mode is used, so start and stop of slave counter
    are controlled by the Master trigger output signal(TIM1 enable event).
  - The TIM3 enable event is used as Trigger Output. 

4/TIM4 is slave for TIM3,
  - Toggle Mode is used
  - The ITR2(TIM3) is used as input trigger
  - Gated mode is used, so start and stop of slave counter
    are controlled by the Master trigger output signal(TIM3 enable event).

  * For Low-density, Medium-density, High-density and Connectivity line devices:
    The TIMxCLK is fixed to 72 MHZ, the Prescaler is equal to 2 so the TIMx clock 
    counter is equal to 24 MHz.
    The Three Timers are running at: 
    TIMx frequency = TIMx clock counter/ 2*(TIMx_Period + 1) = 162.1 KHz.

  * For Low-Density Value line, Medium-Density Value line and High-Density Value line devices:
    The TIMxCLK is fixed to 24 MHz, the Prescaler is equal to 2 so the TIMx clock 
    counter is equal to 8 MHz.
    TIMx frequency = TIMx clock counter/ 2*(TIMx_Period + 1) = 54 KHz.

The starts and stops of the TIM1 counters are controlled by the external trigger.
The TIM3 starts and stops are controlled by the TIM1, and the TIM4 starts and 
stops are controlled by the TIM3.  

@par Directory contents 

  - TIM/ExtTrigger_Synchro/stm32f10x_conf.h    Library Configuration file
  - TIM/ExtTrigger_Synchro/stm32f10x_it.c      Interrupt handlers
  - TIM/ExtTrigger_Synchro/stm32f10x_it.h      Interrupt handlers header file
  - TIM/ExtTrigger_Synchro/main.c              Main program 
  - TIM/ExtTrigger_Synchro/system_stm32f10x.c  STM32F10x system source file
  
@par Hardware and Software environment 

  - This example runs on STM32F10x Connectivity line, High-Density, High-Density 
    Value line, Medium-Density, XL-Density, Medium-Density Value line, Low-Density 
    and Low-Density Value line Devices.
  
  - This example has been tested with STMicroelectronics STM32100E-EVAL (High-Density
    Value line), STM32100B-EVAL (Medium-Density Value line), STM3210C-EVAL (Connectivity line), 
    STM3210E-EVAL (High-Density and XL-Density) and STM3210B-EVAL (Medium-Density) 
    evaluation boards and can be easily tailored to any other supported device 
    and development board.

  - STM32100B-EVAL Set-up 
    - Connect an external trigger, with a frequency <=  xx KHz, to the TIM1 CH2 
      pin (PA.09). In this example the frequency is equal to x KHz.

    - Connect the following pins to an oscilloscope to monitor the different waveforms:
      - TIM1 CH1 (PA.08)
      - TIM3 CH1 (PA.06)
      - TIM4 CH1 (PB.06)
      
  - STM3210C-EVAL Set-up 
    - Connect an external trigger, with a frequency <= 40KHz, to the TIM1 CH2 
      pin (PE.11). In this example the frequency is equal to 5 KHz.

    - Connect the following pins to an oscilloscope to monitor the different waveforms:
      - TIM1 CH1 (PE.09)
      - TIM3 CH1 (PC.06)
      - TIM4 CH1 (PB.06)   

  - STM3210E-EVAL, STM32100E-EVAL and STM3210B-EVAL Set-up 
    - Connect an external trigger, with a frequency <= 40KHz, to the TIM1 CH2 
      pin (PA.09). In this example the frequency is equal to 5 KHz.

    - Connect the following pins to an oscilloscope to monitor the different waveforms:
      - TIM1 CH1 (PA.08)
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
