/**
  @page USART_Smartcard USART Smartcard example
  
  @verbatim
  ******************** (C) COPYRIGHT 2011 STMicroelectronics *******************
  * @file    USART/Smartcard/readme.txt 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Description of the USART Smartcard example.
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

This example provides a description of how  to use the USART in Smartcard mode. 
The example gives only the possibility to read the ATR and decode it into 
predefined buffer.
First, the code is waiting for an card insertion. If a card is detected through
the EXTI Line interrupt (connected to the Smartcard detect pin), a reset signal
is applied to the card through its reset pin.
As response to this reset, the card transmit the ATR which will be stored in 
predefined buffer. Once the ATR is received, it is decoded and stored in a specific
structure (SC_A2R) and the card protocol type is stored in a variable.
ATRDecodeStatus variable must be equal to 1 (PASSED) when the sequence succeed.

The used Smartcard should be ISO7816-3 T=0 compatible.

SC_USART configured as follow:
  - Word Length = 9 Bits
  - 0.5 Stop Bit
  - Even parity
  - BaudRate = 12096 baud
  - Hardware flow control disabled (RTS and CTS signals)
  - Tx and Rx enabled
  - USART Clock enabled
  - USART CPOL: Clock is active low
  - USART CPHA: Data is captured on the second edge 
  - USART LastBit: The clock pulse of the last data bit is not output to 
                   the SCLK pin


@par Directory contents 

  - USART/Smartcard/platform_config.h    Evaluation board specific configuration file
  - USART/Smartcard/stm32f10x_conf.h     Library Configuration file
  - USART/Smartcard/stm32f10x_it.h       Interrupt handlers header file
  - USART/Smartcard/stm32f10x_it.c       Interrupt handlers
  - USART/Smartcard/main.c               Main program
  - USART/Smartcard/system_stm32f10x.c   STM32F10x system source file

@par Hardware and Software environment 

  - This example runs on STM32F10x Connectivity line, High-Density, High-Density
    Value line, Medium-Density, XL-Density, Medium-Density Value line, Low-Density 
    and Low-Density Value line Devices.
  
  - This example has been tested with STMicroelectronics STM3210C-EVAL (Connectivity line),
    STM3210E-EVAL (High-Density and XL-Density) and STM3210B-EVAL (Medium-Density)
    evaluation boards and can be easily tailored to any other supported device and
    development board.
    To select the STMicroelectronics evaluation board used to run the example, 
    uncomment the corresponding line in USART/Smartcard/platform_config.h file

  - STM3210C-EVAL Set-up 
    - Plug a Smartcard (ISO7816-3 T=0 compatible) into  the dedicated Smartcard 
      connector CN5.
      @note In this case USART3 Tx and CK pins are full remapped by software.
            Make sure that Jumper 11 (JP11), Jumper 12 (JP12) and Jumper 13 (JP13)
            are in position 1<-->2.
        
  - STM3210E-EVAL Set-up 
    - Plug a Smartcard (ISO7816-3 T=0 compatible) into  the dedicated Smartcard 
      connector CN18.
      @note Make sure that Jumper 15 (JP15) and Jumper 16 (JP16) are fitted.

  - STM3210B-EVAL Set-up   
    - Plug a Smartcard (ISO7816-3 T=0 compatible) into the dedicated Smartcard 
      connector CN16.

  
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
