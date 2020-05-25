/**
  @page I2S_Interrupt I2S Interrupt example
  
  @verbatim
  ******************** (C) COPYRIGHT 2011 STMicroelectronics *******************
  * @file    I2S/Interrupt/readme.txt 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Description of the I2S Interrupt example.
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
SPIs in I2S mode using interrupts and performing a transfer from Master to Slave.

In the first step, I2S3 is configured as master transmitter and I2S2 as slave 
reliever and both are in Philips standard configuration with 16bit extended to 
32 bit data packet and 48KHz audio frequency. 

The I2S3 transmit interrupt and the I2S2 receive interrupt are both enabled. And
in these interrupts subroutines, the I2S3_Buffer_Tx is transmitted and the received
values are loaded in the I2S2_Buffer_Rx buffer. Only the significant 16 MSBs are
sent and received, while the 32 packet remaining 16 LSBs are filled with 0 values
and don't generate any interrupt.

Once the transfer is completed a comparison is done and TransferStatus1 gives the 
data transfer status where it is PASSED if transmitted and received data are the 
same otherwise it is FAILED.

In the second step both peripherals are configured in I2S Philips standard 24 bits
data length in 32 bits packets and 16KHz audio frequency. The interrupts are
enabled and the transfer is performed from the I2S3 master to the I2S2 slave.
The 24 bits are transmitted then the 8 remaining LSBs are filled automatically
with 0 values.

Once the transfer is completed a comparison is done (on the 24 MSBs only, the 8 
LSBs are replaced by 0) and TransferStatus2 gives the data transfer status where 
it is PASSED if transmitted and received data are the same otherwise it is FAILED.

@par Directory contents 

  - I2S/Interrupt/stm32f10x_conf.h     Library Configuration file
  - I2S/Interrupt/stm32f10x_it.c       Interrupt handlers
  - I2S/Interrupt/stm32f10x_it.h       Header for stm32f10x_it.c
  - I2S/Interrupt/main.c               Main program
  - I2S/Interrupt/system_stm32f10x.c   STM32F10x system source file
  
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
  (SWD or disable both JTAG and SWD), or it is possible to remap the SPI3 pins 
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
