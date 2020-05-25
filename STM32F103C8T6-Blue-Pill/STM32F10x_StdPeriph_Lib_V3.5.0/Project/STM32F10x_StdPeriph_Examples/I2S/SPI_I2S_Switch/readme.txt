/**
  @page I2S_SPI_I2S_Switch SPI_I2S_Switch example
  
  @verbatim
  ******************** (C) COPYRIGHT 2011 STMicroelectronics *******************
  * @file    I2S/SPI_I2S_Switch/readme.txt 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Description of the SPI_I2S_Switch example.
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

This example provides a description of how to set a communication between two
SPIs in I2S mode, and how to switch between SPI and I2S modes, performing a 
transfer from Master to Slave in I2S modes then a transfer from master to slave 
in SPI mode and finally a transfer from Slave to Master in I2S mode.

I2S2 is configured as master transmitter and I2S3 as slave receiver and both are 
in Philips standard configuration with 16bit data size in 32bit packet length 
and 48KHz audio frequency.

In the first phase, the master I2S2 starts the I2S2_Buffer_Tx transfer while the 
slave I2S3 receives and loads the values in I2S3_Buffer_Rx. Once the transfer is 
completed a comparison is done and TransferStatus1 gives the data transfer status 
where it is PASSED if transmitted and received data are the same otherwise it is 
FAILED.

In the second step, both peripherals are configured in SPI modes (simplex 
communication) and SPI2_Buffer_Tx transfer is performed in simplex mode from SPI2 to
SPI3.Once the transfer is completed a comparison is done and TransferStatus2 gives 
the data transfer status where it is PASSED if transmitted and received data are 
the same otherwise it is FAILED.
As the master/slave mode is managed by software (the master is the clock (CK and WS) 
generator), this allows to I2S2 to become slave transmitter and I2S3 to become master 
receiver without hardware modification. 

In the third step, the slave I2S2 prepares the first data to be sent before the 
master is enabled. Once the master is enabled, the clocks are released from the 
master and the data are released on the slave. Once the transfer is completed 
a comparison is done and TransferStatus3 gives the data transfer status where it 
is PASSED if transmitted and received data are the same otherwise it is FAILED.

@par Directory contents 

  - I2S/SPI_I2S_Switch/stm32f10x_conf.h     Library Configuration file
  - I2S/SPI_I2S_Switch/stm32f10x_it.c       Interrupt handlers
  - I2S/SPI_I2S_Switch/stm32f10x_it.h       Header for stm32f10x_it.c
  - I2S/SPI_I2S_Switch/main.c               Main program
  - I2S/SPI_I2S_Switch/system_stm32f10x.c   STM32F10x system source file  

@par Hardware and Software environment 

  - This example runs on STM32F10x High-Density, XL-Density and Connectivity Line 
    Devices.
  
  - This example has been tested with STMicroelectronics STM3210E-EVAL (High-Density
    and XL-Density) and STM3210C-EVAL (Connectivity Line) evaluation boards 
    and can be easily tailored to any other supported device and development board.

  - STM3210C-EVAL Set-up 
     - Connect I2S2 WS (PB.12) pin to I2S3 WS (PA.04) pin
     - Connect I2S2 CK (PB.13) pin to I2S3 CK (PC.10) pin
     - Connect I2S2 SD (PB.15) pin to I2S3 SD (PC.12) pin

  - STM3210E-EVAL Set-up 
     - Connect I2S2 WS (PB.12) pin to I2S3 WS (PA.15) pin
     - Connect I2S2 CK (PB.13) pin to I2S3 CK (PB.03) pin
     - Connect I2S2 SD (PB.15) pin to I2S3 SD (PB.05) pin

Since some SPI3/I2S3 pins are shared with JTAG pins (SPI3_NSS/I2S3_WS with JTDI 
and SPI3_SCK/I2S3_CK with JTDO), they are not controlled by the I/O controller
and are reserved for JTAG usage (after each Reset).
For this purpose prior to configure the SPI3/I2S3 pins: 
- For STM32F10x High-Density devices, the user has to disable the JTAG and use 
  the SWD interface (when debugging the application), or disable both JTAG/SWD 
  interfaces (for standalone application).
- For STM32F10x Connectivity Line devices, the user can use the solution above 
  (SWD or disable bothe JTAG and SWD), or it is possible to remap the SPI3 pins 
  on {PC10, PC11, PC12, PA4} GPIO pins in order to avoid the conflict with JTAG 
  pins (and it is possible in this case to use JTAG interface). This remap is 
  used for STM3210C-EVAL evaluation boards in this example.

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
