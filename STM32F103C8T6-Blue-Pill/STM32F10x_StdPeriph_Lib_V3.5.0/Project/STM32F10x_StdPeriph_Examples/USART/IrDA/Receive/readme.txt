/**
  @page USART_IrDA_Receive USART IrDA Receive example
  
  @verbatim
  ******************** (C) COPYRIGHT 2011 STMicroelectronics *******************
  * @file    USART/IrDA/Receive/readme.txt 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Description of the USART IrDA Receive example.
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

This example provides a basic communication USARTy IrDA receive mode. Four leds
are used to show which byte is received. USARTy can be USART3 or USART2 depending
on the STMicroelectronics EVAL board you are using.

  - LED1 toggles when 0x05 is received
  - LED2 toggles when 0x02 is received
  - LED3 toggles when 0x03 is received
  - LED4 toggles when 0x04 is received
  - LED1, LED2, LED3 and LED4 toggle when 0x01 is received        

USARTy configured as follow:
   - BaudRate = 115200 baud  
   - Word Length = 8 Bits
   - One Stop Bit
   - No parity
   - Hardware flow control disabled (RTS and CTS signals)
   - Receive and transmit enabled

The USART IrDA example provides two IrDA program: transmitter&receiver and 
requires two boards to be able to run the full demonstration:
 - one board will act as IrDA transmitter
 - one board will act as IrDA receiver


@par Directory contents 

  - USART/IrDA/Receive/platform_config.h    Evaluation board specific configuration file
  - USART/IrDA/Receive/stm32f10x_conf.h     Library Configuration file
  - USART/IrDA/Receive/stm32f10x_it.h       Interrupt handlers header file
  - USART/IrDA/Receive/stm32f10x_it.c       Interrupt handlers
  - USART/IrDA/Receive/main.c               Main program
  - USART/IrDA/Receive/system_stm32f10x.c   STM32F10x system source file

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
    uncomment the corresponding line in USART/IrDA/Receive/platform_config.h or stm32_eval.h file

  - STM32100E-EVAL Set-up 
    - Use an IrDA transceiver connected to the USART3 Tx pin (U16 connector, JP15 
      jumper must be fitted).
    - Use LED1, LED2, LED3 and LED4 leds connected respectively to PC.06, PC.07,
      PC.08 and PC.09 pins 
      @note In this case USART3 Tx and Rx pins are remapped by software on PC.10 
            and PC.11 respectively.
            
  - STM32100B-EVAL Set-up 
    - Use an IrDA transceiver connected to the USART3 Tx pin (U14 connector, JP11 
      jumper must be fitted).
    - Use LED1, LED2, LED3 and LED4 leds connected respectively to PC.06, PC.07,
      PC.08 and PC.09 pins 
      @note In this case USART3 Tx and Rx pins are remapped by software on PC.10 
            and PC.11 respectively. 

  - STM3210C-EVAL Set-up 
    - Use an IrDA transceiver connected to the USART2 Tx and Rx pins (U12 
      connector, JP16 should be in position 1<-->2).
    - Use LED1, LED2, LED3 and LED4 connected respectively to PD.07, PD.13, PF.03
      and PD.04 pins
    
  - STM3210E-EVAL Set-up 
    - Use an IrDA transceiver connected to the USART3 Tx pin (U13 connector, JP21
      and JP22 jumper must be fitted).
    - Use LED1, LED2, LED3 and LED4 leds connected respectively to PF.06, PF0.7,
      PF.08 and PF.09 pins

  - STM3210B-EVAL Set-up 
    - Use an IrDA transceiver connected to the USART3 Tx pin (U11 connector, JP5 
      jumper must be fitted).
    - Use LED1, LED2, LED3 and LED4 leds connected respectively to PC.06, PC.07,
      PC.08 and PC.09 pins 

  
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
