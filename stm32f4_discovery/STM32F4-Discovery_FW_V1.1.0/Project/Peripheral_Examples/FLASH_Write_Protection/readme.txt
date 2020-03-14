/**
  @page FLASH_Write_Protection FLASH_Write_Protection
  
  @verbatim
  ******************** (C) COPYRIGHT 2011 STMicroelectronics *******************
  * @file    FLASH_Write_Protection/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    19-September-2011
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
for the STM32F4xx FLASH.

By maintaining the User push-button pressed at Reset, the program will check the 
write protection status of FLASH_WRP_SECTORS (defined in main.c) 
  - If FLASH_WRP_SECTORS are write protected, the write protection will be disabled.
    Then LED4 will turn ON, if the protection disable operation is done correctly,
    otherwise LED5 turns ON.
  - If FLASH_WRP_SECTORS are not write protected, the write protection will be enabled.
    Then LED6 will turn ON, if the protection enable operation is done correctly,
    otherwise LED5 turns ON.

If after reset the User push-button is not pressed, the program will simply turn ON LED3. 


@par Directory contents 

  - FLASH_Write_Protection/system_stm32f4xx.c   STM32F4xx system clock configuration file
  - FLASH_Write_Protection/stm32f4xx_conf.h     Library Configuration file
  - FLASH_Write_Protection/stm32f4xx_it.c       Interrupt handlers
  - FLASH_Write_Protection/stm32f4xx_it.h       Header for stm32f4xx_it.c
  - FLASH_Write_Protection/main.c               Main program

  
      
@par Hardware and Software environment

  - This example runs on STM32F4xx Devices Revision A.
  
  - This example has been tested with STM32F4-Discovery (MB997) RevA and can be
    easily tailored to any other development board

  - STM32F4-Discovery  
    - Use LED4, LED3, LED5 and LED6 connected respectively to PD.12, PD.13, PD.14
      and PD.15 pins
    - Use the User push-button connected to pin PA0 (EXTI Line0)
    
      
@par How to use it ? 

In order to make the program work, you must do the following :

 + EWARM
    - Open the Flash_Write_Protection.eww workspace 
    - Rebuild all files: Project->Rebuild all
    - Load project image: Project->Debug
    - Run program: Debug->Go(F5)

 + MDK-ARM
    - Open the Flash_Write_Protection.uvproj project
    - Rebuild all files: Project->Rebuild all target files
    - Load project image: Debug->Start/Stop Debug Session
    - Run program: Debug->Run (F5)    

 + TASKING
    - Open TASKING toolchain.
    - Click on File->Import, select General->'Existing Projects into Workspace' 
      and then click "Next". 
    - Browse to  TASKING workspace directory and select the project "Flash_Write_Protection"   
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
    - Browse to the TrueSTUDIO workspace directory and select the project "Flash_Write_Protection" 
    - Rebuild all project files: Select the project in the "Project explorer" 
      window then click on Project->build project menu.
    - Run program: Select the project in the "Project explorer" window then click 
      Run->Debug (F11)
    
 * <h3><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h3>
 */
