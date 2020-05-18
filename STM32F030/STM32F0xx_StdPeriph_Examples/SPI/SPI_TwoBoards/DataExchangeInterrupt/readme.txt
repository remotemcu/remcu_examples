                                          /**
  @page SPI_DataExchangeInterrupt SPI Communication Boards Data Exchange using interrupt example
  
  @verbatim
  ******************** (C) COPYRIGHT 2014 STMicroelectronics *******************
  * @file    SPI/SPI_TwoBoards/DataExchangeInterrupt/readme.txt  
  * @author  MCD Application Team
  * @version V1.4.0
  * @date    24-July-2014
  * @brief   Description of the SPI Communication Boards Data Exchange using 
  *          interrupt example.
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

This example provides a small application in which joystick buttons are used 
to trigger SPI communication using interrupts and though using SPI firmware 
library.

- Hardware Description

To use this example, you need to load it on two STM32 boards (let's call them 
Board A and Board B) then connect these two boards through SPI lines and GND.

In the firmware example uncomment the dedicated line in the main.h file to use
the SPI peripheral as STM32 Master device or as STM32 Slave.

The size of transferred data can be configured by uncomment the dedicated size in main.h
file. the same data size must be selected in both boards.

@verbatim
*------------------------------------------------------------------------------*
|                BOARD A                                BOARD B                |
|         ____________________                   ____________________          |
|        |                    |                 |                    |         |
|        |                    |                 |                    |         | 
|        |     __________     |                 |     __________     |         |
|        |    |SPI Master|____|_______SCK_______|____|SPI Slave |    |         |
|        |    |  Device  |____|_______MISO______|____|  Device  |    |         |
|        |    |__________|____|_______MOSI______|____|__________|    |         |
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

@note
- The connection between the pins should use a short wires and a common Ground. 

- Software Description

In master board, SPI peripheral is configured as Master full duplex with Interrupt,
whereas in Slave board, SPI peripheral is configured as Slave Full duplex with Interrupt. 

At each joystick buttons press:
- The Master sends the specific command to the Slave and waits for the ACK command 
  from the Slave

- The Master sends the specific command to the Slave (the command contains the 
  transaction code (CMD_RIGHT, CMD_LEFT, CMD_UP, CMD_DOWN or CMD_SEL) and receives 
  the ACK command from the Slave.

- The Slave receives the command and sends the ACK command.

- The Master sends the defined NumberOfByte from TxBuffer to the Slave and receives
  NumberOfByte from the Slave into Rxbuffer.
  
- The Slave sends the defined NumberOfByte from Txbuffer to the Master and receives 
  NumberOfByte from the Master into Rxbuffer.
  

- The Master compares the received data with TxBuffer and check the ACK command, 
  Received data correctness is signaled by LED lightening and though as follow:
   - Joystick JOY_RIGHT and data correctly received ==> LD2, LD3 ON and LD4 OFF 
   - Joystick JOY_LEFT and data correctly received  ==> LD4 ON, LD2 and LD3 are OFF
   - Joystick JOY_UP and data correctly received    ==> LD2 ON, LD3 and LD4 are OFF
   - Joystick JOY_DOWN and data correctly received  ==> LD3 ON, LD2 and LD4 are OFF
   - Joystick JOY_SEL and data correctly received   ==> LD2, LD3 and LD4 are ON
   
- The Slave after check the received command, compares the received data with TxBuffer. 
  Received data correctness is signaled by LED lightening and though as follow:
   - Received command CMD_RIGHT and data correctly received --> LD2 and LD3 ON and LD4 OFF 
   - Received command CMD_LEFT and data correctly received  --> LD2 ON, LD3 and LD4 are OFF
   - Received command CMD_UP and data correctly received    --> LD2 ON, LD3 and LD4 are OFF
   - Received command CMD_DOWN and data correctly received  --> LD3 ON, LD2 and LD4 are OFF
   - Received command CMD_SEL and data correctly received   --> LD2, LD3 and LD4 leds ON

In both boards, the data transfers is managed using SPI1_IRQHandler in stm32f0xx_it.c 
file.

The SysTick is configured to generate interrupt each 10ms. A dedicated counter 
inside the SysTick ISR is used to toggle the LD1 each 100ms indicating that the 
firmware is running.

A defined communication timeout is insuring that the application will not remain 
stuck if the SPI communication is corrupted.
You can adjust this timeout through the USER_TIMEOUT  define inside main.h file
depending on CPU frequency and application conditions (interrupts routines, 
number of data to transfer, baudrate, CPU frequency...).

These operations can be repeated infinitely.


@par Directory contents 

  - SPI/SPI_TwoBoards/DataExchangeInterrupt/stm32f0xx_conf.h    Library Configuration file
  - SPI/SPI_TwoBoards/DataExchangeInterrupt/stm32f0xx_it.c      Interrupt handlers
  - SPI/SPI_TwoBoards/DataExchangeInterrupt/stm32f0xx_it.h      Interrupt handlers header file
  - SPI/SPI_TwoBoards/DataExchangeInterrupt/main.c              Main program
  - SPI/SPI_TwoBoards/DataExchangeInterrupt/main.h              Main program header file
  - SPI/SPI_TwoBoards/DataExchangeInterrupt/system_stm32f0xx.c  STM32F0xx system source file
  
@note The "system_stm32f0xx.c" is generated by an automatic clock configuration 
      tool and can be easily customized to meet user application requirements. 
      To select different clock setup, use the "STM32F0xx_Clock_Configuration_VX.Y.Z.xls" 
      provided with the AN4055 package available on <a href="http://www.st.com/internet/mcu/class/1734.jsp">  ST Microcontrollers </a>

         
@par Hardware and Software environment

  - This example runs on STM32F0xx devices.
  
  - This example has been tested with STMicroelectronics STM320518-EVAL and
    STM32072B-EVAL including respectively STM32F051R8T6 and STM32F072VBT6 devices
    and can be easily tailored to any other supported device and development board

  - STM320518-EVAL Set-up
      - Use LED1, LED2, LED3 and LED4 connected respectively PC.10, PC.11, PC.12 
         and PD.02 pins for each eval boards.
      - Use The Joystick push buttons 
         - DOWN[PC.07]
         - UP[PC.06]
         - LEFT[PC.09]
         - RIGHT[PC.08] 
         - SEL[PA.00]
     - Connect SPI1 SCK pin (PA.05) to SPI SCK pin
     - Connect SPI1 MISO pin (PB.04) to SPI MISO pin
     - Connect SPI1 MOSI pin (PA.07) to SPI MOSI pin
     - Connect GND pins of two boards. 
  - STM32072B-EVAL Set-up
      - Use LED1, LED2, LED3 and LED4 connected respectively PD.08, PD.09, PD.10 
        and PD.11 pins for each eval boards.
      - Use The Joystick push buttons 
         - DOWN[PF.10]
         - UP[PF.09]
         - LEFT[PE.02]
         - RIGHT[PE.03] 
         - SEL[PA.00]
      - Connect SPI1 SCK pin (PB.03) to SPI SCK pin
      - Connect SPI1 MISO pin (PE.15) to SPI MISO pin
      - Connect SPI1 MOSI pin (PE.14) to SPI MOSI pin
      - Connect GND pins of two boards.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Copy all source files from this example folder to the template folder under
   Project\STM32F0xx_StdPeriph_Templates
 - Open your preferred toolchain 
 - If the used device is STM32F051R8T6 choose STM32F051 project
    - Add the following files to the project source list
       - Utilities\STM32_EVAL\STM320518_EVAL\stm320518_eval.c
 - If the used device is STM32F072VBT6 choose STM32F072 project
    - Add the following files to the project source list
       - Utilities\STM32_EVAL\STM32072B_EVAL\stm32072b_eval.c
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
