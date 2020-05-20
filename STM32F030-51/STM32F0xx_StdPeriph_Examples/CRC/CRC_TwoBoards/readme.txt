/**
  @page CRC_TwoBoards USART Communication Boards with CRC checksum example.
  
  @verbatim
  ******************** (C) COPYRIGHT 2014 STMicroelectronics *******************
  * @file    CRC/CRC_TwoBoards/readme.txt 
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

This example aims to show how to verify CRC checksum while USART communication
between two STM32F0xx boards.

- Hardware Description

To use this example, you need to load it on two STM32F0xx boards (STM32072B-Eval) 
(let's call them Board A and Board B) then connect these two boards through UART 
lines and GND.

In the firmware example uncomment the dedicated line in the main.h file to use
the USART peripheral as TRANSMITTER device or as RECEIVER device.

- The connection between the pins should use a common Ground.

@verbatim
*------------------------------------------------------------------------------*
|                BOARD A                                BOARD B                |
|         ____________________                   ____________________          |
|        |                    |                 |                    |         |
|        |     TRANSMITTER    |                 |      RECEIVER       |         | 
|        |     __________     |                 |     __________     |         |
|        |    |   USART  |____|TX_____________RX|____|   USART  |    |         |
|        |    |  Device1 |____|RX_____________TX|____|  Device2 |    |         |
|        |    |__________|    |                 |    |__________|    |         |
|        |                    |                 |                    |         |
|        |  O LD1             |                 |  O LD1             |         |
|        |  O LD2    Joystick |                 |  O LD2    Joystick |         |
|        |  O LD3        _    |                 |  O LD3        _    |         |
|        |              |_|   |                 |              |_|   |         |
|        |                    |                 |                    |         |
|        |             GND O--|-----------------|--O GND             |         |
|        |____________________|                 |____________________|         |
|                                                                              |
|                                                                              |
*------------------------------------------------------------------------------*
@endverbatim

- Software Description

The CRC peripheral is configured to use the 8-bit CRC polynomial x8 + x7 + x6 + x4 + x2 + 1 
with the initialization value is set to 0.

On Board A, at each joystick SEL button press:

- The USART Board A sends data already defined in TxBuffer.

- CRC value is computed during USART data transfer.  

- After computing CRC of latest transferred byte, TxBuffer is updated with final CRC value.

- The USART Board A sends latest byte which is the final computed CRC value.

On Board B,

- The USART Board B receives the data (data + CRC value) in RxBuffer.

- CRC value of received data are computed in Board B and compared with the received one.
  
  CRC checksum result is signaled by LED lightening as following:
    - CRC received and computed values are equal ==> LD2 toggles, LED3 OFF. 
    - CRC received and computed values are not equal ==> LD2 Off, LED3 ON. 

The SysTick is configured to generate interrupt each 10ms. A dedicated counter 
inside the SysTick ISR is used to toggle the LD1 each 100ms indicating that the 
firmware is running.

These operations can be repeated infinitely.

@par Directory contents 

  - CRC/CRC_TwoBoards/stm32f0xx_conf.h    Library Configuration file
  - CRC/CRC_TwoBoards/stm32f0xx_it.c      Interrupt handlers
  - CRC/CRC_TwoBoards/stm32f0xx_it.h      Interrupt handlers header file
  - CRC/CRC_TwoBoards/main.c              Main program
  - CRC/CRC_TwoBoards/main.h              Main program header file
  - CRC/CRC_TwoBoards/system_stm32f0xx.c  STM32F0xx system source file
  
@note The "system_stm32f0xx.c" is generated by an automatic clock configuration 
      system and can be easily customized to your own configuration. 
      To select different clock setup, use the "STM32F0xx_Clock_Configuration_V1.0.0.xls" 
      provided with the AN4152 package available on <a href="http://www.st.com/internet/mcu/family/141.jsp">  ST Microcontrollers </a>
  
@par Hardware and Software environment

  - This example runs on STM32F072 Devices.
  
  - This example has been tested with STMicroelectronics STM32072B-EVAL (STM32F0xx)
    evaluation board and can be easily tailored to any other supported device 
    and development board.

  - STM32072B-EVAL Set-up
    - Use The Joystick SEL button [PA.0]
    - Connect USART2 TX pin (PD.05) to USART2 RX pin (PD.06)
    - Connect USART2 RX pin (PD.06) to USART2 TX pin (PD.05)

@par How to use it ? 

In order to make the program work, you must do the following :
 - Copy all source files from this example folder to the template folder under
   Project\STM32F0xx_StdPeriph_Templates
 - Open your preferred toolchain 
 - Add the following files to the project source list
     - Utilities\STM32_EVAL\STM32072B_EVAL\stm32072b_eval.c
 - Rebuild all files and load your image into target memory
 - Run the example
    
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
