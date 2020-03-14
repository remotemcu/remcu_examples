/**
  @page DMA_FLASH_RAM DMA_FLASH_RAM
  
  @verbatim
  ******************** (C) COPYRIGHT 2011 STMicroelectronics *******************
  * @file    DMA_FLASH_RAM/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    19-September-2011
  * @brief   Description of the DMA FLASH to RAM example.
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

This example provides a description of how to use a DMA channel to transfer 
a word data buffer from FLASH memory to embedded SRAM memory.

DMA2 Stream0 channel0 is configured to transfer the contents of a 32-word data 
buffer stored in Flash memory to the reception buffer declared in RAM.

The start of transfer is triggered by software. DMA2 Stream0 channel0 memory-to-memory
transfer is enabled. Source and destination addresses incrementing is also enabled.
The transfer is started by setting the Channel enable bit for DMA2 Stream0 channel0.
At the end of the transfer a Transfer Complete interrupt is generated since it
is enabled. The Transfer Complete Interrupt pending bit is then cleared. 
When the DMA transfer is completed the DMA Stream is disabled by hardware.
The main application can check on the Stream Enable status to detect the end of
transfer or can also check on the number of remaining transfers which should be 
equal to 0 at the end of the transfer. 
A comparison between the source and destination buffers is done to check that all 
data have been correctly transferred.

STM32 Eval board's LEDs can be used to monitor the transfer status:
 - LED4 is ON when the program starts.
 - LED3 is ON when the configuration phase is done and the transfer is started.
 - LED5 is ON when the transfer is complete (into the Transfer Complete interrupt
   routine)
 - LED6 is ON when the comparison result between source buffer and destination 
   buffer is passed.

It is possible to select a different Stream and/or channel for the DMA transfer
example by modifying defines values in the file main.h.
Note that only DMA2 Streams are able to perform Memory-to-Memory transfers.

There are different options to check on the DMA end of transfer:
 1. Use DMA Transfer Complete interrupt.
 2. Use DMA enable state (the DMA stream is disabled by hardware when transfer 
    is complete).
 3. Use DMA Stream transfer counter value (the counter value is decremented when
    transfer is ongoing and is equal to 0 at the transfer end).
 4. Use DMA Transfer Complete flag (polling mode).

In this example methods 1, 2 and 3 are provided (you can select between method 2 
and 3 by uncommenting relative code in waiting loop in the main.c file).


@par Directory contents 

  - DMA_FLASH_RAM/system_stm32f40x.c   STM32F4xx system clock configuration file
  - DMA_FLASH_RAM/stm32f4xx_conf.h     Library Configuration file
  - DMA_FLASH_RAM/stm32f4xx_it.c       Interrupt handlers
  - DMA_FLASH_RAM/stm32f4xx_it.h       Interrupt handlers header file
  - DMA_FLASH_RAM/main.c               Main program
  - DMA_FLASH_RAM/main.h               Main program header file

      
@par Hardware and Software environment

  - This example runs on STM32F4xx Devices Revision A.
  
  - This example has been tested with STM32F4-Discovery (MB997) RevA and can be
    easily tailored to any other development board.
    

@par How to use it ? 

In order to make the program work, you must do the following :

 + EWARM
    - Open the DMA_FLASH_RAM.eww workspace 
    - Rebuild all files: Project->Rebuild all
    - Load project image: Project->Debug
    - Run program: Debug->Go(F5)

 + MDK-ARM
    - Open the DMA_FLASH_RAM.uvproj project
    - Rebuild all files: Project->Rebuild all target files
    - Load project image: Debug->Start/Stop Debug Session
    - Run program: Debug->Run (F5)    

 + TASKING
    - Open TASKING toolchain.
    - Click on File->Import, select General->'Existing Projects into Workspace' 
      and then click "Next". 
    - Browse to  TASKING workspace directory and select the project "DMA_FLASH_RAM"   
    - Rebuild all project files: Select the project in the "Project explorer" 
      window then click on Project->build project menu.
    - Run program: Select the project in the "Project explorer" window then click 
      Run->Debug (F11)

 + TrueSTUDIO
    - Open the TrueSTUDIO toolchain.
    - Click on File->Switch Workspace->Other and browse to TrueSTUDIO workspace 
      directory.
    - Click on File->Import, select General->'Existing Projects into Workspace' 
      and then click "Next". 
    - Browse to the TrueSTUDIO workspace directory and select the project "DMA_FLASH_RAM" 
    - Rebuild all project files: Select the project in the "Project explorer" 
      window then click on Project->build project menu.
    - Run program: Select the project in the "Project explorer" window then click 
      Run->Debug (F11)
      
 * <h3><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h3>
 */
