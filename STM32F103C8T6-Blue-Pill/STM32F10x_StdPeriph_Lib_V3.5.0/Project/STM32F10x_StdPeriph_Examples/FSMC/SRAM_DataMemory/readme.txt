/**
  @page FSMC_SRAM_DataMemory FSMC SRAM DataMemory example
  
  @verbatim
  ******************** (C) COPYRIGHT 2011 STMicroelectronics *******************
  * @file    FSMC/SRAM_DataMemory/readme.txt 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Description of the FSMC SRAM DataMemory example.
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

This example shows how to use the external SRAM mounted on STM3210E-EVAL or 
STM32100E-EVAL board as program data memory and internal SRAM for Stack.
This example doesn't use the default library startup files. Use the EWARM, MDK-ARM,
HiTOP, RIDE or TrueSTUDIO startup files provided within this folder.

@par Directory contents 

  - FSMC/SRAM_DataMemory/stm32f10x_conf.h                       Library Configuration file
  - FSMC/SRAM_DataMemory/stm32f10x_it.c                         Interrupt handlers
  - FSMC/SRAM_DataMemory/stm32f10x_it.h                         Header for stm32f10x_it.c  
  - FSMC/SRAM_DataMemory/main.c                                 Main program
  - FSMC/SRAM_DataMemory/system_stm32f10x.c                     STM32F10x system source file
  - FSMC/SRAM_DataMemory/EWARM/startup_stm32f10x_hd.s           STM32F10x High-density EWARM startup file
  - FSMC/SRAM_DataMemory/EWARM/startup_stm32f10x_hd_vl.s        STM32F10x High-density Value line EWARM startup file
  - FSMC/SRAM_DataMemory/EWARM/startup_stm32f10x_xl.s           STM32F10x XL-density EWARM startup file
  - FSMC/SRAM_DataMemory/HiTOP/cstart_thumb2.asm                STM32F10x XL-density EWARM startup file
  - FSMC/SRAM_DataMemory/MDK-ARM/startup_stm32f10x_hd.s         STM32F10x High-density MDK-ARM startup file
  - FSMC/SRAM_DataMemory/MDK-ARM/startup_stm32f10x_hd_vl.s      STM32F10x High-density Value line MDK-ARM startup file
  - FSMC/SRAM_DataMemory/MDK-ARM/startup_stm32f10x_xl.s         STM32F10x XL-density MDK-ARM startup file
  - FSMC/SRAM_DataMemory/RIDE/startup_stm32f10x_hd.s            STM32F10x High-density RIDE startup file
  - FSMC/SRAM_DataMemory/RIDE/startup_stm32f10x_hd_vl.s         STM32F10x High-density Value line RIDE startup file
  - FSMC/SRAM_DataMemory/RIDE/startup_stm32f10x_xl.s            STM32F10x XL-density RIDE startup file
  - FSMC/SRAM_DataMemory/TrueSTUDIO/startup_stm32f10x_hd.s      STM32F10x High-density TrueSTUDIO startup file
  - FSMC/SRAM_DataMemory/TrueSTUDIO/startup_stm32f10x_hd_vl.s   STM32F10x High-density Value line TrueSTUDIO startup file
  - FSMC/SRAM_DataMemory/TrueSTUDIO/startup_stm32f10x_xl.s      STM32F10x XL-density TrueSTUDIO startup file
  
@par Hardware and Software environment 

  - This example runs only on STM32F10x High-Density and XL-Density Devices.
  
  - This example has been tested with STMicroelectronics STM32100E-EVAL (High-Density
    Value line) and STM3210E-EVAL (High-Density and XL-Density) evaluation boards
    and can be easily tailored to any other supported device and development board.
    
@par How to use it ? 

In order to make the program work, you must do the following:
- Create a project and setup all project configuration:
<ul>
 <li> MDK-ARM 
    - in Project->Options for Target window, select 'RAM1'and enter 0x68000000 
      as start address with size of 0x100000 (IRAM1 must be un-checked)
    - uncomment "#define DATA_IN_ExtSRAM " in the "system_stm32f10x.c" file               

 <li> EWARM 
    - use "stm32f10x_flash_extsram.icf" as linker file
    - uncomment "#define DATA_IN_ExtSRAM " in the "system_stm32f10x.c" file

 <li> RIDE 
    - In the Application options -> script menu, set "Use Default Script File" 
      to "No" and use "stm32f10x_flash_extsram.ld" as Script File.
    - uncomment "#define DATA_IN_ExtSRAM " in the "system_stm32f10x.c" file

<li> HiTOP 
    - use "link_extsram" as linker file
    - Add the file "setstack.asm" to the workspace.
    - Use the "cstart_thumb2.asm" file  provided with this example instead of 
      the default one in the workspace.
    - uncomment "#define DATA_IN_ExtSRAM " in the "system_stm32f10x.c" file

  
 <li> TrueSTUDIO 
    - In the project properties window, select 'C/C++ Build'->settings node then 
      the 'C Linker'->General node and use "stm32f10x_flash_extsram.ld" as Script File.
    - uncomment "#define DATA_IN_ExtSRAM " in the "system_stm32f10x.c" file
      
</ul>

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
