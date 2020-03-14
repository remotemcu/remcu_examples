/**
  @page NVIC_VectorTable_Relocation NVIC VectorTable Relocation example
  
  @verbatim
  ******************** (C) COPYRIGHT 2011 STMicroelectronics *******************
  * @file    NVIC/VectorTable_Relocation/readme.txt 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Description of the NVIC VectorTable Relocation example.
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

This example describes how to set the CortexM3 vector table in a specific 
address other than default using the NVIC_SetVectorTable function from the 
misc.h/.c driver.

This can be used to build program which will be loaded into Flash memory by an
application previously programmed from the Flash memory base address. 
Such application can be In-Application Programming (IAP, through USART) or
Device Firmware Upgrade (DFU, through USB).

These applications are available for download from the ST microcontrollers 
website: www.st.com/stm32

The associated program implements a "Delay" function based on SysTick end of count
interrupt, and toggles four leds with timing defined by the "Delay" function.

When using the IAP to load your porgram, the vector table must be relocated at
address 0x08003000.

When using the DFU to load your porgram, the vector table must be relocated at
address 0x08003000.

@par Directory contents 

 - NVIC/VectorTable_Relocation/linker: modified linker to be used for this project 
 - NVIC/VectorTable_Relocation/stm32f10x_conf.h     Library Configuration file 
 - NVIC/VectorTable_Relocation/stm32f10x_it.c       Interrupt handlers 
 - NVIC/VectorTable_Relocation/stm32f10x_it.h       Interrupt handlers header file 
 - NVIC/VectorTable_Relocation/main.c               Main program 
 - NVIC/VectorTable_Relocation/main.h               Header for main.c 
 - NVIC/VectorTable_Relocation/system_stm32f10x.c   STM32F10x system source file

@par Hardware and Software environment 

  - This example runs on STM32F10x Connectivity line, High-Density, High-Density
    Value line, Medium-Density, XL-Density, Medium-Density Value line, Low-Density 
    and Low-Density Value line Devices.
  
  - This example has been tested with STMicroelectronics STM32100E-EVAL (High-Density
    Value line), STM32100B-EVAL (Medium-Density Value line), STM3210C-EVAL (Connectivity line), 
    STM3210E-EVAL (High-Density and XL-Density) and STM3210B-EVAL (Medium-Density) 
    evaluation boards and can be easily tailored to any other supported device 
    and development board.
    To select the STMicroelectronics evaluation board used to run the example, 
    uncomment the corresponding line in stm32_eval.h file (under Utilities\STM32_EVAL)

  - STM32100E-EVAL Set-up 
    - Use LD1, LD2, LD3 and LD4 leds connected respectively to PF.06, PF0.7, PF.08
      and PF.09 pins
      
  - STM32100B-EVAL Set-up   
    - Use LD1, LD2, LD3 and LD4 leds connected respectively to PC.06, PC.07, PC.08
      and PC.09 pins
      
  - STM3210C-EVAL Set-up 
    - Use LED1, LED2, LED3 and LED4 leds connected respectively to PD.07, PD.13, PF.03
      and PD.04 pins
    
  - STM3210E-EVAL Set-up 
    - Use LD1, LD2, LD3 and LD4 leds connected respectively to PF.06, PF0.7, PF.08
      and PF.09 pins

  - STM3210B-EVAL Set-up   
    - Use LD1, LD2, LD3 and LD4 leds connected respectively to PC.06, PC.07, PC.08
      and PC.09 pins
     
@par How to use it ? 

In order to make the program work, you must do the following :
- Create a project and setup all project configuration:
<ul>
    <li> MDK-ARM 
         - In the project option menu, select 'Target' window and enter 0x08003000 
           as IROM start address
         - In the project option menu, select 'Linker' window and enter 0x08003000 
           as R/O base address                    

    <li> EWARM
         - Use "stm32f10x_flash_offset.icf" as linker file

    <li> RIDE 
         - In the Application options -> script menu, set "Use Default Script File" 
           to "No" and use "stm32f10x_flash_offset.ld" as Script File. 
           This linker is configured for STM32 XL-density devices. To use it with
           other STM32 devices, this linker should be updated.
    
    <li> HiTOP 
         - From linker\HiTOP\[target], copy the "link_offset" and "STM32F10x_offset.lsl"
           to "Settings" folder.
         - In project -> settings -> project -> tool settings ->linker 
             use "link_offset.lnk" as linker file. 
         - Add the "setstack.asm" to the workspace.
         
    <li> TrueSTUDIO 
         - In the project properties window, select 'C/C++ Build'->settings node then 
           the 'C Linker'->General node and use "stm32f10x_flash_offset.ld" as Script File.
           This linker is configured for STM32 STM32F10x XL-density devices. To use it with
           other STM32 devices, this linker should be updated. 
         
</ul>        
- Copy all source files from this example folder to the template folder under
  Project\STM32F10x_StdPeriph_Template
      
- In the main.c and linker files, the vector table is relocated at address 0x08003000.
  You can modify this address depending on the requirement of your application.    
- Rebuild all files 
- Convert the program image to a binary file, *.bin, then you can download and
  run it using the IAP or DFU application.

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
