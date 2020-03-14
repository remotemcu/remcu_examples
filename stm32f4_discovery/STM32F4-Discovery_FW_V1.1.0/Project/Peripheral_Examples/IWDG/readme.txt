/**
  @page IWDG_Example IWDG_Example
  
  @verbatim
  ******************** (C) COPYRIGHT 2011 STMicroelectronics *******************
  * @file    IWDG/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    19-September-2011
  * @brief   Description of the IWDG Example.
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

This example shows how to update at regular period the IWDG reload counter and 
how to simulate a software fault generating an MCU IWDG reset on expiry of a 
programmed time period.

The IWDG timeout is set to 250 ms (the timeout may varies due to LSI frequency 
dispersion).

First, the TIM5 timer is configured to measure the LSI frequency as the 
LSI is internally connected to TIM5 CH4, in order to adjust the IWDG clock.

The LSI measurement using the TIM5 is described below:
 - Configure the TIM5 to remap internally the TIM5 CH4 Input Capture to the LSI
   clock output.
 - Enable the TIM5 Input Capture interrupt: after one cycle of LSI clock, the
   period value is stored in a variable and compared to the HCLK clock to get
   its real value. 

Then, the IWDG reload counter is configured to obtain 250 ms according to the 
measured LSI frequency.

The IWDG reload counter is refreshed each 240 ms in the main program infinite loop 
to prevent a IWDG reset.
LED3 is also toggled each 240 ms indicating that the program is running.

An EXTI Line is connected to a GPIO pin, and configured to generate an interrupt
on the rising edge of the signal.

The EXTI Line is used to simulate a software failure: once the EXTI Line event 
occurs, by pressing the User push-button, the corresponding interrupt is served.
In the ISR, a write to invalid address generates a Hardfault exception containing
an infinite loop and preventing to return to main program (the IWDG reload counter 
is not refreshed). LED5 turns on when a Hardfault occurs.
As a result, when the IWDG counter reaches 00h, the IWDG reset occurs.
If the IWDG reset is generated, after the system resumes from reset, LED4 turns on.

If the EXTI Line event does not occur, the IWDG counter is indefinitely refreshed
in the main program infinite loop, and there is no IWDG reset.


@par Directory contents 

  - IWDG/system_stm32f4xx.c   STM32F4xx system clock configuration file 
  - IWDG/stm32f4xx_conf.h     Library Configuration file
  - IWDG/stm32f4xx_it.c       Interrupt handlers
  - IWDG/stm32f4xx_it.h       Header for stm32f4xx_it.c
  - IWDG/main.c               Main program

      
@par Hardware and Software environment

  - This example runs on STM32F4xx Devices Revision A.
  
  - This example has been tested with STM32F4-Discovery (MB997) RevA and can be
    easily tailored to any other development board


  - STM32F4-Discovery 
    - LD1, LD2 and LD3 leds connected respectively to PD.12, PD.13 and PD.14 pins
    - Use the USER push button connected to PA.00 pin (EXTI Line0).    
    
@par How to use it ? 

In order to make the program work, you must do the following :

 + EWARM
    - Open the IWDG.eww workspace 
    - Rebuild all files: Project->Rebuild all
    - Load project image: Project->Debug
    - Run program: Debug->Go(F5)

 + MDK-ARM
    - Open the IWDG.uvproj project
    - Rebuild all files: Project->Rebuild all target files
    - Load project image: Debug->Start/Stop Debug Session
    - Run program: Debug->Run (F5)    

 + TASKING
    - Open TASKING toolchain.
    - Click on File->Import, select General->'Existing Projects into Workspace' 
      and then click "Next". 
    - Browse to  TASKING workspace directory and select the project "IWDG"   
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
    - Browse to the TrueSTUDIO workspace directory and select the project "IWDG" 
    - Rebuild all project files: Select the project in the "Project explorer" 
      window then click on Project->build project menu.
    - Run program: Select the project in the "Project explorer" window then click 
      Run->Debug (F11)


    
 * <h3><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h3>
 */
