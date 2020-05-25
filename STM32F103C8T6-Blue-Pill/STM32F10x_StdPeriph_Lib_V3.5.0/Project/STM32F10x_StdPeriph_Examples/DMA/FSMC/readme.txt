/**
  @page DMA_FSMC DMA FSMC example
  
  @verbatim
  ******************** (C) COPYRIGHT 2011 STMicroelectronics *******************
  * @file    DMA/FSMC/readme.txt 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Description of the DMA FSMC example.
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

This example provides a description of how to use two DMA channels to transfer
a word data buffer from Flash memory to external SRAM memory and to recuperate
the written data from external SRAM to be stored in internal SRAM.

DMA2 Channel5 is configured to transfer, word by word, the contents of a 32-word data 
buffer stored in Flash memory to the external SRAM memory interfaced by FSMC.
The start of transfer is triggered by software. DMA2 Channel5 memory-to-memory
transfer is enabled.
Source and destination address incrementing is also enabled. The transfer is started
by setting the Channel enable bit for DMA2 Channel5. A polling on the Transfer Complete
flag is done to check the end of transfer. The DMA2 Channel5 Transfer complete flag
is then cleared.

DMA1 Channel3 is configured to transfer, byte by byte, the contents of the first
128Bytes of external SRAM to the internal SRAM memory. The start of transfer is
triggered by software. DMA1 Channel3 memory-to-memory transfer is enabled.
Source and destination address incrementing is also enabled. The transfer is started
by setting the Channel enable bit for DMA1 Channel3. A polling on the Transfer Complete
flag is done to check the end of transfer.

A comparison between the source and destination buffers is done to check that all data
have been correctly transferred.


@par Directory contents 

  - DMA/FSMC/stm32f10x_conf.h     Library Configuration file
  - DMA/FSMC/stm32f10x_it.c       Interrupt handlers
  - DMA/FSMC/stm32f10x_it.h       Interrupt handlers header file
  - DMA/FSMC/main.c               Main program  
  - DMA/FSMC/system_stm32f10x.c   STM32F10x system source file
  
@par Hardware and Software environment 

  - This example runs only on STM32F10x High-Density, High-Density Value line
    and XL-Density Devices.
  
  - This example has been tested with STMicroelectronics STM32100E-EVAL (High-Density
    Value line) and STM3210E-EVAL (High-Density and XL-Density) evaluation boards 
    and can be easily tailored to any other supported device and development board.


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
 - High-density devices are STM32F101xx and STM32F103xx microcontrollers where
   the Flash memory density ranges between 256 and 512 Kbytes.
 - High-density Value line devices are STM32F100xx microcontrollers where
   the Flash memory density ranges between 256 and 512 Kbytes.   
 - XL-density devices are STM32F101xx and STM32F103xx microcontrollers where
   the Flash memory density ranges between 512 and 1024 Kbytes.
 - Connectivity line devices are STM32F105xx and STM32F107xx microcontrollers.
      
 * <h3><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h3>
 */
