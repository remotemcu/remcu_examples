/** @page USART_IrDA Use the USART in a basic IrDA communication receive/transmit mode.

  @verbatim
  ******************** (C) COPYRIGHT 2013 STMicroelectronics *******************
  * @file    USART/USART_IrDA/readme.txt 
  * @author  MCD Application Team
  * @version V1.5.2
  * @date    30-September-2014
  * @brief   Description of the USART IrDA Example.
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

  The example provides two IrDA program: transmitter / receiver and requires two
  boards to be able to run the full demonstration:
   - one board will act as IrDA transmitter
   - one board will act as IrDA receiver

  On the Transmitter board: Five pins, configured in input floating mode, are 
  used to select the byte to be sent at each pin state change
  The byte can be:
   - 0x00 if no key pressed
   - 0x01 if Joystick Sel pressed
   - 0x02 if Joystick Down pressed
   - 0x03 if Joystick Left pressed
   - 0x04 if Joystick Right pressed
   - 0x05 if Joystick Up pressed

   On the Receiver board: Four leds are used to show which byte is received. 
  - LED4 toggle when 0x05 is received (Joystick Up pressed in Transmitter board)  
  - LED3 toggle when 0x02 is received (Joystick Down pressed in Transmitter board)  
  - LED2 toggle when 0x03 is received (Joystick Left pressed in Transmitter board)  
  - LED1 toggle when 0x04 is received (Joystick Right pressed in Transmitter board)  
  - LED1 to LED4 toggle when 0x01 is received (Joystick Sel pressed in Transmitter board)         

  USART configured as follow:
   - BaudRate = 9600 baud  
   - Word Length = 8 Bits
   - One Stop Bit
   - No parity
   - Receive and transmit enabled

  
  @par Directory contents

  - USART/USART_IrDA/main.c                   Main file containing the "main" function
  - USART/USART_IrDA/stm8l15x_conf.h          Library Configuration file
  - USART/USART_IrDA/stm8l15x_it.c            Interrupt routines source
  - USART/USART_IrDA/stm8l15x_it.h            Interrupt routines declaration


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
     - IrDA transceiver (U6) is used, make sure that JP5 jumper is in IrDA position.
     - LED1..4
     - Joystick

  - STM8L1526-EVAL Set-up
     - Make sure that the LCD glass daughter board is mounted in IO position.
      For more details please refer to the evaluation board user manual.
     - IrDA transceiver (U2) is used, make sure that JP5 jumper is in IrDA position.
     - LED1..4
     - Joystick
 
  @par How to use it ?

  In order to make the program work, you must do the following :

  - Copy all source files from this example folder to the template folder under
    Project\STM8L15x_StdPeriph_Template
  - Open your preferred toolchain 
  - Add the required file:
    - stm8_eval.c   (under Utilities\STM8_EVAL)
  - In main.c file select if the board will act as
    - transmitter (USART_IrDA_TRANSMIT define enabled)  
    - or receiver (USART_IrDA_RECEIVE define enabled ) 
  - Rebuild all files and load your image into target memory
  - Run the example
   


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
