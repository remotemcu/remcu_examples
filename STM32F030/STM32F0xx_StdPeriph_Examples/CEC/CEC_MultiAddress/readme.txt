/**
  @page CEC_MultiAddress CEC Multiple Address communication example
  
  @verbatim
  ******************** (C) COPYRIGHT 2014 STMicroelectronics *******************
  * @file    CEC/CEC_MultiAddress/readme.txt 
  * @author  MCD Application Team
  * @version V1.4.0
  * @date    24-July-2014
  * @brief   Description of the CEC Multiple Address communication example.
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

This example provides a basic communication between two HDMI-CEC devices using 
interrupts, which the follower one supports two addresses at the same time. 
The first CEC device send a command to the CEC other device. 
The data received by the first and second CEC device are stored respectively in 
ReceiveBuffer. In both boards, the data transfers is managed using CEC_IRQHandler
in stm32F0xx_it.c file.

The example illustrates the use of the CEC communication between two devices. 
A device configured as transmitter can send a frame to the other device by pressing
the Joystick button(Right,Left) on the EVAL board. The receiver device supports
two addresses(0x03=>Tuner, 0x05=>Audio system).

- Hardware Description

To use this example, you need to load it on two two boards (STM320518-EVAL or STM32072B-EVAL)
(let's call them Device_1 and Device_2) then connect these two boards through CEC lines
(PB10 for STM320518-EVAL and PB08 for STM32072B-EVAL  or HDMI connectors) and GND.
In the firmware example, uncomment the dedicated line in the main.h file  to use
the CEC peripheral as STM32 device_1 or as STM32 device_2.

@verbatim
*------------------------------------------------------------------------------*
|       STM320518/STM32072B-EVAL                STM320518/STM32072B-EVAL       |
|         Device Address :0x01                    Device Address1 :0x03        |
|                                                 Device Address2 :0x05        |
|             (Initiator)                             (Follower)               |
|         ____________________                   ____________________          |
|        |                    |                 |                    |         |
|        |                    |                 |                    |         | 
|        |     __________     |                 |     __________     |         |
|        |    |   CEC    |____|____CECLine______|____|   CEC    |    |         |
|        |    | Device_1 |    |                 |    | Device_2 |    |         |
|        |    |__________|    |                 |    |__________|    |         |
|        |                    |                 |                    |         |
|        |  O LD1             |                 |  O LD1             |         |
|        |  O LD2    Joystick |                 |  O LD2    Joystick |         |
|        |  O LD3        _    |                 |  O LD3        _    |         |
|        |  O LD4       |_|   |                 |  O LD4       |_|   |         |
|        |                    |                 |                    |         |
|        |             GND O--|-----------------|--O GND             |         |
|        |____________________|                 |____________________|         |
|                                                                              |
|                                                                              |
*------------------------------------------------------------------------------*
@endverbatim

- Software Description

At each joystick buttons press:
- The CEC Transmitter device sends a command to the specific receiver device.
  - If Joystick RIGHT is pressed: send a CEC message to the Tuner device(0x03),
  - If Joystick LEFT is pressed: send a CEC message to the Audio system device(0x05).
  
- At the end of the header reception, the own address is compared with the received
  destination address. In the case of matching address with 
  - Tuner        => The message is received and Green Led is on,
  - Audio system => The message is received and Blue Led is on.

@par Directory contents 

  - CEC/CEC_MultiAddress/stm32f0xx_conf.h    Library Configuration file
  - CEC/CEC_MultiAddress/stm32f0xx_it.c      Interrupt handlers
  - CEC/CEC_MultiAddress/stm32f0xx_it.h      Interrupt handlers header file
  - CEC/CEC_MultiAddress/main.c              Main program
  - CEC/CEC_MultiAddress/main.h              Header for main.c module
  - CEC/CEC_MultiAddress/system_stm32f0xx.c  STM32F0xx system source file
  
@note The "system_stm32f0xx.c" is generated by an automatic clock configuration 
      tool and can be easily customized to meet user application requirements. 
      To select different clock setup, use the "STM32F0xx_Clock_Configuration_VX.Y.Z.xls" 
      provided with the AN4055 package available on <a href="http://www.st.com/internet/mcu/class/1734.jsp">  ST Microcontrollers </a>
         
@par Hardware and Software environment

  - This example runs on STM32F0xx Devices.
  
  - This example has been tested with STMicroelectronics STM320518-EVAL and
    STM32072B-EVAL including respectively STM32F051R8T6 and STM32F072VBT6 devices
    and can be easily tailored to any other supported device and development board.

  - STM320518-EVAL Set-up
     Connect the boards by using one of the two following alternatives:
      - A HDMI Cables between all boards HDMI-CEC connectors (CN3 or CN4) on 
        STM320518-EVAL . 
      - Use a simple wire between all devices CEC Lines (PB.10), in this case don't 
        forget to connect all boards grounds together.
  - STM32072B-EVAL Set-up
     Connect the boards by using one of the two following alternatives:
      - A HDMI Cables between all boards HDMI-CEC connectors (CN3 or CN5) on 
        STM32072B-EVAL . 
      - Use a simple wire between all devices CEC Lines (PB.08), in this case don't 
        forget to connect all boards grounds together.
      
@par How to use it ? 

In order to make the program work, you must do the following :
 - Copy all source files from this example folder to the template folder under
   Project\STM32F0xx_StdPeriph_Templates
 - Open your preferred toolchain
 - If the used device is STM32F051R8T6 choose STM32F051 project
    - Add the following files to the project source list
       - Utilities\STM32_EVAL\STM320518_EVAL\stm320518_eval.c
       - Utilities\STM32_EVAL\STM320518_EVAL\stm320518_eval_lcd.c
 - If the used device is STM32F072VBT6 choose STM32F072 project
    - Add the following files to the project source list
       - Utilities\STM32_EVAL\STM32072B_EVAL\stm32072b_eval.c
       - Utilities\STM32_EVAL\STM32072B_EVAL\stm32072b_eval_lcd.c 
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
