/**
  @page IWDG_Reset IWDG Reset example
  
  @verbatim
  ******************** (C) COPYRIGHT 2011 STMicroelectronics *******************
  * @file    IWDG/IWDG_Reset/readme.txt 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Description of the IWDG Reset example.
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

This example shows how to update at regular period the IWDG reload counter and 
how to simulate a software fault generating an MCU IWDG reset on expiry of a 
programmed time period.

The IWDG timeout is set to 250 ms (the timeout may varies due to LSI frequency 
dispersion).

First, the TIM5 timer is configured to measure the LSI frequency as the 
LSI is internally connected to TIM5 CH4, in order to adjust the IWDG clock.

The LSI measurement using the TIM5 is described below:
 - Configure the TIM5 to remap internally the TIM5 Channel 4 Input Capture to 
   the LSI clock output.
 - Enable the TIM5 Input Capture interrupt: after one cycle of LSI clock, the
   period value is stored in a variable and compared to the HCLK clock to get
   its real value. 

@note The LSI is internally connected to TIM5 IC4 only on STM32F10x Connectivity 
      line, High-Density Value line, High-Density and XL-Density Devices.
      When using other devices, you should comment the "#define LSI_TIM_MEASURE"
      in main.c file and in this case the LSI frequency is set by default to 40KHz.
   
Then, the IWDG reload counter is configured to obtain 240ms according to the 
measured LSI frequency.

The IWDG reload counter is refreshed each 240ms in the main program infinite loop 
to prevent a IWDG reset.
LED2 is also toggled each 240ms indicating that the program is running.

An EXTI Line is connected to a GPIO pin, and configured to generate an interrupt
on the rising edge of the signal.

The EXTI Line is used to simulate a software failure: once the EXTI Line event 
occurs, by pressing the Key push-button, the corresponding interrupt is served.
In the ISR, a write to invalid address generates a Hardfault exception containing
an infinite loop and preventing to return to main program (the IWDG reload counter 
is not refreshed).
As a result, when the IWDG counter reaches 00h, the IWDG reset occurs.
If the IWDG reset is generated, after the system resumes from reset, LED1 turns on.

If the EXTI Line event does not occur, the IWDG counter is indefinitely refreshed
in the main program infinite loop, and there is no IWDG reset.


In this example the system clock is set to 24 MHz on Value line devices and to
72MHz on other devices.

@note The IWDG Counter can be only written when the RVU flag is reset. 
      In this example, as the SysTick period is too higher than the IWDG Counter
      Update timing (5 Cycles 40KHz RC), the Reload Value Update "RVU" flag is 
      not checked before reloading the counter.

@par Directory contents 

  - IWDG/IWDG_Reset/stm32f10x_conf.h     Library Configuration file
  - IWDG/IWDG_Reset/stm32f10x_it.c       Interrupt handlers
  - IWDG/IWDG_Reset/stm32f10x_it.h       Header for stm32f10x_it.c
  - IWDG/IWDG_Reset/main.c               Main program
  - IWDG/IWDG_Reset/system_stm32f10x.c   STM32F10x system source file
  
@par Hardware and Software environment 

  - This example runs on STM32F10x Connectivity line, High-Density, High-Density
    Value line, Medium-Density, XL-Density, High-Density Value line, Medium-Density 
    Value line, Low-Density and Low-Density Value line Devices.
  
  - This example has been tested with STMicroelectronics STM32100E-EVAL (High-Density
    Value line), STM32100B-EVAL (Medium-Density Value line), STM3210C-EVAL 
    (Connectivity line), STM3210E-EVAL (High-Density and XL-Density), STM32100E-EVAL 
    (High-Density Value line) and STM3210B-EVAL (Medium-Density) evaluation 
    boards and can be easily tailored to any other supported device and development 
    board.
    To select the STMicroelectronics evaluation board used to run the example, 
    uncomment the corresponding line in stm32_eval.h file (under Utilities\STM32_EVAL)

  - STM32100E-EVAL Set-up 
    - Use LD1 and LD2 leds connected respectively to PF.06 and PF.07 pins
    - Use the KEY push button connected to PG.08 pin (EXTI Line8).
    
  - STM32100B-EVAL Set-up  
    - Use LD1 and LD2 leds connected respectively to PC.06 and PC.07 pins
    - Use the KEY push button connected to PB.09 pin (EXTI Line9).
    
  - STM3210C-EVAL Set-up 
    - Use LD1 and LD2 connected respectively to PD.07 and PD.13 pins
    - Use the Key push-button connected to pin PB.09 (EXTI Line9).
    
  - STM3210E-EVAL Set-up 
    - Use LD1 and LD2 leds connected respectively to PF.06 and PF.07 pins
    - Use the KEY push button connected to PG.08 pin (EXTI Line8).

  - STM3210B-EVAL Set-up  
    - Use LD1 and LD2 leds connected respectively to PC.06 and PC.07 pins
    - Use the KEY push button connected to PB.09 pin (EXTI Line9).

    
@par How to use it ? 

In order to make the program work, you must do the following :
 - Copy all source files from this example folder to the template folder under
   Project\STM32F10x_StdPeriph_Template
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory 
 - Link all compiled files and load your image into target memory
 - Run the example in standalone mode (without debugger connection)

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
