/**
  @page Dual_Boot XL-Density devices FLASH Dual Boot capability example
  
  @verbatim
  ******************** (C) COPYRIGHT 2011 STMicroelectronics *******************
  * @file    FLASH/Dual_Boot/readme.txt 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Description of the XL-Density devices FLASH Dual Boot capability example.
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

This example demonstrates the dual Flash boot capability of XL-Density devices:
boot from Flash memory Bank1 or Bank2.

At startup, if BFB2 option bit is reset and the boot pins are in the boot from main
Flash memory configuration, the device boots from Flash memory Bank1 or Bank2,
depending on the activation of the bank. The active banks are checked in the following
order: Bank2, followed by Bank1.
The active bank is identified by the value programmed at the base address of the 
bank (corresponding to the initial stack pointer value in the interrupt vector table). 
For further details, please refer to AN2606 "STM32 microcontroller system memory boot mode."

To demonstrate this feature, this example provides two programs:
  - 1st program will be loaded in Flash Bank1 (starting from @ 0x08000000), for this
     you have to enable BOOT_FROM_BANK1 define in main.c 
  - 2nd program will be loaded in Flash Bank2 (starting from @ 0x08080000), for this
     you have to enable BOOT_FROM_BANK2 define in main.c 

Once these two programs are loaded and boot pins set in boot from Flash memory,
after reset the device will boot from Bank1 (default). Then you have to follow
the instructions provided on the LCD:
  - "Joystick-DOWN: reset BFB2 bit to Boot from Bank2" => when pushing the Joystick
     DOWN button, BFB2 option bit will be reset then a system (SW) reset will be
     generated. After startup from reset, the device will boot from Bank2. 
      - Note: when booting from Bank2 the same menu will be displayed
 
  - "Joystick-UP: set BFB2 bit to Boot from Bank1" => when pushing the Joystick
     UP button, BFB2 option bit will be set then a system (SW) reset will be
     generated. After startup from reset, the device will boot from Bank1.       
 
  - "Joystick-SEL: program to 0x0 the base @ of Bank1/2" => when pushing the Joystick
     SEL button, the content of address 0x08080000 and 0x08000000 will be programmed to 0x0.
       - If the program was previously booting from Bank2 (i.e. BFB2 bit is reset), 
         in this case after reset no program is executed and the Bootloader code
         is executed instead. 
       - If the program was previously booting from Bank1 (i.e. BFB2 bit is set), 
         in this case after reset no program is executed.
       - You have to load again the two programs to Bank1 and Bank2.
 
@b Important Note
=================
When BFB2 bit is cleared and Bank2 or/and Bank1 contain valid user application code,
the Bootloader will always jump to this code and never continue normal code execution 
(i.e. it’s no more possible to use the Bootloader for code upgrade and option bytes
programming). As consequence, if the user has cleared BFB2 bit (to boot from Bank2),
in order to be able to execute the embedded Bootloader code he has to: 
   - either, set BFB2 bit to 1
   - or, program the content of address 0x08080000 and 0x08000000 to 0x0
=> This example allows performing the two actions described above.


@par Directory contents 

  - FLASH/Dual_Boot/stm32f10x_conf.h    Library Configuration file
  - FLASH/Dual_Boot/stm32f10x_it.h      Interrupt handlers header file
  - FLASH/Dual_Boot/stm32f10x_it.c      Interrupt handlers
  - FLASH/Dual_Boot/main.c              Main program
  - FLASH/Dual_Boot/system_stm32f10x.c  STM32F10x system source file

@par Hardware and Software environment 

  - This example runs only on STM32F10x XL-Density Devices.
  
  - This example has been tested with STMicroelectronics STM3210E-EVAL (XL-Density) 
    evaluation board and can be easily tailored to any development board. 

 
@par How to use it ? 
    
In order to load the IAP code, you have to do the following:
 - EWARM:
    - Open the Project.eww workspace
    - In the workspace toolbar select the project config:
        - STM32F10X_XL_BANK1: to load the program in Flash bank1 (BOOT_FROM_BANK1
                              already defined in the project preprocessor)    
        - STM32F10X_XL_BANK2: to load the program in Flash bank2 (BOOT_FROM_BANK2
                              already defined in the project preprocessor)
    - Rebuild all files: Project->Rebuild all
    - Load project image: Project->Debug
    - Run program: Debug->Go(F5)

 - HiTOP
    - Open the HiTOP toolchain.
    - Browse to open:
        -STM32F10X_XL_BANK1.htp: to load the program in Flash bank1 (BOOT_FROM_BANK1
                             already defined in the project preprocessor)
        -STM32F10X_XL_BANK2.htp: to load the program in Flash bank2 (BOOT_FROM_BANK2
                             already defined in the project preprocessor)  
     - A "Download application" window is displayed, click "cancel".
     - Rebuild all files: Project->Rebuild all
     - Load project image : Click "ok" in the "Download application" window.
     - Run the "RESET_GO_MAIN" script to set the PC at the "main"
     - Run program: Debug->Go(F5).
     
 - MDK-ARM 
    - Open Project.uvproj project
    - In the build toolbar select the project config:
        - STM32F10X_XL_BANK1: to load the program in Flash bank1 (BOOT_FROM_BANK1
                              already defined in the project preprocessor)    
        - STM32F10X_XL_BANK2: to load the program in Flash bank2 (BOOT_FROM_BANK2
                              already defined in the project preprocessor)  
    - Rebuild all files: Project->Rebuild all target files
    - Load project image: Debug->Start/Stop Debug Session
    - Run program: Debug->Run (F5)

 - TrueSTUDIO 
    - Open the TrueSTUDIO toolchain.
    - Click on File->Switch Workspace->Other and browse to TrueSTUDIO workspace 
      directory.
    - Click on File->Import, select General->'Existing Projects into Workspace' 
      and then click "Next". 
    - Browse to the TrueSTUDIO workspace directory and select the project: 
        - STM32F10X_XL_BANK1: to load the program in Flash bank1 (BOOT_FROM_BANK1
                              already defined in the project preprocessor)    
        - STM32F10X_XL_BANK2: to load the program in Flash bank2 (BOOT_FROM_BANK2
                              already defined in the project preprocessor) 
    - Under Windows->Preferences->General->Workspace->Linked Resources, add 
      a variable path named "CurPath" which points to the folder containing
      "Libraries", "Project" and "Utilities" folders.
    - Rebuild all project files: Select the project in the "Project explorer" 
      window then click on Project->build project menu.
    - Run program: Select the project in the "Project explorer" window then click 
      Run->Debug (F11)

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
