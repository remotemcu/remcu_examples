/**
  @page USART_DataExchangeInterrupt USART Communication Boards Data Exchange using Interrupt example
  
  @verbatim
  ******************** (C) COPYRIGHT 2014 STMicroelectronics *******************
  * @file    USART/USART_TwoBoards/DataExchangeInterrupt/readme.txt 
  * @author  MCD Application Team
  * @version V1.4.0
  * @date    24-July-2014
  * @brief   Description of the USART Communication Boards Interrupt Data Exchange 
  *          using Interrupt example.
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
to trigger USART communication using interrupts and though using USART firmware 
library.

- Hardware Description

To use this example, you need to load it on two STM32F0xx boards (let's call them 
Board A and Board B) then connect these two boards through UART lines and GND.

@verbatim
*------------------------------------------------------------------------------*
|                BOARD A                                BOARD B                |
|         ____________________                   ____________________          |
|        |                    |                 |                    |         |
|        |                    |                 |                    |         | 
|        |     __________     |                 |     __________     |         |
|        |    |   USART  |____|TX_____________RX|____|   USART  |    |         |
|        |    |  Device1 |____|RX_____________TX|____|  Device2 |    |         |
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

@note
- The connection between the pins should use a common Ground.

- Software Description

On Board A, at each joystick buttons press:
- The USART Board A sends the specific command to the USART Board B (the command 
  contains the transaction code (CMD_RIGHT, CMD_LEFT, CMD_UP, CMD_DOWN or CMD_SEL)
  followed by the number of data to be transmitted (CMD_RIGHT_SIZE, CMD_LEFT_SIZE, 
  CMD_UP_SIZE, CMD_DOWN_SIZE or CMD_SEL_SIZE)

- The USART Board B receives the command and sends the CMD_ACK command to the USART board A

- The USART Board A receives the CMD_ACK command and sends the number of bytes 
  from TxBuffer to the USART Board B.

- The USART Board B compares the number of bytes received with the defined ones into
  his TxBuffer.
  
  Received data correctness is signaled by LED lightening and though as follow:
   - Joystick RIGHT and data correctly received ==> LD2, LD3 ON and LD4 OFF 
   - Joystick LEFT and data correctly received  ==> LD4 ON, LD2 and LD3 are OFF
   - Joystick UP and data correctly received    ==> LD2 ON, LD3 and LD4 are OFF
   - Joystick DOWN and data correctly received  ==> LD3 ON, LD2 and LD4 are OFF
   - Joystick SEL and data correctly received   ==> LD2, LD3 and LD4 are ON

The steps described above can be also initiated and ensured by Board B. 

In both boards(Board A or Board B), the data transfers is managed using USARTx_IRQHandler 
in stm32f0xx_it.c file when USARTx is used.

The SysTick is configured to generate interrupt each 10ms. A dedicated counter 
inside the SysTick ISR is used to toggle the LD1 each 100ms indicating that the 
firmware is running.
A defined communication timeout is insuring that the application will not remain 
stuck if the USART communication is corrupted.
You can adjust this timeout through the USER_TIMEOUT  define inside main.h file
depending on CPU frequency and application conditions (interrupts routines, 
number of data to transfer, baudrate, CPU frequency...).

These operations can be repeated infinitely.


@par Directory contents 

  - USART/USART_TwoBoards/DataExchangeInterrupt/stm32f0xx_conf.h    Library Configuration file
  - USART/USART_TwoBoards/DataExchangeInterrupt/stm32f0xx_it.c      Interrupt handlers
  - USART/USART_TwoBoards/DataExchangeInterrupt/stm32f0xx_it.h      Interrupt handlers header file
  - USART/USART_TwoBoards/DataExchangeInterrupt/main.c              Main program
  - USART/USART_TwoBoards/DataExchangeInterrupt/main.h              Main program header file
  - USART/USART_TwoBoards/DataExchangeInterrupt/system_stm32f0xx.c  STM32F0xx system source file
  
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
    - Connect USART1 TX pin (PA.09) to USARTx RX pin
    - Connect USART1 RX pin (PA.10) to USARTx TX pin
    - Connect GND pins of two boards  
    - JP3 should be in 5-6 position
    
  - STM32072B-EVAL Set-up
    - Connect USART2 TX pin (PD.05) to USARTx RX pin
    - Connect USART2 RX pin (PD.06) to USARTx TX pin
    - Connect GND pins of two boards 
    - JP17 should be in 5-6 position


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
