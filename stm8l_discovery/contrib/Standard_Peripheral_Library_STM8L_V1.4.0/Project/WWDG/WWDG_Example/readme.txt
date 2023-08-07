/** @page WWDG_Example WWDG counter refresh and simulation of WWDG reset

  @verbatim
  ******************** (C) COPYRIGHT 2013 STMicroelectronics *******************
  * @file    WWDG/WWDG_Example/readme.txt
  * @author  MCD Application Team
  * @version V1.5.2
  * @date    30-September-2014
  * @brief   Description of the WWDG Example.
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

  This example shows how to refresh the WWDG counter. The WWDG timeout is set to
  251.9 ms.
  
  As AllowedRefresh variable is set to ENABLE the WWDG counter is refreshed 
  preventing a WWDG reset and LED2 is toggling.
  When user press key button, an EXTI event is triggered and the corresponding
  ISR is served setting AllowedRefresh variable to DISABLE. So no WWDG counter
  refresh is performed and when WWDG counter reaches 63 an MCU reset will occur.

  When user press joystick down button, an EXTI event is triggered and the 
  corresponding ISR is served setting NonAlowedRefresh variable to ENABLE. 
  So WWDG counter refresh is performed in the non allowed window and as a result
  an MCU reset will occur.

  If a WWDG reset is generated, after system startup LED1 toggles otherwise it
  turns off.


  @par Directory contents

  - WWDG/WWDG_Example/main.c             Main file containing the "main" function
  - WWDG/WWDG_Example/stm8l15x_conf.h    Library Configuration file
  - WWDG/WWDG_Example/stm8l15x_it.c      Interrupt routines source 
  - WWDG/WWDG_Example/stm8l15x_it.h      Interrupt routines declaration


  @par Hardware and Software environment

    - This example runs on STM8L15x High-Density, Medium-Density Plus, Medium-Density
    and Low-Density Devices.

  - This example has been tested with STMicroelectronics STM8L1528-EVAL 
    (STM8L15x High-Density devices) and STM8L1526-EVAL (STM8L15x Medium-Density 
     and Low-Density devices) evaluation boards and can be easily tailored to any other
    development board.

  - STM8L1528-EVAL Set-up
     - Make sure that the LCD glass daughter board is mounted in IO position.
       For more details please refer to the evaluation board user manual.
     - leds LD1, LD2, Key and joystick down push buttons are used.

  - STM8L1526-EVAL Set-up
     - Make sure that the LCD glass daughter board is mounted in IO position.
       For more details please refer to the evaluation board user manual.
     - leds LD1, LD2, Key and joystick down push buttons are used.
     - Make sure that JP7 is in key position.
  
  
  @par How to use it ?

  In order to make the program work, you must do the following :

  - Copy all source files from this example folder to the template folder under
    Project\STM8L15x_StdPeriph_Template
  - Open your preferred toolchain 
  - Add the required file:
    - stm8_eval.c   (under Utilities\STM8_EVAL)
  - Rebuild all files and load your image into target memory
  - Run the example
  - LED 2 is toggling, WWDG counter is refreshed in allowed window so no reset.
  - When you press key button, WWDG counter is never refreshed and when it
    reaches 63, LED1 toggles indicating a WWDG reset.
  - When you press joystick down button, WWDG counter is refreshed in the non
    allowed window so LED1 toggles indicating a WWDG reset.



 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
