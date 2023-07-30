/** @page USART_Printf Retarget the C library printf/getchar functions to the USART

  @verbatim
  ******************** (C) COPYRIGHT 2013 STMicroelectronics *******************
  * @file    USART/USART_Printf/readme.txt 
  * @author  MCD Application Team
  * @version V1.5.2
  * @date    30-September-2014
  * @brief   Description of the USART retarget C library Example.
  ******************************************************************************
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
   @endverbatim

  @par Example description
  This example shows how to retarget the C library printf/getchar functions
  to the USART. 
  This implementation output the printf message on the Hyperterminal using USART 
  and keep waiting a character to be entered by the user.

  The USART is configured as follows:
    - BaudRate = 115200 baud  
    - Word Length = 8 Bits
    - One Stop Bit
    - No parity
    - Receive and transmit enabled
  
  @par Directory contents

  - USART/USART_Printf/main.c            Main file containing the "main" function
  - USART/USART_Printf/stm8l15x_conf.h   Library Configuration file
  - USART/USART_Printf/stm8l15x_it.c     Interrupt routines source
  - USART/USART_Printf/stm8l15x_it.h     Interrupt routines declaration


  @par Hardware and Software environment

    - This example runs on STM8L15x High-Density, Medium-Density Plus, Medium-Density
    and Low-Density Devices.
  
  - This example has been tested with STMicroelectronics STM8L1528-EVAL 
    (STM8L15x High-Density devices) and STM8L1526-EVAL (STM8L15x Medium-Density and
    Low-Density devices) evaluation boards and can be easily tailored to any other
    development board.

  - STM8L1528-EVAL Set-up 
    - Make sure that the LCD glass daughter board is mounted in IO position.
      For more details please refer to the evaluation board user manual.
    - Connect a null-modem female/female RS232 cable between the DB9 connector 
       CN7 and PC serial port.
    - Make sure that JP5 jumper is in RS232 position
    - Make sure that JP11 jumper is in RS232 position
    - Make sure that JP12 jumper is in RS232 position
    
  - STM8L1526-EVAL Set-up
     - Make sure that the LCD glass daughter board is mounted in IO position.
      For more details please refer to the evaluation board user manual.
     - Connect a null-modem female/female RS232 cable between the DB9 connector 
       CN1 and PC serial port.
     - Make sure that JP5 jumper is in RS232 position
    
  - HyperTerminal configuration:
    - Word Length = 8 Bits
    - One Stop Bit
    - No parity
    - BaudRate = 115200 baud


  @par How to use it ?

  In order to make the program work, you must do the following :

  - Copy all source files from this example folder to the template folder under
    Project\STM8L15x_StdPeriph_Template
  - Open your preferred toolchain 
  - Add the required file:
    - stm8_eval.c   (under Utilities\STM8_EVAL)
  - Rebuild all files and load your image into target memory
  - Run the example
   


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
