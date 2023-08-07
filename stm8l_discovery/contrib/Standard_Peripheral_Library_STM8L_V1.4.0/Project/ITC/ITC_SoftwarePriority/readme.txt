/** @page ITC_SoftwarePriority Changing software priority of two external interrupt lines

  @verbatim
  ******************** (C) COPYRIGHT 2013 STMicroelectronics *******************
  * @file    ITC/ITC_SoftwarePriority/readme.txt
  * @author  MCD Application Team
  * @version V1.5.2
  * @date    30-September-2014
  * @brief   Description of the ITC Software priority Example.
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

  This example shows how to change the software priority of two external 
  interrupt lines to allow nested interrupt management.
  In this example, we set the Key IRQ software priority level to 1 and set the 
  joystick Down IRQ software priority level to 2.
  Thus joystick Down button IRQ software priority is higher than Key one and 
  pressing joystick Down button interrupts the execution of the Key Interrupt 
  Sub-Routine.
  If the Key IRQ and joystick Down IRQ had the same software priority, neither
  Key IRQ could interrupt joystick Down IRQ nor joystick Down could interrupt 
  Key IRQ.

  In STM8L1528-EVAL, Key button is connected to EXTI6 and joystick Down button
  is connected to EXTI1.
  In STM8L1526-EVAL, Key button is connected to EXTI1 and joystick Down button
  is connected to EXTI4.

  @par Directory contents

  - ITC/ITC_SoftwarePriority/main.c            Main file containing the "main" function
  - ITC/ITC_SoftwarePriority/stm8l15x_conf.h   Library Configuration file
  - ITC/ITC_SoftwarePriority/stm8l15x_it.c     Interrupt routines source
  - ITC/ITC_SoftwarePriority/stm8l15x_it.h     Interrupt routines declaration


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
     - Two leds LD2 and LD3
     - Key and Joystick Down buttons

  - STM8L1526-EVAL Set-up
     - Make sure that the LCD glass daughter board is mounted in IO position.
       For more details please refer to the evaluation board user manual.
     - Two leds LD2 and LD3
     - Key and Joystick Down buttons
     - Make sure that the JP7 jumper is in Key position.
  
  
  @par How to use it ?

  In order to make the program work, you must do the following :

  - Copy all source files from this example folder to the template folder under
    Project\STM8L15x_StdPeriph_Template
  - Open your preferred toolchain
  - Add the required file:
    - stm8_eval.c   (under Utilities\STM8_EVAL) 
  - Rebuild all files and load your image into target memory
  - Run the example
  - Leds LD2 and LD3 are ON
  - When the Joystick Down button is pressed, the LD2 toggles and LD3 is OFF
  - When the Key button is pressed, the LD3 toggles and LD2 is OFF
  - Play with the Key button and the Joystick Down in order to see which one
    has the highest priority:
    - press and maintain the Key button -> LD3 toggles, LD2 is OFF
    - Continue pressing the Key button and press the joystick Down button 
        -> LD2 toggles instead, LD3 is OFF (Joystick IRQ is higher priority  
     If you do the reverse, you can see that the joystick button ISR is not 
     interrupted when the Key button is pressed.



 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
