/**
  @page SPI_Simplex_Interrupt SPI Simplex Interrupt example
  
  @verbatim
  ******************** (C) COPYRIGHT 2011 STMicroelectronics *******************
  * @file    SPI/Simplex_Interrupt/readme.txt 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Description of the SPI Simplex Interrupt example.
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
SPIs in simplex mode and performs a data buffer transfer from SPI_MASTER to
SPI_SLAVE using TxE interrupt for master and RxNE interrupt for slave.
SPI_MASTER and SPI_SLAVE can be SPI1 and SPI2 or SPI3 and SPI2, depending on the
STMicroelectronics EVAL board you are using.

Both SPIs are configured with 8bit data frame and a 9Mbit/s communication speed
(for Value line devices the communication speed is set to 3Mbit/s).
The TxE interrupt is enabled for the master and the RxNE interrupt is enabled for
the slave.

Once both SPIs are enabled, first TxE interrupt is generated for the master
and in its interrupt service routine the first data is sent from SPI_MASTER_Buffer_Tx. 
Once this data is received by the slave the RxNE interrupt is generated and in
the routine this data is stored in the SPI_SLAVE_Buffer_Rx.

The same procedure is followed for the remaining SPI_MASTER_Buffer_Tx data.
Once all data buffer are received by the slave the TxE interrupt is disabled.
A comparison is done and TransferStatus variable gives the data transfer status
where it is PASSED if transmitted and received data are the same otherwise it is FAILED.


@par Directory contents 

  - SPI/Simplex_Interrupt/platform_config.h    Evaluation board specific configuration file
  - SPI/Simplex_Interrupt/stm32f10x_conf.h     Library Configuration file
  - SPI/Simplex_Interrupt/stm32f10x_it.c       Interrupt handlers
  - SPI/Simplex_Interrupt/stm32f10x_it.h       Header for stm32f10x_it.c
  - SPI/Simplex_Interrupt/main.c               Main program
  - SPI/Simplex_Interrupt/system_stm32f10x.c   STM32F10x system source file

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
    uncomment the corresponding line in SPI/Simplex_Interrupt/platform_config.h file.  

  - STM32100E-EVAL Set-up 
    - Connect SPI1 SCK pin (PA.05) to SPI2 SCK pin (PB.13)
    - Connect SPI1 MOSI pin (PA.07) to SPI2 MISO pin (PB.14)
    
  - STM32100B-EVAL Set-up 
    - Connect SPI1 SCK pin (PA.05) to SPI2 SCK pin (PB.13)
    - Connect SPI1 MOSI pin (PA.07) to SPI2 MISO pin (PB.14)

  - STM3210C-EVAL Set-up 
    - Connect SPI3 SCK pin (PC.10) to SPI2 SCK pin (PB.13)
    - Connect SPI3 MOSI pin (PC.12) to SPI2 MISO pin (PB.14)
    @note In this case SPI3 pins are remapped by software.

  - STM3210E-EVAL Set-up 
    - Connect SPI1 SCK pin (PA.05) to SPI2 SCK pin (PB.13)
    - Connect SPI1 MOSI pin (PA.07) to SPI2 MISO pin (PB.14)
    @note The jumper 14 (USB Disconnect) must be set in position 1<->2 in order
          to not interfer with SPI2 MISO pin PB14.

  - STM3210B-EVAL Set-up 
    - Connect SPI1 SCK pin (PA.05) to SPI2 SCK pin (PB.13)
    - Connect SPI1 MOSI pin (PA.07) to SPI2 MISO pin (PB.14)
    
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
