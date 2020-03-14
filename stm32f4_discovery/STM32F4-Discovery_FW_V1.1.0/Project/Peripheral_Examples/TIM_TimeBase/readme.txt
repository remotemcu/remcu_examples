/**
  @page TIM_TimeBase TIM_TimeBase
  
  @verbatim
  ******************************************************************************
  * @file    TIM_TimeBase/readme.txt
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    19-September-2011
  * @brief   Description of the TIM Time Base example.
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

This example shows how to configure the TIM peripheral in Output Compare Timing 
mode with the corresponding Interrupt requests for each channel in order to generate
4 different time bases.

The TIM3CLK frequency is set to SystemCoreClock / 2 (Hz), to get TIM3 counter 
clock at 500 KHz so the Prescaler is computed as following:
   - Prescaler = (TIM3CLK / TIM3 counter clock) - 1

SystemCoreClock is set to 168 MHz for STM32F4xx Devices Revision A.

The TIM3 CC1 register value is equal to 54618, 
CC1 update rate = TIM3 counter clock / CCR1_Val = 9.154 Hz,
so the TIM3 Channel 1 generates an interrupt each 109.2ms

The TIM3 CC2 register is equal to 27309, 
CC2 update rate = TIM3 counter clock / CCR2_Val = 18.31 Hz
so the TIM3 Channel 2 generates an interrupt each 54.6ms

The TIM3 CC3 register is equal to 13654, 
CC3 update rate = TIM3 counter clock / CCR3_Val = 36.62 Hz
so the TIM3 Channel 3 generates an interrupt each 27.3ms

The TIM3 CC4 register is equal to 6826, 
CC4 update rate = TIM3 counter clock / CCR4_Val =  73.25 Hz
so the TIM3 Channel 4 generates an interrupt each 13.65ms.

When the counter value reaches the Output compare registers values, the Output 
Compare interrupts are generated and, in the handler routine, 4 pins(PD.12, PD.13,
PD.14 and PD.15) are toggled with the following frequencies: 

- PD.12:  4.57 Hz (CC1)
- PD.13:  9.15 Hz (CC2)
- PD.14: 18.31 Hz (CC3) 
- PD.15: 36.62 Hz (CC4)

@par Directory contents 

  - TIM_TimeBase/stm32f4xx_conf.h     Library Configuration file
  - TIM_TimeBase/stm32f4xx_it.c       Interrupt handlers
  - TIM_TimeBase/stm32f4xx_it.h       Interrupt handlers header file
  - TIM_TimeBase/main.c               Main program 
  - TIM_TimeBase/system_stm32f4xx.c   STM32F4xx system clock configuration file
  
 

@par Hardware and Software environment 

  - This example runs on STM32F4xx Devices Revision A.
  
  - This example has been tested with STM32F4-Discovery (MB997) RevA and can be
    easily tailored to any other development board.
    

  - STM32F4-Discovery  
    - Use LED4, LED3, LED5 and LED6 connected respectively to PD.12, PD.13,D.14 
    and PD.15 pins and connect them on an oscilloscope to show the different 
    Time Base signals.  

@par How to use it ? 

In order to make the program work, you must do the following :

 + EWARM
    - Open the TIM_TimeBase.eww workspace 
    - Rebuild all files: Project->Rebuild all
    - Load project image: Project->Debug
    - Run program: Debug->Go(F5)

 + MDK-ARM
    - Open the TIM_TimeBase.uvproj project
    - Rebuild all files: Project->Rebuild all target files
    - Load project image: Debug->Start/Stop Debug Session
    - Run program: Debug->Run (F5)    

 + TASKING
    - Open TASKING toolchain.
    - Click on File->Import, select General->'Existing Projects into Workspace' 
      and then click "Next". 
    - Browse to  TASKING workspace directory and select the project "TIM_TimeBase"   
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
    - Browse to the TrueSTUDIO workspace directory and select the project "TIM_TimeBase" 
    - Rebuild all project files: Select the project in the "Project explorer" 
      window then click on Project->build project menu.
    - Run program: Select the project in the "Project explorer" window then click 
      Run->Debug (F11)
   
 * <h3><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h3>
 */
