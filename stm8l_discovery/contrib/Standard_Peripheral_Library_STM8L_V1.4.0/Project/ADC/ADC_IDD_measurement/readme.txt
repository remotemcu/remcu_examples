/** @page ADC_IDD_Measurement Use ADC to measure live IDD current in Run mode

  @verbatim
  ******************** (C) COPYRIGHT 2013 STMicroelectronics *******************
  * @file    ADC/ADC_IDD_Measurement/readme.txt 
  * @author  MCD Application Team
  * @version V1.5.2
  * @date    30-September-2014
  * @brief   Description of the ADC IDD measurement Example.
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

  This example provides a short description of how to use the ADC peripheral
  to measure IDD in Run and Halt modes by using the IDD measurement circuit 
  available on the STM8L152x-EVAL board.
  When running the example, the IDD measurement menu is displayed.

  To navigate the IDD measurement menu, perform the following actions:
      - RIGHT: Navigates to the next submenu items on the right.
      - LEFT: Navigates to the next submenu items on the left.
      - SEL: Enters submenu.
      - UP: Exits from a submenu.
  Once an IDD Measurement submenu is selected by pressing the Joystick SEL
  push-button the IDD value is continuously displayed on LCD, until the joystick 
  UP push-button is pressed.
  If the joystick UP push-button has been pressed, the MCU exits the IDD Mode 
  submenu and goes back to the IDD measurement menu.

  The STM8L1528-EVAL board is provided with an IDD measurement circuit, more
  details can be provided in STM8L1528-EVAL user manual.
  The STM8L1526-EVAL board is provided with an IDD measurement circuit, more
  details can be provided in STM8L1526-EVAL user manual.

  @par Directory contents

  - ADC/ADC_IDD_Measurement/main.c                   Main file
  - ADC/ADC_IDD_Measurement/stm8l15x_conf.h          Library Configuration file
  - ADC/ADC_IDD_Measurement/stm8l15x_it.c            Interrupt routines source
  - ADC/ADC_IDD_Measurement/stm8l15x_it.h            Interrupt routines declaration


  @par Hardware and Software environment

    - This example runs on STM8L15x High-Density, Medium-Density Plus, Medium-Density
    and Low-Density Devices..
  
  - This example has been tested with STMicroelectronics STM8L1528-EVAL 
    (STM8L15x High-Density devices) and STM8L1526-EVAL (STM8L15x Medium-Density and
    Low-Density devices) evaluation boards and can be easily tailored to any other
    development board.

  - STM8L1528-EVAL Set-up
     - LCD display
     - Before Running the example make sure that:
         - Jumper JP4 is in IDD position (1-2  .
         - Jumpers JP11 and JP12 are in IDD position.
     - Make sure that the LCD glass daughter board is mounted in IO position.
     For more details please refer to the evaluation board user manual.
    
  - STM8L1526-EVAL Set-up
     - LCD display
     - Before Running the example make sure that jumper JP11 is in IDD position (1-2  .
     - Make sure that the LCD glass daughter board is mounted in IO position.
     For more details please refer to the evaluation board user manual.
  

  @par How to use it ?

  In order to make the program work, you must do the following :

  - Copy all source files from this example folder to the template folder under
    Project\STM8L15x_StdPeriph_Template
  - Open your preferred toolchain 
  - Add the required file:
    - stm8_eval.c       (under Utilities\STM8_EVAL)
    - stm8_eval_lcd.c   (under Utilities\STM8_EVAL\Common)
    - timing_delay.c      (under Utilities\Misc)
  - Rebuild all files and load your image into target memory
  - Run the example
  - The IDD Measurement menu is displayed.

 


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
