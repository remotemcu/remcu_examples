/**
  @page NVIC_DMA_WFIMode DMA in WFI mode example
  
  @verbatim
  ******************** (C) COPYRIGHT 2011 STMicroelectronics *******************
  * @file    NVIC/DMA_WFIMode/readme.txt 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Description of the NVIC DMA in WFI mode example.
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

This example shows how to enters the system to WFI mode with DMA transfer enabled
and wake-up from this mode by the DMA End of Transfer interrupt.

In the associated software, the system clock is set to 72 MHz (24MHz on Value line), 
the DMA1 Channely is configured to transfer 10 data from the EVAL COM1 USART data 
register to a predefined buffer, DST_Buffer, and to generate an interrupt at the 
end of the transfer.
The EVAL COM1 USART receives data from HyperTerminal.
A LED1 is toggled with a frequency depending on the system clock, this is used 
to indicate whether the MCU is in WFI or RUN mode. 

A falling edge on the selected EXTI Line will put the core in the WFI mode, 
causing the led pin to stop toggling.
To wake-up from WFI mode you have to send the sequence (0, 1, 2, 3, 4, 5, 6, 7, 8, 9)
from the Hyperterminal to the EVAL COM1 USART. These bytes will be transferred 
by the DMA from the EVAL COM1 receive data register to the predefined buffer, 
then generates an interrupt which exits the system from WFI mode. 
The LED1 restarts toggling and a LED2 will toggle if the buffer is correctly 
received else a LED3 is toggled.


@par Directory contents 

  - NVIC/DMA_WFIMode/stm32f10x_conf.h     Library Configuration file
  - NVIC/DMA_WFIMode/stm32f10x_it.c       Interrupt handlers
  - NVIC/DMA_WFIMode/stm32f10x_it.h       Interrupt handlers header file
  - NVIC/DMA_WFIMode/main.c               Main program
  - NVIC/DMA_WFIMode/system_stm32f10x.c   STM32F10x system source file

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
    uncomment the corresponding line in stm32_eval.h file (under Utilities\STM32_EVAL)

  - STM32100E-EVAL Set-up 
    - Use LED1, LED2, LED3 and LED4 leds connected respectively to PF.06, PF0.7, PF.08
      and PF.09 pins
    - The USART1 signals (Rx, Tx) must be connected to a DB9 connector using a RS232
      transceiver.
    - Connect a null-modem female/female RS232 cable between the DB9 connector,
      CN12 on STM3210E-EVAL board, and PC serial port.   
    - Hyperterminal configuration:
       - Word Length = 8 Bits
       - One Stop Bit
       - No parity
       - BaudRate = 115200 baud
       - flow control: None 
    - Use the Key push-button connected to pin PG.08 (EXTI Line8). 
        
  - STM32100B-EVAL Set-up   
    - Use LED1, LED2, LED3 and LED4 leds connected respectively to PC.06, PC.07, PC.08
      and PC.09 pins
    - The USART1 signals (Rx, Tx) must be connected to a DB9 connector using a RS232
      transceiver.
    - Connect a null-modem female/female RS232 cable between the DB9 connector, 
      CN10 on STM32100B-EVAL board, and PC serial port.   
    - Hyperterminal configuration:
       - Word Length = 8 Bits
       - One Stop Bit
       - No parity
       - BaudRate = 115200 baud
       - flow control: None   
    - Use the Key push-button connected to pin PB.09 (EXTI Line9).
    
  - STM3210C-EVAL Set-up 
    - Use LED1, LED2, LED3 and LED4 connected respectively to PD.07, PD.13, PF.03
      and PD.04 pins
    - The USART2 signals (Rx, Tx) must be connected to a DB9 connector using a RS232
      transceiver.
    - Connect a null-modem female/female RS232 cable between the DB9 connector,
      CN6 on STM3210C-EVAL board, and PC serial port.   
    - Hyperterminal configuration:
       - Word Length = 8 Bits
       - One Stop Bit
       - No parity
       - BaudRate = 115200 baud
       - flow control: None 
    - Use the Key push-button connected to pin PB.09 (EXTI Line9). 
    
  - STM3210E-EVAL Set-up 
    - Use LED1, LED2, LED3 and LED4 leds connected respectively to PF.06, PF0.7, PF.08
      and PF.09 pins
    - The USART1 signals (Rx, Tx) must be connected to a DB9 connector using a RS232
      transceiver.
    - Connect a null-modem female/female RS232 cable between the DB9 connector,
      CN12 on STM3210E-EVAL board, and PC serial port.   
    - Hyperterminal configuration:
       - Word Length = 8 Bits
       - One Stop Bit
       - No parity
       - BaudRate = 115200 baud
       - flow control: None 
    - Use the Key push-button connected to pin PG.08 (EXTI Line8).   

  - STM3210B-EVAL Set-up   
    - Use LED1, LED2, LED3 and LED4 leds connected respectively to PC.06, PC.07, PC.08
      and PC.09 pins
    - The USART1 signals (Rx, Tx) must be connected to a DB9 connector using a RS232
      transceiver.
    - Connect a null-modem female/female RS232 cable between the DB9 connector, 
      CN6 on STM3210B-EVAL board, and PC serial port.   
    - Hyperterminal configuration:
       - Word Length = 8 Bits
       - One Stop Bit
       - No parity
       - BaudRate = 115200 baud
       - flow control: None   
    - Use the Key push-button connected to pin PB.09 (EXTI Line9).

	
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
