/**
  @page CAN_Networking CAN Networking example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    CAN/CAN_Networking/readme.txt 
  * @author  MCD Application Team
  * @version V1.8.0
  * @date    04-November-2016
  * @brief   Description of the CAN Networking example.
  ******************************************************************************
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  @endverbatim

@par Example Description 

This example shows how to configure the CAN peripheral to send and receive 
CAN frames in normal mode. The sent frames are used to control LEDs by pressing  
KEY push-button.
	
The CAN serial communication link is a bus to which a number of units may be
connected. This number has no theoretical limit. Practically the total number
of units will be limited by delay times and/or electrical loads on the bus line.

This program behaves as follows:
  - After reset LED1 is ON
  - By Pressing on KEY Button : LED2 turns ON and all other LEDs are OFF, on the N
    eval boards connected to the bus. 
  - Press on KEY Button again to send CAN Frame to command LEDn+1 ON, all other LEDs 
    are OFF on the N eval boards.
    
 @note This example is tested with a bus of 3 units. The same program example is 
       loaded in all units to send and receive frames.

 @note Any unit in the CAN bus may play the role of sender (by pressing KEY button)
       or receiver.

The CAN is configured as follows:
    - Bit Rate   = 1 Mbit/s  
    - CAN Clock  = external clock (HSE)
		- ID Filter  = All identifiers are allowed
		- RTR = Data
		- DLC = 1 byte
		- Data: LED number that should be turned ON


@par Directory contents 
  
  - CAN/CAN_Networking/system_stm32f4xx.c   STM32F4xx system clock configuration file
  - CAN/CAN_Networking/stm32f4xx_conf.h     Library Configuration file
  - CAN/CAN_Networking/stm32f4xx_it.c       Interrupt handlers
  - CAN/CAN_Networking/stm32f4xx_it.h       Interrupt handlers header file
  - CAN/CAN_Networking/main.c               Main program
  - CAN/CAN_Networking/main.h               Main program header file


@par Hardware and Software environment
  
  - This example runs on STM32F405xx/407xx, STM32F415xx/417xx, STM32F427xx/437xx and 
    STM32F429xx/439xx devices.
    
  - This example has been tested with STMicroelectronics STM324xG-EVAL (STM32F40xx/
    STM32F41xx Devices), STM32437I-EVAL (STM32F427xx/STM32F437xx Devices) and 
    STM324x9I-EVAL RevB (STM32F429xx/STM32F439xx Devices) evaluation boards and 
    can be easily tailored to any other supported device and development board.

  - STM324xG-EVAL and STM32437I-EVAL Set-up 
    - Use LED1, LED2, LED3 and LED4 connected respectively to PG.06, PG.08, PI.09
      and PC.07 pins
    - Use KEY Push Button connected to PG15 
    - Connect a female/female CAN cable between at least 2 EVAL CAN connectors (CN10)
    - You have to configure the jumpers as follows:

  @verbatim    
       +-------------------------------+
       |   Jumper    |       CAN1      |
       +-------------------------------+
       |   JP3       |       2-3       |
       |   JP10      |       2-3       |
       |   JP7       |       1-2       |
       |   JP9       |      fitted     |
       +-------------------------------+

  @endverbatim
  
  - STM324x9I-EVAL RevB Set-up 
    - Use LED1, LED2, LED3 and LED4 connected respectively to PG.06, PG.07, PG.10
      and PG.12 pins
    - Use KEY Push Button connected to PC13 
    - Connect a female/female CAN cable between at least 2 EVAL CAN connectors (CN22)
    - You have to configure the jumpers as follows: 
    
  @verbatim    
       +-------------------------------+
       |   Jumper    |       CAN1      |
       +-------------------------------+
       |   JP10      |       1-2       |
       |   JP11      |      fitted     |
       |   JP16      |      fitted     |
       +-------------------------------+

  @endverbatim  
   

@par How to use it ? 

In order to make the program work, you must do the following:
 - Copy all source files from this example folder to the template folder under
   Project\STM32F4xx_StdPeriph_Templates
 - Open your preferred toolchain 
 - Select the project workspace related to the used device 
   - If "STM32F40_41xxx" is selected as default project Add the following files in the project source list:
     - Utilities\STM32_EVAL\STM3240_41_G_EVAL\stm324xg_eval.c
        
   - If "STM32F427_437xx" is selected as default project Add the following files in the project source list:
     - Utilities\STM32_EVAL\STM324x7I_EVAL\stm324x7i_eval.c
          
   - If "STM32F429_439xx" is selected as default project Add the following files in the project source list:
     - Utilities\STM32_EVAL\STM324x9I_EVAL\stm324x9i_eval.c
                 
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
