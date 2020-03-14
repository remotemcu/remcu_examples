/**
  @page USART_HyperTerminal_Interrupt USART Hyperterminal interrupt example
  
  @verbatim
  ******************** (C) COPYRIGHT 2011 STMicroelectronics *******************
  * @file    USART/HyperTerminal_Interrupt/readme.txt 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Description of the USART Hyperterminal interrupt example.
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

This example provides a description of how to use the EVAL_COM1 Transmit and Receive 
interrupts to communicate with the hyperterminal.
EVAL_COM1 can be USART1 or USART2 depending on the STMicroelectronics EVAL board
you are using.

First, the EVAL_COM1 sends the TxBuffer to the hyperterminal and still waiting for
a string from the hyperterminal that you must enter.
The string that you have entered is stored in the RxBuffer array. The receive
buffer have a RxBufferSize bytes as maximum (the reception is stopped when this
maximum receive value is reached).

The EVAL_COM1 is configured as follow:
    - BaudRate = 9600 baud  
    - Word Length = 8 Bits (7 data bit + 1 parity bit)
    - Two Stop Bit
    - Odd parity
    - Hardware flow control disabled (RTS and CTS signals)
    - Receive and transmit enabled

@note When the parity is enabled, the computed parity is inserted at the MSB
position of the transmitted data.
 
  
@par Directory contents 

  - USART/HyperTerminal_Interrupt/stm32f10x_conf.h    Library Configuration file
  - USART/HyperTerminal_Interrupt/stm32f10x_it.h      Interrupt handlers header file
  - USART/HyperTerminal_Interrupt/stm32f10x_it.c      Interrupt handlers source file
  - USART/HyperTerminal_Interrupt/main.c              Main program
  - USART/HyperTerminal_Interrupt/system_stm32f10x.c  STM32F10x system source file

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
    uncomment the corresponding line in stm32_eval.h file

  - STM32100B-EVAL Set-up 
    - Connect a null-modem female/female RS232 cable between the DB9 connector 
     CN10 and PC serial port.  

  - STM3210C-EVAL Set-up 
    - Connect a null-modem female/female RS232 cable between the DB9 connector 
      CN6 and PC serial port.
      @note Make sure that jumpers JP19 and JP18 are open.

  - STM3210E-EVAL Set-up 
    - Connect a null-modem female/female RS232 cable between the DB9 connector 
     CN12 and PC serial port.  

  - STM3210B-EVAL Set-up 
    - Connect a null-modem female/female RS232 cable between the DB9 connector 
     CN6 and PC serial port.  

  - STM32100E-EVAL Set-up 
    - Connect a null-modem female/female RS232 cable between the DB9 connector 
     CN10 and PC serial port. 
           
  - Hyperterminal configuration:
    - Word Length = 7 Bits
    - Two Stop Bit
    - Odd parity
    - BaudRate = 9600 baud
    - flow control: None 
      
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
