/**
  @page FSMC_OneNAND FSMC OneNAND example
  
  @verbatim
  ******************** (C) COPYRIGHT 2011 STMicroelectronics *******************
  * @file    FSMC/OneNAND/readme.txt 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Description of the FSMC OneNAND example.
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

This example shows how to configure the FSMC to drive the OneNAND memory mounted 
on STM32100E-EVAL board. 

In this example a basic example of how to use the FSMC firmware library and
an associate driver to perform read/write operations on the KFG1216U2A/B-DIB6 
OneNAND memory.

The different OneNAND memory operations (Unlock, Erase, Write, Read) results are 
signalled using four LEDs as follows:
     - LED2, LED3 and LED4 are ON: OneNAND ID read failed
     - LED3 and LED4 ON: Unlock Block failed
     - LED2 and LED4 ON: Erase Block failed
     - LED4 ON: Write Buffer failed
     - LED2 ON: Asynchronous Read failed
     - LED3 ON: Synchronous Read 
     - LED1 ON: All OneNAND memory operations are OK


@par Directory contents

  - FSMC/OneNAND/stm32f10x_conf.h     Library Configuration file
  - FSMC/OneNAND/stm32f10x_it.c       Interrupt handlers
  - FSMC/OneNAND/stm32f10x_it.h       Header for stm32f10x_it.c
  - FSMC/OneNAND/main.c               Main program
  - FSMC/OneNAND/system_stm32f10x.c   STM32F10x system source file

@par Hardware and Software environment 

  - This example runs on STM32F10x High-density value line Devices.
  
  - This example has been tested with STM32100E-EVAL and can be easily tailored
    to any other development board.
    Make sure that the jumper JP6 is in position 2<->3

    
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
