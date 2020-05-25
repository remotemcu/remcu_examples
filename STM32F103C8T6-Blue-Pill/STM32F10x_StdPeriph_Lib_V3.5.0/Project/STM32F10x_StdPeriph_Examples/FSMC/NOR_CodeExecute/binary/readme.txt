/**
  @page FSMC_NOR_CodeExecute_binary FSMC NOR CodeExecute example binary directory
  
  @verbatim
  ******************** (C) COPYRIGHT 2011 STMicroelectronics *******************
  * @file    FSMC/NOR_CodeExecute/binary/readme.txt 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Description of the FSMC NOR CodeExecute example binary directory.
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

This directory contains a set of sources files that build the application to be
loaded into the NOR memory mounted on STM3210E-EVAL board.

The GPIO IOToggle example provided within the STM32F10x Firmware library is used
as illustration. In this example four LEDs: LED1, LED2, LED3 and LED4 are 
toggled in an infinite loop.

@par Directory contents 

  - FSMC/NOR_CodeExecute/binary/stm32f10x_conf.h        Library Configuration file
  - FSMC/NOR_CodeExecute/binary/stm32f10x_it.c          Interrupt handlers
  - FSMC/NOR_CodeExecute/binary/stm32f10x_it.h          Header for stm32f10x_it.c  
  - FSMC/NOR_CodeExecute/binary/main.c                  Main program
  - FSMC/NOR_CodeExecute/binary/FLASH_NOR.ini           NOR Initialization File for MDK-ARM toolchain. 
  - FSMC/NOR_CodeExecute/binary/STM3210E-EVAL_NOR.FLM   STM3210E-EVAL board NOR flasher for MDK-ARM toolchain.
  - FSMC/NOR_CodeExecute/binary/system_stm32f10x.c      STM32F10x system source file
  
@par Hardware and Software environment 

  - This example runs only on STM32F10x High-Density and XL-Density Devices.
  
  - This example has been tested with STMicroelectronics STM3210E-EVAL (High-Density
    and XL-Density) evaluation board.
    To select the STMicroelectronics evaluation board used to run the example, 
    uncomment the corresponding line in stm32_eval.h file (under Utilities\STM32_EVAL)
  
@par How to use it ? 

In order to make the program work, you must do the following:
- Create a project and setup all project configuration:
<ul>
    <li> MDK-ARM 
         - copy "STM3210E-EVAL_NOR.FLM" file under "C:\Keil\ARM\Flash" directory
         - in Project->Options->Target window, select 'ROM1', enter 0x64000000 as
           start address with size of 0x1000000 and check "Startup" option 
          (IROM1 must be un-checked)
         - in Project->Options->Linker window, uncheck "Use memory Layout from 
           Target Dialog" then enter 0x64000000 as R/O base address and chek 
           again "Use memory Layout from Target Dialog"                  
         - in Project->Options->Utilities window, select "Use Target Driver for 
           Flash Programming"
         - Click on 'Settings' button then select "STM3210E_EVAL NOR Flash" as
           Programming Algorithms (start:0x64000000, size:0x1000000)
         - in Project->Options->Debug select "FLASH_NOR.ini" provided within this 
           example directory as initialization file.       

    <li> EWARM
         - use "stm32f10x_nor.icf" as linker file
         - in project->options ->debugger-> setup , the "run to main" option 
           should be un-checked
         - in project->options ->J-Link/J-Trace-> setup , set the "Reset" to 
           "Normal"
         - in project->options ->debugger->download, click on "edit" button
           ("Use flash loader(s)" option should be checked)
         - select the default flash loader then click on "edit" button
         - check "override default flash loader path" option and select
           "FlashSTM32F10x_NOR.xml" as flash loader 
           ($TOOLKIT_DIR$\config\flashloader\ST\FlashSTM32F10x_NOR.xml)
</ul>

- Copy all source files from this example folder to the template folder under
   Project\STM32F10x_StdPeriph_Template
- Open your preferred toolchain 
- Rebuild all files and load your image into target memory
- Run the example       
- Link all compiled files and load your image into NOR memory

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
