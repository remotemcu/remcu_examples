/**
  @page RTC_Calendar RTC Calendar example
  
  @verbatim
  ******************** (C) COPYRIGHT 2011 STMicroelectronics *******************
  * @file    RTC/Calendar/readme.txt 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Description of the RTC Calendar example.
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

This example demonstrates and explains how to use the RTC peripheral. 
As an application example, it demonstrates how to setup the RTC peripheral, in terms
of prescaler and interrupts, to be used to keep time and to generate Second interrupt. 

The Low Speed External (LSE) clock is used as RTC clock source. 
The RTC clock can be output on the Tamper pin (PC.13). To enable this functionality,
uncomment the corresponding line: #define RTCClockOutput_Enable in the main.c file.

The RTC is in the backup (BKP) domain, still powered by VBAT when VDD is switched off,
so the RTC configuration is not lost if a battery is connected to the VBAT pin. 
A key value is written in backup data register1 (BKP_DR1) to indicate if the RTC
is already configured.

The program behaves as follows:

1. After startup the program checks the backup data register1 value:
    - register1 value not correct: (BKP_DR1 value is not correct or has not yet
      been programmed when the program is executed for the first time) the RTC is
      configured and the user is asked to set the time (entered on HyperTerminal).
    
    - register1 value correct: this means that the RTC is configured and the time
      is displayed on HyperTerminal.

2. When an External Reset occurs the BKP domain is not reset and the RTC configuration
   is not lost.

3. When power on reset occurs:
    - If a battery is connected to the VBAT pin: the BKP domain is not reset and
      the RTC configuration is not lost.
      
    - If no battery is connected to the VBAT pin: the BKP domain is reset and the
      RTC configuration is lost.

In the RTC interrupt service routine, the LED1 toggles every 1 s.
The C library printf function is retargeted to the USART, that is, the printf
message is output to the HyperTerminal using USART1 or USART2 depending on the 
EVAL board you are using.

 
@par Directory contents 

  - RTC/Calendar/stm32f10x_conf.h     Library Configuration file
  - RTC/Calendar/stm32f10x_it.c       Interrupt handlers
  - RTC/Calendar/stm32f10x_it.h       Header for stm32f10x_it.c
  - RTC/Calendar/main.c               Main program
  - RTC/Calendar/system_stm32f10x.c   STM32F10x system source file

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
    - Use LED1 connected to PF.06.
    - Connect a null-modem female/female RS232 cable between the DB9 connector 
      CN10(when USART1 is used) and PC serial port. 
    - Make sure that jumper JP1 is in position 1-2 to connect the 3V battery to VBAT pin
    
  - STM32100B-EVAL Set-up   
    - Use LED1 connected to PC.06.
    - Connect a null-modem female/female RS232 cable between the DB9 connector 
      CN10(when USART1 is used) and PC serial port.
    - Make sure that jumper JP9 is in position 1-2 to connect the 3V battery to VBAT pin
    
  - STM3210C-EVAL Set-up 
    - Use LED1 connected to PD.07.
    - Connect a null-modem female/female RS232 cable between the DB9 connector 
      CN6 (USART2) and PC serial port .
    @note Make sure that jumpers JP19 and JP18 are open.    
    - Make sure that jumper JP24 is in position 1-2 to connect the 3V battery to VBAT pin
    
  - STM3210E-EVAL Set-up 
    - Use LED1 connected to PF.06.
    - Connect a null-modem female/female RS232 cable between the DB9 connector 
      CN12(when USART1 is used) and PC serial port. 
    - Make sure that jumper JP1 is in position 1-2 to connect the 3V battery to VBAT pin
    
  - STM3210B-EVAL Set-up   
    - Use LED1 connected to PC.06.
    - Connect a null-modem female/female RS232 cable between the DB9 connector 
      CN6(when USART1 is used) and PC serial port.
    - Make sure that jumper JP11 is in position 1-2 to connect the 3V battery to VBAT pin

  - Hyperterminal configuration:
    - Word Length = 8 Bits
    - One Stop Bit
    - No parity
    - BaudRate = 115200 baud
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
