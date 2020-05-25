/**
  @page SPI_FullDuplex_SoftNSS SPI Full Duplex Software NSS example
  
  @verbatim
  ******************** (C) COPYRIGHT 2011 STMicroelectronics *******************
  * @file    SPI/FullDuplex_SoftNSS/readme.txt 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Description of the SPI Full Duplex Software NSS example.
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

This example provides a description of how to set a communication between SPIy and
SPIz in full-duplex mode and performs a transfer from Master to Slave and then 
Slave to Master in the same application with software NSS management.
SPIy and SPIz can be SPI1 and SPI2 or SPI3 and SPI2, depending on the STMicroelectronics
EVAL board you are using.

Both SPIs are configured with 8bit data frame and a 9Mbit/s communication speed.
(for Value line devices the speed is set at 6Mbit/s).
In the first phase, the master SPIy starts the SPIy_Buffer_Tx transfer while the 
slave SPIz transmit SPIz_Buffer_Tx. Once the transfer is completed a comparison 
is done and TransferStatus1 and TransferStatus2 gives the data transfer status for
each data transfer direction where it is PASSED if transmitted and received data 
are the same otherwise it is FAILED.

As the NSS pin is managed by software, this permit to SPIy to become slave and SPIz
to become master without hardware modification.
In the second step, the slave SPIy starts the SPIy_Buffer_Tx transfer while the 
master SPIz transmit SPIz_Buffer_Tx. Once the transfer is completed a comparison 
is done and TransferStatus3 and TransferStatus4 gives the data transfer status for
each data transfer direction where it is PASSED if transmitted and received data 
are the same otherwise it is FAILED.


@par Directory contents 

  - SPI/FullDuplex_SoftNSS/platform_config.h   Evaluation board specific configuration file
  - SPI/FullDuplex_SoftNSS/stm32f10x_conf.h    Library Configuration file
  - SPI/FullDuplex_SoftNSS/stm32f10x_it.c      Interrupt handlers
  - SPI/FullDuplex_SoftNSS/stm32f10x_it.h      Interrupt handlers header file
  - SPI/FullDuplex_SoftNSS/main.c              Main program
  - SPI/FullDuplex_SoftNSS/system_stm32f10x.c  STM32F10x system source file

@par Hardware and Software environment 

  - This example runs on STM32F10x Connectivity line, High-Density, High-Density
    Value line, Medium-Density, XL-Density, Medium-Density Value line, Low-Density
    and Low-Density Value line Devices.
  
  - This example has been tested with STMicroelectronics STM32100E-EVAL (High-Density 
    Value line), STM32100B-EVAL (Medium-Density Value line), STM3210C-EVAL (Connectivity line), 
    STM3210E-EVAL (High-Density and XL-Density) and STM3210B-EVAL (Medium-Density) 
    evaluation boards and can be easily tailored to any other supported device 
    and development board.
    To select the STMicroelectronics evaluation board used to run the example, 
    uncomment the corresponding line in SPI/FullDuplex_SoftNSS/platform_config.h file.  

  - STM32100E-EVAL Set-up 
    - Connect SPI1 SCK pin (PA.05) to SPI2 SCK pin	(PB.13)
    - Connect SPI1 MISO pin (PA.06) to SPI2 MISO pin (PB.14)
    - Connect SPI1 MOSI pin (PA.07) to SPI2 MOSI pin (PB.15)
    
  - STM32100B-EVAL Set-up 
    - Connect SPI1 SCK pin (PA.05) to SPI2 SCK pin	(PB.13)
    - Connect SPI1 MISO pin (PA.06) to SPI2 MISO pin (PB.14)
    - Connect SPI1 MOSI pin (PA.07) to SPI2 MOSI pin (PB.15)
    
  - STM3210C-EVAL Set-up 
    - Connect SPI3 SCK pin (PC.10) to SPI2 SCK pin	(PB.13)
    - Connect SPI3 MISO pin (PC.11) to SPI2 MISO pin (PB.14)
    - Connect SPI3 MOSI pin (PC.12) to SPI2 MOSI pin (PB.15)
    @note In this case SPI3 pins are remapped by software.

  - STM3210E-EVAL Set-up 
    - Connect SPI1 SCK pin (PA.05) to SPI2 SCK pin	(PB.13)
    - Connect SPI1 MISO pin (PA.06) to SPI2 MISO pin (PB.14)
    - Connect SPI1 MOSI pin (PA.07) to SPI2 MOSI pin (PB.15)
    @note The jumper 14 (USB Disconnect) must be set in position 1<->2 in order
          to not interfer with SPI2 MISO pin PB14.

  - STM3210B-EVAL Set-up 
    - Connect SPI1 SCK pin (PA.05) to SPI2 SCK pin	(PB.13)
    - Connect SPI1 MISO pin (PA.06) to SPI2 MISO pin (PB.14)
    - Connect SPI1 MOSI pin (PA.07) to SPI2 MOSI pin (PB.15)
    
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
