/**
  @page CEC_DataExchangeInterrupt CEC Data Exchange using Interrupt example
  
  @verbatim
  ******************** (C) COPYRIGHT 2011 STMicroelectronics *******************
  * @file    CEC/DataExchangeInterrupt/readme.txt 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Description of the CEC Data Exchange using Interrupt example.
  ******************************************************************************
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  ******************************************************************************
   @endverbatim

@par Description 

This example provides a basic communication between two HDMI-CEC devices using 
interrupts. 
The first and second CEC device send TransmitBuffer to the CEC other device. 
The data received by the first and second CEC device  are stored respectively in 
ReceiveBuffer. The data transfer is managed in CEC_IRQHandler in 
stm32f10x_it.c file.

The example illustrates the use of the CEC communication between two devices 
(2 x STM32100B-EVAL boards).
Each device can send a frame to the other device by pressing the user key 
button on the EVAL board. 

- Edit CEC/stm32f10x_conf.h to select the the corresponding device (CEC_DEVICE1,
CEC_DEVICE2).
 
@par Directory contents 

  - CEC/DataExchangeInterrupt/stm32f10x_conf.h    Library Configuration file
  - CEC/DataExchangeInterrupt/stm32f10x_it.h      Interrupt handlers header file
  - CEC/DataExchangeInterrupt/stm32f10x_it.c      Interrupt handlers
  - CEC/DataExchangeInterrupt/main.c              Main program
  - CEC/DataExchangeInterrupt/system_stm32f10x.c  STM32F10x system source file
           
@par Hardware and Software environment

  - This example runs on STM32F10x High-Density Value line, Medium-Density Value 
    line and Low-Density Value line Devices.
  
  - This example has been tested with STMicroelectronics STM32100E-EVAL 
    (STM32F10x High-Density Value line), STM32100B-EVAL 
    (STM32F10x Medium-Density Value line) evaluation boards and can be easily 
    tailored to any other supported device and development board.
    
  - Connect the boards by using one of the two following alternatives:
  - A HDMI Cables between all boards HDMI-CEC connectors (CN15 or CN16) on 
    STM32100B-EVAL or between connectors (CN3 or CN4) on STM32100E-EVAL board. 
  - Use a simple wire between all devices CEC Lines (PB.08), in this case don't 
    forget to connect all boards grounds together.
   
@note
 - You can also use more than two CEC devices as much as you want by changing 
   only the Device address and selecting the corresponding followers.
 - You can use also the STM3210B-EVAL with the UM0685 associated firmware as a
   CEC device. This configuration is available only when use a simple wire connected
   between STM3210B-EVAL PA.00 and STM32100B-EVAL PB.08 or STM32100E-EVAL PB.08 
   pins. Don't forget to add a 27KOhm pull-up resistor on the STM3210B-EVAL PA.00 
   and to to connect all boards grounds together.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Copy all source files from this example folder to the template folder under
   Project\STM32F10x_StdPeriph_Template
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Load the project image of First_Device (choose CEC_DEVICE1 when compiling) to 
   the first board.
 - Load the project image of Second_Device (choose CEC_DEVICE2 when compiling) to 
   the second board.
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
 - High-density devices are STM32F101xx and STM32F103xx microcontrollers where
   the Flash memory density ranges between 256 and 512 Kbytes.
 - High-density Value line devices are STM32F100xx microcontrollers where
   the Flash memory density ranges between 256 and 512 Kbytes.   
 - XL-density devices are STM32F101xx and STM32F103xx microcontrollers where
   the Flash memory density ranges between 512 and 1024 Kbytes.
 - Connectivity line devices are STM32F105xx and STM32F107xx microcontrollers. 

 * <h3><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h3>
 */
