/** @page USART_MultiProcessor Use the USART in multi-processor mode

  @verbatim
  ******************** (C) COPYRIGHT 2013 STMicroelectronics *******************
  * @file    USART/USART_MultiProcessor/readme.txt 
  * @author  MCD Application Team
  * @version V1.5.2
  * @date    30-September-2014
  * @brief   Description of the USART multi-processor Example.
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

  This example provides a description of how to use the USART in multi-processor mode.
  The USART Master and USART Slave addresses are set to 0x1 and 0x2, 
  respectively.
   
  - In the Master board (define USART_MASTER in main.h  , USART Master 
    continuously sends the 0x33 character to second board (USART Slave  . 
  - In the Slave board, four LEDs are toggled while the USART receives 0x33.
  - In the Slave board when KEY pushbutton is pressed, an interrupt is generated
    and in the EXTI Interrupt service routine, the USART enters mute mode and 
    remains in this mode (no LEDs toggling   
  - In the Master board when KEY pushbutton is pressed, an interrupt is 
    generated and in the EXTI Interrupt service routine, USART Master sends the 
    address mark character (0x102) to wake up USART Slave. 
  - In the Slave board the USART slave receives the wake up and the LEDs restart 
    toggling. 

  USART Master and Slave configured as follow:
  - BaudRate = 9600 baud  
  - Word Length = 9 Bits
  - One Stop Bit
  - No parity
  - Receive and transmit enabled

              
   @par Directory contents

  - USART\USART_MultiProcessor\main.c            Main file containing the "main" function
  - USART\USART_MultiProcessor\main.h            Main functions prototype
  - USART\USART_MultiProcessor\stm8l15x_conf.h   Library Configuration file
  - USART\USART_MultiProcessor\stm8l15x_it.c     Interrupt routines source
  - USART\USART_MultiProcessor\stm8l15x_it.h     Interrupt routines declaration


   @par Hardware and software environment

    - This example runs on STM8L15x High-Density, Medium-Density Plus, Medium-Density
    and Low-Density Devices.
  
  - This example has been tested with STMicroelectronics STM8L1528-EVAL 
    (STM8L15x High-Density devices) and STM8L1526-EVAL (STM8L15x Medium-Density and
    Low-Density devices) evaluation boards and can be easily tailored to any other
    development board.

  - STM8L1528-EVAL Set-up 
     - Make sure that the LCD glass daughter board is mounted in IO position.
      For more details please refer to the evaluation board user manual.
     - To run this example it is necessary to use two STM8L1528-EVAL boards: 
       one to run the USART Master and another to run the USART Salve. 
       Connect USART2_Tx(PH.05) and USART2_Rx(PH.04) on the first board to 
       respectively USART2_Rx(PH.04) and USART2_Tx(PH.05) on the second board 
       (or connect a null-modem female/female RS232 cable between the two 
       boards: CN7 connector on each board).
       - Make sure that JP5 jumper is in RS232 position.
       - Make sure that JP11 jumper is in RS232 position if a null-modem 
             female/female RS232 cable is used to connect the two boards
             otherwise make sure that JP11 is not in RS232 position.
       - Make sure that the two boards are connected to the same ground.
       - Four leds LD1, LD2, LD3 and LD4

  - STM8L1526-EVAL Set-up 
     - Make sure that the LCD glass daughter board is mounted in IO position.
      For more details please refer to the evaluation board user manual.
     - Make sure that Jumper JP7 is set in KEY position. 
     - To run this example it is necessary to use two STM8L1526-EVAL boards: one
       to run the USART1 Master and another to run the USART1 Salve. 
       Connect USART1_Tx(PC.03   and USART1_Rx(PC.02) on the first board to 
       respectively USART1_Rx(PC.02) and USART1_Tx(PC.03) on the second board (
       or connect a null-modem female/female RS232 cable between the two boards: 
       CN1 connector on each board).
       - Make sure that JP5 jumper is in RS232 position.
       - Make sure that the two boards are connected to the same ground.
     - Four leds LD1, LD2, LD3 and LD4

   
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
