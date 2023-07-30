/** @page CLK_SYSCLKSwitch  System clock switching between HSE, HSI, LSI and LSE

  @verbatim
  ******************** (C) COPYRIGHT 2013 STMicroelectronics *******************
  * @file    CLK/CLK_SYSCLKSwitch/readme.txt 
  * @author  MCD Application Team
  * @version V1.5.2
  * @date    30-September-2014
  * @brief   Description of the CLK System clock switch Example.
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

  This example demonstrates how to switch the System clock from one source to another.
  
  To select a clock source, push the joystick as follow:
    - UP = HSE used as System clock source
    - RIGHT = HSI used as System clock source
    - DOWN = LSI used as System clock source
    - LEFT = LSE used as System clock source
  The selected clock source will be displayed on the LCD and the LEDs toggling 
  frequency will be changed according to the selected clock source.


  @par Directory contents

  - CLK/CLK_SYSCLKSwitch/main.c                   Main file
  - CLK/CLK_SYSCLKSwitch/stm8l15x_conf.h          Library Configuration file
  - CLK/CLK_SYSCLKSwitch/stm8l15x_it.c            Interrupt routines source
  - CLK/CLK_SYSCLKSwitch/stm8l15x_it.h            Interrupt routines declaration


  @par Hardware and Software environment

    - This example runs on STM8L15x High-Density, Medium-Density Plus, Medium-Density
    and Low-Density Devices.
  
  - This example has been tested with STMicroelectronics STM8L1528-EVAL 
    (STM8L15x High-Density devices) and STM8L1526-EVAL (STM8L15x Medium-Density and
    Low-Density devices) evaluation boards and can be easily tailored to any other
    development board.

  - STM8L1528-EVAL and STM8L1526-EVAL Set-up
     - Make sure that the LCD glass daughter board is mounted in IO position.
       For more details please refer to the evaluation board user manual.
     - LD1..4
     - Joystick buttons.
     - Dot matrix LCD
     
  @par How to use it ?

  In order to make the program work, you must do the following :

  - Copy all source files from this example folder to the template folder under
    Project\STM8L15x_StdPeriph_Template
  - Open your preferred toolchain 
  - Add the required files:
    - stm8_eval.c   (under Utilities\STM8_EVAL)
    - stm8_eval_lcd.c   (under Utilities\STM8_EVAL\Common)
    - timing_delay.c      (under Utilities\Misc)
  - Rebuild all files and load your image into target memory
  - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
