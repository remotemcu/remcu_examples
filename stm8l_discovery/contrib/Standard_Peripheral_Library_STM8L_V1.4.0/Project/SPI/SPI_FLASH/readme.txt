/** @page SPI_FLASH Use the SPI and an associated driver to communicate with an M25P128 FLASH

  @verbatim
  ******************** (C) COPYRIGHT 2013 STMicroelectronics *******************
  * @file    SPI/SPI_FLASH/readme.txt 
  * @author  MCD Application Team
  * @version V1.5.2
  * @date    30-September-2014
  * @brief   Description of the SPI FLASH Example.
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

  This example provides a description of how to use the SPI firmware library and
  an associate SPI FLASH driver to communicate with an M25P128 FLASH.

  At startup a message is displayed on the HyperTerminal (through COM1) asking
  user to enter the data to write into the SPI FLASH. Each byte received is sent
  back to the HyperTerminal and stored in the predefined buffer RxBuffer1.
  The communication is managed by COM1 Transmit and Receive interrupts.

  If user press keyboard ENTER key or RxBuffer1 length is reached (defined by 
  RX_BUFFER_SIZE in main.c), the program starts programming the SPI FLASH with
  data entered by user.  
  
  First the SPI Flash ID is read, if the ID is correct LED1 is turned on 
  otherwise LED2 is turned on, in that case the used eval board should be 
  powered Off and On again.  
  
  Then the program performs an erase of the sector to be accessed, a write of
  RxBuffer1 (entered by user from HyperTerminal) to the memory followed by a 
  read of the written data. Then data read from the memory stored in the 
  RxBuffer2 are compared with the expected values of the RxBuffer1. The result 
  of this comparison is stored in the "TransferStatus1" variable.
  
  Finally a second erase of the same sector is done and a test is done to be 
  sure that all the data written there are erased. All the data location is read
  and checked with 0xFF value. The result of this test is stored in
  "TransferStatus2" variable which is FAILED in case of error.

  The FLASH_WRITE_ADDRESS and the FLASH_READ_ADDRESS where the program start the
  write and the read operations are defined in the main.c file.


  @par Directory contents

  - SPI/SPI_FLASH/main.c                       Main file containing the "main" function
  - SPI/SPI_FLASH/stm8l15x_conf.h              Library Configuration file
  - SPI/SPI_FLASH/stm8l15x_it.c                Interrupt routines source
  - SPI/SPI_FLASH/stm8l15x_it.h                Interrupt routines declaration


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
    - LED1 and LED2 connected respectively to PH.0 and PH.1 pins
    - M25P128 FLASH is already available on this board.
  
  - STM8L1526-EVAL Set-up 
    - Make sure that the LCD glass daughter board is mounted in IO position.
       For more details please refer to the evaluation board user manual.
    - Connect a null-modem female/female RS232 cable between the DB9 connector 
       CN1 and PC serial port.
    - Make sure that JP5 jumper is in RS232 position
    - LED1 and LED2 connected respectively to PA.04 and PA.05 pins
    - M25P128 FLASH is already available on this board.


  - HyperTerminal configuration:
     - Word Length = 7 Bits
     - One Stop Bit
     - Odd parity
     - BaudRate = 115200 baud

  - Other platform Set-up
    - Use STM8L1526-EVAL hardware configuration defines.
    - Connect LED1 and LED2 respectively to PA4 and PA5 pins.
    - Connect both SPI1 and SPI FLASH pins as following:
      - Connect SPI1_NSS (PB.04) pin to SPI Flash chip select (pin1).
      - Connect SPI1_SCLK (PB.05) pin to SPI Flash serial clock (pin6).
      - Connect SPI1_MOSI (PB.06) pin to SPI Flash serial data input (pin5).
      - Connect SPI1_MISO (PB.07) pin to SPI Flash serial data output (pin2).
      - Connect SPI Flash Write Protect (pin3)to Vdd.
      - Connect SPI Flash Hold (pin7) to Vdd.
      - Connect SPI Flash Vcc (pin8) to Vdd.
      - Connect SPI Flash Vss (pin4) to Vss.


  @par How to use it ?

  In order to make the program work, you must do the following :

  - Copy all source files from this example folder to the template folder under
    Project\STM8L15x_StdPeriph_Template
  - Open your preferred toolchain 
  - Add the required Library files :
     - stm8_eval.c   (under Utilities\STM8_EVAL)
     - stm8_eval_spi_flash.c (under Utilities\STM8_EVAL\Common)
  - Rebuild all files and load your image into target memory
  - Run the example
  


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
