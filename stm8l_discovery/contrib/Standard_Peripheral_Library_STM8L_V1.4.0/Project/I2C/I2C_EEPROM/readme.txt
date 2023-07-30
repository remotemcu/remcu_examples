/** @page I2C_EEPROM I2C1 in communication with I2C EEPROM.

  @verbatim
  ******************** (C) COPYRIGHT 2013 STMicroelectronics *******************
  * @file    I2C/I2C_EEPROM/readme.txt 
  * @author  MCD Application Team
  * @version V1.5.2
  * @date    30-September-2014
  * @brief   Description of the I2C EEPROM Example.
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

  This example provides a basic example of how to use the I2C software library
  and an associated I2C EEPROM driver to communicate with an I2C EEPROM device 
  (here the example is interfacing with E24C64 EEPROMs  .

  At startup a message is displayed on the HyperTerminal (through USART   asking
  user to enter the data to write into the SPI FLASH. Each byte received is sent
  back to the HyperTerminal and stored in the predefined buffer RxBuffer1.
  The communication is managed by USART Transmit and Receive interrupts.

  If user presses keyboard ENTER key or RxBuffer1 length is reached (defined by 
  RX_BUFFER1_SIZE in main.c  , the program starts programming the I2C EEPROM with
  data entered by the user.  
  
  Next, the content of RxBuffer1 is written to the EEPROM_WriteAddress1 and the
  written data is read. The written and the read buffers data are then compared
  and the result of this comparison is stored in the "TransferStatus1" variable.
  Following the read operation, the program wait that the EEPROM reverts to its 
  Standby state. 
  A second write operation is, then, performed and this time, TxBuffer2 is 
  written to EEPROM_WriteAddress2, which represents the address just after the 
  last written one in the first write.
  After completion of the second  write operation, the written data are read. 
  The contents of the written and the read buffers are compared and the result
  of this comparison is stored in the "TransferStatus2" variable. 

  I2C peripheral is configured in Master transmitter during write operation and
  in Master receiver during read operation from I2C EEPROM. The speed is set to
  200kHz.

  For E24C64 device all the memory is accessible through the two-bytes 
  addressing mode and need to define block addresses. In this case, only the 
  physical address has to be defined (according to the address pins (E0,E1 and 
  E2) connection.
  This address is defined in stm8_eval_i2c_ee.h (default is 0xA0: E0, E1 and E2 
  tied to ground. 
  The EEPROM addresses where the program start the write and the read operations
  is defined in the main.c file.


  @par Directory contents

  - I2C/I2C_EEPROM/main.c                       Main file
  - I2C/I2C_EEPROM/stm8l15x_conf.h              Library Configuration file
  - I2C/I2C_EEPROM/stm8l15x_it.c                Interrupt routines source
  - I2C/I2C_EEPROM/stm8l15x_it.h                Interrupt routines declaration


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
    - For write operations, make sure the jumper JP6 "E2P_WP" is connected on 
      the board (If this jumper is not connected, the EEPROM will be write-
      protected).
    - Connect a null-modem female/female RS232 cable between the DB9 connector 
       CN1 and PC serial port.
    - Make sure that JP5 jumper is in RS232 position
    - Make sure that JP11 jumper is in RS232 position
    - Make sure that JP12 jumper is in RS232 position
        
  - STM8L1526-EVAL Set-up 
    - Make sure that the LCD glass daughter board is mounted in IO position.
       For more details please refer to the evaluation board user manual.
    - Make sure that Jumper JP7 is set in position 1-2 (I2C position  .
    - For write operations, make sure the jumper JP14 "E2P_WP" is connected on 
      the board (If this jumper is not connected, the EEPROM will be write-protected).
    - Connect a null-modem female/female RS232 cable between the DB9 connector 
      CN1 and PC serial port.
    - Make sure that JP5 jumper is in RS232 position
    - Make sure that the LCD glass daughter board is mounted in IO position.

  - Other platform Set-up
    - Use STM8L1526-EVAL hardware configuration defines.
    - Connect I2C1 SCL pin (PC.01   to I2C EEPROM SCL (pin6  
    - Connect I2C1 SDA pin (PC.00   to I2C EEPROM SDA (pin5  
    - Check that a pull-up resistor (4.7K) is connected on one I2C SDA pin
    - Check that a pull-up resistor (4.7K) is connected on one I2C SCL pin
    - Connect I2C EEPROM Vcc (pin8) to Vdd
    - Connect I2C EEPROM Vss (pin4) to Vss
    - For E24C64: Connect I2C EEPROM E0, E1 and E2 (pin1, pin2 and pin3   to Vss
  @note The pull-up resistors are already implemented on the STM8L1528-EVAL and 
         STM8L1526-EVAL evaluation boards.

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
    - stm8_eval_i2c_ee.c (under Utilities\STM8_EVAL\Common)
  - Rebuild all files and load your image into target memory
  - Run the example
  


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
