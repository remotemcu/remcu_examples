/**
  @page GPIO_JTAG_Remap GPIO JTAG Remap example
  
  @verbatim
  ******************** (C) COPYRIGHT 2011 STMicroelectronics *******************
  * @file    GPIO/JTAG_Remap/readme.txt 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Description of the GPIO JTAG Remap example.
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

This example provides a short description of how to use the JTAG IOs as standard 
GPIOs and gives a configuration sequence. 

First, the SWJ-DP is disabled. The SWJ-DP pins are configured as output push-pull.
Five LEDs connected to the PA.13(JTMS/SWDAT), PA.14(JTCK/SWCLK), PA.15(JTDI), 
PB.03(JTDO) and PB.04(JTRST) pins are toggled in an infinite loop.

Note that once the JTAG IOs are disabled, the connection with the host debugger is
lost and cannot be re-established as long as the JTAG IOs remain disabled. 

To avoid this situation, a specified pin is connected to a push-button that is used
to disable or not the JTAG IOs:
 1. push-button pressed at reset: JTAG IOs disabled	and LED1 turned on
 2. push-button not pressed at reset: JTAG IOs unchanged and LED2 turned on

Before starting this example, you should disconnect your tool chain debugging probe
and run the example in standalone mode.

@par Directory contents 

  - GPIO/JTAG_Remap/stm32f10x_conf.h     Library Configuration file
  - GPIO/JTAG_Remap/stm32f10x_it.c       Interrupt handlers
  - GPIO/JTAG_Remap/stm32f10x_it.h       Header for stm32f10x_it.c
  - GPIO/JTAG_Remap/main.c               Main program
  - GPIO/JTAG_Remap/system_stm32f10x.c   STM32F10x system source file
  
@par Hardware and Software environment 

  - This example runs on STM32F10x Connectivity line, High-Density, High-Density
    Value line, Medium-Density, XL-Density, Medium-Density Value line, Low-Density 
    and Low-Density Value line Devices.
  
  - This example has been tested with STMicroelectronics STM32100E-EVAL (High-Density
    Value line), STM32100B-EVAL (Medium-Density Value line), STM3210C-EVAL 
    (Connectivity line), STM3210E-EVAL (High-Density and XL-Density) and STM3210B-EVAL 
    (Medium-Density) evaluation boards and can be easily tailored to any other
    supported device and development board.
    To select the STMicroelectronics evaluation board used to run the example, 
    uncomment the corresponding line in stm32_eval.h file (under Utilities\STM32_EVAL)

  - STM32100E-EVAL Set-up 
    - Connect five leds to pins PA.13, PA.14, PA.15, PB.03 and PB.04. 
    - Use the Key push-button connected to pin PG.08 (EXTI Line8).
    - Use LD1 and LD2 connected respectively to PF.06 and PF.07
    
  - STM32100B-EVAL Set-up  
    - Connect five leds to pins PA.13, PA.14, PA.15, PB.03 and PB.04. 
    - Use the Key push-button connected to pin PB.09 (EXTI Line9).
    - Use LD1 and LD2 connected respectively to PC.06 and PC.07
    
  - STM3210C-EVAL Set-up 
    - Connect five leds to pins PA.13, PA.14, PA.15, PB.03 and PB.04. 
    - Use the Key push-button connected to pin PB.09 (EXTI Line9).
    - Use LD1 and LD2 connected respectively to PC.06 and PC.07

  - STM3210E-EVAL Set-up 
    - Connect five leds to pins PA.13, PA.14, PA.15, PB.03 and PB.04. 
    - Use the Key push-button connected to pin PG.08 (EXTI Line8).
    - Use LD1 and LD2 connected respectively to PF.06 and PF.07
    
  - STM3210B-EVAL Set-up  
    - Connect five leds to pins PA.13, PA.14, PA.15, PB.03 and PB.04. 
    - Use the Key push-button connected to pin PB.09 (EXTI Line9).
    - Use LD1 and LD2 connected respectively to PC.06 and PC.07
 
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
