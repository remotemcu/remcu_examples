/**
  @page EXTI_Example EXTI Configuration Example
  
  @verbatim
  ******************** (C) COPYRIGHT 2011 STMicroelectronics *******************
  * @file    EXTI/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    19-September-2011
  * @brief   Description of the EXTI example.
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

This example shows how to configure external interrupt lines.
In this example, the EXTI Line0 (connected to PA0 pin) is configured  to generate
an interrupt on each rising edge. In the interrupt routine a led connected to
PD.12 pin is toggled.

@note In this example, EXTILine0_Config() function can be substituted by 
      STM_EVAL_PBInit(Button_USER, Mode_EXTI) provided in the STM32F4-Discovery driver.


@par Directory contents 

  - EXTI/stm32f4xx_conf.h    Library Configuration file
  - EXTI/stm32f4xx_it.h      Interrupt handlers header file
  - EXTI/stm32f4xx_it.c       Interrupt handlers
  - EXTI/main.c              Main program
  - EXTI/system_stm32f4xx.c  STM32F4xx system source file


@par Hardware and Software environment

  - This example runs on STM32F4xx Devices Revision A.

  - This example has been tested with STM32F4-Discovery (MB997) RevA and can be
    easily tailored to any other development board.

  - STM32F4-Discovery  
    - Use the User push-button connected to pin PA0 (EXTI Line0)


@par How to use it ? 

In order to make the program work, you must do the following :

 + EWARM
    - Open the Exti.eww workspace 
    - Rebuild all files: Project->Rebuild all
    - Load project image: Project->Debug
    - Run program: Debug->Go(F5)

 + MDK-ARM
    - Open the Exti.uvproj project
    - Rebuild all files: Project->Rebuild all target files
    - Load project image: Debug->Start/Stop Debug Session
    - Run program: Debug->Run (F5)    

 + TASKING
    - Open TASKING toolchain.
    - Click on File->Import, select General->'Existing Projects into Workspace' 
      and then click "Next". 
    - Browse to  TASKING workspace directory and select the project "Exti"   
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
    - Browse to the TrueSTUDIO workspace directory and select the project  "Exti" 
    - Rebuild all project files: Select the project in the "Project explorer" 
      window then click on Project->build project menu.
    - Run program: Select the project in the "Project explorer" window then click 
      Run->Debug (F11)

   
 * <h3><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h3>
 */
