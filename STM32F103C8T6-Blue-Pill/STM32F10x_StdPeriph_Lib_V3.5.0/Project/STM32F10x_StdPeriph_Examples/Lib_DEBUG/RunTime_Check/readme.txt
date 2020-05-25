/**
  @page Lib_DEBUG_RunTime_Check Lib_DEBUG Run Time Checking example
  
  @verbatim
  ******************** (C) COPYRIGHT 2011 STMicroelectronics *******************
  * @file    Lib_DEBUG/RunTime_Check/readme.txt 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Description of the Lib_DEBUG Run Time Checking example.
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

This example demonstrates how to declare a dynamic peripherals pointers used for
Debug mode.  

To use Debug mode you have to add the stm32f10x_ip_dbg.c file to your application.
This creates a pointer to the peripheral structure in SRAM. Debugging consequently
becomes easier and all register settings can be obtained by dumping a peripheral 
variable.

When the "USE_FULL_ASSERT" label is uncommented (in stm32f10x_conf.h file),
the assert_param macro is expanded and runtime checking is enabled in the firmware
library code. The runtime checking allows checking that all the library functions
input value lies within the parameter allowed values.

The associated program simulates wrong parameter passed to library function and the
source of the error is printed on HyperTerminal (through USART).

@note
The Debug mode increases the code size and reduces the code performance. For this
reason, it is recommended to used it only when debugging the application and to
remove it from the final application code.


@par Directory contents 

  - Lib_DEBUG/RunTime_Check/stm32f10x_conf.h             Library Configuration file
  - Lib_DEBUG/RunTime_Check/stm32f10x_it.c               Interrupt handlers
  - Lib_DEBUG/RunTime_Check/stm32f10x_it.h               Header for stm32f10x_it.c
  - Lib_DEBUG/RunTime_Check/main.c                       Main program
  - Lib_DEBUG/RunTime_Check/stm32f10x_ip_dbg.c           Peripherals pointers initialization
  - Lib_DEBUG/RunTime_Check/stm32f10x_ip_dbg.h           Header for lib_dbg.c 
  - Lib_DEBUG/RunTime_Check/system_stm32f10x.c           STM32F10x system source file
  
@par Hardware and Software environment 

  - This example runs on STM32F10x Connectivity line, High-Density, High-Density
    Value line, Medium-Density, XL-Density, High-Density Value line, Medium-Density 
    Value line, Low-Density and Low-Density Value line Devices.
  
  - This example has been tested with STMicroelectronics STM32100E-EVAL (High-Density
    Value line), STM32100B-EVAL (Medium-Density Value line), STM3210C-EVAL 
    (Connectivity line), STM3210E-EVAL (High-Density and XL-Density) and STM3210B-EVAL 
    (Medium-Density) evaluation boards and can be easily tailored to any other 
    supported device and development board.
    To select the STMicroelectronics evaluation board used to run the example, 
    uncomment the corresponding line in stm32_eval.h file (under Utilities\STM32_EVAL)

  - STM32100E-EVAL Set-up 
    - Connect a null-modem female/female RS232 cable between the DB9 connector 
      CN10 and PC serial port.
      
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
 - Add the required example files
   - stm32f10x_ip_dbg.c

@note 
  - The free Lite version of TrueSTUDIO do not support printf() redirection.
    For more information, see "note.txt" under "STM32F10x_StdPeriph_Template\TrueSTUDIO".
     
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
