/** @page SPI_SlaveDMA  Use SPI as slave receiver in DMA mode while USART sends data in interrupt mode

  @verbatim
  ******************** (C) COPYRIGHT 2013 STMicroelectronics *******************
  * @file    SPI/SPI_SlaveDMA/readme.txt 
  * @author  MCD Application Team
  * @version V1.5.2
  * @date    30-September-2014
  * @brief   Description of the SPI SLave DMA Example.
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


  @par Example description

  This example provides a description of how to use the SPI firmware library to
  configure SPI as Slave receiver in DMA mode.

  At startup a message is displayed on the HyperTerminal asking user to enter
  the data to be sent to SPI. Each byte received is sent back to the 
  HyperTerminal and stored in the predefined buffer RxBuffer1.
  
  If user press keyboard ENTER key or RxBuffer1 length is reached (defined by 
  RX_BUFFER_SIZE in main.c), USART is reconfigured to send the received Data 
  to SPI already configured in Slave receiver with DMA.
  
  The received SPI data is compared with the one Sent by USART and the result of 
  this comparison is stored in the "TransferStatus1" variable.
  And depending on it a message is displayed on Hyperterminal indicating the
  consistency or not of the communication operation.

  For medium density devices, the communication is managed by USART1 Transmit 
  and Receive interrupts.
  For high density devices, the communication is managed by USART2 Transmit
  and Receive interrupts.
  
  @par Directory contents

  - SPI/SPI_SlaveDMA/main.c             Main file containing the "main" function
  - SPI/SPI_SlaveDMA/stm8l15x_conf.h    Library Configuration file
  - SPI/SPI_SlaveDMA/stm8l15x_it.c      Interrupt routines source
  - SPI/SPI_SlaveDMA/stm8l15x_it.h      Interrupt routines declaration


  @par Hardware and Software environment

    - This example runs on STM8L15x High-Density, Medium-Density Plus, Medium-Density
    and Low-Density Devices.
  
  - This example has been tested with STMicroelectronics STM8L1528-EVAL 
    (STM8L15x High-Density devices) and STM8L1526-EVAL (STM8L15x Medium-Density and
    Low-Density devices) evaluation boards and can be easily tailored to any other
    development board.

  - STM8L1528-EVAL Set-up 
    - Make sure that the LCD glass daughter board is mounted in IO position.
      For more details please refer to the evaluation board user manual.
    - Connect a null-modem female/female RS232 cable between the DB9 connector 
       CN7 and PC serial port.
    - Make sure that JP5 jumper is in RS232 position
    - Make sure that JP11 jumper is in RS232 position
    - Make sure that JP12 jumper is in RS232 position
    - Connect USART to SPI as follows:
      - USART2 CK (PH6) ->  SPI1 SCK  (PB5)   
      - USART2 TX (PH5) ->  SPI1 MOSI (PB6)   

  - STM8L1526-EVAL Set-up 
    - Make sure that the LCD glass daughter board is mounted in IO position.
      For more details please refer to the evaluation board user manual.
    - Connect a null-modem female/female RS232 cable between the DB9 connector 
       CN1 and PC serial port.
    - Make sure that JP5 jumper is in RS232 position
    - Connect USART to SPI as follows:
      - USART1 CK (PC4) ->  SPI1 SCK  (PB5)  
      - USART1 TX (PC3) ->  SPI1 MOSI (PB6)   


  - HyperTerminal configuration:
     - Word Length = 7 Bits
     - One Stop Bit
     - Odd parity
     - BaudRate = 115200 baud

  @par How to use it ?

  In order to make the program work, you must do the following :

  - Copy all source files from this example folder to the template folder under
    Project\STM8L15x_StdPeriph_Template
  - Open your preferred toolchain 
  - Add the required file:
    - stm8_eval.c   (under Utilities\STM8_EVAL)
  - Rebuild all files and load your image into target memory
  - Run the example
  

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
