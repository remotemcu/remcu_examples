/**
  @page DAC_SignalsGen DAC Signals generation example
  
  @verbatim
  ******************** (C) COPYRIGHT 2011 STMicroelectronics *******************
  * @file    DAC_SignalsGeneration/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    19-September-2011
  * @brief   Description of the DAC Signals generation example.
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

This example provides a short description of how to use the DAC peripheral to 
generate several signals using DMA controller.
When the user presses the USER push-button, DMA transfers the two selected 
waveforms to the DAC.
For each press on USER button, 2 signals has been selected and can be monitored on  
the two DAC channels:
    - Escalator waveform (Channel 1) and Sine waveForm (Channel 2).
    - Noise waveform (Channel 1) and Triangle waveform (Channel 2).


@par Directory contents 

  - DAC_SignalsGeneration/stm32f4xx_conf.h    Library Configuration file
  - DAC_SignalsGeneration/stm32f4xx_it.c      Interrupt handlers
  - DAC_SignalsGeneration/stm32f4xx_it.h      Interrupt handlers header file
  - DAC_SignalsGeneration/main.c              Main program
  - DAC_SignalsGeneration/system_stm32f4xx.c  STM32F4xx system source file
  
 


@par Hardware and Software environment

  - This example runs on STM32F4xx Devices Revision A.
  
  - This example has been tested with STM32F4-Discovery (MB997) RevA and can be
    easily tailored to any other development board.

  - STM32F4-Discovery 	
     - Use USER push-button connected to PA.00.
     - Connect PA4 (DAC Channel1) and PA5 (DAC Channel2) pins to an oscilloscope.

@par How to use it ? 

In order to make the program work, you must do the following :

 + EWARM
    - Open the DAC_SignalsGeneration.eww workspace 
    - Rebuild all files: Project->Rebuild all
    - Load project image: Project->Debug
    - Run program: Debug->Go(F5)

 + MDK-ARM
    - Open the DAC_SignalsGeneration.uvproj project
    - Rebuild all files: Project->Rebuild all target files
    - Load project image: Debug->Start/Stop Debug Session
    - Run program: Debug->Run (F5)    

 + TASKING
    - Open TASKING toolchain.
    - Click on File->Import, select General->'Existing Projects into Workspace' 
      and then click "Next". 
    - Browse to  TASKING workspace directory and select the project "DAC_SignalsGeneration"   
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
    - Browse to the TrueSTUDIO workspace directory and select the project "DAC_SignalsGeneration" 
    - Rebuild all project files: Select the project in the "Project explorer" 
      window then click on Project->build project menu.
    - Run program: Select the project in the "Project explorer" window then click 
      Run->Debug (F11)
  
 * <h3><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h3>
 */

