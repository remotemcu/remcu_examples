/**
  @page FLASH_Write_Protection FLASH Write Protection example
  
  @verbatim
  ******************** (C) COPYRIGHT 2011 STMicroelectronics *******************
  * @file    FLASH/Write_Protection/readme.txt 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Description of the FLASH Write Protection example.
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

This example provides a description of how to enable and disable the write protection
for the STM32F10x FLASH:

- Enable Write protection: 
   To enable the Write Protection, uncomment the line "#define WRITE_PROTECTION_ENABLE"
   in main.c file.  
   To protect a set of pages, the user has to call the function FLASH_EraseOptionBytes
   to erase all the option bytes then to call the function  FLASH_EnableWriteProtection.
   The parameter of the later function will define the number of pages to be protected
   (The desired pages and already protected pages).
   To load the new option byte values, a system Reset is necessary, for this, the
   function NVIC_SystemReset() is used.
 
- Disable Write protection:
   To disable the Write Protection, uncomment the line "#define WRITE_PROTECTION_DISABLE"
   in main.c file.
   To disable the write protection of the STM32F10x Flash, an erase of the Option 
   Bytes is necessary by the function FLASH_EraseOptionBytes, then call the function 
   FLASH_EnableWriteProtection to protect the pags that are already protected.
   To load the new option byte values, a system Reset is necessary, for this, the
   function NVIC_SystemReset() is used.
   
- Program the selected pages:
  To program the desired pages (if the flash is not write protected) uncomment the line
  "#define FLASH_PAGE_PROGRAM" in main.c file. 

If the desired pages are not write protected, an erase and a write operation are
performed.


@par Directory contents 

  - FLASH/Write_Protection/stm32f10x_conf.h     Library Configuration file
  - FLASH/Write_Protection/stm32f10x_it.h       Interrupt handlers header file
  - FLASH/Write_Protection/stm32f10x_it.c       Interrupt handlers
  - FLASH/Write_Protection/main.c               Main program
  - FLASH/Write_Protection/system_stm32f10x.c   STM32F10x system source file

@par Hardware and Software environment 

  - This example runs on STM32F10x Connectivity line, High-Density, Medium-Density, 
    High-Density Value line, XL-Density, Medium-Density Value line, Low-Density 
    and Low-Density Value line Devices.
  
  - This example has been tested with STMicroelectronics STM32100E-EVAL (High-Density 
    Value line), STM32100B-EVAL (Medium-Density Value line), STM3210C-EVAL 
    (Connectivity line), STM3210E-EVAL (High-Density and XL-Density) and STM3210B-EVAL 
    (Medium-Density) evaluation boards and can be easily tailored to any other 
    supported device and development board.  

  
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
