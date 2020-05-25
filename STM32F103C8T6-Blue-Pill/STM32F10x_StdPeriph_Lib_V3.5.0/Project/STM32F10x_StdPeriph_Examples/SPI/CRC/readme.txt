/**
  @page SPI_CRC SPI CRC example
  
  @verbatim
  ******************** (C) COPYRIGHT 2011 STMicroelectronics *******************
  * @file    SPI/CRC/readme.txt 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Description of the SPI CRC example.
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
SPIs in full-duplex mode and performs a transfer from Master to Slave and  
Slave to Master followed by CRC transmission.

SPI1 is configured as master and SPI2 as slave and both are in full-duplex
configuration mode with 16bit data size and a 4.5 Mbit/s communication speed 
(for Value line devices the speed is set at 1.5 Mbit/s).
CRC calculation is enabled for both SPIs.

After enabling both SPIs, the first data from SPI2_Buffer_Tx is transmitted from
slave followed by the first data from SPI1_Buffer_Tx send by the master. A test 
on RxNE flag is done for both master and slave to check the reception of data on
their respective data register. The same procedure is done for the remaining data
to transfer except the last ones. 

Last data from SPI1_Buffer_Tx is transmitted followed by enabling CRC transmission
for SPI1 and the last data from SPI2_Buffer_Tx is transmitted followed by enabling
CRC transmission for SPI2: user must take care to reduce code on this phase for
high speed communication. 

Last transmitted buffer data and CRC value are then received successively on
master and slave data registers. The received CRC value are stored on CRC1Value
and CRC2Value respectively for SPI1 and SPI2.

Once the transfer is completed a comparison is done and TransferStatus1 and 
TransferStatus2 gives the data transfer status for each data transfer direction 
where it is PASSED if transmitted and received data are the same otherwise it 
is FAILED.
A check of CRC error flag, for the master and the salve, is done after receiving
CRC data.

@par Directory contents 

  - SPI/CRC/stm32f10x_conf.h     Library Configuration file
  - SPI/CRC/stm32f10x_it.c       Interrupt handlers
  - SPI/CRC/stm32f10x_it.h       Header for stm32f10x_it.c
  - SPI/CRC/main.c               Main program
  - SPI/CRC/system_stm32f10x.c   STM32F10x system source file
  
@par Hardware and Software environment 

  - This example runs on STM32F10x Connectivity line, High-Density, High-Density 
    Value line, Medium-Density, XL-Density, Medium-Density Value line, Low-Density 
    and Low-Density Value line Devices.
  
  - This example has been tested with STMicroelectronics STM32100E-EVAL (High-Density 
    Value line), STM32100B-EVAL(STM32F10x Medium-Density Value line), STM3210E-EVAL 
    (STM32F10x High-Density) and STM3210B-EVAL (STM32F10x Medium-Density) 
    evaluation boards and can be easily tailored to any other supported 
    device and development board.
    This example can't be tested with STMicroelectronics STM3210C-EVAL (STM32F10x 
    Connectivity Line) evaluation board.

  - STM32100E-EVAL Set-up 
     - Connect SPI1 SCK (PA.05) pin to SPI2 SCK (PB.13) pin
     - Connect SPI1 MISO (PA.06) pin to SPI2 MISO (PB.14) pin
     - Connect SPI1 MOSI (PA.07) pin to SPI2 MOSI (PB.15) pin
         
  - STM32100B-EVAL Set-up 
     - Connect SPI1 SCK (PA.05) pin to SPI2 SCK (PB.13) pin
     - Connect SPI1 MISO (PA.06) pin to SPI2 MISO (PB.14) pin
     - Connect SPI1 MOSI (PA.07) pin to SPI2 MOSI (PB.15) pin

  - STM3210E-EVAL Set-up 
     - Connect SPI1 SCK (PA.05) pin to SPI2 SCK (PB.13) pin
     - Connect SPI1 MISO (PA.06) pin to SPI2 MISO (PB.14) pin
     - Connect SPI1 MOSI (PA.07) pin to SPI2 MOSI (PB.15) pin
    @note The jumper 14 (USB Disconnect) must be set in position 1<->2 in order
          to not interfer with SPI2 MISO pin PB14.

  - STM3210B-EVAL Set-up 
     - Connect SPI1 SCK (PA.05) pin to SPI2 SCK (PB.13) pin
     - Connect SPI1 MISO (PA.06) pin to SPI2 MISO (PB.14) pin
     - Connect SPI1 MOSI (PA.07) pin to SPI2 MOSI (PB.15) pin
	 
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
