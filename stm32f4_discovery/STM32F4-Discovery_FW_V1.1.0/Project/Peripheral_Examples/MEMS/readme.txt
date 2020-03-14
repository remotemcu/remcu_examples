/**
  @page LIS302DL_Example MEMS Accelerometer Configuration example
  
  @verbatim
  ******************** (C) COPYRIGHT 2011 STMicroelectronics *******************
  * @file    MEMS/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    19-September-2011
  * @brief   Description of the MEMS Configuration example.
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

This example shows how to configure the MEMS accelerometre to detect acceleration on X/Y axis
and to detect the click/double click on Z axis.

After startup the program checks the MEMS accelerometre status registers and behaves as follows:
  1. If the board is moved the acceleration is detected on x/Y axis and LEDs toggles according to
     the motion direction and speed.

  2. If a click is detected on Z axis LED3 and LED6 toggles during 3s. 

  3. If a double click is detected on Z axis all LEDs toggles during 3s. 

Note: The LEDs lighting is managed by TIM4 capture compare channels

 
@par Directory contents 

  - MEMS/stm32f4xx_conf.h     Library Configuration file
  - MEMS/stm32f4xx_it.c       Interrupt handlers
  - MEMS/stm32f4xx_it.h       Header for stm32f4xx_it.c 
  - MEMS/main.c               Main program
  - MEMS/main.h               Header for main.c
  - MEMS/system_stm32f4xx.c   STM32F4xx system source file
  
         
@par Hardware and Software environment

  - This example runs on STM32F4xx Devices Revision A.
  
  - This example has been tested with STM32F4-Discovery (MB997) RevA and can be
    easily tailored to any other development board

  - STM32F4-Discovery
    - Use MEMS accelerometre LIS302DL (U5)

      
@par How to use it ? 

In order to make the program work, you must do the following :

 + EWARM
    - Open the LIS302DL.eww workspace 
    - Rebuild all files: Project->Rebuild all
    - Load project image: Project->Debug
    - Run program: Debug->Go(F5)

 + MDK-ARM
    - Open the LIS302DL.uvproj project
    - Rebuild all files: Project->Rebuild all target files
    - Load project image: Debug->Start/Stop Debug Session
    - Run program: Debug->Run (F5)    

 + TASKING
    - Open TASKING toolchain.
    - Click on File->Import, select General->'Existing Projects into Workspace' 
      and then click "Next". 
    - Browse to  TASKING workspace directory and select the project "LIS302DL"   
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
    - Browse to the TrueSTUDIO workspace directory and select the project  "LIS302DL"
    - Rebuild all project files: Select the project in the "Project explorer" 
      window then click on Project->build project menu.
    - Run program: Select the project in the "Project explorer" window then click 
      Run->Debug (F11)
    
 * <h3><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h3>
 */
