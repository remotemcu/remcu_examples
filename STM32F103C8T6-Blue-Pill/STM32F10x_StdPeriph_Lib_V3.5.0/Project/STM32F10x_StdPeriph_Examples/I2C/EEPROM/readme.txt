/**
  @page I2C_EEPROM I2C and M24CXX EEPROM communication example
  
  @verbatim
  ******************** (C) COPYRIGHT 2011 STMicroelectronics *******************
  * @file    I2C/EEPROM/readme.txt 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Description of the I2C and M24CXX EEPROM communication example.
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

This example provides a basic example of how to use the I2C firmware library and
an associate I2C EEPROM driver to communicate with an I2C EEPROM device (here the
example is interfacing with M24CXX EEPROMs where XX={01, 02, 04, 08, 16, 32, 64}.

I2C peripheral is configured in Master transmitter during write operation and in
Master receiver during read operation from I2C EEPROM. 

The peripheral used is I2C1 but can be configured by modifying the defines values
in stm32_eval_i2c_ee.c file. The speed is set to 200kHz and can be configured to
other values by setting the define I2C_SPEED in stm32_eval_i2c_ee.h file.
All transfers are performed through DMA access (except for One Byte Receiving case)
which allows user application to perform parallel tasks while transfer to/from 
EEPROM is ongoing.

For M24C02 to M24C16 devices, one I2C EEPROM Block address where the program will 
write the buffer have to be selected from the four address available and defined 
in the stm32_eval_i2c_ee.h file.

For M24C32 and M24C64 devices all the memory is accessible through the two-bytes 
addressing mode and need to define block addresses. In this case, only the physical 
address has to be defined (according to the address pins (E0,E1 and E2) connection).
This address is defined in stm32_eval_i2c_ee.c.h (default is 0xA0: E0, E1 and E2 
tied to ground). 
The EEPROM addresses where the program start the write and the read operations 
is defined in the main.c file. 

First, the content of Tx1_Buffer is written to the EEPROM_WriteAddress1 and the
written data are read. The written and the read buffers data are then compared.
Following the read operation, the program waits that the EEPROM reverts to its 
Standby state. A second write operation is, then, performed and this time, Tx2_Buffer
is written to EEPROM_WriteAddress2, which represents the address just after the last 
written one in the first write. After completion of the second write operation, the 
written data are read. The contents of the written and the read buffers are compared.


@par Directory contents 

  - I2C/EEPROM/stm32f10x_conf.h    Library Configuration file
  - I2C/EEPROM/stm32f10x_it.c      Interrupt handlers
  - I2C/EEPROM/stm32f10x_it.h      Interrupt handlers header file
  - I2C/EEPROM/main.c              Main program
  - I2C/EEPROM/system_stm32f10x.c  STM32F10x system source file  

@par Hardware and Software environment 

  - This example runs on STM32F10x Connectivity line, High-Density, High-Density
    Value line, Medium-Density, XL-Density, Medium-Density Value line, Low-Density 
    and Low-Density Value line Devices.
  
  - This example has been tested with STM32100E-EVAL (STM32F10x High-Density Value 
    line) evaluation board (implemented EEPROM is M24C64) and STM3210C-EVAL 
    (STM32F10x Connectivity-Line) evaluation board (implemented EEPROM is M24C64) 
    with no additional hardware, and can be easily tailored to any other supported 
    device and development board.

  - STM32100E-EVAL Set-up 
    - Make sure the jumper JP14 "ROM_WP" is fitted on the board.
    - No additional Hardware connections are needed.


  - STM3210C-EVAL Set-up 
    - Make sure the Jumper JP17 "I2C_SCK" is fitted on the board.
    - Make sure the jumper JP9 "ROM_WP" is fitted on the board.
    - No additional Hardware connections are needed.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Copy all source files from this example folder to the template folder under
   Project\STM32F10x_StdPeriph_Template
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example 

@note
 - Low-density Value line devices are STM32F100xx microcontrollers where the 
   Flash memory density ranges between 16 and 32 Kbytes.
 - Low-density devices are STM32F101xx, STM32F102xx and STM32F103xx 
   microcontrollers where the Flash memory density ranges between 16 and 32 Kbytes.
 - Medium-density Value line devices are STM32F100xx microcontrollers where
   the Flash memory density ranges between 64 and 128 Kbytes.  
 - Medium-density devices are STM32F101xx, STM32F102xx and STM32F103xx 
   microcontrollers where the Flash memory density ranges between 64 and 128 Kbytes.
 - High-density Value line devices are STM32F100xx microcontrollers where
   the Flash memory density ranges between 256 and 512 Kbytes.   
 - High-density devices are STM32F101xx and STM32F103xx microcontrollers where
   the Flash memory density ranges between 256 and 512 Kbytes.
 - XL-density devices are STM32F101xx and STM32F103xx microcontrollers where
   the Flash memory density ranges between 512 and 1024 Kbytes.
 - Connectivity line devices are STM32F105xx and STM32F107xx microcontrollers.
   
 * <h3><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h3>
 */
