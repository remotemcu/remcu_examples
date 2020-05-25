/**
  @page CortexM3_MPU CortexM3 MPU example
  
  @verbatim
  ******************** (C) COPYRIGHT 2011 STMicroelectronics *******************
  * @file    CortexM3/MPU/readme.txt 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Description of the CortexM3 MPU example.
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

This example presents the MPU features on STM32F10x XL-density devices and it 
can be easily ported to any other STM32 device supporting MPU.

The example purpose (ACCESS_PERMISSION) is to configure a memory region as 
privileged read only region and tries to perform read and write operation in 
different mode.
If the access is permitted LED1 is toggling. If the access is not permitted, 
a memory management fault is generated and LED2 is ON.
To generate an MPU memory fault exception due to an access right error, uncomment
the following line "privilegedreadonlyarray[0] = 'e';" in the 
"accesspermission.c " file.
 
@par Directory contents 

  - CortexM3/MPU/stm32f10x_conf.h     Library Configuration file
  - CortexM3/MPU/stm32f10x_it.c       Interrupt handlers
  - CortexM3/MPU/stm32f10x_it.h       Header for stm32f10x_it.c
  - CortexM3/MPU/main.c               Main program
  - CortexM3/MPU/system_stm32f10x.c   STM32F10x system source file
  - CortexM3/MPU/accesspermission.c   Cortex-M3 MPU regions Access rights file
               
@par Hardware and Software environment
  
  - This example runs on STM32F10x XL-density Devices.

  - This example has been tested with STMicroelectronics STM3210E-EVAL (High-Density
    and XL-Density) evaluation board and can be easily tailored to any other 
    supported device and development board.
     
@par How to use it ? 

In order to make the program work, you must do the following :
 - Copy all source files from this example folder to the template folder under
   Project\STM32F10x_StdPeriph_Template
 - Open your preferred toolchain and setup your project configuration as follows
 - Add the required example files
   - accesspermission.c
<ul>
- For RIDE and TrueSTUDIO toolchains you have to follow these instructions

    <li> RIDE 
        - In the Application options -> script menu, set "Use Default Script File" 
          to "No" and use "stm32f_flash_ROAarray.ld" as Script File. 
          This linker is configured for STM32F. To use it with  other STM32 devices.
          This linker should be updated.      
        

    <li> TrueSTUDIO 
          - In the project properties window, select C/C++ Build->settings node then 
           the C Linker->General node and use "stm32f10x_flash_ROArray.ld" as Script File.
           This linker is configured for XL-density devices. To use it with
           other STM32 devices, this linker should be updated.           
</ul>    
 
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
