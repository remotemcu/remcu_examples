/**
  @page HiTOP5_STM32100B HiTOP Project Template for STM32F10x Medium-density Value line devices
  
  @verbatim
  ******************** (C) COPYRIGHT 2011 STMicroelectronics *******************
  * @file    readme.txt
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   This sub directory contains all the user modifiable files needed
  *          to create a new project linked with the STM32F10x Standard Peripheral 
  *          Library and working with HiTOP software toolchain (version 5.40 and later).
  ******************************************************************************
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
  * AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
  * INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
  * CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
  * INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  ******************************************************************************
  @endverbatim
 
@par Directory contents

 - Project.htp: A pre-configured project file with the provided library 
                structure that produces an executable image with HiTOP

 - cstart_thumb2.asm: This file initializes the stack pointer and copy initialized
                      sections from ROM to RAM.                 

 - Objects: This mandatory directory contains the executable images.

 - Settings: This directory contains the linker and script files.                  
     - arm_arch.lsl: This file is used to place program code (readonly)
                     in internal FLASH and data (readwrite, Stack and Heap)
                     in internal SRAM.

     - link.lnk: This file is the HiTOP linker it invokes the stm32f10x_MD_VL.lsl.
                                   
     - reset_appl.scr: This file is a HiTOP script it performs a target reset.
     
     - reset_go_main.scr: This file is a HiTOP script and it sets the Program 
                          Counter at the "main" instruction.

     - StartupScript.scr: This file is a HiTOP script and it performs a target 
                          reset before loading The executable image.

     - stm32f10x_MD_VL.lsl: This file is used to place program code (readonly)
                            in internal FLASH and data (readwrite, Stack and Heap)
                            in internal SRAM.
                            It contains also the vector table of the STM32 
                            Medium-density Value line devices.
                            You can customize this file to your need.                                                              

@par How to use it ?

- Open the HiTOP toolchain.
- Browse to open the project.htp
- A "Download application" window is displayed, click "cancel".
- Rebuild all files: Project->Rebuild all
- Load project image : Click "ok" in the "Download application" window.
- Run the "RESET_GO_MAIN" script to set the PC at the "main"
- Run program: Debug->Go(F5).

@note
 - Low-density Value line devices are STM32F100xx microcontrollers where the 
   Flash memory density ranges between 16 and 32 Kbytes.
 - Low-density devices are STM32F101xx, STM32F102xx and STM32F103xx 
   microcontrollers where the Flash memory density ranges between 16 and 32 Kbytes.
 - Medium-density Value line devices are STM32F100xx microcontrollers where
   the Flash memory density ranges between 64 and 128 Kbytes.  
 - Medium-density devices are STM32F101xx, STM32F102xx and STM32F103xx 
   microcontrollers where the Flash memory density ranges between 64 and 128 Kbytes.
 - High-density Value line devices are STM32F100xx microcontrollers where the 
   Flash memory density ranges between 256 and 512 Kbytes. 
 - High-density devices are STM32F101xx and STM32F103xx microcontrollers where
   the Flash memory density ranges between 256 and 512 Kbytes.
 - XL-density devices are STM32F101xx and STM32F103xx microcontrollers where
   the Flash memory density ranges between 512 and 1024 Kbytes.
 - Connectivity line devices are STM32F105xx and STM32F107xx microcontrollers.

 * <h3><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h3>
 */
