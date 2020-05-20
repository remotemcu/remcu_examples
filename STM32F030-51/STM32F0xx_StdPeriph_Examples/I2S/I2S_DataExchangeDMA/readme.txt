/**
  @page I2S_DataExchangeDMA  I2S Data Exchange using DMA example
  
  @verbatim
  ******************** (C) COPYRIGHT 2014 STMicroelectronics *******************
  * @file    I2S/I2S_DataExchangeDMA/readme.txt 
  * @author  MCD Application Team
  * @version V1.4.0
  * @date    24-July-2014
  * @brief   Description of two boards I2S data exchange using DMA example.
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

This example provides a description of how to set a communication between two SPIs
in I2S mode using DMA and performing a transfer from Master to Slave.

- Hardware Description

To use this example, you need to load it on two STM32 boards (let's call them 
Board A and Board B) then connect these two boards through SPI lines and GND.
In the firmware example uncomment the dedicated line in the main.h file  to use
the SPI peripheral as STM32 Master device or as STM32 Slave .

@verbatim
*------------------------------------------------------------------------------*
|                BOARD A                                BOARD B                |
|         ____________________                   ____________________          |
|        |                    |                 |                    |         |
|        |                    |                 |                    |         | 
|        |     __________     |                 |     __________     |         |
|        |    |SPI Master|____|_______SD________|____|SPI Slave |    |         |
|        |    |  Device  |____|_______WS________|____|  Device  |    |         |
|        |    |__________|____|_______CK________|____|__________|    |         |
|        |                    |                 |                    |         |
|        |                    |                 |  O LD1             |         |
|        |            TAMPER  |                 |  O LD2             |         |
|        |               _    |                 |  O LD3             |         |
|        |              |_|   |                 |  O LD4             |         |
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

In master board, I2S peripheral is configured as Master Transmitter, whereas in 
Slave board, I2S peripheral is configured as Slave Receiver. The data transfer is 
managed with DMA. 
The both devices are in Philips standard configuration with 16bit extended to 
32 bit data packet and 48KHz audio frequency. 

The I2S_Buffer_Tx is transmitted using DMA1 channel3 and the received values are 
loaded in the I2S_Buffer_Rx buffer using DMA1 channel2. Only the significant 16 
MSBs are sent and received, while the 32 packet remaining 16 LSBs are filled with 0 values
and don't generate any interrupt.

When the user press on the TAMPER Button on the Master Board, the master transmitter
start sending data stored in the I2S_Buffer_Tx buffer while the Slave Board receives
data I2S_Buffer_Rx buffer.

Once the transfer is completed a comparison is done and TransferStatus gives the 
data transfer status:
  - TransferStatus PASSED if transmitted and received data are the same: Green LED is On.
  - TransferStatus FAILED if transmitted and received data are the different: Red LED is on.


@par Directory contents 

  - I2S/I2S_DataExchangeDMA/stm32f0xx_conf.h    Library Configuration file
  - I2S/I2S_DataExchangeDMA/stm32f0xx_it.c      Interrupt handlers
  - I2S/I2S_DataExchangeDMA/stm32f0xx_it.h      Interrupt handlers header file
  - I2S/I2S_DataExchangeDMA/main.c              Main program
  - I2S/I2S_DataExchangeDMA/main.h              Header for main.c module
  - I2S/I2S_DataExchangeDMA/system_stm32f0xx.c  STM32F0xx system source file
  
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
      - Two boards should be connected as follows:
        - Connect I2S1 SCK pin (PB.03) to I2S1 SCK pin
        - Connect I2S1 WS pin (PA.15) to I2S1 WS pin 
        - Connect I2S1 SD pin (PB.05) to I2S1 SD pin 
        - Connect Gnd pins of two boards. 
  - STM32072B-EVAL Set-up
      - Two boards should be connected as follows:
        - Connect I2S1 SCK pin (PA.05) to I2S1 SCK pin 
        - Connect I2S1 WS pin (PA.04) to I2S1 WS pin 
        - Connect I2S1 SD pin (PA.07) to I2S1 SD pin 
        - Connect Gnd pins of two boards.


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
